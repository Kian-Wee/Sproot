
/*
Read pot on AIN1 convert value to voltage
then display on Arduino serial monitor.

By Lewis Loflin lewis@bvu.net
http://www.sullivan-county.com/main.htm
Electronics website:
http://www.bristolwatch.com/index.htm 

https://www.bristolwatch.com/arduino2/ard0.htm

CHANGE TO LOW POWER MODE
*/

#include <Wire.h> // specify use of Wire.h library
#define ASD1115 0x48

unsigned int val = 0;
byte writeBuf[3];
byte buffer[3];

const float VPS = 4.096 / 32768.0; // volts per step

//#define I2C_SDA 36
//#define I2C_SCL 39

void ads1115setup()   {

  Serial.println("Initialising I2C for ADC");

  // Wire.begin(0,16); // begin I2C
  Wire.begin(); // begin I2C

  // ASD1115
  // set config register and start conversion
  // ANC1 and GND, 4.096v, 128s/s
  writeBuf[0] = 1;    // config register is 1
  writeBuf[1] = 0b11010010; // 0xC2 single shot off
  // bit 15 flag bit for single shot
  // Bits 14-12 input selection:
  // 100 ANC0; 101 ANC1; 110 ANC2; 111 ANC3
  // Bits 11-9 Amp gain. Default to 010 here 001 P19
  // Bit 8 Operational mode of the ADS1115.
  // 0 : Continuous conversion mode
  // 1 : Power-down single-shot mode (default)
  // 0b 11010010 - 0xc2

  writeBuf[2] = 0b10000101; // bits 7-0  0x85
  // Bits 7-5 data rate default to 100 for 128SPS
  // Bits 4-0  comparator functions see spec sheet.

  // setup ADS1115
  Wire.beginTransmission(ASD1115);  // ADC 
  Wire.write(writeBuf[0]); 
  Wire.write(writeBuf[1]);
  Wire.write(writeBuf[2]);  
  Wire.endTransmission();  

  delay(500);

}  // end setup

void ads1115read(){
  // Serial.println("Begin Reading ADC");
  buffer[0] = 0; // pointer
  Wire.beginTransmission(ASD1115);  // DAC
  Wire.write(buffer[0]);  // pointer
  Wire.endTransmission();

  Wire.requestFrom(ASD1115, 2);
  buffer[1] = Wire.read();  // 
  buffer[2] = Wire.read();  // 
  Wire.endTransmission();  

  // convert display results
  val = buffer[1] << 8 | buffer[2]; 

  if (val > 32768) val = 0;

//   Serial.println(val * VPS);
  Serial.println(val);

  moistsensreading=val;

  delay(1000);
  
  // just an indicator
//   digitalWrite(13, HIGH);
//   delay(500);
//   digitalWrite(13, LOW);
//   delay(500);

} // end loop