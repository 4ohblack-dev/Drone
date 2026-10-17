#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <RadioLib.h>


constexpr uint8_t CEpin = 27;
constexpr uint8_t CSNpin = 26;
constexpr uint8_t SCKpin = 18;
constexpr uint8_t MISOpin = 19;
constexpr uint8_t MOSIpin = 23;

constexpr uint8_t SDApin = 21;
constexpr uint8_t SCLpin = 22;

nRF24 nrf24 = new Module(CSNpin,CEpin,RADIOLIB_NC,RADIOLIB_NC);
Adafruit_MPU6050 mpu;

void setup(){
    Serial.begin(115200);
    SPI.begin(SCKpin,MISOpin,MOSIpin,CSNpin);

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