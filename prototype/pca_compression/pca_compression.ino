#define REAL_NUMBER float
#define INTEGER uint8_t
#define DIGITS_PRECISION 3
#define DEBUG

extern "C" {
  #include "jacobi.h"
}

#define LED_BUILTIN 13


enum State {
  READING,
  PROCESSING,
  WAITING
};

State state = State::READING;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

}

int size = 0;
int totalSize = 0;
float data[64];

void read() {
  if (Serial.available()) {
    
    int i = 0;
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
          // data = new float[totalSize];
          // out = new float[totalSize];
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
      for (int j = 0; j < totalSize; j++) {
        Serial.print(data[j]);
        Serial.print(" ");
      }
      Serial.println();
      #endif
      state = State::PROCESSING;
    }
  }
}

void print(char* val) {
  Serial.print(val);
}

void printInt(float val){
  Serial.print(val);
}


void process() {
  int sizes[3] = {size, size, size};

  float* out = new float[totalSize];

  jacobi(data, size, out, -1, 0);

  Serial.print("<");
  for (vec_size i = 0; i < totalSize; ++i) {
    Serial.print(out[i]);
    Serial.print(",");
  }
  Serial.print(">");
    Serial.print("<");
  for (vec_size i = 0; i < totalSize; ++i) {
    Serial.print(data[i]);
    Serial.print(",");
  }
  Serial.print(">");
  Serial.print('!');
  state = State::READING;
}

void loop() {
  // put your main code here, to run repeatedly:
  serialLog = print;
  serialLogInt = printInt;

  switch (state) {
    case State::READING:
      read();
      break;
    case State::PROCESSING:
      process();
      break;
  }

}
