#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <RadioLib.h>
#include<ControlPacket.h>

constexpr uint8_t CEpin = 27;
constexpr uint8_t CSNpin = 26;
constexpr uint8_t SCKpin = 18;
constexpr uint8_t MISOpin = 19;
constexpr uint8_t MOSIpin = 23;

constexpr uint8_t SDApin = 21;
constexpr uint8_t SCLpin = 22;

constexpr uint8_t UART_RX = 4;
constexpr uint8_t UART_TX = 5;
constexpr uint32_t serialBaud = 115200;
HardwareSerial espLink(1);
uint32_t lastSendTime = 0;
ControlPacket txPacket;

nRF24 nrf24 = new Module(CSNpin,CEpin,RADIOLIB_NC,RADIOLIB_NC);
Adafruit_MPU6050 mpu;

void setup(){
    Serial.begin(serialBaud);
    SPI.begin(SCKpin,MISOpin,MOSIpin,CSNpin);
    espLink.begin(serialBaud,SERIAL_8N1,UART_RX,UART_TX);

    int state = nrf24.begin();
    if(state != RADIOLIB_ERR_NONE){
        Serial.print("Radio init failed");
        Serial.println(state);
        while(true) delay(100);
    }
    Serial.println("RX ready");

    Wire.begin(SDApin,SCLpin);
    if(!mpu.begin()){
        Serial.println("mpu6050 not found");
        delay(1000);
    }
    mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.println("MPU6050 ready.");
}

void loop(){

}