#include <Wire.h>

#define SLAVE_ADDRESS 0x6b

uint8_t number[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int number2;
int values[7] = {5096, 10000, 8000, 15000, 20000, 7586, 23456};
const int NUM_DOUBLES_ALPHA = 3; 
const int NUM_DOUBLES_BETA = 3; 
const int TOTAL_DOUBLES = NUM_DOUBLES_ALPHA + NUM_DOUBLES_BETA + 1; 

void setup() {
  Serial.begin(9600);
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  Serial.println("Ready!");
}

void loop() {
  delay(100);
}

void receiveData(int byteCount) {
  double receivedData[TOTAL_DOUBLES];
  byte buffer[sizeof(double) * TOTAL_DOUBLES];
  
  int index = 0;
  while (Wire.available() && index < sizeof(buffer)) {
    buffer[index++] = Wire.read();
  }

  if (index == sizeof(buffer)) {
    for (int i = 0; i < TOTAL_DOUBLES; i++) {
      memcpy(&receivedData[i], &buffer[i * sizeof(double)], sizeof(double));
    }

    Serial.println("Alpha list:");
    for (int i = 0; i < NUM_DOUBLES_ALPHA; i++) {
      Serial.print("Received double: ");
      Serial.println(receivedData[i], 8);
    }

    Serial.println("Beta list:");
    for (int i = NUM_DOUBLES_ALPHA; i < NUM_DOUBLES_ALPHA + NUM_DOUBLES_BETA; i++) {
      Serial.print("Received double: ");
      Serial.println(receivedData[i], 8);
    }

    Serial.println("Check value:");
    Serial.print("Received double: ");
    Serial.println(receivedData[NUM_DOUBLES_ALPHA + NUM_DOUBLES_BETA], 8);
    
  } else {
    Serial.println("Error: Received data size mismatch");
  }
}

void sendData() {
  for (int i = 0; i < 7; i++) {
    int highByte = (values[i] >> 8) & 0xFF;
    int lowByte = values[i] & 0xFF;
    number[i * 2] = highByte;
    number[i * 2 + 1] = lowByte;
  }

  Serial.print("Sending values: ");
  for (int i = 0; i < 7; i++) {
    Serial.print(values[i]);
    if (i < 6) {
      Serial.print(", ");
    }
  }
  Serial.println();
  
  Wire.write(number, sizeof(number));
}
