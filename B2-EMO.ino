//                          Last Revised Date: 02/18/2023
//                             Revised By: craiglindsay
//                Inspired by the PADAWAN / KnightShade SHADOW effort
// =======================================================================================
//
//         This program is free software: you can redistribute it and/or modify it for
//         your personal use and the personal use of other astromech club members.
//
//         This program is distributed in the hope that it will be useful
//         as a courtesy to fellow astromech club members wanting to develop
//         their own droid control system.
//
//         IT IS OFFERED WITHOUT ANY WARRANTY; without even the implied warranty of
//         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
//         You are using this software at your own risk and, as a fellow club member, it is
//         expected you will have the proper experience / background to handle and manage that
//         risk appropriately.  It is completely up to you to insure the safe operation of
//         your droid and to validate and test all aspects of your droid control system.
//
// =======================================================================================
//   Note: You will need a Arduino Mega ADK rev3 to run this sketch,
//
//   This is written to be a SPECIFIC Sketch - supporting only one type of controller
//      - PS3 Move Navigation
//
//   PS3 Bluetooth library - developed by Kristian Lauszus (kristianl@tkjelectronics.com)
//   For more information visit my blog: http://blog.tkjelectronics.dk/
//
//
// =======================================================================================
//  Serial3 Intended to be used for LED lighting effects triggered by serial command like 
//  Serial3.print("C"); sent from Body ADK to another arduino listening for serial commands
//  Software serial used to send commands to the DF player just because, might move to HW


// ---------------------------------------------------------------------------------------
// Libraries
// ---------------------------------------------------------------------------------------
#include <usbhub.h>
#include <PS3BT.h>
#include <Servo.h>
#include <DFRobotDFPlayerMini.h>

// ---------------------------------------------------------------------------------------
// General User Settings
// ---------------------------------------------------------------------------------------
String PS3ControllerFootMac = "00:26:43:CF:E9:8F";   //Set this to your FOOT PS3 controller MAC address
String PS3ControllerBackupFootMac = "E0:AE:5E:91:8A:21";  //Set to the MAC Address of your BACKUP FOOT controller (Optional)
byte drivespeed1 = 65;   //For Speed Setting (Normal): set this to whatever speeds works for you. 0-stop, 127-full speed.
byte drivespeed2 = 127;  //For Speed Setting (Over Throttle): set this for when needing extra power. 0-stop, 127-full speed.
byte turnspeed = 60;      // the higher this number the faster it will spin in place, lower - the easier to control. Recommend beginner: 40 to 50, experienced: 50+, I like 75
byte joystickDeadZoneRange = 10;  // For controllers that centering problems, use the lowest number with no drift
byte volume = 25;  // inistial df player volume (1-30) above 25 seems distorted to me
byte ramping = 8;        // Ramping- the lower this number the longer R2 will take to speedup or slow down,

// ---------------------------------------------------------------------------------------
// B2 Servo Extents
// ---------------------------------------------------------------------------------------

int DriveMin = 1000;
int DriveMax = 2000;
int DriveStop = 1500;
int RotateMin = 1000;
int RotateMax = 2000;
int RotateStop = 1500;
int StrafeMin = 1000;
int StrafeMax = 2000;
int StrafeStop = 1500;
int HeadRightMin = 1125;
int HeadRightMax = 1950;
int HeadRightCenter = 1500;   // Level for the R head servo
int HeadLeftMin = 1200;
int HeadLeftMax = 2000;
int HeadLeftCenter = 1650;   // Level for the L servo
int HeadZMin = 200;   //Min is all the way up in this case
int HeadZMax = 1925;  //Max is all the way down in this case
int HeadZMid = (HeadZMin + HeadZMax)/2;
int HeadTMin = 200;     // head turning left/right
int HeadTMax = 2800;    // head turning left/right
int HeadTCenter = 1465;  // set this to the center
int BodyZ1Min = 1100; // This is body Z1 closed 
int BodyZ1Max = 1900; // This is body Z1 open
int BodyZ2Min = 1000; // This is body Z1 open  
int BodyZ2Max = 1950; // This is body Z1 closed 
int FLSpreadMin = 800;
int FLSpreadMax = 2000;
int FRSpreadMin = 900;
int FRSpreadMax = 2200;
int BRSpreadMin = 900;
int BRSpreadMax = 2000;
int BLSpreadMin = 800;
int BLSpreadMax = 2000;
int FLHeightMin = 600;
int FLHeightMax = 2000;
int FRHeightMin = 900;
int FRHeightMax = 2200;
int BRHeightMin = 600;
int BRHeightMax = 2000;
int BLHeightMin = 1000;
int BLHeightMax = 2400;


// ---------------------------------------------------------------------------------------
// Initial Servo Values at power on
// ---------------------------------------------------------------------------------------

int Drive = 1500;       //1500 is stopped
int Rotate = 1500;		//1500 is stopped
int Strafe = 1500;		//1500 is stopped
int HeadRight = HeadRightCenter;
int HeadLeft = HeadLeftCenter;
int HeadT = HeadTCenter;
int HeadZ = 1200;
int BodyZ1 = BodyZ1Max;  //Best to expand body at startup so lower panels can be removed
int BodyZ2 = BodyZ2Min;  //Best to expand body at startup so lower panels can be removed
int FLSpread = (FLSpreadMin + FLSpreadMax)/2;
int FRSpread = (FRSpreadMin + FRSpreadMax)/2;
int BRSpread = (BRSpreadMin + BRSpreadMax)/2;
int BLSpread = (BLSpreadMin + BLSpreadMax)/2;
int FLHeight = (FLHeightMin + FLHeightMax)/2;
int FRHeight = (FRHeightMin + FRHeightMax)/2;
int BRHeight = (BRHeightMin + BRHeightMax)/2;
int BLHeight = (BLHeightMin + BLHeightMax)/2;
int turnAmount = HeadTCenter;



// ---------------------------------------------------------------------------------------
// Servo and Motor Pin Setup
// ---------------------------------------------------------------------------------------

#define PinDrive 22
#define PinRotate 24
#define PinStrafe 26
#define PinHeadRight 28
#define PinHeadLeft 30
#define PinHeadT 32
#define PinHeadZ 34
#define PinBodyZ1 36
#define PinBodyZ2 38
#define PinFLHeight 40
#define PinFRHeight 42
#define PinBRHeight 44
#define PinBLHeight 6
#define PinFLSpread 35
#define PinFRSpread 37
#define PinBRSpread 39
#define PinBLSpread 41
#define PinUpperMotorA 2
#define PinUpperMotorB 3
#define PinLowerMotorA 4
#define PinLowerMotorB 5
#define Pin1FeetServoAttached 7
#define Pin2FeetServoAttached 8

// ---------------------------------------------------------------------------------------
// Name the Servos
// ---------------------------------------------------------------------------------------

Servo ServoDrive;
Servo ServoRotate;
Servo ServoStrafe;
Servo ServoHeadRight;
Servo ServoHeadLeft;
Servo ServoHeadZ;
Servo ServoHeadT;
Servo ServoBodyZ1;
Servo ServoBodyZ2;
Servo ServoFLHeight;
Servo ServoFRHeight;
Servo ServoBRHeight;
Servo ServoBLHeight;
Servo ServoFLSpread;
Servo ServoFRSpread;
Servo ServoBRSpread;
Servo ServoBLSpread;

// ---------------------------------------------------------------------------------------
// Setup Sound Settings
// ---------------------------------------------------------------------------------------
DFRobotDFPlayerMini myDFPlayer;          

// ---------------------------------------------------------------------------------------
// Other Variables
// ---------------------------------------------------------------------------------------
long previousFootMillis = millis();
long previousSpeedToggleMillis = millis();
long currentMillis = millis();
long footServoMillis = millis();

int serialLatency = 25;  //This is a delay factor in ms to prevent queueing of the Serial data. 25ms seems approprate for HardwareSerial, values of 50ms or larger are needed for Softare Emulation
int speedToggleButtonCounter = 0;
boolean footServosPowered = true;

unsigned long soundprevmillis = 0;    // storage for sound timer
unsigned long soundcurrentmillis = 0; // storage for sound timer
unsigned long soundTimer;             // storage for sound timer
int soundNum = 0;                     // sound number


int animationStep = 0;                  // animation step for routines
unsigned long animatecurrentmillis = 0; // storage for animation timer
unsigned long animateprevmillis = 0;    // storage for animation timer
unsigned long animateTimer;             // storage for animation timer
unsigned long animateDelay = 0;
int animationTrigger = 0;
int secondsInAnimation =0;

boolean CenterHeadTrigger = false;
boolean LevelHeadTrigger = false;
boolean TiltHeadUpTrigger = false;
boolean TiltHeadDownTrigger = false;
boolean TiltHeadLeftTrigger = false;
boolean TiltHeadRightTrigger = false;
boolean HeadUpTrigger = false;
boolean HeadDownTrigger = false;
boolean HeadMidTrigger = false;
boolean TurnHeadTrigger = false;
boolean BodyTuckInTrigger = false;
boolean BodyExpandTrigger = false;

// ---------------------------------------------------------------------------------------
// Setup for USB and Bluetooth Devices////////////////////////////
// ---------------------------------------------------------------------------------------
USB Usb;
BTD Btd(&Usb);
PS3BT *PS3Nav=new PS3BT(&Btd);
uint32_t msgLagTime = 0;
uint32_t lastMsgTime = 0;
uint32_t currentTime = 0;
uint32_t lastLoopTime = 0;
int badPS3Data = 0;
boolean firstMessage = true;
boolean isFootMotorStopped = true;
boolean overSpeedSelected = false;
boolean isPS3NavigatonInitialized = false;
boolean isStickEnabled = true;
boolean WaitingforReconnect = false;
boolean mainControllerConnected = false;
int StickY = 0;
int StickX = 0;
// =======================================================================================
// Initialize - Setup Function
// =======================================================================================

void setup()
{
    //setup for DF Mini player
    Serial2.begin(9600);       //Setup Serial for DF player
    myDFPlayer.begin(Serial2); //Use softwareSerial to communicate with dfplayer
    myDFPlayer.volume(volume);          //Set dfplayer volume value above
    myDFPlayer.play(16);                //Play the 21st mp3 as a startup sound

    //Setup for serial 3
    Serial3.begin(115200);            //Serial3:: Used for lighting to arduino nano
  
	Serial.begin(115200);             // For debugging, in each section can uncomment serial outputs
	while (!Serial);
	if (Usb.Init() == -1)
	{
	Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
	}
	Serial.print(F("\r\nBluetooth Library Started"));
  
  //Setup for PS3
  PS3Nav->attachOnInit(onInitPS3Nav); // onInitPS3Nav is called upon a new connection

// =======================================================================================
// Attach Servos  - inital connection of all Servos
// =======================================================================================  

ServoDrive.attach(PinDrive);
ServoRotate.attach(PinRotate);
ServoStrafe.attach(PinStrafe);
ServoHeadRight.attach(PinHeadRight);
ServoHeadLeft.attach(PinHeadLeft);
ServoHeadZ.attach(PinHeadZ);
ServoHeadT.attach(PinHeadT);;
ServoBodyZ1.attach(PinBodyZ1);
ServoBodyZ2.attach(PinBodyZ2);
ServoFLHeight.attach(PinFLHeight);
ServoFRHeight.attach(PinFRHeight);
ServoBRHeight.attach(PinBRHeight);
ServoBLHeight.attach(PinBLHeight);
ServoFLSpread.attach(PinFLSpread);
ServoFRSpread.attach(PinFRSpread);
ServoBRSpread.attach(PinBRSpread);
ServoBLSpread.attach(PinBLSpread);

// =======================================================================================
// Set Initial Servo Positions
// =======================================================================================  
ServoDrive.writeMicroseconds(Drive);
ServoRotate.writeMicroseconds(Rotate);
ServoStrafe.writeMicroseconds(Strafe);
ServoHeadRight.writeMicroseconds(HeadRight);
ServoHeadLeft.writeMicroseconds(HeadLeft);
ServoHeadZ.writeMicroseconds(HeadZ);
ServoHeadT.writeMicroseconds(HeadT);
ServoBodyZ1.writeMicroseconds(BodyZ1);
ServoBodyZ2.writeMicroseconds(BodyZ2);
ServoFLHeight.writeMicroseconds(FLHeight);
ServoFRHeight.writeMicroseconds(FRHeight);
ServoBRHeight.writeMicroseconds(BRHeight);
ServoBLHeight.writeMicroseconds(BLHeight);
ServoFLSpread.writeMicroseconds(FLSpread);
ServoFRSpread.writeMicroseconds(FRSpread);
ServoBRSpread.writeMicroseconds(BRSpread);
ServoBLSpread.writeMicroseconds(BLSpread);

// =======================================================================================
// Setup Motor Controller for rings/spinners
// =======================================================================================  
pinMode(PinUpperMotorA, OUTPUT);    // Set motor controller pins to output
pinMode(PinUpperMotorB, OUTPUT);
pinMode(PinLowerMotorA, OUTPUT);
pinMode(PinLowerMotorB, OUTPUT);
pinMode(Pin1FeetServoAttached, OUTPUT);
pinMode(Pin2FeetServoAttached, OUTPUT);
digitalWrite(PinUpperMotorA, HIGH);  //Write High (Disable) to all 4 motor controller pins
digitalWrite(PinUpperMotorB, HIGH);  
digitalWrite(PinLowerMotorA, HIGH);  
digitalWrite(PinLowerMotorB, HIGH);  
digitalWrite(Pin1FeetServoAttached, HIGH);  
digitalWrite(Pin2FeetServoAttached, HIGH);  

}
// =======================================================================================
// Main Program Loop - This is the recurring check loop for entire sketch
// =======================================================================================
void loop()   	//LOOP through functions from highest to lowest priority.
{
	if ( !readUSB() ) {
		//We have a fault condition that we want to ensure that we do NOT process any controller data!!!
		return; }
	MotorDrive();     // reads and acts on PS3 remote input
	toggleSettings(); // reads the PS3 remote for other button presses
	footServoPowerDown(); // Looks to see if it has be 5 seconds since last foot height adjustment
	currentMillis = millis();
	if (soundNum >0) { animationSounds(); }       // trigger for sound looping
	if (animationTrigger == 1) { animation1(); }  // trigger animation 1, head movements
	if (animationTrigger == 2) { animation2(); }  // trigger animation 2, head and some body movements
	if (HeadZ > HeadZMid) {LevelHeadTrigger=1;}   // failsafe to level head if it gets lowered beyond the mid-point
	animations();
}

// =======================================================================================
// Drive Motor Control Section
// =======================================================================================
boolean ps3MotorDrive(PS3BT* myPS3 = PS3Nav)
{
	int StickX = 0;
	int StickY = 0;
	if (isPS3NavigatonInitialized)
	{
		// Additional fault control.  Do NOT send additional commands if no controllers have initialized.
		if (!isStickEnabled)
		{
            //Serial.println("stick is not enabled ");
			if (!isFootMotorStopped)
			{
			//Serial.println("Stick not enabled but drive was running, motors stopped");
			Drive=DriveStop;
			Rotate=RotateStop;
			Strafe=StrafeStop;  
			ServoDrive.writeMicroseconds(Drive);
			ServoRotate.writeMicroseconds(Rotate);
			ServoStrafe.writeMicroseconds(Strafe);
			isFootMotorStopped = true;
            StickX = 0; 
            StickY = 0; 
			}
			return false;
		} else if (!myPS3->PS3NavigationConnected)
		{
            //Serial.println("Controller is disconnected ");
			if (!isFootMotorStopped)
			{
            //Serial.println("Drive was running while controller disconnected, motors stopped");
			Drive=DriveStop;
			Rotate=RotateStop;
			Strafe=StrafeStop;  
			ServoDrive.writeMicroseconds(Drive);
			ServoRotate.writeMicroseconds(Rotate);
			ServoStrafe.writeMicroseconds(Strafe);
			isFootMotorStopped = true;
			StickX = 0; 
      StickY = 0; 
			}
			return false;	
		} else
		{
			int joystickPosition = myPS3->getAnalogHat(LeftHatY);

			if (overSpeedSelected) //Over throttle is selected
			{
				StickY = (map(joystickPosition, 0, 255, -drivespeed2, drivespeed2));
			  } else {
				StickY = (map(joystickPosition, 0, 255, -drivespeed1, drivespeed1));
			}


      if ( abs(joystickPosition-128) < joystickDeadZoneRange)
      {
        // This is RAMP DOWN code when stick is now at ZERO but prior FootSpeed > 20
        if (abs(1500 - Drive) > 50)
        {
          if (Drive > 0) { Drive -= 3; } else { Drive += 3; }
        } else if (abs(Drive) > 20) { if (Drive > 0) { Drive -= 2; } else { Drive += 2; }       
        } else { Drive = 0; }
      } else { isFootMotorStopped = false;
        if (1500 - Drive/5.262 < StickY) {
          if ((StickY-(1500 - Drive/5.262))>(ramping+1)) {
            Drive+=ramping;
          } else
          Drive = 1500 - 5.262 * StickY;
        } else if (Drive > StickY)
        { if ((1500 - 5.262 * Drive-StickY)>(ramping+1)) {    
            Drive-=ramping;  
          } else
          Drive = 1500 - 5.262 * StickY;
        } else { Drive = 1500 - StickY * 5.262; }
      }
			StickX = (myPS3->getAnalogHat(LeftHatX));  //Maping sifferent regions of the HAT to different values to allow finer control at slow speeds
			if ( abs(StickY) > 50)
			StickX = (map(myPS3->getAnalogHat(LeftHatX), 54, 200, -(turnspeed/4), (turnspeed/4)));
			else if (StickX <= 200 && StickX >= 54)
			StickX = (map(myPS3->getAnalogHat(LeftHatX), 54, 200, -(turnspeed/3), (turnspeed/3)));
			else if (StickX > 200)
			StickX = (map(myPS3->getAnalogHat(LeftHatX), 201, 255, turnspeed/3, turnspeed));
			else if (StickX < 54)
			StickX = (map(myPS3->getAnalogHat(LeftHatX), 0, 53, -turnspeed, -(turnspeed/3)));
			if (abs(StickX) > 5) { isFootMotorStopped = false; }
			currentMillis = millis();
			if ( (currentMillis - previousFootMillis) > serialLatency  ) {
				if (StickY != 0 || abs(StickX) > 1) 
				 {

		  //**************************************************
		  // L1 and Stick - HeadZ / HeadTurn
		  //**************************************************  
         if (myPS3->getButtonPress(L1) && !myPS3->getButtonPress(L2) && !myPS3->getButtonPress(L3))  
          {
          //Serial.print("Head Z/T ");
          HeadZ = HeadZ + (StickY/1.5);
          if (HeadZ > HeadZMax) HeadZ=HeadZMax;   
          if (HeadZ < HeadZMin) HeadZ=HeadZMin;     
          HeadT = HeadT - ((StickX)/1.5);
          if (HeadT > HeadTMax) HeadT=HeadTMax;
          if (HeadT < HeadTMin) HeadT=HeadTMin;
          ServoHeadZ.writeMicroseconds(HeadZ);
          ServoHeadT.writeMicroseconds(HeadT);
          //Serial.print("HeadZ: ");
          //Serial.print(HeadZ); 
          //Serial.print(" HeadT: ");
          //Serial.println(HeadT);
          } 
		  //**************************************************
		  // L2 and Stick - Head L & R Servo 
		  //**************************************************  
          else if (myPS3->getButtonPress(L2) && !myPS3->getButtonPress(L1) && !myPS3->getButtonPress(L3)) 
           {
           HeadLeft = HeadLeft - ((StickY*-1) + (StickX))/1.5;                    // New method for incrementing with stick movement but staying in place
		   HeadRight = HeadRight - ((StickY) + (StickX))/1.5;                       // New method for incrementing with stick movement but staying in place
           //HeadLeft = ((1500 - (12*StickY*-1)) + (1500 + (12*StickX*-1)))/2; // Old method for moving with stick, too jerky
           //HeadRight = ((1500 - (12*StickY)) + (1500 - (12*StickX)))/2;       // Old method for moving with stick, too jerky
           if (HeadRight > HeadRightMax) HeadRight=HeadRightMax;
           if (HeadRight < HeadRightMin) HeadRight=HeadRightMin;
           if (HeadLeft > HeadLeftMax) HeadLeft=HeadLeftMax;
           if (HeadLeft < HeadLeftMin) HeadLeft=HeadLeftMin;
           ServoHeadRight.writeMicroseconds(HeadRight);
           ServoHeadLeft.writeMicroseconds(HeadLeft);
           //Serial.print("HeadRight: ");
           //Serial.print(HeadRight); 
           //Serial.print(" HeadLeft: ");
           //Serial.println(HeadLeft); 
          }
		  //**************************************************
		  // L3 Stick - Drive / Strafe 
		  //**************************************************  
           else if (myPS3->getButtonPress(L3) && !myPS3->getButtonPress(L1) && !myPS3->getButtonPress(L2))  // L3 Stick - Drive / Strafe 
          {
           //Serial.print("Drive/Strafe ");
           Drive = (1500 - 5.262 * StickY);
           if (Drive > DriveMax) Drive=DriveMax;
           if (Drive < DriveMin) Drive=DriveMin;
           if ((Drive > 1500) && (Drive - joystickDeadZoneRange < 1500)) Drive=DriveStop;
           if ((Drive < 1500) && ((1500 - Drive) < joystickDeadZoneRange)) Drive=DriveStop;
           Strafe = (1500 + 6.25 * StickX);
           if (Strafe > StrafeMax) Strafe=StrafeMax;
           if (Strafe < StrafeMin) Strafe=StrafeMin;
           if ((Strafe > 1500) && (Strafe - joystickDeadZoneRange < 1500)) Strafe=StrafeStop;
           if ((Strafe < 1500) && ((1500 - Strafe) < joystickDeadZoneRange)) Strafe=StrafeStop;
           ServoDrive.writeMicroseconds(Drive);
           ServoStrafe.writeMicroseconds(Strafe);
           //Serial.print("Drive: ");
           //Serial.print(Drive); 
           //Serial.print(" Strafe: ");
           //Serial.println(Strafe); 
          }
		  //***************************************************
		  // Stick L1+L2+L3 Feet height for directional leaning
		  //***************************************************  
          else if (myPS3->getButtonPress(L1) && myPS3->getButtonPress(L2) && myPS3->getButtonPress(L3))  
          {
          // Serial.print("Head Foot Up/Down ");
          //For Testing X and devloping the mix
          //FLHeight = (1500 - (6.5*StickX*-1));
          //BRHeight = (1500 - (6.5*StickX));
          //FRHeight = (1500 + (6.5*StickX));
          //BLHeight = (1500 + (6.5*StickX*-1));
                    
          //For Testing Y and devloping the mix
          //FLHeight = (1500 - (5.6*StickY*-1));
          //BRHeight = (1500 - (5.6*StickY));
          //FRHeight = (1500 + (5.6*StickY*-1));
          //BLHeight = (1500 + (5.6*StickY));

          //Combined  above X + Y / 2
		  FLHeight = ((1500 + (8*StickX*-1)) + (1500 + (8*StickY*-1)))/2;
          BRHeight = ((1500 + (8*StickX)) + (1500 + (8*StickY)))/2;
          FRHeight = ((1500 - (8*StickX)) + (1500 - (8*StickY*-1)))/2;
          BLHeight = ((1500 - (8*StickX*-1)) + (1500 - (8*StickY)))/2;
		  if (FLHeight > FLHeightMax) FLHeight=FLHeightMax;
          if (FLHeight < FLHeightMin) FLHeight=FLHeightMin;
          if (BRHeight > BRHeightMax) BRHeight=BRHeightMax;
          if (BRHeight < BRHeightMin) BRHeight=BRHeightMin;
          if (FRHeight > FRHeightMax) FRHeight=FRHeightMax;
          if (FRHeight < FRHeightMin) FRHeight=FRHeightMin;
          if (BLHeight > BLHeightMax) BLHeight=BLHeightMax;
          if (BLHeight < BLHeightMin) BLHeight=BLHeightMin;
		  FootServoPowerUp();
          ServoFLHeight.writeMicroseconds(FLHeight);
          ServoFRHeight.writeMicroseconds(FRHeight);
          ServoBRHeight.writeMicroseconds(BRHeight);
          ServoBLHeight.writeMicroseconds(BLHeight);
          //Serial.print ("  FL:");
          //Serial.print (FLHeight);
          //Serial.print (" FR:");
          //Serial.print (FRHeight);
          //Serial.print (" BR:");
          //Serial.print (BRHeight);
          //Serial.print (" BL:");
          //Serial.println (BLHeight);
          }
		  //**************************************************
		  // Stick - Drive/Rotate
		  //**************************************************  
          else if (!myPS3->getButtonPress(L1) && !myPS3->getButtonPress(L2) && !myPS3->getButtonPress(L3))  // Stick - Drive/Rotate 
          {
           Drive = (1500 - 5.262 * StickY);
           if (Drive >  DriveMax) Drive= DriveMax;
           if (Drive <  DriveMin) Drive= DriveMin;
           if ((Drive > 1500) && (Drive - joystickDeadZoneRange < 1500)) Drive=DriveStop;
           if ((Drive < 1500) && ((1500 - Drive) < joystickDeadZoneRange)) Drive=DriveStop;
           Rotate = (1500 - 6.25 * StickX);
           if (Rotate > RotateMax) Rotate=RotateMax;
           if (Rotate < RotateMin) Rotate=RotateMin;
           if ((Rotate > 1500) && (Rotate - joystickDeadZoneRange < 1500)) Rotate=RotateStop;
           if ((Rotate < 1500) && ((1500 - Rotate) < joystickDeadZoneRange)) Rotate=RotateStop;
           ServoDrive.writeMicroseconds(Drive);
           ServoRotate.writeMicroseconds(Rotate);
           //Serial.print("Drive: ");
           //Serial.print(Drive); 
           //Serial.print(" Rotate: ");
           //Serial.println(Rotate); 
				  }
          } else {
					if (!isFootMotorStopped) //stop feet
					{
           Drive=DriveStop;
           Rotate=RotateStop;
           Strafe=StrafeStop;  
           ServoDrive.writeMicroseconds(Drive);
           ServoRotate.writeMicroseconds(Rotate);
           ServoStrafe.writeMicroseconds(Strafe);
           Serial.println("Stopping drive system");
           isFootMotorStopped = true;
           StickX = 0; 
           StickY = 0; 
					}
				}
				previousFootMillis = currentMillis;
				return true; //we sent a foot command
			}
		}
	return false;
}
}
void MotorDrive()
{
	//Flood control prevention
	if ((millis() - previousFootMillis) < serialLatency) return;
	if (PS3Nav->PS3NavigationConnected) ps3MotorDrive(PS3Nav);
}

// =======================================================================================
// PS3 Remote Control Section
// =======================================================================================
void ps3ToggleSettings(PS3BT* myPS3 = PS3Nav)
{

// =======================================================================================
// Circle Button Combos 
// =======================================================================================
	// Circle + Up 
	if(myPS3->getButtonPress(CIRCLE) && myPS3->getButtonClick(UP))
	{
		HeadUpTrigger=1;
	}
	// Circle + Down 
	if(myPS3->getButtonPress(CIRCLE) && myPS3->getButtonClick(DOWN))
	{
		HeadDownTrigger=1;
	}
	// Circle + Left 
	if(myPS3->getButtonPress(CIRCLE) && myPS3->getButtonClick(LEFT))
	{
		CenterHeadTrigger=1;
	}
	// Circle + Right 
	if(myPS3->getButtonPress(CIRCLE) && myPS3->getButtonClick(RIGHT))
	{
		LevelHeadTrigger=1;
	}
	// Circle + Cross 
	if(myPS3->getButtonPress(CIRCLE) && myPS3->getButtonClick(CROSS))
	{
	}
	// Circle + L1  
	if(myPS3->getButtonPress(CIRCLE) && myPS3->getButtonClick(L1))
	{
		CenterHeadTrigger=1;
		LevelHeadTrigger=1;
	}
	// Circle + L2 
	if(myPS3->getButtonPress(CIRCLE) && myPS3->getButtonClick(L2))
	{
		CenterHeadTrigger=1;
		LevelHeadTrigger=1;
	}
	// Circle + L3 
	if(myPS3->getButtonPress(CIRCLE) && myPS3->getButtonClick(L3))
	{
	}
	// Circle + PS 
	if(myPS3->getButtonPress(CIRCLE) && myPS3->getButtonClick(PS))
	{
	}

// =======================================================================================
// Cross Button Combos 
// =======================================================================================
	// Cross + Up - Spin rings
	if(myPS3->getButtonPress(CROSS) && myPS3->getButtonClick(UP))
	{
	
	wobble(100);
	}
	// Cross + Down - Spin rings
	if(myPS3->getButtonPress(CROSS) && myPS3->getButtonClick(DOWN))
	{
	spinTLBL(500);
	}
	// Cross + Left - Spin rings
	if(myPS3->getButtonPress(CROSS) && myPS3->getButtonClick(LEFT))
	{
	spinTRBL(250);
	}
	// Cross + Right - Spin rings
	if(myPS3->getButtonPress(CROSS) && myPS3->getButtonClick(RIGHT))
	{
	spinTLBR(150);
	}
	// Cross + Circle
	if(myPS3->getButtonPress(CROSS) && myPS3->getButtonClick(CIRCLE))
	{
	}
	// Cross + L1 
	if(myPS3->getButtonPress(CROSS) && myPS3->getButtonClick(L1))
	{
	}
	// Cross + L2 
	if(myPS3->getButtonPress(CROSS) && myPS3->getButtonClick(L2))
	{
	}
	// Cross + L3 
	if(myPS3->getButtonPress(CROSS) && myPS3->getButtonClick(L3))
	{
	}
	// Cross + PS 
	if(myPS3->getButtonPress(CROSS) && myPS3->getButtonClick(PS))
	{
		if (isStickEnabled)
		{
        myDFPlayer.play(187); // disable sound (187)
        Drive=DriveStop;
        Rotate=RotateStop;
        Strafe=StrafeStop;  
        ServoDrive.writeMicroseconds(Drive);
        ServoRotate.writeMicroseconds(Rotate);
        ServoStrafe.writeMicroseconds(Strafe);
	    isFootMotorStopped = true;
		isStickEnabled = false;
        StickY = 0; 
        StickX = 0; 
		} else
		{
        myDFPlayer.play(188); // Enable sound (188)
		isStickEnabled = true;
		}	
	}

// =======================================================================================
// PS Button Combos 
// =======================================================================================
	// PS + Up 
	if(myPS3->getButtonPress(PS) && myPS3->getButtonClick(UP))
	{
	BodyExpandTrigger=1;
	myDFPlayer.play(11);
	}
	// PS + Down
	if(myPS3->getButtonPress(PS) && myPS3->getButtonClick(DOWN))
	{
	BodyTuckInTrigger=1;
	myDFPlayer.play(11);
	}
	// PS + Left - 
	if(myPS3->getButtonPress(PS) && myPS3->getButtonClick(LEFT))
	{
	wobble(150);
	}
	// PS + Right 
	if(myPS3->getButtonPress(PS) && myPS3->getButtonClick(RIGHT))
	{
	spinTLBR(400);
	myDFPlayer.play(11);
	}
	// PS + Circle -
	if(myPS3->getButtonPress(PS) && myPS3->getButtonClick(CIRCLE))
	{
	}
	// PS + Cross - Automation routine toggle, Off, Low, High
	if(myPS3->getButtonPress(PS) && myPS3->getButtonClick(CROSS))
	{
	if (animationTrigger == 0) {
		animationTrigger = 1;            
		animationStep = 1;
		animateDelay=0;
		animatecurrentmillis = millis();
		animateprevmillis = animatecurrentmillis;
		Serial.print ("Animation");
        Serial.println (animationTrigger);
		} 
		else if (animationTrigger == 1) {
		animationTrigger = 2;
		animationStep = 1;
		animateDelay=0;
		resetTriggers();
		Serial.print ("Animation");
        Serial.println (animationTrigger);
		} else {
		animationTrigger = 0;
		animateDelay=0;
		resetTriggers();
		animationStep = 0;
		Serial.print ("Animation");
        Serial.println (animationTrigger);
		}
	}
	// PS + L1 
	if(!myPS3->getButtonPress(L2) && myPS3->getButtonPress(PS) && myPS3->getButtonClick(L1))
	{
	}
	// PS + L2 
	if(!myPS3->getButtonPress(L1) && myPS3->getButtonPress(PS) && myPS3->getButtonClick(L2))
	{
	}
	// PS + L3 Toggle Drive Overspeed
	if (myPS3->getButtonPress(L3) && myPS3->getButtonPress(PS) && isStickEnabled)
	{
		if ((millis() - previousSpeedToggleMillis) > 1000)
		{
		speedToggleButtonCounter = 0;
		previousSpeedToggleMillis = millis();
		}
		speedToggleButtonCounter += 1;
		if (speedToggleButtonCounter == 1)
		{
			if (!overSpeedSelected)
			{
				overSpeedSelected = true;
				myDFPlayer.play(16); // Enable sound (16)
			} else {
				overSpeedSelected = false;
				myDFPlayer.play(15); // Diable sound (15)
			}
		}
	}

// =======================================================================================
// L1 + L2 Button Combos 
// =======================================================================================
	// L1 + L2 + Up
	if(myPS3->getButtonPress(L2) && myPS3->getButtonPress(L1) && myPS3->getButtonClick(UP))
	{
	myDFPlayer.play(17);
	//Serial2.print("0");  // future lighting test
	}
	//  L1 + L2 + Down
	if(myPS3->getButtonPress(L2) && myPS3->getButtonPress(L1) && myPS3->getButtonClick(DOWN))
	{
	myDFPlayer.play(18);
	//Serial2.print("1");  // future lighting test
	}
	// L1 + L2 + Left
	if(myPS3->getButtonPress(L2) && myPS3->getButtonPress(L1) && myPS3->getButtonClick(LEFT))
	{
	myDFPlayer.play(20);
	}
	// L1 + L2 + Right 
	if(myPS3->getButtonPress(L2) && myPS3->getButtonPress(L1) && myPS3->getButtonClick(RIGHT))
	{
	myDFPlayer.play(21);
	}
	// L1 + L2 + Circle 
	if(myPS3->getButtonPress(L2) && myPS3->getButtonPress(L1) && myPS3->getButtonClick(CIRCLE))
	{
	myDFPlayer.play(22);
	}
	// (L1 + L2 + Cross) 
	if(myPS3->getButtonPress(L2) && myPS3->getButtonPress(L1) && myPS3->getButtonClick(CROSS))
	{
	myDFPlayer.play(23);
	}
	// (L1 + L2 + PS) 
	if(myPS3->getButtonPress(L2) && myPS3->getButtonPress(L1) && myPS3->getButtonClick(PS))
	{
	myDFPlayer.play(14);
	}

// =======================================================================================
// L2 Button Combos 
// =======================================================================================
	// L2 + Up
	if(!myPS3->getButtonPress(L1) && myPS3->getButtonPress(L2) && myPS3->getButtonClick(UP))
	{
    BodyExpandTrigger=1;
	HeadUpTrigger=1;
	}
	// L2 + Down 
	if(!myPS3->getButtonPress(L1) && myPS3->getButtonPress(L2) && myPS3->getButtonClick(DOWN))
	{
	LevelHeadTrigger=1;
	CenterHeadTrigger=1;
	BodyTuckInTrigger=1;
	HeadDownTrigger=1;
	}
	// L2 + Left 
	if(!myPS3->getButtonPress(L1) && myPS3->getButtonPress(L2) && myPS3->getButtonClick(LEFT))
	{
          //Serial.print("FL:");
          //Serial.print(FLSpread);
          //Serial.print(" BR:");
          //Serial.print(BRSpread);
          //Serial.print(" FR:");
          //Serial.print(FRSpread);
          //Serial.print(" BL:");
          //Serial.println(BLSpread);
		  //Serial.println("Activating Spread Servos");
		  FootServoPowerUp();
		  ServoFLSpread.writeMicroseconds(FLSpreadMin);  // Move the spread servos
          ServoBRSpread.writeMicroseconds(BRSpreadMin);
          ServoFRSpread.writeMicroseconds(FRSpreadMax);
          ServoBLSpread.writeMicroseconds(BLSpreadMax);
	}
	// L2 + Right 
	if(!myPS3->getButtonPress(L1) && myPS3->getButtonPress(L2) && myPS3->getButtonClick(RIGHT))
	{
          //Serial.print("FL:");
          //Serial.print(FLSpread);
          //Serial.print(" BR:");
          //Serial.print(BRSpread);
          //Serial.print(" FR:");
          //Serial.print(FRSpread);
          //Serial.print(" BL:");
          //Serial.println(BLSpread);
		  //Serial.println("Activating Spread Servos");
		  FootServoPowerUp();
		  //Serial.println("Moving Spread Servos");
          ServoFLSpread.writeMicroseconds(FLSpreadMax);  // Move the spread servos
          ServoBRSpread.writeMicroseconds(BRSpreadMax);
          ServoFRSpread.writeMicroseconds(FRSpreadMin);
          ServoBLSpread.writeMicroseconds(BLSpreadMin);
	}
	// L2 + Circle 
	if(!myPS3->getButtonPress(L1) && myPS3->getButtonPress(L2) && myPS3->getButtonClick(CIRCLE))
	{
	}
	// L2 + Cross 
	if(!myPS3->getButtonPress(L1) && myPS3->getButtonPress(L2) && myPS3->getButtonClick(CROSS))
	{
	}
	// L2 + PS 
	if(!myPS3->getButtonPress(L1) && myPS3->getButtonPress(L2) && myPS3->getButtonClick(PS))
	{
	}

// =======================================================================================
// L1 Button Combos 
// =======================================================================================
	// L1 + Up 
	if(myPS3->getButtonPress(L1) && myPS3->getButtonClick(UP) && !myPS3->getButtonPress(L2))
	{
		myDFPlayer.play(1);
	}
	// L1 + Down 
	if(myPS3->getButtonPress(L1) && myPS3->getButtonClick(DOWN) && !myPS3->getButtonPress(L2))
	{
		myDFPlayer.play(2);
	}
	// L1 + Left
	if(myPS3->getButtonPress(L1) && myPS3->getButtonClick(LEFT) && !myPS3->getButtonPress(L2))
	{
		myDFPlayer.play(5);
	}
	// L1 + Right 
	if(myPS3->getButtonPress(L1) && myPS3->getButtonClick(RIGHT) && !myPS3->getButtonPress(L2))
	{
		myDFPlayer.play(6);
	}
	// L1 + Circle 
	if(myPS3->getButtonPress(L1) && myPS3->getButtonClick(CIRCLE) && !myPS3->getButtonPress(L2))
	{
		myDFPlayer.play(10);
	}
	// L1 + Cross 
	if(myPS3->getButtonPress(L1) && myPS3->getButtonClick(CROSS) && !myPS3->getButtonPress(L2))
	{
		myDFPlayer.play(13);
	}
	// L1 + PS 
	if(!myPS3->getButtonPress(L2) && myPS3->getButtonPress(L1) && myPS3->getButtonClick(PS))
	{
	myDFPlayer.play(23);
	}
	// L1 + L3 
	if(!myPS3->getButtonPress(L2) && myPS3->getButtonPress(L1) && myPS3->getButtonClick(L3))
	{
	}

// =======================================================================================red
// Arrow Controls
// =======================================================================================
	// Up - 
	if (PS3Nav->getButtonClick(UP) && !PS3Nav->getButtonPress(CROSS) && !PS3Nav->getButtonPress(CIRCLE) && !PS3Nav->getButtonPress(L1) && !PS3Nav->getButtonPress(L2) && !PS3Nav->getButtonPress(PS)) {
	if (soundNum == 0) {
		soundNum = 1;
		soundcurrentmillis = millis();
		soundprevmillis = soundcurrentmillis;
	}
	}
	// Down  - relax
	if (PS3Nav->getButtonClick(DOWN) && !PS3Nav->getButtonPress(CROSS) && !PS3Nav->getButtonPress(CIRCLE) && !PS3Nav->getButtonPress(L1) && !PS3Nav->getButtonPress(L2) && !PS3Nav->getButtonPress(PS)) {
	resetTriggers();
	animationTrigger=0;
	soundNum=0;
	relax();
	}
	// Left - 
	if (PS3Nav->getButtonClick(LEFT) && !PS3Nav->getButtonPress(CROSS) && !PS3Nav->getButtonPress(CIRCLE) && !PS3Nav->getButtonPress(L1) && !PS3Nav->getButtonPress(L2) && !PS3Nav->getButtonPress(PS)) {
	myDFPlayer.play(4);
	}
	// Right - 
	if (PS3Nav->getButtonClick(RIGHT) && !PS3Nav->getButtonPress(CROSS) && !PS3Nav->getButtonPress(CIRCLE) && !PS3Nav->getButtonPress(L1) && !PS3Nav->getButtonPress(L2) && !PS3Nav->getButtonPress(PS)) {
	myDFPlayer.play(7);
	}
}

// =======================================================================================
// Functions - Called from various locations
// =======================================================================================

void toggleSettings()
{
	if (PS3Nav->PS3NavigationConnected) ps3ToggleSettings(PS3Nav);
}

void FootServoPowerUp()
{
    if (footServosPowered == false)
	{
	digitalWrite(Pin1FeetServoAttached, HIGH);
	digitalWrite(Pin2FeetServoAttached, HIGH);
	footServosPowered = true;
	}
	footServoMillis = currentMillis;
	Serial.println("Foot Height Servos Attached");
}
 
void footServoPowerDown()
{
	if (footServosPowered == true)   // disconnects servos & middles after 10 seconds of inactivity
	{
   	   if (currentMillis - footServoMillis > 10000)
	   {
		FLHeight = (FLHeightMin + FLHeightMax)/2;
		FRHeight = (FRHeightMin + FRHeightMax)/2;
		BRHeight = (BRHeightMin + BRHeightMax)/2;
		BLHeight = (BLHeightMin + BLHeightMax)/2;
	    ServoFLHeight.writeMicroseconds(FLHeight);  // Move the spread servos
        ServoBRHeight.writeMicroseconds(BRHeight);
        ServoFRHeight.writeMicroseconds(FRHeight);
        ServoBLHeight.writeMicroseconds(BLHeight);
		delay(500);
		footServosPowered = false;
		digitalWrite(Pin1FeetServoAttached, LOW);  // Disables power to feet servos
		digitalWrite(Pin2FeetServoAttached, LOW);  // Disables power to feet servos
		//Serial.println("Foot Height Servos Detached and values reset");
	   }
	//Serial.print("Foot Height Servos Countdown Timer: ");
	//Serial.println(currentMillis - footServoMillis);
    }
}

void animationSounds()     // to be replaced with routine
{
	soundcurrentmillis = millis();  // update timer
	soundTimer = soundcurrentmillis - soundprevmillis;
	if (soundTimer > 15000 && soundNum == 1)
	{
		myDFPlayer.play(2);
		soundNum =2;
	}
	if (soundTimer > 35000 && soundNum == 2)
	{ 
		myDFPlayer.play(3);
		soundNum =3;
	}
	if (soundTimer > 60000 && soundNum == 3)
	{ 
		myDFPlayer.play(5);
		soundNum =4;
	}
	if (soundTimer > 85000 && soundNum == 4)
	{ 
		myDFPlayer.play(13);
		soundNum =5;
	}
	if (soundTimer > 115000 && soundNum == 5)
	{ 
		myDFPlayer.play(9);
		soundNum =6;
	}
	if (soundTimer > 145000 && soundNum == 6)
	{ 
		myDFPlayer.play(4);
		soundNum =7;
	}
	if (soundTimer > 185000 && soundNum == 7)
	{
		myDFPlayer.play(10);
		soundprevmillis = soundcurrentmillis;
		soundNum =1;
	}
	    //Serial.println("Animation Sound: ");
		//Serial.println(soundNum);
}

void animations()   //Loops through the possible routines and triggers them set, executes one granular move until the animation is complete and the trigger reset
{
centerHead();
levelHead();
tiltHeadUp();
tiltHeadDown();
tiltHeadLeft();
tiltHeadRight();
raiseHead();
midHead();
lowerHead();
expandBody();  
tuckInBody();
turnHead();
delay(1);          // very small delay added in to slow animations
}

// =======================================================================================
// Animation Routines
// =======================================================================================
	//
	//***Body triggers***
	// BodyExpandTrigger=1;  BodyTuckInTrigger=1;
	//
	//***Head Z***
	//HeadUpTrigger=1; HeadMidTrigger=1; HeadDownTrigger=1; 
	//
	//***head triggers***
	//  CenterHeadTrigger=1; LevelHeadTrigger=1; TiltHeadUpTrigger=1; TiltHeadDownTrigger=1; TiltHeadLeftTrigger=1; TiltHeadRightTrigger=1;
	//  TurnHeadTrigger=1; + turnAmount=amount; 
	//
	//***ring subroutines***
	//  spinTRBR(Delay); spinTLBL(Delay);  spinTLBR(Delay); spinTRBL(Delay); wobble (delay);

void animation1() // Head Only Movements
{
	animatecurrentmillis = millis();  // update timer
	animateTimer = animatecurrentmillis - animateprevmillis;
	if (animateTimer > animateDelay && animationStep == 1)
	{
		//What to trigger for this step
		HeadUpTrigger=1;
		CenterHeadTrigger=1;
		secondsInAnimation = 3; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 2)
	{ 
		TiltHeadUpTrigger=1;
		secondsInAnimation = 2; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 3)
	{ 
		TiltHeadLeftTrigger=1;
		secondsInAnimation = 1; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
		
	}
	if (animateTimer > animateDelay && animationStep == 4)
	{ 
		LevelHeadTrigger=1;
		secondsInAnimation = 2; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 5)
	{ 
		turnAmount=1350;
		TiltHeadUpTrigger=1;
		TurnHeadTrigger=1;
		secondsInAnimation = 2; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 6)
	{ 
		turnAmount=1750;
		TurnHeadTrigger=1;
		secondsInAnimation = 2; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 7)
	{ 
		turnAmount=1600;
		TurnHeadTrigger=1;
		TiltHeadLeftTrigger=1;
		secondsInAnimation = 2; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 8)
	{ 
		turnAmount=1750;
		TurnHeadTrigger=1;
		TiltHeadRightTrigger=1;
		secondsInAnimation = 2; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 9)
	{ 
		turnAmount=1340;
		TurnHeadTrigger=1;
		LevelHeadTrigger=1;
		secondsInAnimation = 3; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 10)
	{ 
		CenterHeadTrigger=1;
		secondsInAnimation = 3; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 11)
	{ 
		TiltHeadUpTrigger=1;
		secondsInAnimation = 3; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 12)
	{ 
		LevelHeadTrigger=1;
		turnAmount=1400;
		TurnHeadTrigger=1;
		HeadMidTrigger=1;
		secondsInAnimation = 2; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 13)
	{ 
		turnAmount=1600;
		TurnHeadTrigger=1;
		secondsInAnimation = 2; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 14)
	{ 
		turnAmount=1600;
		TurnHeadTrigger=1;
		TiltHeadRightTrigger=1;
		HeadUpTrigger=1;
		secondsInAnimation = 2; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 15)
	{ 
		turnAmount=1700;
		TurnHeadTrigger=1;
		TiltHeadLeftTrigger=1;
		secondsInAnimation = 1; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 16)
	{ 
		HeadMidTrigger=1;
		LevelHeadTrigger=1;
		turnAmount=1400;
		TurnHeadTrigger=1;
		secondsInAnimation = 2; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 17)
	{ 
		HeadUpTrigger=1;
		secondsInAnimation = 1; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 18)
	{ 
		HeadMidTrigger=1;
		turnAmount=1600;
		TurnHeadTrigger=1;
		secondsInAnimation = 2; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 19)
	{ 
		CenterHeadTrigger=1;
		HeadUpTrigger=1;
		LevelHeadTrigger=1;
		secondsInAnimation = 4; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 20)
	{
		HeadDownTrigger=1;
		secondsInAnimation = 4;// How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 21)
	{
		HeadUpTrigger=1;
		secondsInAnimation = 30;// How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	
	if (animateTimer > animateDelay && animationStep == 22) 
	{
		animateDelay=0;
		animationStep = 1;
		animatecurrentmillis = millis();
		animateprevmillis = animatecurrentmillis;
		BodyExpandTrigger=0;
		BodyTuckInTrigger=0;
		HeadUpTrigger=0;
		HeadMidTrigger=0;
		HeadDownTrigger=0; 
		CenterHeadTrigger=0;
		LevelHeadTrigger=0;
		TiltHeadUpTrigger=0;
		TiltHeadDownTrigger=0;
		TiltHeadLeftTrigger=0;
		TiltHeadRightTrigger=0;
		TurnHeadTrigger=0;
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	//Serial.println("Animation Low");
}
	

void animation2()   // Head and other movements
{
	animatecurrentmillis = millis();  // update timer
	animateTimer = animatecurrentmillis - animateprevmillis;
	if (animateTimer > animateDelay && animationStep == 1)
	{
		//What to trigger for this step
		BodyExpandTrigger=1;
		HeadUpTrigger=1;
		CenterHeadTrigger=1;
		secondsInAnimation = 5; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 2)
	{ 
		TiltHeadUpTrigger=1;
		secondsInAnimation = 2; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 3)
	{ 
		TiltHeadLeftTrigger=1;
		secondsInAnimation = 1; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
		
	}
	if (animateTimer > animateDelay && animationStep == 4)
	{ 
		LevelHeadTrigger=1;
		spinTRBL(400);
		secondsInAnimation = 2; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 5)
	{ 
		turnAmount=1350;
		TiltHeadUpTrigger=1;
		TurnHeadTrigger=1;
		secondsInAnimation = 2; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 6)
	{ 
		turnAmount=1750;
		TurnHeadTrigger=1;
		secondsInAnimation = 2; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 7)
	{ 
		turnAmount=1600;
		TurnHeadTrigger=1;
		TiltHeadLeftTrigger=1;
		secondsInAnimation = 2; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 8)
	{ 
		turnAmount=1750;
		TurnHeadTrigger=1;
		TiltHeadRightTrigger=1;
		secondsInAnimation = 2; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 9)
	{ 
		turnAmount=1340;
		TurnHeadTrigger=1;
		LevelHeadTrigger=1;
		secondsInAnimation = 3; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 10)
	{ 
		CenterHeadTrigger=1;
		secondsInAnimation = 3; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 11)
	{ 
		TiltHeadUpTrigger=1;
		secondsInAnimation = 3; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 12)
	{ 
		LevelHeadTrigger=1;
		turnAmount=1400;
		TurnHeadTrigger=1;
		HeadMidTrigger=1;
		secondsInAnimation = 2; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 13)
	{ 
		turnAmount=1500;
		TurnHeadTrigger=1;
		secondsInAnimation = 2; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 14)
	{ 
		turnAmount=1600;
		TurnHeadTrigger=1;
		TiltHeadRightTrigger=1;
		HeadUpTrigger=1;
		secondsInAnimation = 2; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 15)
	{ 
		turnAmount=1700;
		TurnHeadTrigger=1;
		TiltHeadLeftTrigger=1;
		secondsInAnimation = 1; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 16)
	{ 
		HeadMidTrigger=1;
		LevelHeadTrigger=1;
		secondsInAnimation = 2; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 17)
	{ 
		HeadUpTrigger=1;
		secondsInAnimation = 1; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 18)
	{ 
		HeadMidTrigger=1;
		secondsInAnimation = 2; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 19)
	{ 
		CenterHeadTrigger=1;
		HeadUpTrigger=1;
		LevelHeadTrigger=1;
		secondsInAnimation = 5; // How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 20)
	{
		BodyTuckInTrigger=1;
		HeadDownTrigger=1;
		secondsInAnimation = 30;// How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	if (animateTimer > animateDelay && animationStep == 21)
	{
		BodyTuckInTrigger=1;
		HeadUpTrigger=1;
		secondsInAnimation = 10;// How long should it take
		animationStep++;        // This increments the step
		animateDelay = (animateDelay + (1000 * secondsInAnimation));  // This sets the next trigger time
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	
	if (animateTimer > animateDelay && animationStep == 22) 
	{
		animateDelay=0;
		animationStep = 1;
		animatecurrentmillis = millis();
		animateprevmillis = animatecurrentmillis;
		BodyExpandTrigger=0;
		BodyTuckInTrigger=0;
		HeadUpTrigger=0;
		HeadMidTrigger=0;
		HeadDownTrigger=0; 
		CenterHeadTrigger=0;
		LevelHeadTrigger=0;
		TiltHeadUpTrigger=0;
		TiltHeadDownTrigger=0;
		TiltHeadLeftTrigger=0;
		TiltHeadRightTrigger=0;
		TurnHeadTrigger=0;
		Serial.print ("Step:");
        Serial.print (animationStep);
		Serial.print ("  Delay:");
        Serial.println (animateDelay/1000);
	}
	//Serial.println("Animation High");
}

void resetTriggers()
{
BodyExpandTrigger=0;
BodyTuckInTrigger=0;
HeadUpTrigger=0;
HeadMidTrigger=0;
HeadDownTrigger=0; 
CenterHeadTrigger=0;
LevelHeadTrigger=0;
TiltHeadUpTrigger=0;
TiltHeadDownTrigger=0;
TiltHeadLeftTrigger=0;
TiltHeadRightTrigger=0;
TurnHeadTrigger=0;
}

void turnHead()
{
	if (TurnHeadTrigger) {
	CenterHeadTrigger=0;
	if (HeadT < turnAmount) { 
		HeadT++; 
		ServoHeadT.writeMicroseconds(HeadT);   // loop HeadT to deg
		}
	if (HeadT > turnAmount) {
		HeadT--;
		ServoHeadT.writeMicroseconds(HeadT);   // loop HeadT to deg
		}
	if (HeadT == turnAmount) {TurnHeadTrigger=0;}
	Serial.print ("Turning Head to: ");
    Serial.println (turnAmount);
	}
}

void centerHead()
{
  if (CenterHeadTrigger) {
	TurnHeadTrigger = 0;
	turnAmount=HeadTCenter;
	if (HeadT < HeadTCenter) { 
		HeadT++;
		ServoHeadT.writeMicroseconds(HeadT);   // loop HeadT to center
		}
	if (HeadT > HeadTCenter) {
		HeadT--;
		ServoHeadT.writeMicroseconds(HeadT);   // loop HeadT to center
		}
	 if (HeadT == HeadTCenter) {CenterHeadTrigger=0;}
	 	Serial.println ("Centering Head");
  }
}

void levelHead()
{
  if (LevelHeadTrigger) {
	TiltHeadUpTrigger=0;
	TiltHeadDownTrigger=0;
	TiltHeadLeftTrigger=0;
	TiltHeadRightTrigger=0;
	if (HeadRight < HeadRightCenter) {
	HeadRight++;
	ServoHeadRight.writeMicroseconds(HeadRight);   // loop HeadRight to center
	}
	if (HeadRight > HeadRightCenter) {
	HeadRight--;
    ServoHeadRight.writeMicroseconds(HeadRight);   // loop HeadRight to center
	}
	if (HeadLeft < HeadLeftCenter) { 
	HeadLeft++; 
	ServoHeadLeft.writeMicroseconds(HeadLeft);   // loop HeadLeft to center
	}
	if (HeadLeft > HeadLeftCenter) {
	HeadLeft--;
	ServoHeadLeft.writeMicroseconds(HeadLeft);   // loop HeadLeft to center
	}
	if ((HeadLeft == HeadLeftCenter) && (HeadRight == HeadRightCenter)) {LevelHeadTrigger=0;}
	Serial.println ("Leveling Head");
  }
}

void tiltHeadUp()
{
  if (TiltHeadUpTrigger) {
	LevelHeadTrigger=0;
	TiltHeadDownTrigger=0;
	TiltHeadLeftTrigger=0;
	TiltHeadRightTrigger=0;
	if (HeadRight > HeadRightMin) {	
	HeadRight--;
	ServoHeadRight.writeMicroseconds(HeadRight);   // loop HeadRight to HeadRightMin
	}
	if (HeadLeft < HeadLeftMax) {
	HeadLeft++;
	ServoHeadLeft.writeMicroseconds(HeadLeft);   // loop HeadLeft to HeadLeftMax
	}
	if ((HeadLeft == HeadLeftMax) && (HeadRight == HeadRightMin)) {TiltHeadUpTrigger=0;}
	Serial.println ("Tilting Head Up");
  }
}

void tiltHeadDown()
{
  if (TiltHeadDownTrigger) {
	LevelHeadTrigger=0;
	TiltHeadUpTrigger=0;
	TiltHeadLeftTrigger=0;
	TiltHeadRightTrigger=0;
	if (HeadRight < HeadRightMax) {
	HeadRight++;
	ServoHeadRight.writeMicroseconds(HeadRight);   // loop HeadRight to HeadRightMax
	}
	if (HeadLeft > HeadLeftMin) {
	HeadLeft--;
	ServoHeadLeft.writeMicroseconds(HeadLeft);   // loop HeadLeft to HeadLeftMin
	}
	if ((HeadLeft == HeadLeftMin) && (HeadRight == HeadRightMax)) {TiltHeadDownTrigger=0;}
	Serial.println ("Tilting Head Down");
  }
}

void tiltHeadLeft()
{
	if (TiltHeadLeftTrigger) {
	LevelHeadTrigger=0;
	TiltHeadUpTrigger=0;
	TiltHeadDownTrigger=0;
	TiltHeadRightTrigger=0;
	if (HeadRight < HeadRightMax) {
	HeadRight++;
	ServoHeadRight.writeMicroseconds(HeadRight);   // loop HeadRight to HeadRightMax
	}
	if (HeadLeft < HeadLeftMax) {
	HeadLeft++;
	ServoHeadLeft.writeMicroseconds(HeadLeft);   // loop HeadLeft to HeadLeftMin
	}
	if ((HeadLeft == HeadLeftMax) && (HeadRight == HeadRightMax)) {TiltHeadLeftTrigger=0;}
	Serial.println ("Tilting Head Left");
  }
}

void tiltHeadRight()
{
  if (TiltHeadRightTrigger) {
	LevelHeadTrigger=0;
	TiltHeadUpTrigger=0;
	TiltHeadDownTrigger=0;
	TiltHeadLeftTrigger=0;
	if (HeadRight > HeadRightMin) {
	HeadRight--;
	ServoHeadRight.writeMicroseconds(HeadRight);   // loop HeadRight to HeadRightMax
	}
	if (HeadLeft > HeadLeftMin) {
	HeadLeft--;
	ServoHeadLeft.writeMicroseconds(HeadLeft);   // loop HeadLeft to HeadLeftMin
	}
	if ((HeadLeft == HeadLeftMin) && (HeadRight == HeadRightMin)) {TiltHeadRightTrigger=0;}
	Serial.println ("Tilting Head Right");
  }
}

void raiseHead()  // min value is up in this case
{
	if (HeadUpTrigger) {
	HeadDownTrigger=0;
	HeadMidTrigger=0;
	if (HeadZ > HeadZMin) {
	HeadZ = HeadZ - 3 ;
	ServoHeadZ.writeMicroseconds(HeadZ);   // loop Head Z to to min value (opened)
	}
	if (HeadZ <= HeadZMin) {
		HeadZ = HeadZMin;
		HeadUpTrigger=0;}
	Serial.println ("Rasing Head Up");
	}
}

void midHead()
{
    if (HeadMidTrigger) {
	HeadDownTrigger=0;
	HeadUpTrigger=0;
	if (HeadZ < HeadZMid) { 
	HeadZ++;
	ServoHeadZ.writeMicroseconds(HeadZ);   // loop HeadZ to HeadZMid
	}
	if (HeadZ > HeadZMid) {
	HeadZ--;
	}
	if (HeadZ == HeadZMid) {HeadMidTrigger=0;}
	Serial.println ("Head to Middle");
	}
}

void lowerHead()  // max values is down in this case
{
	if (HeadDownTrigger) {
	HeadUpTrigger=0;
	HeadMidTrigger=0;
	if (HeadZ < HeadZMax) {
	HeadZ = HeadZ + 3 ;
	ServoHeadZ.writeMicroseconds(HeadZ);   // loop Head Z to to min value (opened)
	}
	if (HeadZ >= HeadZMax) {
		HeadZ = HeadZMax;
		HeadDownTrigger=0;}
	Serial.println ("Lowering Head Down");
	}
}


void tuckInBody()   // to be replaced with routine
{   
	if (BodyTuckInTrigger) {
	BodyExpandTrigger=0;
    if (BodyZ2 < BodyZ2Max) {
		BodyZ2++;
	ServoBodyZ2.writeMicroseconds(BodyZ2);   // loop body z2 to to max value (closed)
	}
	if (BodyZ1 > BodyZ1Min) {
		BodyZ1--;
	ServoBodyZ1.writeMicroseconds(BodyZ1);   // loop body z1 to min (closed)
	}
	if ((BodyZ1 <= BodyZ1Min) && (BodyZ2 >= BodyZ2Max)) {
		BodyZ1 = BodyZ1Min;
		BodyZ2 = BodyZ2Max;
		BodyTuckInTrigger=0;}
	Serial.println ("Tucking in body");
    }
}

void expandBody()   // to be replaced with routine
{
	//Serial.println("Expand");
	if (BodyExpandTrigger) {
	BodyTuckInTrigger = 0;
    if (BodyZ2 > BodyZ2Min) {
		BodyZ2--;
		ServoBodyZ2.writeMicroseconds(BodyZ2);   // loop body z2 to to max value (closed)
	}
	if (BodyZ1 < BodyZ1Max) {
		BodyZ1++;
		ServoBodyZ1.writeMicroseconds(BodyZ1);   // loop body z1 to min (closed)
	}
	if ((BodyZ1 >= BodyZ1Max) && (BodyZ2 <= BodyZ2Min)) {
		BodyZ1 = BodyZ1Max; 
		BodyZ2 = BodyZ2Min;
		BodyExpandTrigger=0;}
	Serial.println ("Expanding Body");
    }
}

void wobble(int speedDelay)
{  
	Serial.println ("Wobble rings");
	myDFPlayer.play(4);
    digitalWrite(PinUpperMotorA, LOW);
    digitalWrite(PinUpperMotorB, HIGH);
    digitalWrite(PinLowerMotorA, LOW);
    digitalWrite(PinLowerMotorB, HIGH);
    delay(speedDelay);
	digitalWrite(PinUpperMotorA, HIGH);
    digitalWrite(PinUpperMotorB, LOW);
    digitalWrite(PinLowerMotorA, HIGH);
    digitalWrite(PinLowerMotorB, LOW);
    delay(speedDelay);
	digitalWrite(PinUpperMotorA, LOW);
    digitalWrite(PinUpperMotorB, HIGH);
    digitalWrite(PinLowerMotorA, LOW);
    digitalWrite(PinLowerMotorB, HIGH);
    delay(speedDelay);
	digitalWrite(PinUpperMotorA, HIGH);
    digitalWrite(PinUpperMotorB, LOW);
    digitalWrite(PinLowerMotorA, HIGH);
    digitalWrite(PinLowerMotorB, LOW);
    delay(speedDelay);
    digitalWrite(PinUpperMotorA, LOW);
    digitalWrite(PinUpperMotorB, HIGH);
    digitalWrite(PinLowerMotorA, LOW);
    digitalWrite(PinLowerMotorB, HIGH);
    delay(speedDelay);
	digitalWrite(PinUpperMotorA, HIGH);
    digitalWrite(PinUpperMotorB, LOW);
    digitalWrite(PinLowerMotorA, HIGH);
    digitalWrite(PinLowerMotorB, LOW);
    delay(speedDelay);
	digitalWrite(PinUpperMotorA, LOW);
    digitalWrite(PinUpperMotorB, HIGH);
    digitalWrite(PinLowerMotorA, LOW);
    digitalWrite(PinLowerMotorB, HIGH);
    delay(speedDelay);
	digitalWrite(PinUpperMotorA, HIGH);
    digitalWrite(PinUpperMotorB, LOW);
    digitalWrite(PinLowerMotorA, HIGH);
    digitalWrite(PinLowerMotorB, LOW);
    delay(speedDelay);
	digitalWrite(PinUpperMotorA, LOW);
    digitalWrite(PinUpperMotorB, HIGH);
    digitalWrite(PinLowerMotorA, LOW);
    digitalWrite(PinLowerMotorB, HIGH);
    delay(speedDelay);
	digitalWrite(PinUpperMotorA, HIGH);
    digitalWrite(PinUpperMotorB, LOW);
    digitalWrite(PinLowerMotorA, HIGH);
    digitalWrite(PinLowerMotorB, LOW);
    delay(speedDelay);
    digitalWrite(PinUpperMotorA, LOW);
    digitalWrite(PinUpperMotorB, HIGH);
    digitalWrite(PinLowerMotorA, LOW);
    digitalWrite(PinLowerMotorB, HIGH);
    delay(speedDelay);
	digitalWrite(PinUpperMotorA, HIGH);
    digitalWrite(PinUpperMotorB, LOW);
    digitalWrite(PinLowerMotorA, HIGH);
    digitalWrite(PinLowerMotorB, LOW);
    delay(speedDelay);
    digitalWrite(PinUpperMotorA, HIGH);
    digitalWrite(PinUpperMotorB, HIGH);
    digitalWrite(PinLowerMotorA, HIGH);
    digitalWrite(PinLowerMotorB, HIGH);
}


void spinTRBR(int speedDelay) //
{  
    digitalWrite(PinUpperMotorA, HIGH);
    digitalWrite(PinUpperMotorB, LOW);
    digitalWrite(PinLowerMotorA, HIGH);
    digitalWrite(PinLowerMotorB, LOW);
    //Serial.println ("Top Right, Bottom Right");
	delay(speedDelay);
    digitalWrite(PinUpperMotorA, HIGH);
    digitalWrite(PinUpperMotorB, HIGH);
    digitalWrite(PinLowerMotorA, HIGH);
    digitalWrite(PinLowerMotorB, HIGH);
	
}

void spinTLBL(int speedDelay) //
{ 
    digitalWrite(PinUpperMotorA, LOW);
    digitalWrite(PinUpperMotorB, HIGH);
    digitalWrite(PinLowerMotorA, LOW);
    digitalWrite(PinLowerMotorB, HIGH);
    //Serial.println ("Top Left, Bottom Left");
	delay(speedDelay);
    digitalWrite(PinUpperMotorA, HIGH);
    digitalWrite(PinUpperMotorB, HIGH);
    digitalWrite(PinLowerMotorA, HIGH);
    digitalWrite(PinLowerMotorB, HIGH);
	
}

void spinTLBR(int speedDelay) //
{ 
    digitalWrite(PinUpperMotorA, HIGH);
    digitalWrite(PinUpperMotorB, LOW);
    digitalWrite(PinLowerMotorA, LOW);
    digitalWrite(PinLowerMotorB, HIGH);
    //Serial.println ("Top Left, Bottom Right");
	delay(speedDelay);
    digitalWrite(PinUpperMotorA, HIGH);
    digitalWrite(PinUpperMotorB, HIGH);
    digitalWrite(PinLowerMotorA, HIGH);
    digitalWrite(PinLowerMotorB, HIGH);
	
}

void spinTRBL(int speedDelay) //
{ 
	digitalWrite(PinUpperMotorA, LOW);
    digitalWrite(PinUpperMotorB, HIGH);
    digitalWrite(PinLowerMotorA, HIGH);
    digitalWrite(PinLowerMotorB, LOW);
    //Serial.println ("Top Right, Bottom Left");
	delay(speedDelay);
    digitalWrite(PinUpperMotorA, HIGH);
    digitalWrite(PinUpperMotorB, HIGH);
    digitalWrite(PinLowerMotorA, HIGH);
    digitalWrite(PinLowerMotorB, HIGH);
	
}

void relax()   // Move all positions to the middle of the extents
{
	Drive=DriveStop;
    Rotate=RotateStop;
	Strafe=StrafeStop;
	ServoDrive.writeMicroseconds(Drive);
	ServoRotate.writeMicroseconds(Rotate);
	ServoStrafe.writeMicroseconds(Strafe); 
	LevelHeadTrigger=1;
	CenterHeadTrigger=1;
	HeadMidTrigger=1;
	BodyExpandTrigger=1;
	//Serial.println("Relaxed");
}

void centerRelax()   // Move all positions to the middle of the extents
{
	Drive=DriveStop;
    Rotate=RotateStop;
	Strafe=StrafeStop;
	ServoDrive.writeMicroseconds(Drive);
	ServoRotate.writeMicroseconds(Rotate);
	ServoStrafe.writeMicroseconds(Strafe); 
	LevelHeadTrigger=1;
	CenterHeadTrigger=1;
	HeadMidTrigger=1;
	BodyExpandTrigger=1;
	if (footServosPowered == true) {
	FLSpread = (FLSpreadMin + FLSpreadMax)/2;
	FRSpread = (FRSpreadMin + FRSpreadMax)/2;
	BRSpread = (BRSpreadMin + BRSpreadMax)/2;
	BLSpread = (BLSpreadMin + BLSpreadMax)/2;
	FLHeight = (FLHeightMin + FLHeightMax)/2;
	FRHeight = (FRHeightMin + FRHeightMax)/2;
	BRHeight = (BRHeightMin + BRHeightMax)/2;
	BLHeight = (BLHeightMin + BLHeightMax)/2;
	ServoFLSpread.writeMicroseconds(FLSpread);  // Move the spread servos
    ServoBRSpread.writeMicroseconds(BRSpread);
    ServoFRSpread.writeMicroseconds(FRSpread);
    ServoBLSpread.writeMicroseconds(BLSpread);
	}
	//Serial.println("Centered and Relaxed");
}





// =======================================================================================
// PS3 Controller Device Mgt Functions
// =======================================================================================
void onInitPS3Nav()
{
	String btAddress = getLastConnectedBtMAC();
	PS3Nav->setLedOn(LED1);
	isPS3NavigatonInitialized = true;
	badPS3Data = 0;
	if (btAddress == PS3ControllerFootMac || btAddress == PS3ControllerBackupFootMac)
	{
		mainControllerConnected = true;
		WaitingforReconnect = true;
		
	} else
	{
		// Prevent connection from anything but the MAIN controllers
		isFootMotorStopped = true;
		StickY = 0;
		PS3Nav->setLedOff(LED1);
		PS3Nav->disconnect();
		isPS3NavigatonInitialized = false;
		mainControllerConnected = false;
	}
}

String getLastConnectedBtMAC()
{
	String btAddress = "";
	for(int8_t i = 5; i > 0; i--)
	{
		if (Btd.disc_bdaddr[i]<0x10)
		{
			btAddress +="0";
		}
		btAddress += String(Btd.disc_bdaddr[i], HEX);
		btAddress +=(":");
	}
	btAddress += String(Btd.disc_bdaddr[0], HEX);
	btAddress.toUpperCase();
	return btAddress;
}

boolean criticalFaultDetect()
{
	if (PS3Nav->PS3NavigationConnected || PS3Nav->PS3Connected)
	{
		currentTime = millis();
		lastMsgTime = PS3Nav->getLastMessageTime();
		msgLagTime = currentTime - lastMsgTime;
		if (WaitingforReconnect)
		{
			if (msgLagTime < 200)
			{	
				WaitingforReconnect = false;	
			}
			lastMsgTime = currentTime;
		}
		if ( currentTime >= lastMsgTime)
		{
			msgLagTime = currentTime - lastMsgTime;
		} else
		{
			msgLagTime = 0;
		}
		if (msgLagTime > 300 && !isFootMotorStopped)
		{
		//Serial.println("Crirical Fault section 1");
		Drive=DriveStop;
		Rotate=RotateStop;
		Strafe=StrafeStop;  
		ServoDrive.writeMicroseconds(Drive);
		ServoRotate.writeMicroseconds(Rotate);
		ServoStrafe.writeMicroseconds(Strafe);	
		isFootMotorStopped = true;
		StickX = 0; 
        StickY = 0; 
		}
		
		if ( msgLagTime > 10000 )
		{
        //Serial.println("Lag > 10000) ");
		Drive=DriveStop;
		Rotate=RotateStop;
		Strafe=StrafeStop;  
		ServoDrive.writeMicroseconds(Drive);
		ServoRotate.writeMicroseconds(Rotate);
		ServoStrafe.writeMicroseconds(Strafe);
		isFootMotorStopped = true;
		StickX = 0;
    	StickY = 0;
		PS3Nav->disconnect();
		WaitingforReconnect = true;
		return true;
		}
		//Check PS3 Signal Data
		if(!PS3Nav->getStatus(Plugged) && !PS3Nav->getStatus(Unplugged))
		{
		Usb.Task();
		lastMsgTime = PS3Nav->getLastMessageTime();
		if(!PS3Nav->getStatus(Plugged) && !PS3Nav->getStatus(Unplugged))
	    	{
	    		badPS3Data++;
	    		return true;
		    }
		}
		else if (badPS3Data > 0)
		{
			badPS3Data = 0;
		}
		if ( badPS3Data > 10 )
		{
        //Serial.println("Drive Stopped Bad Data >10");
	    Drive=DriveStop;
	    Rotate=RotateStop;
	    Strafe=StrafeStop;  
    	ServoDrive.writeMicroseconds(Drive);
    	ServoRotate.writeMicroseconds(Rotate);
    	ServoStrafe.writeMicroseconds(Strafe);
    	isFootMotorStopped = true;
    	StickX = 0;
        StickY = 0;
    	//PS3Nav->disconnect();
    	//WaitingforReconnect = true;
    	return true;
	    }
	}
	else if (!isFootMotorStopped)
	{
	//Serial.println("Bad data >0 <10 ");
	Drive=DriveStop;
	Rotate=RotateStop;
	Strafe=StrafeStop;  
	ServoDrive.writeMicroseconds(Drive);
	ServoRotate.writeMicroseconds(Rotate);
	ServoStrafe.writeMicroseconds(Strafe);
	isFootMotorStopped = true;
	StickX = 0; 
    StickY = 0; 
	WaitingforReconnect = true;
	return true;
	}
	return false;
}

// =======================================================================================
//           USB Read Function - Supports Main Program Loop
// =======================================================================================
boolean readUSB()
{
	Usb.Task();
	//The more devices we have connected to the USB or BlueTooth, the more often Usb.Task need to be called to eliminate latency.
	if (PS3Nav->PS3NavigationConnected)
	{
		if (criticalFaultDetect())
		{
			//We have a fault condition that we want to ensure that we do NOT process any controller data!!!
			return false;
		}
	} else if (!isFootMotorStopped)
	{
    //Serial.println("Second critical fault section ");
	Drive=DriveStop;
	Rotate=RotateStop;
	Strafe=StrafeStop;  
	ServoDrive.writeMicroseconds(Drive);
	ServoRotate.writeMicroseconds(Rotate);
	ServoStrafe.writeMicroseconds(Strafe);
	isFootMotorStopped = true;
	StickX = 0; 
    StickY = 0; 
	WaitingforReconnect = true;
	}
   	return true;
}
