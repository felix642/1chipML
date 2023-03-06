#define REAL_NUMBER float
#define INTEGER uint8_t
#define DIGITS_PRECISION 3
#define MATRIX_SIZE 8
// #define DEBUG

extern "C" {
  #include "jacobi.h"
}

enum State {
  READING,
  PROCESSING,
  WAITING
};

State state = State::READING;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

INTEGER size = 0;
INTEGER totalSize = 0;
REAL_NUMBER data[MATRIX_SIZE * MATRIX_SIZE];

void read() {
  if (Serial.available()) {
    INTEGER i = 0;
    String input = Serial.readString();
    if (input.length() > 0) {
      // parse input string to extract size and data
      char* str = new char[input.length() + 1];
      strcpy(str, input.c_str());
      char* token = strtok(str, ",");
      while (token != NULL) {
        if (i == 0) {
          size = atoi(token);
          totalSize = size * size;
        } else {
          data[i - 1] = atof(token);
        }
        token = strtok(NULL, ",");
        i++;
      }
      delete[] str;
      // print size and data to serial monitor
      #ifdef DEBUG
        Serial.print("Size of array: ");
        Serial.println(totalSize);
        Serial.print("Data: ");
        for (INTEGER j = 0; j < totalSize; j++) {
          Serial.print(data[j]);
          Serial.print(" ");
        }
        Serial.println();
      #endif
      state = State::PROCESSING;
    }
  }
}


void process() {
  INTEGER sizes[3] = {size, size, size};

  REAL_NUMBER* out = new REAL_NUMBER[totalSize];

  jacobi(data, size, out, -1, 0);

  Serial.print("<");
  for (vec_size i = 0; i < totalSize; ++i) {
    Serial.print(out[i]);
    Serial.print(",");
  }
  Serial.print("><");
  for (vec_size i = 0; i < totalSize; ++i) {
    Serial.print(data[i]);
    Serial.print(",");
  }
  Serial.print(">");
  Serial.print('!');

  memset(data, 0, sizeof(REAL_NUMBER) * MATRIX_SIZE * MATRIX_SIZE);
  delete[] out;
  
  state = State::READING;
}

void loop() {
  switch (state) {
    case State::READING:
      read();
      break;
    case State::PROCESSING:
      process();
      break;
  }

}
