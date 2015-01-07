#include <Adafruit_NeoPixel.h>

#define SW_BRIGHTNESS  255    //0-255
#define SW_BRIGHTNESS_low 20

#define PIXEL_PIN    3
#define PIXEL_COUNT 60
#define BRIGHTNESS  25      //0-255  

#define MAX_MODES 4

int SWPwrBl = 4;
int SWModeGr = 8;

int SWPwrBlLED = 5;
int SWModeGrLED = 6; 

int mode = 0;


Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  pinMode(SWPwrBlLED, OUTPUT);
  pinMode(SWModeGrLED, OUTPUT);

  pinMode(SWPwrBl, INPUT_PULLUP);
  pinMode(SWModeGr, INPUT_PULLUP);  

  strip.begin();
  strip.show();
  strip.setBrightness(BRIGHTNESS);
}


void loop() {
  
  if (digitalRead(SWPwrBl) == LOW)    
  {
    analogWrite(SWPwrBlLED, SW_BRIGHTNESS); 


    // LED Mode change
    if (digitalRead(SWModeGr) == LOW) 
    {
      analogWrite(SWModeGrLED, SW_BRIGHTNESS); 
      mode ++;
      if (mode >= MAX_MODES) {mode = 0;}
      delay(250);
    }
    else
    {
      analogWrite(SWModeGrLED, SW_BRIGHTNESS_low);
    }


    switch (mode)
    {
      case 0:
        PixelsSolidSet(255,0,0);
        break;
      case 1:
        PixelsSolidSet(0,255,0);
        break;
      case 2:
         PixelsSolidSet(0,0,255);
         break;
      case 3:
         PixelsSolidSet(255,255,255);
         break;
      default:
         PixelsSolidSet(50,50,50);
    }





  }
  else  // Power is off
  {
    analogWrite(SWPwrBlLED, SW_BRIGHTNESS_low); 
    analogWrite(SWModeGrLED, 0);
    PixelsSolidSet(0,0,0);
  }
  
}


void PixelsSolidSet(uint8_t r, uint8_t g, uint8_t b){
    // Flash Black
    for(int i = 0; i < PIXEL_COUNT; i++) 
    {
      strip.setPixelColor(i, r,g,b);
    }
      strip.show();
}
