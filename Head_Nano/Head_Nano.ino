#include <Adafruit_NeoPixel.h>
#define PINE 2     // Eye
#define PINL 3     // Logic
#define PINF8 4    // Front 8 segment riser
#define PINF3 5    // Front 3 segment riser
#define PINB3 6    // Back 3 segment riser
#define NUM_LEDS 32
// ------------------------------------------
int fun;          // variable "fun" is the function to run, set by serial command received
int loopCount;
int f3flash;
int r3flash;
boolean flash =0 ;
#define loopNum 32767

//  LEDs strips triggered by serial command like Serial3.print("0")

//  0 = All LEDs off
//  1 = Light Up blue
//  2 = Theater chase rainbow
//  3 = Red, White Blue
//  4 = Blue fast chase
//  5 = Fade in out blue
//  6 = Reaper - Rainbow faster
//  7 = Purple fade in out
//  8 = Red then Sparkle out
//  9 = Red wipe, black faster wipe
//  A = Circling Blue / White
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
//  M = All pixel Rainbow
//  N = All pixels Rainbow faster
//  O = Blue cylon
//  P = White fast chase
//  Q = Snow sparkle blue
//  R = Red fast Chase
//  S = Eye Ridge Red
//  T = Eye Ridge Blue
//  U = Eye Ridge White
//  V = Eye Red
//  W = Red - Ridge Black
//  X = Eye Orange
//  Y = Eye Off
//
//  Z = 7v battery1 <10%
//  a = 7v battery1 100%
//  b = 7v battery1 90%
//  c = 7v battery1 80%
//  d = 7v battery1 70%
//  e = 7v battery1 60%
//  f = 7v battery1 50%
//  g = 7v battery1 40%
//  h = 7v battery1 30%
//  i = 7v battery1 20%
//  j = 7v battery1 10%
//
//  k = 24v battery2 100%
//  l = 24v battery2 90%
//  m = 24v battery2 80%
//  n = 24v battery2 70%
//  o = 24v battery2 60%
//  p = 24v battery2 50%
//  q = 24v battery2 40%
//  r = 24v battery2 30%
//  s = 24v battery2 20%
//  t = 24v battery2 10%
//  u = 24v battery2 <10%
//
//  v = Logic kkkk
//  w = Logic rrbk
//  x = Logic kgbk
//  y = Logic kgbk
//  z = Logic rrbb 
//  ! = toggel front3/back3 flashing routine
//
// ------------------------------------------
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, PINE, NEO_GRB + NEO_KHZ800);// #define 32 LEDs for strip
Adafruit_NeoPixel stripLogic = Adafruit_NeoPixel(19, PINL, NEO_GRB + NEO_KHZ800);// #define 16 LEDs for strip
Adafruit_NeoPixel stripFront8 = Adafruit_NeoPixel(40, PINF8, NEO_GRB + NEO_KHZ800);// #define 40 LEDs for strip
Adafruit_NeoPixel stripFront3 = Adafruit_NeoPixel(15, PINF3, NEO_GRB + NEO_KHZ800);// #define 15 LEDs for strip
Adafruit_NeoPixel stripBack3 = Adafruit_NeoPixel(15, PINB3, NEO_GRB + NEO_KHZ800);// #define 15 LEDs for strip

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

void setup() {
  fun = 0;
  Serial.begin(115200);     // Hardware serial for receiving from B2's mega
  strip.begin();  // Eye initilization
  stripLogic.begin();  // Logic initilization
  stripFront3.begin();  // Front 3 Segment Riser
  stripFront8.begin();  // Front 8 Segment Riser
  stripBack3.begin();  // Back 3 segment riser
  }
 
void loop() {
     handleSerial();   // Reads serial input looking for commands from the Mega
     if (loopCount >= loopNum) {
     if (flash==1) {flashF3();}
	   if (flash==1) {flashR3();}
     loopCount = 0; }
     loopCount++;
     if (fun == 0 ) // Turn all light off
     {
     strip.clear(); 
     strip.show();
     stripLogic.clear(); 
     stripLogic.show();
     stripFront8.clear(); 
     stripFront8.show();
     stripFront3.clear(); 
     stripFront3.show();
     stripBack3.clear(); 
     stripBack3.show();
     flash = 0;
     fun = 100;
     }
     if (fun == 1 ) // Surfin Safari
     { 
     strip.clear(); 
     strip.fill(strip.Color(128, 0, 0),0,32); 
	   strip.show();
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
     fun = 99;   // send to sparkle routine
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
     strip.fill(strip.Color(0, 0, 0),0,32); // Turn Black
     strip.fill(strip.Color(128, 0, 0),1,5); // Turn ridge Red
     strip.show();
     fun = 100;      
     }
	 if (fun == 29 ) 
     {
     strip.fill(strip.Color(0, 0, 0),0,32); // Turn Black
     strip.fill(strip.Color(0, 0, 128),1,5); // Turn ridge Blue
     strip.show();
     fun = 100;  
     }
	 if (fun == 30 ) 
     {
     strip.fill(strip.Color(0, 0, 0),0,32); // Turn Black
     strip.fill(strip.Color(128, 128, 128),1,5); // Turn Ridge White
     strip.show();
     fun = 100;  
     }	 
	 if (fun == 31 ) 
     {
     strip.fill(strip.Color(255, 0, 0),0,32); // Turn Bright Red
     strip.show();
     fun = 100;  
     }	 
	 if (fun == 32 ) 
     {
     strip.fill(strip.Color(255, 0, 0),0,32); // Turn Red
     strip.fill(strip.Color(0, 0, 0),0,7); // Turn ridge Black
     strip.show();
     fun = 100;  
     }	 
	 if (fun == 34 ) 
     {
     strip.fill(strip.Color(0, 0, 0),0,32); // Turn OFF
     strip.show();
     fun = 100;  
     }	 
	 if (fun == 33 ) 
     {    
     strip.fill(strip.Color(96, 64, 0),0,32); // Turn Orangish
     strip.show();
     fun = 100;  
     }	 
	 if (fun == 35 )  // a bat1 <10%
     {
     stripFront8.fill(stripFront8.Color(0, 0, 0),0,20); // Turn Black
     stripFront8.show();
	   fun = 100;
     }	 
	 if (fun == 36 ) // a Bat1 100%
     {
 	   stripFront8.fill(stripFront8.Color(0, 128, 0),0,20); // Turn Green
     stripFront8.show();
	   fun = 100;
     }
 	 if (fun == 37 ) // b Bat1 90%
     {
	   stripFront8.fill(stripFront8.Color(0, 128, 0),0,15);    // Turn Green
     stripFront8.fill(stripFront8.Color(128, 128, 0),15,5); // Turn Yellow
     stripFront8.show();
	   fun = 100;
     }	 
 	  if (fun == 38 ) // c Bat1 80% 
     {
	   stripFront8.fill(stripFront8.Color(0, 128, 0),0,10);     // Turn Green
     stripFront8.fill(stripFront8.Color(128, 128, 0),10,10); // Turn Yellow
	   stripFront8.show();
	   fun = 100;
     }	 
 	 if (fun == 39 ) // d Bat1 70%
     {
	   stripFront8.fill(stripFront8.Color(0, 128, 0),0,5);    // Turn Green
     stripFront8.fill(stripFront8.Color(128, 128, 0),5,15); // Turn Yellow
	   stripFront8.show();
	   fun = 100;
     }	 
 	 if (fun == 40 ) // e Bat1 60%
     {
	   stripFront8.fill(stripFront8.Color(128, 128, 0),0,20); // Turn Yellow
	   stripFront8.show();
	   fun = 100;
     }	 
 	 if (fun == 41 ) // f Bat1 50%
     {
	   stripFront8.fill(stripFront8.Color(128, 128, 0),0,15); // Turn Yellow
     stripFront8.fill(stripFront8.Color(128, 0, 0),15,5);  // Turn Red
	   stripFront8.show();
	   fun = 100;
     }	 
 	 if (fun == 42 ) // g Bat1 40%
     {
	   stripFront8.fill(stripFront8.Color(128, 128, 0),0,10); // Turn Yellow
     stripFront8.fill(stripFront8.Color(128, 0, 0),10,10);  // Turn Red
	   stripFront8.show();
	   fun = 100;
     }	 
 	 if (fun == 43 ) // h Bat1 30%
     {
	   stripFront8.fill(stripFront8.Color(128, 0, 0),0,20);  // Turn Red
	   stripFront8.show();
	   fun = 100;
     }	 
 	 if (fun == 44 ) // i Bat1 20%
     {
     stripFront8.fill(stripFront8.Color(128, 0, 0),0,10);  // Turn Red
     stripFront8.fill(stripFront8.Color(0, 0, 0),10,10);  // Turn Black
 	   stripFront8.show();
	  fun = 100;
     }	 
 	 if (fun == 45 ) // j Bat1 10%
     {
     stripFront8.fill(stripFront8.Color(128, 0, 0),0,5);  // Turn Red
     stripFront8.fill(stripFront8.Color(0, 0, 0),5,15);  // Turn Black
     stripFront8.show();
	   fun = 100;
     }	 
 	 if (fun == 46 ) // k Bat2 100%
     {
	   stripFront8.fill(stripFront8.Color(0, 128, 0),20,20); // Turn Green
	   stripFront8.show();
	   fun = 100;
	   }	 
 	 if (fun == 47 ) //l Bat2 90%
     {
	   stripFront8.fill(stripFront8.Color(0, 128, 0),20,15);    // Turn Green
     stripFront8.fill(stripFront8.Color(128, 128, 0),35,5); // Turn Yellow
	   stripFront8.show();
	   fun = 100;
     }	 
 	 if (fun == 48 ) // m Bat2 80%
     {
	   stripFront8.fill(stripFront8.Color(0, 128, 0),20,10);    // Turn Green
     stripFront8.fill(stripFront8.Color(128, 128, 0),30,10); // Turn Yellow
	   stripFront8.show();
	   fun = 100;
     }	 
 	 if (fun == 49 ) // n Bat2 70%
     {
	   stripFront8.fill(stripFront8.Color(0, 128, 0),20,5);    // Turn Green
     stripFront8.fill(stripFront8.Color(128, 128, 0),25,15); // Turn Yellow
	   stripFront8.show();
	   fun = 100;
     }	 
 	 if (fun == 50 ) // o Bat2 60%
     {
	   stripFront8.fill(stripFront8.Color(128, 128, 0),20,20); // Turn Yellow
	   stripFront8.show();
	   fun = 100;
     }	 
 	 if (fun == 51 ) // p Bat2 50%
	   {
	   stripFront8.fill(stripFront8.Color(128, 128, 0),20,10); // Turn Yellow
     stripFront8.fill(stripFront8.Color(128, 0, 0),30,10);  // Turn Red
	   stripFront8.show();
	   fun = 100;
     }	 
 	 if (fun == 52 ) // q Bat2 40%
     {
	   stripFront8.fill(stripFront8.Color(128, 128, 0),20,5); // Turn Yellow
     stripFront8.fill(stripFront8.Color(128, 0, 0),25,15);  // Turn Red
	   stripFront8.show();
	   fun = 100;
     }	 
 	 if (fun == 53 ) // r Bat2 30%
     {
	   stripFront8.fill(stripFront8.Color(128, 0, 0),20,20);  // Turn Red
	   stripFront8.show();
	   fun = 100;
     }	 
 	 if (fun == 54 ) // s Bat2 20%
     {
	   stripFront8.fill(stripFront8.Color(0, 0, 0),20,10);  // Turn Black
     stripFront8.fill(stripFront8.Color(128, 0, 0),30,10);  // Turn Red
	   stripFront8.show();
	   fun = 100;
     }	 
 	 if (fun == 55 ) // t Bat2 10%
     {
	   stripFront8.fill(stripFront8.Color(0, 0, 0),20,15);  // Turn Black
     stripFront8.fill(stripFront8.Color(128, 0, 0),35,5);  // Turn Red
	   stripFront8.show();
	   fun = 100;
     }	 
 	 if (fun == 56 ) // t Bat2 <10%
     {
     stripFront8.fill(stripFront8.Color(0, 0, 0),20,20);  // Turn Black
     stripFront8.show();
	   fun = 100;
     }	 
 	 if (fun == 57 ) 
     {
      stripLogic.fill(stripLogic.Color(0, 0, 0),0,7);  // Black   
      stripLogic.fill(stripLogic.Color(0, 0, 0),7,5);  // Black
      stripLogic.fill(stripLogic.Color(0, 0, 0),12,4);  // Black
      stripLogic.fill(stripLogic.Color(0, 0, 0),16,3);  // Black
      stripLogic.show();
     fun = 100;
     }	 
 	 if (fun == 58 ) 
     {
      stripLogic.fill(stripLogic.Color(64, 0, 0),0,7);  // Red
      stripLogic.fill(stripLogic.Color(64, 0, 0),7,5);  // Red
      stripLogic.fill(stripLogic.Color(64, 0, 0),12,4);  // Blue
      stripLogic.fill(stripLogic.Color(0, 0, 0),16,3);  // black
      stripLogic.show();
     fun = 100;
     }	 
 	 if (fun == 59 ) 
     {
      stripLogic.fill(stripLogic.Color(0, 0, 0),0,7);  // Black      
      stripLogic.fill(stripLogic.Color(64, 0, 0),7,5);  // Blue
      stripLogic.fill(stripLogic.Color(0, 0, 64),12,4);  // Red
      stripLogic.fill(stripLogic.Color(0, 0, 0),16,3);  // Black
      stripLogic.show();
     fun = 100;
     }	 
 	 if (fun == 60 ) 
     {
      stripLogic.fill(stripLogic.Color(0, 0, 0),0,7);  // Black  
      stripLogic.fill(stripLogic.Color(0, 64, 0),7,5);  // Green
      stripLogic.fill(stripLogic.Color(0, 0, 64),12,4);  // Blue
      stripLogic.fill(stripLogic.Color(0, 0, 0),16,3);  // Black
      stripLogic.show();
     fun = 100;
     }	 
 	 if (fun == 61 ) 
     {
      stripLogic.fill(stripLogic.Color(64, 0, 0),0,7);  // Red
      stripLogic.fill(stripLogic.Color(64, 0, 0),7,5);  // Red
      stripLogic.fill(stripLogic.Color(0, 0, 64),12,4);  // Blue
      stripLogic.fill(stripLogic.Color(0, 0, 64),16,3);  // Blue
      stripLogic.show();
     fun = 100;
     }	 
   if (fun == 62 ) 
     {
     if (flash == 0) {flash = 1;} else {flash=0;}
     fun = 100;
     }  
	 if (fun == 99) // Sparkle out
	 {
	 flashRandom(0, 1);  // First number is 'wait' delay, shorter num == shorter twinkle
                        // Second number is how many neopixels to simultaneously light up 
	 }
	

  } // end loop

void handleSerial() {
 while (Serial.available() > 0) {
   char incomingCharacter = Serial.read();
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
  case 'a': 
    fun = 36;
    break;
  case 'b': 
    fun = 37;
    break;
  case 'c': 
    fun = 38;
    break;
  case 'd': 
    fun = 39;
    break;
  case 'e': 
    fun = 40;
    break;
  case 'f': 
    fun = 41;
    break;
  case 'g': 
    fun = 42;
    break;
  case 'h': 
    fun = 43;
    break;
  case 'i': 
    fun = 44;
    break;
  case 'j': 
    fun = 45;
    break;
  case 'k': 
    fun = 46;
    break;
  case 'l': 
    fun = 47;
    break;
  case 'm': 
    fun = 48;
    break;
  case 'n': 
    fun = 49;
    break;
  case 'o': 
    fun = 50;
    break;	
  case 'p': 
    fun = 51;
    break;
  case 'q': 
    fun = 52;
    break;
  case 'r': 
    fun = 53;
    break;
  case 's': 
    fun = 54;
    break;
  case 't': 
    fun = 55;
    break;
  case 'u': 
    fun = 56;
    break;
  case 'v': 
    fun = 57;
    break;
  case 'w': 
    fun = 58;
    break;
  case 'x': 
    fun = 59;
    break;
  case 'y': 
    fun = 60;
    break;
  case 'z': 
    fun = 61;
    break;
  case '!': 
    fun = 62;
    break;
  }
 }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
     strip.setPixelColor(i, Wheel((i+j) & 256));
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
    stripLogic.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    stripFront8.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    stripFront3.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    stripBack3.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    stripLogic.show();
    stripFront8.show();
    stripFront3.show();
    stripBack3.show();
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
    // int value;
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

void flashF3()
{
  if (f3flash == 1) {
  stripFront3.fill( stripFront3.Color(0, 0, 128),0,15); // Turn all blue
	stripFront3.show(); 
  }
  if (f3flash == 2) {
  stripFront3.fill( stripFront3.Color(0, 128, 0),5,5); // Middle Red
	stripFront3.show();
  }
  if (f3flash == 3) {
  stripFront3.fill( stripFront3.Color(0, 0, 0),5,10); // Bottom 2 Black
	stripFront3.show();
  }
  if (f3flash == 4) {
  stripFront3.fill( stripFront3.Color(0, 0, 0),0,5); // Turn Top Black
  stripFront3.fill( stripFront3.Color(0, 128, 0),5,5); // Middle Red
	stripFront3.show();
  }
  if (f3flash == 5) {
  stripFront3.fill( stripFront3.Color(0, 0, 128),0,5); // Turn top Blue
	stripFront3.show();
  }
  if (f3flash == 6) {
  stripFront3.fill( stripFront3.Color(0, 0, 0),5,10); // Turn all Black
  stripFront3.fill( stripFront3.Color(0, 0, 128),0,5); // Turn top Blue
	stripFront3.show();
  }
  if (f3flash == 7) {
  stripFront3.fill( stripFront3.Color(0, 0, 0),0,5); // Turn top black
  stripFront3.fill( stripFront3.Color(128, 0, 0),5,5); // Turn middle red
  stripFront3.show();
  }
  if (f3flash == 8) {
  stripFront3.fill( stripFront3.Color(0, 0, 128),10,5); // Turn middle lighter red
	stripFront3.show();
  }
  if (f3flash == 9) {
  stripFront3.fill( stripFront3.Color(0, 0, 128),5,10); // Turn bottom blue
	stripFront3.show();
  }
  if (f3flash == 10) {
  stripFront3.fill( stripFront3.Color(0, 0, 0),0,5); // Turn Black
  stripFront3.fill( stripFront3.Color(128, 0, 0),5,5); // Turn Blue
  stripFront3.fill( stripFront3.Color(0, 0, 0),10,5); // Turn Black
	stripFront3.show();
  }
  if (f3flash == 11) {
  stripFront3.fill( stripFront3.Color(0, 0, 0),1,5); 
  stripFront3.fill( stripFront3.Color(128, 0, 0),5,5); 
  stripFront3.fill( stripFront3.Color(128, 0, 0),10,5); 
	stripFront3.show();
  }
  if (f3flash == 12) {
  stripFront3.fill( stripFront3.Color(0, 0, 0),5,5); // Turn Black
	stripFront3.show();
  }
  if (f3flash == 13) {
  stripFront3.fill( stripFront3.Color(0, 0, 128),0,15); // Turn all 3 blue
	stripFront3.show();
  }
  if (f3flash == 14) {
  stripFront3.fill( stripFront3.Color(0, 0, 0),0,10); // Turn top 2 Black
	stripFront3.show();
  }
  if (f3flash == 15) {
  stripFront3.fill( stripFront3.Color(128, 128, 0),0,10); // Turn top yellow
	stripFront3.show();
  }
  f3flash++;
  if (f3flash > 15) {
  f3flash = 1;
  }
}


void flashR3()
{
  if (r3flash == 1) {
  stripBack3.fill( stripBack3.Color(0, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 0),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),10,5); 
	stripBack3.show();
  }
  if (r3flash == 2) {
  stripBack3.fill( stripBack3.Color(0, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(128, 0, 0),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),10,5); 
	stripBack3.show();
  }
  if (r3flash == 3) {
  stripBack3.fill( stripBack3.Color(0, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(128, 0, 0),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 0),10,5); 
	stripBack3.show();
  }
  if (r3flash == 4) {
  stripBack3.fill( stripBack3.Color(0, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(128, 0, 0),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),10,5); 
	stripBack3.show();
  }
  if (r3flash == 5) {
  stripBack3.fill( stripBack3.Color(0, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),10,5); 
	stripBack3.show();
  }
  if (r3flash == 6) {
  stripBack3.fill( stripBack3.Color(0, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(128, 0, 0),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),10,5); 
	stripBack3.show();
  }
  if (r3flash == 7) {
  stripBack3.fill( stripBack3.Color(0, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 0),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),10,5); 
	stripBack3.show();
  }
  if (r3flash == 8) {
  stripBack3.fill( stripBack3.Color(0, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(128, 0, 0),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),10,5); 
	stripBack3.show();
  }
  if (r3flash == 9) {
  stripBack3.fill( stripBack3.Color(0, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 0),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),10,5); 
	stripBack3.show();
  }
  if (r3flash == 10) {
  stripBack3.fill( stripBack3.Color(0, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(128, 0, 0),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),10,5); 
	stripBack3.show();
  }
  if (r3flash == 11) {
  stripBack3.fill( stripBack3.Color(128, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(128, 0, 0),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),10,5); 
	stripBack3.show();
  }
  if (r3flash == 12) {
  stripBack3.fill( stripBack3.Color(128, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 0),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 0),10,5); 
	stripBack3.show();
  }
  if (r3flash == 13) {
  stripBack3.fill( stripBack3.Color(128, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(128, 0, 0),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),10,5); 
	stripBack3.show();
  }
  if (r3flash == 14) {
  stripBack3.fill( stripBack3.Color(128, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 0),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),10,5); 
	stripBack3.show();
  }
  if (r3flash == 15) {
  stripBack3.fill( stripBack3.Color(0, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 0),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),10,5); 
	stripBack3.show();
  }
  if (r3flash == 16) {
  stripBack3.fill( stripBack3.Color(0, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),10,5); 
	stripBack3.show();
  }
  if (r3flash == 17) {
  stripBack3.fill( stripBack3.Color(0, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),10,5); 
	stripBack3.show();
  }
  if (r3flash == 18) {
  stripBack3.fill( stripBack3.Color(0, 0, 128),1,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),10,5); 
	stripBack3.show();
  }
  if (r3flash == 19) {
  stripBack3.fill( stripBack3.Color(0, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),10,5); 
	stripBack3.show();
  }
  if (r3flash == 20) {
  stripBack3.fill( stripBack3.Color(128, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),10,5); 
	stripBack3.show();
  }
  if (r3flash == 21) {
  stripBack3.fill( stripBack3.Color(128, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(128, 0, 0),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),10,5); 
	stripBack3.show();
  }
  if (r3flash == 22) {
  stripBack3.fill( stripBack3.Color(0, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(128, 0, 0),5,5); 
  stripBack3.fill( stripBack3.Color(128, 0, 0),10,5); 
	stripBack3.show();
  }
  if (r3flash == 23) {
  stripBack3.fill( stripBack3.Color(0, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 0),5,5); 
  stripBack3.fill( stripBack3.Color(128, 0, 0),10,5); 
	stripBack3.show();
  }
  if (r3flash == 24) {
  stripBack3.fill( stripBack3.Color(0, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 0),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),10,5); 
	stripBack3.show();
  }
  if (r3flash == 25) {
  stripBack3.fill( stripBack3.Color(0, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),10,5); 
	stripBack3.show();
  }
  if (r3flash == 26) {
  stripBack3.fill( stripBack3.Color(0, 0, 128),1,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 0),10,5); 
	stripBack3.show();
  }
  if (r3flash == 27) {
  stripBack3.fill( stripBack3.Color(0, 0, 128),1,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 0),5,5); 
  stripBack3.fill( stripBack3.Color(128, 0, 0),10,5); 
	stripBack3.show();
  }
  if (r3flash == 28) {
  stripBack3.fill( stripBack3.Color(0, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(128, 0, 0),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 0),10,5); 
	stripBack3.show();
  }
  if (r3flash == 29) {
  stripBack3.fill( stripBack3.Color(0, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),5,5); 
  stripBack3.fill( stripBack3.Color(128, 0, 0),10,5); 
	stripBack3.show();
  }
  if (r3flash == 30) {
  stripBack3.fill( stripBack3.Color(0, 0, 0),1,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 128),5,5); 
  stripBack3.fill( stripBack3.Color(0, 0, 0),10,5); 
	stripBack3.show();
  }
  r3flash++;
  if (r3flash > 30) {
  r3flash = 1;
  }
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
