//Arduino Spectrum Analyzer with RGB-LED Stripe and MSGEQ7 by HeinzKetchup 
// declarations for the Neopixel by Adafruit
//https://create.arduino.cc/projecthub/HeinzKetschup/spectrum-analyzer-with-rgb-leds-eb9785?ref=similar&ref_id=43304&offset=0
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 6 // Pin for the RGB Stripe
#define NUMPIXELS 64  // Number of Pixels
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int strobePin = 3;  // Strobe Pin on the MSGEQ7
int resetPin = 4; // Reset Pin on the MSGEQ7
int outPin = A1;  // Output Pin on the MSGEQ7
int level[7]; // An array to hold the values from the 7 frequency bands
int l;
uint32_t aus =  pixels.Color(0,0,0);
uint32_t gr =  pixels.Color(0,200,0);
uint32_t grb =  pixels.Color(0,160,40);
uint32_t grbl =  pixels.Color(0,120,80);
uint32_t gbl =  pixels.Color(0,80,120);
uint32_t bl =  pixels.Color(0,40,160);
uint32_t blr =  pixels.Color(0,0,200);
uint32_t blro =  pixels.Color(40,0,160);
uint32_t bro =  pixels.Color(80,0,120);
uint32_t ro =  pixels.Color(120,0,80);
uint32_t rog =  pixels.Color(160,0,0);

void setup() 
{
  Serial.begin(9600);
  pinMode(strobePin, OUTPUT); // Define our pin modes
  pinMode(resetPin,  OUTPUT);
  pinMode(outPin, INPUT);
  pinMode(3,OUTPUT);
  digitalWrite(resetPin, LOW);  // Create an initial state for our pins
  digitalWrite(strobePin, LOW);
  delay(1);
  digitalWrite(resetPin, HIGH); // Reset the MSGEQ7 as per the datasheet timing diagram
  delay(1);
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
  delay(1); 
  pixels.begin(); // enables Adafruit Neopixels
  pixels.setBrightness(5); // Set BRIGHTNESS to about 1/5 (max = 255)
  pixels.show();  // reset Pixels
  for (int i = 0; i < 56; i++)
  {
    int ii = i-5; //snake effect at the start
    pixels.setPixelColor(i, gr);
    pixels.setPixelColor(ii, ro);
    pixels.show();
    delay(20);
  }
  for (int i = 0; i < 56; i++)
  {
    int ii = i-5; //snake effect at the start
    pixels.setPixelColor(i, blr);
    pixels.setPixelColor(ii, ro);
    pixels.show();
    delay(20);
  }
}
void LEDaus1(int a,int b,int c,int d,int e,int f,int g,int h)
{
   for (int i=a; i <=h; i++)
   {                      
    pixels.setPixelColor(i, aus);
    pixels.show();
   }
}
void LEDaus2(int a,int b,int c,int d,int e,int f,int g,int h)
{
   for (int i=a; i >=h; i--)
   {                      
    pixels.setPixelColor(i, aus);
    pixels.show();
   }
}
void LED0(int a,int b,int c,int d,int e,int f,int g,int h, uint32_t gr)
{
 pixels.setPixelColor(a, gr);
 for (int i=b; i <= h; i++) pixels.setPixelColor(i, aus);
}
void LED1(int a,int b,int c,int d,int e,int f,int g,int h)
{
 pixels.setPixelColor(a, gr);    
 pixels.setPixelColor(b, grb);
 for (int i=h; i <= c; i++) pixels.setPixelColor(i, aus);
}
void LED2(int a,int b,int c,int d,int e,int f,int g,int h)
{
 pixels.setPixelColor(a, gr);
 pixels.setPixelColor(b, grb);
 pixels.setPixelColor(c, grbl);
 for (int i=d; i <= h; i++) pixels.setPixelColor(i, aus);
}
void LED3(int a,int b,int c,int d,int e,int f,int g,int h)
{
 pixels.setPixelColor(a, gr);
 pixels.setPixelColor(b, grb);
 pixels.setPixelColor(c, grbl);
 pixels.setPixelColor(d, gbl);
 for (int i=h; i <= e; i++) pixels.setPixelColor(i, aus);
}
void LED4(int a,int b,int c,int d,int e,int f,int g,int h)
{
 pixels.setPixelColor(a, gr);
 pixels.setPixelColor(b, grb);
 pixels.setPixelColor(c, grbl);
 pixels.setPixelColor(d, gbl);
 pixels.setPixelColor(e, bl);
 for (int i=f; i <= h; i++) pixels.setPixelColor(i, aus);
}
void LED5(int a,int b,int c,int d,int e,int f,int g,int h)
{
 pixels.setPixelColor(a, gr);
 pixels.setPixelColor(b, grb);
 pixels.setPixelColor(c, grbl);
 pixels.setPixelColor(d, gbl);
 pixels.setPixelColor(e, bl);
 pixels.setPixelColor(f, blr);
 for (int i=h; i <= g; i++) pixels.setPixelColor(i, aus);
}
void LED6(int a,int b,int c,int d,int e,int f,int g,int h)
{
 pixels.setPixelColor(a, gr);
 pixels.setPixelColor(b, grb);
 pixels.setPixelColor(c, grbl);
 pixels.setPixelColor(d, gbl);
 pixels.setPixelColor(e, bl);
 pixels.setPixelColor(f, blr);
 pixels.setPixelColor(g, ro);
// pixels.setPixelColor(h, bro);
//// for (int i=h; i <= k; i++) pixels.setPixelColor(i, aus);
}
void LED7(int a,int b,int c,int d,int e,int f,int g,int h)
{
 pixels.setPixelColor(a, gr);
 pixels.setPixelColor(b, grb);
 pixels.setPixelColor(c, grbl);
 pixels.setPixelColor(d, gbl);
 pixels.setPixelColor(e, bl);
 pixels.setPixelColor(f, blr);
 pixels.setPixelColor(g, ro);
 pixels.setPixelColor(h, rog);
// for (int i=k; i <= j; i++) pixels.setPixelColor(i, aus);
}
/*void LED8(int a,int b,int c,int d,int e,int f,int g,int h,int j,int k)
{
 pixels.setPixelColor(a, gr);
 pixels.setPixelColor(b, grb);
 pixels.setPixelColor(c, grbl);
 pixels.setPixelColor(d, gbl);
 pixels.setPixelColor(e, bl);
 pixels.setPixelColor(f, blr);
 pixels.setPixelColor(g, blro);
 pixels.setPixelColor(h, bro);
 pixels.setPixelColor(j, ro);
 pixels.setPixelColor(k, aus); 
}
void LED9(int a,int b,int c,int d,int e,int f,int g,int h,int j,int k)
{
 pixels.setPixelColor(a, gr);
 pixels.setPixelColor(b, grb);
 pixels.setPixelColor(c, grbl);
 pixels.setPixelColor(d, gbl);
 pixels.setPixelColor(e, bl);
 pixels.setPixelColor(f, blr);
 pixels.setPixelColor(g, blro);
 pixels.setPixelColor(h, bro);
 pixels.setPixelColor(j, ro);
 pixels.setPixelColor(k, rog); 
}
*/
void abfolge(int a,int b,int c,int d,int e,int f,int g,int h)
{
 switch (l)
 {
  case 93 ... 104:/*<---------------------------------------first LED area--------------------------------------------------------*/
  LED0(a,b,c,d,e,f,g,h,gr);
  break;
  case 105 ... 139:/*<---------------------------------------second LED area------------------------------------------------------*/
  LED1(a,b,c,d,e,f,g,h);
  break;
  case 140 ... 164:/*<---------------------------------------third LED area-------------------------------------------------------*/     
  LED2(a,b,c,d,e,f,g,h);
  break;
  case 165 ... 199:/*<---------------------------------------fourth LED area------------------------------------------------------*/
  LED3(a,b,c,d,e,f,g,h);
  break;
  case 200 ... 234:/*<---------------------------------------fith LED area--------------------------------------------------------*/
  LED4(a,b,c,d,e,f,g,h);
  break;
  case 235 ... 269:/*<---------------------------------------sixth LED area-------------------------------------------------------*/
  LED5(a,b,c,d,e,f,g,h);
  break;
  case 270 ... 304:/*<---------------------------------------seventh LED area-----------------------------------------------------*/
  LED6(a,b,c,d,e,f,g,h);
  break;
  case 305 ... 339:/*<---------------------------------------eighth LED area------------------------------------------------------*/
  LED7(a,b,c,d,e,f,g,h);
  break;
  //case 340 ... 374:/*<---------------------------------------nineth LED area------------------------------------------------------*/
  //LED8(a,b,c,d,e,f,g,h);
 // break;
  //case 375 ... 1000:/*<---------------------------------------tenth LED area------------------------------------------------------*/  
  //LED9(a,b,c,d,e,f,g,h);
 // break;     
 }
}
void loop() 
{
 // Cycle through each frequency band by pulsing the strobe.
 for (int i = 0; i < 7; i++) 
 {
  digitalWrite       (strobePin, LOW);
  delayMicroseconds  (100);                    // Delay necessary due to timing diagram
  level[i] =         analogRead (outPin);
  digitalWrite       (strobePin, HIGH);
  delayMicroseconds  (100);                    // Delay necessary due to timing diagram  
 }
 /*-----------------------------------------------------------Band1(For the first LED stripe)---------------------------------------------------------------*/                   
 // allocation for the Numbers of LEDs
 l = level [0];
 abfolge(7,6,5,4,3,2,1,0);
 if (l < 92) LEDaus1(7,6,5,4,3,2,1,0);
 /*-----------------------------------------------------------Band2(For the second LED stripe)---------------------------------------------------------------*/                     
 // allocation for the Numbers of LEDs
 l = level [1];
 abfolge(15,14,13,12,11,10,9,8);
 if (l < 92) LEDaus2(15,14,13,12,11,10,9,8);
 /*-----------------------------------------------------------Band3(For the third LED stripe)---------------------------------------------------------------*/                    
 // allocation for the Numbers of LEDs
 l = level [2];
 abfolge(23,22,21,20,19,18,17,16);
 if (l < 92) LEDaus1(23,22,21,20,19,18,17,16);
 /*-----------------------------------------------------------Band4(For the fourth LED stripe)---------------------------------------------------------------*/                    
 // allocation for the Numbers of LEDs
 l = level [3];
 abfolge(31,30,29,28,27,26,25,24);
 if (l < 92) LEDaus2(31,30,29,28,27,26,25,24);  
 /*-----------------------------------------------------------Band5(For the fifth LED stripe)---------------------------------------------------------------*/                       
 // allocation for the Numbers of LEDs
 l = level [4];
 abfolge(39,38,37,36,35,34,33,32);
 if (l < 92) LEDaus1(39,38,37,36,35,34,33,32);   
 /*-----------------------------------------------------------Band6(For the sixth LED stripe)---------------------------------------------------------------*/                    
 // allocation for the Numbers of LEDs
 l = level [5];
 abfolge(47,46,45,44,43,42,41,40);
 if (l < 92) LEDaus2(47,46,45,44,43,42,41,40);   
 /*-----------------------------------------------------------Band7(For the seventh LED stripe)---------------------------------------------------------------*/                      
 // allocation for the Numbers of LEDs
 l = level [6];             
 abfolge(55,54,53,52,51,50,49,48);
 if (l < 92) LEDaus1(55,54,53,52,51,50,49,48);     


//l = level [7];             
 //abfolge(63,62,61,60,59,58,57,56);
 //if (l < 92) LEDaus1(63,62,61,60,59,58,57,56); 
                        
} 

