#include "SegmentDriver.h"
#include "spark_wiring_usbserial.h"
#include "spark_wiring_i2c.h"
#include "spark_wiring_constants.h"
#include "spark_wiring.h"

bool SegmentLEDDriver::init(){
    Wire.begin();
    Wire.beginTransmission(0);
    byte status = Wire.endTransmission();
    if(status !=0){
        Serial.println("Controller did not respond");
        return false;
    }else{
        Serial.println("Controller responded");
        delay(10);
        if(!regWrite(0x09,0x07)){
            return false;
        }
        delay(10);
        if(!regWrite(0x0A,0xFF)){
            return false;
        }
        delay(10);
        if(!regWrite(0x0C,0x01)){
            return false;
        }
        delay(10);
        if(!regWrite(0x0E,0x00)){
            return false;
        }
        delay(10);
        if(!regWrite(0x0B,0x03)){
            return false;
        }
        Serial.println("Setup complete");
        return true;
    }
}

bool SegmentLEDDriver::displayWriteInt(int val){
    String valString = String(val);
    int len = valString.length();
    delay(10);
    //Set characters
    for(int i = 0; i < len; i++){
        char a = valString.charAt(i);
        String aString = String(a);
        int n = aString.toInt();
        if(!regWrite(i+1, n)){
            return false;
        }
    }
    //Turn off unused characters
    for(int i = len; i < 3; i++){
        if(!regWrite(i+1, 15)){
            return false;
        }
        delay(10);
    }
}

bool SegmentLEDDriver::displayWriteInt(String val){
    int len = val.length();
    
    //Set characters
    for(int i = 0; i < len; i++){
        char a = val.charAt(i);
        String aString = String(a);
        int n = aString.toInt();
        if(!regWrite(i+1, n)){
            return false;
        }
        delay(10);
    }
    //Turn off unused characters
    for(int i = len; i < 3; i++){
        if(!regWrite(i+1, 15)){
            return false;
        }
        delay(10);
    }
}

bool SegmentLEDDriver::regWrite(int reg, int val){
    Wire.beginTransmission(0);
    Wire.write(reg);
    Wire.write(val);
    byte status = Wire.endTransmission();
    if(status != 0){
        return false;
    }
    return true;
}
