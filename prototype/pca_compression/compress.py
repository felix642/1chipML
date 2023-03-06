import serial
import numpy as np
from PIL import Image
import re
import matplotlib.pyplot as plt

def encode_arduino_input(matrix):
    return matrix.shape[0], str(matrix.shape[0]) + ',' + ','.join(map(lambda x: str(x), matrix.flatten())) + '\n'

def decode_arduino_output(response: str, size):
    m = re.findall('<([^>]*)>', response)
    
    if m[0] == '' or m[1] == '':
        raise Exception("Could not parse response, response is empty")

    vectors = []    
    group = m[0].split(',')
    for value in group:
        if value != '':
            vectors.append(float(value))
    group = m[1].split(',')

    values = []
    for value in group:
        if value != '':
            values.append(float(value))
    
    return np.array(vectors).reshape((size, size)), np.array(values).reshape((size, size))

def main():
    size_of_image = 8

    # Read img
    img = np.array(Image.open(f'seven_{size_of_image}.bmp').convert('L'))
    np.set_printoptions(suppress=True)

    figure, axis = plt.subplots(3, 3)
    figure.delaxes(axis[2, 2])

    # Compute covariance matrix
    mean_values = img.mean(axis=0)
    centered_data = img - mean_values
    covariance_matrix = np.round(np.cov(centered_data.T), 3)

    # Encode covariance matrix and send it to arduino
    size, data = encode_arduino_input(covariance_matrix)
    print('Size : ', size)
    print('Data : ', data, '\n')
    port = serial.Serial('/dev/cu.usbmodem21201', 9600)
    port.write(data.encode(encoding='utf-8'))

    # Retrieve eigenvalues and eigenvectors
    print('Trying to read from port')
    response = port.read_until(bytes(b'!')).decode(encoding='utf-8').strip()
    print('Response : ', response)
    eig_vec, eig_val = decode_arduino_output(response, size)
    print('Eigenvectors : ', eig_vec)
    print('Eigenvalues : ', eig_val)
    port.close()

    # Sort eigenvalues and eigenvectors on descending order and take only k components
    for components in range(1, size_of_image + 1, size_of_image // 8):
        indices = np.argsort(-eig_val.diagonal())
        new_eig_vec = eig_vec[:,indices][:components, :]
        pca_data = np.dot(centered_data, new_eig_vec.T)
        reconstructed = np.dot(pca_data, new_eig_vec) + mean_values
        axis[(components - 1) // 3, (components - 1) % 3].imshow(reconstructed, cmap='gray')
        axis[(components - 1) // 3, (components - 1) % 3].set_title(f'Components={components}')
        axis[(components - 1) // 3, (components - 1) % 3].axis('off')
    
    plt.show()

if __name__ == "__main__":
    main()

