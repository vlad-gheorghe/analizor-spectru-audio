/*

Code Example for jolliFactory I2C OLED Display Audio spectrum Visualizer example 1.0

Please visit http://www.instructables.com/id/Arduino-Based-Mini-Audio-Spectrum-Visualizer/ for detail on this project

Adapted from the following tutorial
Example 48.2 - tronixstuff.com/tutorials > chapter 48 - 30 Jan 2013 

*/



#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

//SSD1283A display(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9, /*LED=*/ 7); //hardware spi,cs,cd,reset,led
U8G2_ST7565_NHD_C12864_F_4W_SW_SPI display(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);


int temp=0;
int old_temp=0;
int maxCount = 3;
int count = 0;


//MSGEQ7
int strobe =3; // strobe pins on digital 4
int res = 4;    // reset pins on digital 5
int left[7];    // store left audio band values in these arrays
int right[7];   // store right audio band values in these arrays
int band;
int noiseLevel = 127;  // change this value to suppress display due to noise pickup 

//Rain Effect
int falling_left[7];
int falling_right[7];


//SW
int buttonPin = 2;
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
long lastDebounceTime = 0;   // the last time the output pin was toggled
long debounceDelay = 50;     // the debounce time;



//**********************************************************************************************************************************************************
void setup()
{
  pinMode(buttonPin, INPUT_PULLUP);   // switch to select visual effects
 
  pinMode(res, OUTPUT);      // reset
  pinMode(strobe, OUTPUT);   // strobe
  
  digitalWrite(res,LOW); 
  digitalWrite(strobe,HIGH); 
 
  // Set up the OLED display
  //display.begin(SH1106_SWITCHCAPVCC, 0x3C); 
  
 // display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Initialize with the I2C addr 0x3C if not working use 0x3D (for the 128x64)
  //display.init();
  //display.setTextColor(WHITE);
 
  //display.fillScreen(BLACK);
 display.begin(); 
}



//**********************************************************************************************************************************************************
void readMSGEQ7()
// Function to read 7 band equalizers
{
  digitalWrite(res, HIGH);
  digitalWrite(res, LOW);

  for( band = 0; band < 7; band++ )
  {
    digitalWrite(strobe,LOW); // strobe pin on the shield - kicks the IC up to the next band 
    delayMicroseconds(30); // 
    left[band] = analogRead(A0); // store left band reading
    right[band] = analogRead(A1); // ... and the right
    digitalWrite(strobe,HIGH); 
  }
}



//**********************************************************************************************************************************************************
 void loop()
{
  checkButton();

  
display.clearBuffer();
 
  readMSGEQ7();
  
  //*********************************************************************
  // display values of left channel
  for( band = 0; band < 7; band++ )
  {
    if (count == 1)  //Histogram Effect
    {    
      if (left[band] < noiseLevel)   
        left[band] = 0;

      temp = map(left[band], 0, 1023, 0, 55);
  display.drawBox(band*9, 64-temp, 8, temp);
    }
    else if (count == 2) //Mirror Effect
    {
      if (left[band] < noiseLevel)   
        left[band] = 0;

      temp = map(left[band], 0, 1023, 0, 55);
      display.drawBox(band*9, 8+(64-temp)/2, 8, temp);
    }
    else   //Histogram cum Rain Effect
    {
      if (left[band] < noiseLevel)   
        left[band] = 0;

      if (falling_left[band] > left[band])
        falling_left[band]=falling_left[band]-20;
      else
        falling_left[band] = left[band]+100;

      if (falling_left[band] < noiseLevel)   
        falling_left[band] = 0;

      temp = map(left[band], 0, 1023, 0, 55);
      display.drawBox(band*9, 64-temp, 8, temp);
display.drawBox(band*9, 64-temp, 8, temp);
      temp = map(falling_left[band], 0, 1023, 0, 55);
    
      display.drawBox(band*9, 64-temp, 8, 2);
    }  
  }
 
 
  //*********************************************************************
  // display values of right channel
  for( band = 0; band < 7; band++ )
  {
    if (count == 1)  //Histogram Effect
    {
      if (right[band] < noiseLevel)   
        right[band] = 0;

      temp = map(right[band], 0, 1023, 0, 55);
      display.drawBox(64+(6-band)*9, 64-temp, 8, temp);
 
    }
    else if (count == 2) //Mirror Effect
    {
      if (right[band] < noiseLevel)   
        right[band] = 0;

      temp = map(right[band], 0, 1023, 0, 55);
   
       display.drawBox(64+(6-band)*9, 8+(64-temp)/2, 8, temp);
    }
    else   //Histogram cum Rain Effect
    {
      if (right[band] < noiseLevel)   
        right[band] = 0;

      if (falling_right[band] > right[band])
        falling_right[band]=falling_right[band]-20;
      else
        falling_right[band] = right[band]+100;

      if (falling_right[band] < noiseLevel)   
        falling_right[band] = 0;

      temp = map(right[band], 0, 1023, 0, 55);   
       display.drawBox(64+(6-band)*9, 64-temp, 8, temp);

      temp = map(falling_right[band], 0, 1023, 0, 55);      
     display.drawBox(64+(6-band)*9, 64-temp, 8, 2); 
  
    }
  }
  //display.drawBox(2, 10, 50, 40);
 display.sendBuffer();
//  display.display(); 
}



//**********************************************************************************************************************************************************
void checkButton()
{
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from HIGH to LOW),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) 
  {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) 
    {
      buttonState = reading;

      // only perform task if the new button state is LOW
      if (buttonState == LOW) 
      {        
        count = count + 1;

        if (count > maxCount)
          count = 1;   //reset count
      }
    }
  }

  lastButtonState = reading;  
}  
/*void loop(void) {
  // picture loop  
  display.clearBuffer();
 // draw();
 display.drawBox(2, 10, 50, 40);
  display.sendBuffer();
  
  // increase the state
  //draw_state++;
 // if ( draw_state >= 12*8 )
  //  draw_state = 0;

  // deley between each page
  delay(100);

}
*/
