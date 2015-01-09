#include <Adafruit_NeoPixel.h>

#define SW_BRIGHTNESS  255    //0-255
#define SW_BRIGHTNESS_low 20

#define PIXEL_PIN    3
#define PIXEL_COUNT 60
#define BRIGHTNESS  25      //0-255  

#define FADE_SPAWN_RATE 10  // How many fade cycles, before drawing something new. Default: 20
#define FADE_SPEED 5        // How long it takes to fade out. Default: 5


#define MAX_MODES 7

uint8_t SWPwrBl = 4;
uint8_t SWModeGr = 8;

uint8_t SWPwrBlLED = 5;
uint8_t SWModeGrLED = 6; 

uint8_t mode = 0;

// Define colors
                    //    Red, Grn, Blu, Ylw, Wht
uint8_t userRed[6] = { 0, 255,   0,   0, 200, 255 };
uint8_t userGrn[6] = { 0,   0, 255,   0, 150, 255 };
uint8_t userBlu[6] = { 0,   0,   0, 255,   0, 255 };


uint8_t valRed[PIXEL_COUNT];
uint8_t valGrn[PIXEL_COUNT];
uint8_t valBlu[PIXEL_COUNT];


Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  randomSeed(analogRead(0));
  
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
         PixelsSolidSet(200,150,0);  // Yellow
         break;
      case 4:
         PixelsSolidSet(255,255,255); // White
         break;
      case 5:
          PixelRandom();
          break;
      case 6:
          PixelRandomFade();
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


// Functions

void PixelRandom() {
    uint8_t num = random(PIXEL_COUNT);
    uint8_t col = random(5) + 1;
    
    strip.setPixelColor(num, userRed[col], userGrn[col], userBlu[col]);
    strip.show();
    delay(10);
}

void PixelRandomFade(){
    uint8_t num = random(PIXEL_COUNT);
    uint8_t col = random(5) + 1;
  
    valRed[num] = userRed[col];
    valGrn[num] = userGrn[col];
    valBlu[num] = userBlu[col];
 
   FadeDimPixels(FADE_SPAWN_RATE);
}


void FadeDraw(){
    uint32_t tempColor;
    for(uint8_t i = 0; i < PIXEL_COUNT; i++) {
      tempColor = strip.Color(valRed[i], valGrn[i], valBlu[i]);
      strip.setPixelColor(i, tempColor);
    }
    strip.show();  
}


// dims pixels, but doesn't account for colors like yellow, which fade to red(200), once green(150) runs out.
void FadeDimPixels(uint8_t count) {
  for(uint8_t num = 0; num < count; num++) {
    for(uint8_t i = 0; i < PIXEL_COUNT; i++) {
      if(valRed[i] > 0) { valRed[i]--; }
      if(valGrn[i] > 0) { valGrn[i]--; }
      if(valBlu[i] > 0) { valBlu[i]--; }
    } 
    FadeDraw();
    delay(FADE_SPEED);
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
