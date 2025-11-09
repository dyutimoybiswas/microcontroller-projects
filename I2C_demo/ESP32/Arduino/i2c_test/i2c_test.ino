#include "Wire.h"

#define I2C_DEV_ADDR 0x58
#define SCL          25
#define SDA          26
#define FREQ         100000

uint32_t i = 0;
#if 0
void onRequest(){
  Wire.print(i++);
  Wire.print(" Packets.");
  Serial.println("onRequest");
}
#endif
void onReceive(int len){
  Serial.printf("onReceive[%d]: ", len);
  while(Wire.available()){
    Serial.write(Wire.read());
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Wire.onReceive(onReceive);
  // Wire.onRequest(onRequest);
  Wire.begin((uint8_t)I2C_DEV_ADDR, SDA, SCL, FREQ);
}

void loop() {
  // empty
}