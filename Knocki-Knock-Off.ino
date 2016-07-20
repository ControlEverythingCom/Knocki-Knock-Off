// This #include statement was automatically added by the Particle IDE.
#include "SegmentDriver.h"

SegmentLEDDriver display;

#define Addr 0x50

int knocks = 0;

unsigned long knocksTimout = 1000;

bool knocking = false;

unsigned long knocksStart = 0;

unsigned long disableStart = 0;

unsigned long disableTimeout = 200;

int peakArray[4] ={0,0,0,0};

int peakArrayIndex = 0;

int threshold = 12;

//Display screen variables
unsigned long displayUpdateTime = 0;
unsigned long displayResetTime = 1500;
bool displayReset = false;

void setup() {
    Wire.begin();
    
    display.init();
}

void loop() {
    
    //Check if we need to reset the display
    if(!displayReset && millis() > displayUpdateTime + displayResetTime){
        display.displayWriteInt(0);
        displayReset = true;
    }
    
    if(millis() > knocksStart + disableTimeout){
        peakArray[peakArrayIndex] = getReading();
        int total = 0;
        for(int i = 0; i < 4; i++){
            total += peakArray[i];
        }
        if(total >= threshold){
            knocking = true;
            knocks++;
            knocksStart = millis();
            for(int i = 0; i < 4; i++){
                peakArray[i] = 0;
            }
        }
        if(peakArrayIndex == 3){
            peakArrayIndex = 0;
            
        }else{
            peakArrayIndex++;
        }
    }

    if(knocking && millis() >= knocksStart+knocksTimout){
        //Publish event and print to serial console
        char buffer[50];
        sprintf(buffer, "%i-knocks", knocks);
        Particle.publish("knocks", buffer);
        Serial.printf("Number of Knocks: %i \n", knocks);
        
        //Update display
        display.displayWriteInt(knocks);
        displayReset = false;
        displayUpdateTime = millis();
        
        //Reset Variables
        knocks = 0;
        knocking = false;
    }
}

int getReading(){
    unsigned int data[2];
    // Start I2C transmission
    Wire.beginTransmission(Addr); 
    // Calling conversion result register, 0x00(0)
    Wire.write(0x00);
    // Stop I2C transmission
    Wire.endTransmission();

    // Request 2 bytes
    Wire.requestFrom(Addr, 2);
    
    // Read 2 bytes of data, raw_adc msb, raw_adc lsb
    if(Wire.available() == 2)
    {  
        data[0] = Wire.read();
        data[1] = Wire.read();
    }
    
    // Convert the data to 12 bits
    return ((data[0] * 256) + data[1]) & 0x0FFF;
}