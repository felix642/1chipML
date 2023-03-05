import serial
import numpy as np
from PIL import Image
import re
import matplotlib.pyplot as plt
from sklearn.decomposition import PCA

def encode_arduino_input(matrix):
    return matrix.shape[0], str(matrix.shape[0]) + ',' + ','.join(map(lambda x: str(x), matrix.flatten())) + '\n'

def decode_arduino_output(response: str, size):
    m = re.findall('<([^>]*)>', response)
    vectors = []
    values = []
    group = m[0].split(',')
    for value in group:
        if value != '':
            vectors.append(float(value))
    group = m[1].split(',')
    for value in group:
        if value != '':
            values.append(float(value))
    
    return np.array(vectors).reshape((size, size)), np.array(values).reshape((size, size))

def main():

    # Read img
    img = np.array(Image.open('seven.bmp').convert('L'))
    np.set_printoptions(suppress=True)

    figure, axis = plt.subplots(3, 3)

    # PCA of sklearn
    pca_8 = PCA(n_components=8)
    img_reduced = pca_8.fit_transform(img)
    img_recovered = pca_8.inverse_transform(img_reduced)
    image_pca_8 = img_recovered.reshape([8,8])
    axis[2, 2].imshow(image_pca_8, cmap='gray')
    axis[2, 2].set_title('Sklearn')

    # Compute covariance matrix
    mean_values = img.mean(axis=0)
    centered_data = img - mean_values
    covariance_matrix = np.round(np.cov(centered_data.T), 3)

    # Encode covariance matrix and send it to arduino
    size, data = encode_arduino_input(covariance_matrix)
    print('Data : ', data, '\n')
    port = serial.Serial('/dev/cu.usbmodem1101', 9600)
    port.write(data.encode(encoding='utf-8'))

    # Retrieve eigenvalues and eigenvectors
    print('Trying to read from port')
    response = port.read_until(bytes(b'!')).decode(encoding='utf-8').strip()  # Wait for response and decode bytes to string
    eig_vec, eig_val = decode_arduino_output(response, size)
    print('Eigenvectors : ', eig_vec)
    print('Eigenvalues : ', eig_val)
    port.close()  # Close serial port

    # Sort eigenvalues and eigenvectors on descending order and take only 3 components
    for components in range(1, 9):
        
        indices = np.argsort(-eig_val.diagonal())
        # print('indices : ', indices)
        new_eig_val = eig_val[indices][:components, :]
        new_eig_vec = eig_vec[:,indices][:components, :]
        # print("Sorted Eigen vectors ", new_eig_vec)
        # print("Sorted Eigen values ", new_eig_val, "\n")

        pca_data = np.dot(centered_data, new_eig_vec.T)
        # print("Transformed data ", pca_data.shape)

        reconstructed = np.dot(pca_data, new_eig_vec) + mean_values

        # Show 
        axis[(components - 1) // 3, (components - 1) % 3].imshow(reconstructed, cmap='gray')
        axis[(components - 1) // 3, (components - 1) % 3].set_title(f'Arduino {components}')
    
    plt.show()

if __name__ == "__main__":
    main()

