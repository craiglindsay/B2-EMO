#include <Adafruit_NeoPixel.h>
//#include <Adafruit_AW9523.h>
#define PINR 4    // Right
#define NUM_LEDS 120
// ------------------------------------------
int fun;          // variable "fun" is the serial command received from the mega

//  LEDs strips triggered by serial command like Serial3.print("0") sent from Mega to Seeed

//  0 = Eye LED (nOOd) off
//  1 = Eye LED (nOOd) on  - formerly Surfin Safari - lights blue
//  2 = Misirlou - theater chase rainbow
//  3 = Surfing USA - Red, White Blue
//  4 = Pipeline - blue fast chase
//  5 = GhostRiders - Fade in out blue
//  6 = Reaper - Rainbow faster
//  7 = Grim Grinning - Purple fade in out
//  8 = Stranger Things - Red then Sparkle out
//  9 = Thriller  / Red wipe, black faster wipe
//  A = Ghostbusters / Circling Blue / White
//  B = I want Candy - Rainbow
//  C = Meteor rain
//  D = Rainbow chase
//  E = Blue chase
//  F = Color wipe
//  G = Running lights
//  H = Fade in/out
//  I = Snow sparkle
//  J = Cylon bounce
//  K = New kitt
//  L = Cirq
//  M = Rainbow
//  N = Rainbow faster
//  O = Blue cylon
//  P = White fast chase
//  Q = Snow sparkle blue
//  R = Red fast Chase
//  S = 
//  T = 
//  U = 
//  V = 
//  W = 
//  X = 
//  Y = 
//  Z = 
// ------------------------------------------
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(120, PINR, NEO_GRB + NEO_KHZ800);// #define 120 LEDs for strip

// Here is where you can put in your favorite colors that will appear!
// just add new {nnn, nnn, nnn}, lines. They will be picked out randomly
//                          R   G   B
uint8_t myFire[][3] = {{232, 100, 255},   // purple
                        {200, 200, 20},   // yellow 
                        {30, 200, 200},   // blue
                        {237,159,176},    // pink
                        {255, 0, 0},      // red
                        {108, 87,229},     // Dark blue
                        {162,211,172},     // Lt Green
                          };

uint8_t myColors[][3] = {{232, 100, 255},   // purple
                        {200, 200, 20},   // yellow 
                        {30, 0, 0},       // light red
                        {237,100,100},    // some red
                        {255, 0, 0},      // red
                        {150, 20,20},     // Med red
                        {200,5,5},        // Bright red
                          };

#define FAVCOLORS sizeof(myColors) / 3
#define FIRECOLORS sizeof(myFire) / 3

/*
RX is digital pin 2 (connect to TX of other device)
TX is digital pin 3 (connect to RX of other device)
*/
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX

void setup() {
  fun = 0;
  mySerial.begin(115200);     // Hardware serial for receiving from R2's mega
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(4, LOW);       // Set pin 4 as ground
  strip.begin();  // Right foot initilization
  strip.show();   // Initialize all pixels to 'off'
  }
 
void loop() {
  handleSerial();   // Reads serial input looking for commands from the Mega
 
     if (fun == 0 ) // Turn all light off
     {
     digitalWrite(6, HIGH);       // nOOd off
     //strip.clear(); 
     //strip.show();
     fun = 100;
     }
     if (fun == 1 ) // Surfin Safari
     { 
     digitalWrite(6, LOW);       // nOOd on
     //strip.clear(); 
     //strip.fill( strip.Color(0, 0, 128),0,120); 
	 //strip.show();
	 fun = 100;
     }
     if (fun == 2 ) // Misirlou
     {
     theaterChaseRainbow(35);
     }
    if (fun == 3 ) // Surfing USA - Red, White Blue
     {
	 FadeInOut(255, 0, 0); // red
	 FadeInOut(255, 255, 255); // white
	 FadeInOut(0, 0, 255); // blue
     }
     if (fun == 4 ) // Pipeline 
     {
     theaterChase(0,0,128,35);  // Blue fast Chase
     }    
     if (fun == 5 ) // GhostRiders - Fade in out blue
     {
     FadeInOut(0, 0, 255); // blue
     }
     if (fun == 6 ) // Reaper - Rainbow faster
     {
     rainbowCycle(1); 
     }
     if (fun == 7 ) // Grim Grinning - Purple fade in out
     {
	 FadeInOut(133, 000, 204); // blue
     }
     if (fun == 8 ) // Stranger Things - Red then Sparkle out
     {
     strip.fill( strip.Color(255, 0, 0),0,120); // Turn Red
     strip.show();
     fun = 37;   // send to sparkle routine
     }
     if (fun == 9 ) // Thriller  / Red wipe, black faster wipe
     {
	 colorWipe(255,0,0, 9);
     colorWipe(0,0,0, 1);
     }      
     if (fun == 10 ) // Ghostbusters / Circling Blue / White
     { 
     CirqBW(0xff, 0, 0, 8, 10,90);
     }
     if (fun == 11 ) // I want Candy - Rainbow
     {
     rainbowCycle(1); 
     }
     if (fun == 12 ) //meteor Rain
     {
     meteorRain(255,255,255,10, 64, true, 30);
     }
     if (fun == 13 ) // Rainbow Chase
     {
	 theaterChaseRainbow(35);
     }
	 if (fun == 14 ) 
     {
     theaterChase(255,0,0,40);  // regular chase
     }
	 if (fun == 15 )  // color wipe
     {
	 colorWipe(0x00,0x00,0xff, 25);
     colorWipe(0x00,0x00,0x00, 1);
     } 
	 if (fun == 16 ) //running lights
     {
 	 RunningLights(0xff,0,0, 40);        // red
     RunningLights(0xff,0xff,0xff, 40);  // white
     RunningLights(0,0,0xff, 40);        // blue
	 }
	 if (fun == 17 ) //fade in/out red,white,blue
     {
	 FadeInOut(0xff, 0x00, 0x00); // red
	 FadeInOut(0xff, 0xff, 0xff); // white
	 FadeInOut(0x00, 0x00, 0xff); // blue
     }
	 if (fun == 18 )   // snow sparkle
     {
	 SnowSparkle(0x05, 0x05, 0x05, 20, 200);
     }
     if (fun == 19 )   // cylon bounce red
     {
     CylonBounce(0xff, 0, 0, 4, 10, 50);
     }
     if (fun == 20 ) // New kitt
     {
     NewKITT(255, 0, 0, 12, 5, 20);
     }
     if (fun == 21 ) // Cirq
     {
     Cirq(0xff, 0, 0, 8, 10, 50);
     }
	 if (fun == 22 ) // Rainbow
     {
	 rainbowCycle(1); 
     }
	 if (fun == 23 ) // Rainbow faster
     {
	 rainbowCycle(1);
     }
	 if (fun == 24 ) // Cylon bounce blue
     {
	 CylonBounce(0, 0, 255, 4, 10, 50);  
     }
	 if (fun == 25 ) // white fast Chase
     {
	 theaterChase(128,128,128,35);  
     }
	 if (fun == 26 ) // snow sparkle blue
     {
	 SnowSparkle(0, 0, 0x05, 20, 200); 
     }
	 if (fun == 27 ) // Red fast Chase
     {
	 theaterChase(128,0,0,35);
     }
	 if (fun == 28 ) 
     {
     }
	 if (fun == 29 ) 
     {
     }
	 if (fun == 30 ) 
     {
     }	 
	 if (fun == 31 ) 
     {
     }	 
	 if (fun == 32 ) 
     {
     }	 
	 if (fun == 33 ) 
     {
     }	 
	 if (fun == 34 ) 
     {
     }	 
	 if (fun == 35 ) 
     {
     }	 
	 if (fun == 36 ) 
     {
     }	 
	 if (fun == 37) // Sparkle out
	 {
	 flashRandom(0, 1);  // First number is 'wait' delay, shorter num == shorter twinkle
                        // Second number is how many neopixels to simultaneously light up 
	 }
	

  } // end loop

void handleSerial() {
 while (mySerial.available() > 0) {
   char incomingCharacter = mySerial.read();
   switch (incomingCharacter) {
  case '0': // off
    fun = 0;
    break;
  case '1':  
    fun = 1;
    break;
  case '2':  
    fun = 2;
    break;
  case '3': 
    fun = 3; 
    break;
  case '4': 
    fun = 4;
    break;
  case '5': 
    fun = 5; 
    break;
  case '6': 
    fun = 6; 
    break;
  case '7': 
    fun = 7;
    break;
  case '8': 
    fun = 8;
    break;
  case '9': 
    fun = 9;
    break;
  case 'A': 
    fun = 10;
    break;
  case 'B': 
    fun = 11;
    break;
  case 'C': 
    fun = 12;
    break;
  case 'D': 
    fun = 13;
    break;
  case 'E':
    fun = 14;
    break;
  case 'F': 
    fun = 15;
    break;
  case 'G': 
    fun = 16;
    break;
  case 'H': 
    fun = 17;
    break;
  case 'I': 
    fun = 18;
    break;
  case 'J': 
    fun = 19;
    break;
  case 'K': 
    fun = 20;
    break;
  case 'L': 
    fun = 21;
    break;
  case 'M': 
    fun = 22;
    break;
  case 'N': 
    fun = 23;
    break;
  case 'O': 
    fun = 24;
    break;
  case 'P': 
    fun = 25;
    break;
  case 'Q': 
    fun = 26;
    break;
  case 'R': 
    fun = 27;
    break;
  case 'S': 
    fun = 28;
    break;
  case 'T': 
    fun = 29;
    break;
  case 'U': 
    fun = 30;
    break;
  case 'V': 
    fun = 31;
    break;
  case 'W': 
    fun = 32;
    break;
  case 'X': 
    fun = 33;
    break;
  case 'Y': 
    fun = 34;
    break;
  case 'Z': 
    fun = 35;
    break;
	
  }
 }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
     strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*1; j++) { // 1 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
    strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
  }
}

//Random flashing to off
void flashRandom(int wait, uint8_t howmany) {
  for(uint16_t i=0; i<howmany; i++) {
    // pick a random favorite color!
    int c = random(FAVCOLORS);
    int red = myColors[c][0];
    int green = myColors[c][1];
    int blue = myColors[c][2]; 
 
    // get a random pixel from the list
    int j = random(strip.numPixels());
    
    // now we will 'fade' it in 3 steps
    for (int x=0; x < 3; x++) {
      int r = red * (x+1); r /= 5;
      int g = green * (x+1); g /= 5;
      int b = blue * (x+1); b /= 5;
      strip.setPixelColor(j, strip.Color(r, g, b));
      strip.show();
    }
    // & fade out in 3 steps
    for (int x=3; x >= 0; x--) {
      int r = red * x; r /= 5;
      int g = green * x; g /= 5;
      int b = blue * x; b /= 5;
      strip.setPixelColor(j, strip.Color(r, g, b));
      strip.show();

    }
  }
}

void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
  setAll(0,0,0);
 
  for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
   
   
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
   
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
        setPixel(i-j, red, green, blue);
      }
    }
   
    showStrip();
    delay(SpeedDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
    oldColor = strip.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);
    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
    strip.setPixelColor(ledNo, r,g,b);
}


void theaterChaseRainbow(int SpeedDelay) {
  byte *c;
 
  for (int j=0; j < 256; j=j+3) {     // cycle all 256 colors in the wheel by 3
    for (int q=0; q < 3; q++) {
        for (int i=0; i < NUM_LEDS; i=i+3) {
          c = Wheel2( (i+j) % 255);
          setPixel(i+q, *c, *(c+1), *(c+2));    //turn every third pixel on
        }
        showStrip();
        delay(SpeedDelay);
        for (int i=0; i < NUM_LEDS; i=i+3) {
          setPixel(i+q, 0,0,0);        //turn every third pixel off
        }
    }
  }
}

byte * Wheel2(byte WheelPos) {
  static byte c[3];
 
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}


void theaterChase(byte red, byte green, byte blue, int SpeedDelay) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < NUM_LEDS; i=i+3) {
        setPixel(i+q, red, green, blue);    //turn every third pixel on
      }
      showStrip();
     
      delay(SpeedDelay);
     
      for (int i=0; i < NUM_LEDS; i=i+3) {
        setPixel(i+q, 0,0,0);        //turn every third pixel off
      }
    }
  }
}

void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for(uint16_t i=0; i<NUM_LEDS; i++) {
      setPixel(i, red, green, blue);
      showStrip();
      delay(SpeedDelay);
  }
}


void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position=0;
 
  for(int j=0; j<NUM_LEDS*2; j++)
  {
      Position++; // = 0; //Position + Rate;
      for(int i=0; i<NUM_LEDS; i++) {
        // sine wave, 3 offset waves make a rainbow!
        //float level = sin(i+Position) * 127 + 128;
        //setPixel(i,level,0,0);
        //float level = sin(i+Position) * 127 + 128;
        setPixel(i,((sin(i+Position) * 127 + 128)/255)*red,
                   ((sin(i+Position) * 127 + 128)/255)*green,
                   ((sin(i+Position) * 127 + 128)/255)*blue);
      }
     
      showStrip();
      delay(WaveDelay);
  }
}


void FadeInOut(byte red, byte green, byte blue){
  float r, g, b;
     
  for(int k = 0; k < 256; k=k+3) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
     
  for(int k = 255; k >= 0; k=k-5) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
}

void SnowSparkle(byte red, byte green, byte blue, int SparkleDelay, int SpeedDelay) {
  setAll(red,green,blue);
 
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,0xff,0xff,0xff);
  showStrip();
  delay(SparkleDelay);
  setPixel(Pixel,red,green,blue);
  showStrip();
  delay(SpeedDelay);
}

void showStrip() {
  strip.show();
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){

  for(int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for(int i = NUM_LEDS-EyeSize-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
 
  delay(ReturnDelay);
}

void NewKITT(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){
  RightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  LeftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  OutsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  CenterToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  LeftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  RightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  OutsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  CenterToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
}

void Cirq(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){
  RightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
}

void CirqBW(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){
  RightToLeft(0, 0, 255, EyeSize, SpeedDelay, ReturnDelay);
  RightToLeft(255, 255, 255, EyeSize, SpeedDelay, ReturnDelay);
}

void CenterToOutside(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i =((NUM_LEDS-EyeSize)/2); i>=0; i--) {
    setAll(0,0,0);
   
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
   
    setPixel(NUM_LEDS-i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(NUM_LEDS-i-j, red, green, blue);
    }
    setPixel(NUM_LEDS-i-EyeSize-1, red/10, green/10, blue/10);
   
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void OutsideToCenter(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i = 0; i<=((NUM_LEDS-EyeSize)/2); i++) {
    setAll(0,0,0);
   
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
   
    setPixel(NUM_LEDS-i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(NUM_LEDS-i-j, red, green, blue);
    }
    setPixel(NUM_LEDS-i-EyeSize-1, red/10, green/10, blue/10);
   
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void LeftToRight(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void RightToLeft(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i = NUM_LEDS-EyeSize-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
