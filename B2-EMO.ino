//                          Last Revised Date: 02/12/2023
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
#include <SoftwareSerial.h>    
#include <Servo.h>
#include <DFRobotDFPlayerMini.h>


// ---------------------------------------------------------------------------------------
// General User Settings
// ---------------------------------------------------------------------------------------
String PS3ControllerFootMac = "00:26:43:CF:E9:8F";   //Set this to your FOOT PS3 controller MAC address
String PS3ControllerBackupFootMac = "00:07:04:B7:98:7D";  //Set to the MAC Address of your BACKUP FOOT controller (Optional)
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
int HeadLMin = 1000;
int HeadLMax = 2000;
int HeadLCenter = 1500;
int HeadRMin = 1000;
int HeadRMax = 2000;
int HeadRCenter = 1650;
int HeadZMin = 200;
int HeadZMax = 1925;
int HeadTMin = 200;
int HeadTMax = 2800;
int HeadTCenter = 1400;
int BodyZ1Min = 1100;
int BodyZ1Max = 1900;
int BodyZ2Min = 1000;
int BodyZ2Max = 1950;
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

int Drive = 1500;
int Rotate = 1500;
int Strafe = 1500;
int HeadL = HeadLCenter;
int HeadR = HeadRCenter;
int HeadT = HeadTCenter;
int HeadZ = 1200;
int BodyZ1 = 1500;
int BodyZ2 = 1500;
int FLSpread = 1500;
int FRSpread = 1500;
int BRSpread = 1500;
int BLSpread = 1500;
int FLHeight = 1500;
int FRHeight = 1500;
int BRHeight = 1500;
int BLHeight = 1500;

// ---------------------------------------------------------------------------------------
// Servo and Motor Pin Setup
// ---------------------------------------------------------------------------------------

#define PinDrive 22
#define PinRotate 24
#define PinStrafe 26
#define PinHeadL 28
#define PinHeadR 30
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

// ---------------------------------------------------------------------------------------
// Name the Servos
// ---------------------------------------------------------------------------------------

Servo ServoDrive;
Servo ServoRotate;
Servo ServoStrafe;
Servo ServoHeadL;
Servo ServoHeadR;
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
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX Could be moved to serial 1 or 2 on mega
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
boolean footServosAttached = false;

unsigned long soundprevmillis = 0;    // storage for sound timer
unsigned long soundcurrentmillis = 0; // storage for sound timer
unsigned long soundTimer;             // storage for sound timer
int soundNum;                         // Sound number

int animate = 0; 
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
	Serial.begin(115200);             // For debugging, in each section can uncomment serial outputs
	while (!Serial);
	if (Usb.Init() == -1)
	{
	Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
	}
	Serial.print(F("\r\nBluetooth Library Started"));
	
  //setup for DF Mini player
  mySoftwareSerial.begin(9600);       //Setup Serial for DF player
  myDFPlayer.begin(mySoftwareSerial); //Use softwareSerial to communicate with dfplayer
  myDFPlayer.volume(volume);          //Set dfplayer volume value above
  myDFPlayer.play(16);                //Play the 21st mp3 as a startup sound
  
  //Setup for serial 3
  Serial3.begin(115200);            //Serial3:: Used for lighting to arduino nano
  
  //Setup for PS3
  PS3Nav->attachOnInit(onInitPS3Nav); // onInitPS3Nav is called upon a new connection

// =======================================================================================
// Attach Servos  - inital connection of all Servos
// =======================================================================================  

ServoDrive.attach(PinDrive);
ServoRotate.attach(PinRotate);
ServoStrafe.attach(PinStrafe);
ServoHeadL.attach(PinHeadL);
ServoHeadR.attach(PinHeadR);
ServoHeadZ.attach(PinHeadZ);
ServoHeadT.attach(PinHeadT);;
ServoBodyZ1.attach(PinBodyZ1);
ServoBodyZ2.attach(PinBodyZ2);

// =======================================================================================
// Set Initial Servo Positions
// =======================================================================================  
ServoDrive.writeMicroseconds(Drive);
ServoRotate.writeMicroseconds(Rotate);
ServoStrafe.writeMicroseconds(Strafe);
ServoHeadL.writeMicroseconds(HeadL);
ServoHeadR.writeMicroseconds(HeadR);
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
digitalWrite(PinUpperMotorA, HIGH);  //Write High (Disable) to all 4 motor controller pins
digitalWrite(PinUpperMotorB, HIGH);  
digitalWrite(PinLowerMotorA, HIGH);  
digitalWrite(PinLowerMotorB, HIGH);  

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
	footServoDetach(); // Looks to see if it has be 5 seconds since last foot height adjustment
	currentMillis = millis();
	if (soundNum >0) { animationSounds(); }
	if (animate == 1) { animationLow(); }
	if (animate == 2) { animationHigh(); }
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
          if ((StickY-(1500 - Drive/5.26))>(ramping+1)) {
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
         if (myPS3->getButtonPress(L1) && !myPS3->getButtonPress(L2) && !myPS3->getButtonPress(L3))  // L1 and Stick - HeadZ / HeadTurn
          {
          //Serial.print("Head Z/T ");
          HeadZ = HeadZ + (StickY/2);
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
          else if (myPS3->getButtonPress(L2) && !myPS3->getButtonPress(L1) && !myPS3->getButtonPress(L3))  // L2 and Stick - Head L & R Servo 
           {
           Serial.print("Head R/L ");
		   HeadR = HeadR - ((StickY*-1) + (StickX))/2;                     // New method for incrementing with stick movement but staying in place
		   HeadL = HeadL - ((StickY) + (StickX))/2;                        // New method for incrementing with stick movement but staying in place
           //HeadR = ((1500 - (12*StickY*-1)) + (1500 + (12*StickX*-1)))/2; // Old method for moving with stick
           //HeadL = ((1500 - (12*StickY)) + (1500 - (12*StickX)))/2;       // Old method for moving with stick
           if (HeadL > HeadLMax) HeadL=HeadLMax;
           if (HeadL < HeadLMin) HeadL=HeadLMin;
           if (HeadR > HeadRMax) HeadR=HeadRMax;
           if (HeadR < HeadRMin) HeadR=HeadRMin;
           ServoHeadL.writeMicroseconds(HeadL);
           ServoHeadR.writeMicroseconds(HeadR);
           //Serial.print("HeadL: ");
           //Serial.print(HeadL); 
           //Serial.print(" HeadR: ");
           //Serial.println(HeadR); 
          } 
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
          else if (myPS3->getButtonPress(L1) && myPS3->getButtonPress(L2) && myPS3->getButtonPress(L3))  // Stick L1+L2+L3 Feet control for direction leaning 
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
		  FootServoAttach();
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
		raiseHead();
	}
	// Circle + Down 
	if(myPS3->getButtonPress(CIRCLE) && myPS3->getButtonClick(DOWN))
	{
		lowerHead();
	}
	// Circle + Left 
	if(myPS3->getButtonPress(CIRCLE) && myPS3->getButtonClick(LEFT))
	{
		centerHead();
	}
	// Circle + Right 
	if(myPS3->getButtonPress(CIRCLE) && myPS3->getButtonClick(RIGHT))
	{
		levelHead();
	}
	// Circle + Cross 
	if(myPS3->getButtonPress(CIRCLE) && myPS3->getButtonClick(CROSS))
	{
	}
	// Circle + L1  
	if(myPS3->getButtonPress(CIRCLE) && myPS3->getButtonClick(L1))
	{
	}
	// Circle + L2 
	if(myPS3->getButtonPress(CIRCLE) && myPS3->getButtonClick(L2))
	{
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
	digitalWrite(PinUpperMotorA, LOW);
    digitalWrite(PinUpperMotorB, HIGH);
    digitalWrite(PinLowerMotorA, HIGH);
    digitalWrite(PinLowerMotorB, LOW);
    myDFPlayer.play(11);
	delay(700);
    digitalWrite(PinUpperMotorA, HIGH);
    digitalWrite(PinUpperMotorB, HIGH);
    digitalWrite(PinLowerMotorA, HIGH);
    digitalWrite(PinLowerMotorB, HIGH);
    //  if (volume < 28) volume = volume +3;
	//myDFPlayer.volume(volume);  // Volume up
	}
	// Cross + Down - Spin rings
	if(myPS3->getButtonPress(CROSS) && myPS3->getButtonClick(DOWN))
	{
    digitalWrite(PinUpperMotorA, HIGH);
    digitalWrite(PinUpperMotorB, LOW);
    digitalWrite(PinLowerMotorA, LOW);
    digitalWrite(PinLowerMotorB, HIGH);
    myDFPlayer.play(11);
	delay(700);
    digitalWrite(PinUpperMotorA, HIGH);
    digitalWrite(PinUpperMotorB, HIGH);
    digitalWrite(PinLowerMotorA, HIGH);
    digitalWrite(PinLowerMotorB, HIGH);
	//if (volume >2) volume = volume -3;
	//myDFPlayer.volume(volume);  //Volume down 
	}
	// Cross + Left - Spin rings
	if(myPS3->getButtonPress(CROSS) && myPS3->getButtonClick(LEFT))
	{
    digitalWrite(PinUpperMotorA, LOW);
    digitalWrite(PinUpperMotorB, HIGH);
    digitalWrite(PinLowerMotorA, LOW);
    digitalWrite(PinLowerMotorB, HIGH);
    myDFPlayer.play(11);
	delay(700);
    digitalWrite(PinUpperMotorA, HIGH);
    digitalWrite(PinUpperMotorB, HIGH);
    digitalWrite(PinLowerMotorA, HIGH);
    digitalWrite(PinLowerMotorB, HIGH);
	}
	// Cross + Right - Spin rings
	if(myPS3->getButtonPress(CROSS) && myPS3->getButtonClick(RIGHT))
	{
    digitalWrite(PinUpperMotorA, HIGH);
    digitalWrite(PinUpperMotorB, LOW);
    digitalWrite(PinLowerMotorA, HIGH);
    digitalWrite(PinLowerMotorB, LOW);
    myDFPlayer.play(11);
	delay(700);
    digitalWrite(PinUpperMotorA, HIGH);
    digitalWrite(PinUpperMotorB, HIGH);
    digitalWrite(PinLowerMotorA, HIGH);
    digitalWrite(PinLowerMotorB, HIGH);
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
	// PS + Up - B2 Expands
	if(myPS3->getButtonPress(PS) && myPS3->getButtonClick(UP))
	{
		expandBody();
		raiseHead();
		levelHead();
		centerHead();
	}
	// PS + Down  - B2 Tucks-In
	if(myPS3->getButtonPress(PS) && myPS3->getButtonClick(DOWN))
	{
		levelHead();
		centerHead();
		lowerHead();
		tuckInBody();

	}
	// PS + Left - Toggles automatic sounds
	if(myPS3->getButtonPress(PS) && myPS3->getButtonClick(LEFT))
	{
		if (soundNum == 0) {
			soundNum = 1;
			soundprevmillis = soundcurrentmillis;
			} else {soundNum = 0;}
	}
	// PS + Right 
	if(myPS3->getButtonPress(PS) && myPS3->getButtonClick(RIGHT))
	{
	}
	// PS + Circle -
	if(myPS3->getButtonPress(PS) && myPS3->getButtonClick(CIRCLE))
	{
	}
	// PS + Cross - Automation routine toggle, Off, Low, High
	if(myPS3->getButtonPress(PS) && myPS3->getButtonClick(CROSS))
	{
			if (animate == 0) {
			animate = 1;
			} else if (animate == 1) {
			animate = 2;
			} else {animate = 0;}	
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
				myDFPlayer.play(20); // Enable sound (20)
			} else {
				overSpeedSelected = false;
				myDFPlayer.play(21); // Diable sound (21)
			}
		}
	}

// =======================================================================================
// L1 + L2 Button Combos 
// =======================================================================================
	// L1 + L2 + Up
	if(myPS3->getButtonPress(L2) && myPS3->getButtonPress(L1) && myPS3->getButtonClick(UP))
	{
	}
	//  L1 + L2 + Down
	if(myPS3->getButtonPress(L2) && myPS3->getButtonPress(L1) && myPS3->getButtonClick(DOWN))
	{
	}
	// L1 + L2 + Left
	if(myPS3->getButtonPress(L2) && myPS3->getButtonPress(L1) && myPS3->getButtonClick(LEFT))
	{
	}
	// L1 + L2 + Right 
	if(myPS3->getButtonPress(L2) && myPS3->getButtonPress(L1) && myPS3->getButtonClick(RIGHT))
	{
	}
	// L1 + L2 + Circle 
	if(myPS3->getButtonPress(L2) && myPS3->getButtonPress(L1) && myPS3->getButtonClick(CIRCLE))
	{
	}
	// (L1 + L2 + Cross) 
	if(myPS3->getButtonPress(L2) && myPS3->getButtonPress(L1) && myPS3->getButtonClick(CROSS))
	{
	}
	// (L1 + L2 + PS) 
	if(myPS3->getButtonPress(L2) && myPS3->getButtonPress(L1) && myPS3->getButtonClick(PS))
	{
	}

// =======================================================================================
// L2 Button Combos 
// =======================================================================================
	// L2 + Up
	if(!myPS3->getButtonPress(L1) && myPS3->getButtonPress(L2) && myPS3->getButtonClick(UP))
	{
    expandBody();
	}
	// L2 + Down 
	if(!myPS3->getButtonPress(L1) && myPS3->getButtonPress(L2) && myPS3->getButtonClick(DOWN))
	{
	tuckInBody();
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
		  Serial.println("Activating Spread Servos");
		  ServoFLSpread.attach(PinFLSpread);  // Enable the spread servos
          ServoFRSpread.attach(PinFRSpread);
          ServoBRSpread.attach(PinBRSpread);
		  ServoBLSpread.attach(PinBLSpread);
		  Serial.println("Moving Spread Servos");
          ServoFLSpread.writeMicroseconds(FLSpreadMin);  // Move the spread servos
          ServoBRSpread.writeMicroseconds(BRSpreadMin);
          ServoFRSpread.writeMicroseconds(FRSpreadMax);
          ServoBLSpread.writeMicroseconds(BLSpreadMax);
		  Serial.println("Pausing for expand movement");
		  delay(1500);             // wait for the feet to move
          Serial.println("Deativating Spread Servos");
		  ServoFLSpread.detach();  // Disable the spread servos (to save power)
          ServoFRSpread.detach();
          ServoBRSpread.detach();
		  ServoBLSpread.detach();


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
		  Serial.println("Activating Spread Servos");
		  ServoFLSpread.attach(PinFLSpread);  // Enable the spread servos
          ServoFRSpread.attach(PinFRSpread);
          ServoBRSpread.attach(PinBRSpread);
		  ServoBLSpread.attach(PinBLSpread);
		  Serial.println("Moving Spread Servos");
          ServoFLSpread.writeMicroseconds(FLSpreadMax);  // Move the spread servos
          ServoBRSpread.writeMicroseconds(BRSpreadMax);
          ServoFRSpread.writeMicroseconds(FRSpreadMin);
          ServoBLSpread.writeMicroseconds(BLSpreadMin);
		  Serial.println("Pausing for contract movement");
		  delay(1500);             // wait for the feet to move
          Serial.println("Deativating Spread Servos");
		  ServoFLSpread.detach();  // Disable the spread servos (to save power)
          ServoFRSpread.detach();
          ServoBRSpread.detach();
		  ServoBLSpread.detach();

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
	}
	// L1 + L3 
	if(!myPS3->getButtonPress(L2) && myPS3->getButtonPress(L1) && myPS3->getButtonClick(L3))
	{
	}

// =======================================================================================
// Arrow Controls
// =======================================================================================
	// Up - 
	if (PS3Nav->getButtonClick(UP) && !PS3Nav->getButtonPress(CROSS) && !PS3Nav->getButtonPress(CIRCLE) && !PS3Nav->getButtonPress(L1) && !PS3Nav->getButtonPress(L2) && !PS3Nav->getButtonPress(PS)) {
	myDFPlayer.play(12);
	}
	// Down  - 
	if (PS3Nav->getButtonClick(DOWN) && !PS3Nav->getButtonPress(CROSS) && !PS3Nav->getButtonPress(CIRCLE) && !PS3Nav->getButtonPress(L1) && !PS3Nav->getButtonPress(L2) && !PS3Nav->getButtonPress(PS)) {
	myDFPlayer.play(2);
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

void FootServoAttach()
{
    if (footServosAttached == false)
	{
	ServoFLHeight.attach(PinFLHeight);
    ServoFRHeight.attach(PinFRHeight);
    ServoBRHeight.attach(PinBRHeight);
    ServoBLHeight.attach(PinBLHeight);
	footServosAttached = true;
	}
	footServoMillis = currentMillis;
	//Serial.print("Foot Height Servos Attached, Timer: ");
	Serial.println(footServoMillis-currentMillis);
}
 
void footServoDetach()
{
	if (footServosAttached == true)   // disconnects servos & middles after 5000 ms of inactivity
	{
   	   if (currentMillis - footServoMillis > 5000)
	   {
		FLHeight = (FLHeightMin + FLHeightMax)/2;
		FRHeight = (FRHeightMin + FRHeightMax)/2;
		BRHeight = (BRHeightMin + BRHeightMax)/2;
		BLHeight = (BLHeightMin + BLHeightMax)/2;
        ServoFLHeight.writeMicroseconds(FLHeight);
        ServoFRHeight.writeMicroseconds(FRHeight);
        ServoBRHeight.writeMicroseconds(BRHeight);
    	ServoBLHeight.writeMicroseconds(BLHeight);
		delay(1000);
		ServoFLHeight.detach();
   		ServoFRHeight.detach();
   		ServoBRHeight.detach();
   		ServoBLHeight.detach();
	   footServosAttached = false;
	   //Serial.println("Foot Height Servos Detached");
	   }
    }
}

void animationSounds()     // to be replaced with routine
{
	soundcurrentmillis = millis();  // update timer
	soundTimer = soundcurrentmillis - soundprevmillis;
	if (soundTimer > 15000 && soundNum == 1)
	{
		myDFPlayer.play(4);
		soundNum =2;
	}
	if (soundTimer > 35000 && soundNum == 2)
	{ 
		myDFPlayer.play(13);
		soundNum =3;
	}
	if (soundTimer > 60000 && soundNum == 3)
	{ 
		myDFPlayer.play(1);
		soundNum =4;
	}
	if (soundTimer > 85000 && soundNum == 4)
	{ 
		myDFPlayer.play(3);
		soundNum =5;
	}
	if (soundTimer > 115000 && soundNum == 5)
	{ 
		myDFPlayer.play(14);
		soundNum =6;
	}
	if (soundTimer > 145000 && soundNum == 6)
	{ 
		myDFPlayer.play(9);
		soundNum =7;
	}
	if (soundTimer > 185000 && soundNum == 7)
	{
		myDFPlayer.play(11);
		soundprevmillis = soundcurrentmillis;
		soundNum =1;
	}
	    Serial.println("Animation Sound: ");
		Serial.println(soundNum);
}

void animationLow()  // to be replaced with routines - idea is to create building blocks and call them here in order
{
   // if not expanded to full, expand to full
   // if not head raised, raise head
   // look left
   // look forward
   // look left slowly
   // look forward
   // look right
   // look all the way right slowly
   // back to center slowly
   // tilt head around
   // say casa
   // look down
   // look up
   // level head
   // tuck head in
   // wait a bit
   // start loop again
   
   //Serial.println("Animation Low");
}

void animationHigh()   // to be replaced with routines - idea is to create building blocks and call them here in order
{
	// if not expanded to full, expand to full
	// if not head raised, raise head
	// timer - look left
	// look more left slowly
	// slowly back to center
	// say something
	// timer - look right
	// time  - tilt head some direction
	// timer - look more right
	// time  - tilt head more direction
	// timer - look forward
	// spin rings and make a sound
	// timer - lower a half
	// stop spinning rings
	// say something else
	// expand up
	// look left then right, then left, right, forward
	// say something
	// tilt backward and look forward and up
	// say casa, casa, i miss marva
	// wait a short while then tuck in
    animationRelaxed(); // relax
	// wait a while
	// start loop again

	//Serial.println("Animation High");
}

void centerHead()
{
	if (HeadT < HeadTCenter) { 
		for (int i=HeadT; i<HeadTCenter; i++) {
		ServoHeadT.writeMicroseconds(i);   // loop HeadT to to center
		}
	} else {
		for (int i=HeadT; HeadTCenter<i; i--) {
		ServoHeadT.writeMicroseconds(i);   // loop HeadT to center
		}
	}
	HeadT = HeadTCenter;
	ServoHeadT.writeMicroseconds(HeadT);
}

void centerHeadSlow()
{
	if (HeadT < HeadTCenter) { 
		for (int i=HeadT; i<HeadTCenter; i++) {
		ServoHeadT.writeMicroseconds(i);   // loop HeadT to to center
		delay(5);
		}
	} else {
		for (int i=HeadT; HeadTCenter<i; i--) {
		ServoHeadT.writeMicroseconds(i);   // loop HeadT to center
		delay(5);
		}
	}
	HeadT = HeadTCenter;
	ServoHeadT.writeMicroseconds(HeadT);
}

void centerHeadMed()
{
	if (HeadT < HeadTCenter) { 
		for (int i=HeadT; i<HeadTCenter; i++) {
		ServoHeadT.writeMicroseconds(i);   // loop HeadT to to center
		delay(1);
		}
	} else {
		for (int i=HeadT; HeadTCenter<i; i--) {
		ServoHeadT.writeMicroseconds(i);   // loop HeadT to center
		delay(1);
		}
	}
	HeadT = HeadTCenter;
	ServoHeadT.writeMicroseconds(HeadT);
}

void levelHead()
{
	if (HeadL < HeadLCenter) { 
		for (int i=HeadL; i<HeadLCenter; i++) {
		ServoHeadL.writeMicroseconds(i);   // loop HeadT to to center
		}
	} else {
		for (int i=HeadL; HeadLCenter<i; i--) {
		ServoHeadL.writeMicroseconds(i);   // loop HeadT to center
		}
	}
	HeadL = HeadLCenter;
	ServoHeadL.writeMicroseconds(HeadL);

	if (HeadR < HeadRCenter) { 
		for (int i=HeadR; i<HeadRCenter; i++) {
		ServoHeadR.writeMicroseconds(i);   // loop HeadT to to center
		}
	} else {
		for (int i=HeadR; HeadRCenter<i; i--) {
		ServoHeadR.writeMicroseconds(i);   // loop HeadT to center
		}
	}
	HeadR = HeadRCenter;
	ServoHeadR.writeMicroseconds(HeadR);
}

void levelHeadMed()
{
	if (HeadL < HeadLCenter) { 
		for (int i=HeadL; i<HeadLCenter; i++) {
		ServoHeadL.writeMicroseconds(i);   // loop HeadT to to center
		delay(1);
		}
	} else {
		for (int i=HeadL; HeadLCenter<i; i--) {
		ServoHeadL.writeMicroseconds(i);   // loop HeadT to center
		delay(1);
		}
	}
	HeadL = HeadLCenter;
	ServoHeadL.writeMicroseconds(HeadL);

	if (HeadR < HeadRCenter) { 
		for (int i=HeadR; i<HeadRCenter; i++) {
		ServoHeadR.writeMicroseconds(i);   // loop HeadT to to center
		delay(1);
		}
	} else {
		for (int i=HeadR; HeadRCenter<i; i--) {
		ServoHeadR.writeMicroseconds(i);   // loop HeadT to center
		delay(1);
		}
	}
	HeadR = HeadRCenter;
	ServoHeadR.writeMicroseconds(HeadR);
}

void levelHeadSlow()
{
	if (HeadL < HeadLCenter) { 
		for (int i=HeadL; i<HeadLCenter; i++) {
		ServoHeadL.writeMicroseconds(i);   // loop HeadT to to center
		delay(5);
		}
	} else {
		for (int i=HeadL; HeadLCenter<i; i--) {
		ServoHeadL.writeMicroseconds(i);   // loop HeadT to center
		delay(5);
		}
	}
	HeadL = HeadLCenter;
	ServoHeadL.writeMicroseconds(HeadL);

	if (HeadR < HeadRCenter) { 
		for (int i=HeadR; i<HeadRCenter; i++) {
		ServoHeadR.writeMicroseconds(i);   // loop HeadT to to center
		delay(5);
		}
	} else {
		for (int i=HeadR; HeadRCenter<i; i--) {
		ServoHeadR.writeMicroseconds(i);   // loop HeadT to center
		}delay(5);

	}
	HeadR = HeadRCenter;
	ServoHeadR.writeMicroseconds(HeadR);
}

void raiseHead()
{
	for (int i=HeadZ; HeadZMin<i; i--) {
	ServoHeadZ.writeMicroseconds(i);   // loop head Z to to min value (opened)
	}
	HeadZ = HeadZMin;
}

void raiseHeadMed()
{
	for (int i=HeadZ; HeadZMin<i; i--) {
	ServoHeadZ.writeMicroseconds(i);   // loop head Z to to min value (opened)
	delay(1);
	}
	HeadZ = HeadZMin;
}

void raiseHeadSlow()
{
	for (int i=HeadZ; HeadZMin<i; i--) {
	ServoHeadZ.writeMicroseconds(i);   // loop head Z to to min value (opened)
	delay(5);
	}
	HeadZ = HeadZMin;
}

void lowerHead()
{
	for (int i=HeadZ; i<HeadZMax; i++) {
	ServoHeadZ.writeMicroseconds(i);   // loop head Z to to max value (closed)
	}
	HeadZ = HeadZMax;
}

void lowerHeadMed()
{
	for (int i=HeadZ; i<HeadZMax; i++) {
	ServoHeadZ.writeMicroseconds(i);   // loop head Z to to max value (closed)
	delay(1);
	}
	HeadZ = HeadZMax;
}

void lowerHeadSlow()
{
	for (int i=HeadZ; i<HeadZMax; i++) {
	ServoHeadZ.writeMicroseconds(i);   // loop head Z to to max value (closed)
	delay(5);
	}
	HeadZ = HeadZMax;
}


void tuckInBody()   // to be replaced with routine
{
    Serial.println("Tuck In");
	myDFPlayer.play(11);
	for (int i=BodyZ2; i<BodyZ2Max; i++) {
	ServoBodyZ2.writeMicroseconds(i);   // loop body z2 to to max value (closed)
	}
	for (int i=BodyZ1; BodyZ1Min<i; i--) {
	ServoBodyZ1.writeMicroseconds(i);   // loop body z1 to min (closed)
	}
	BodyZ1 = BodyZ1Min;
	BodyZ2 = BodyZ2Max;
}

void tuckInBodyMed()   // to be replaced with routine
{
    Serial.println("Tuck In");
	myDFPlayer.play(11);
	for (int i=BodyZ2; i<BodyZ2Max; i++) {
	ServoBodyZ2.writeMicroseconds(i);   // loop body z2 to to max value (closed)
	delay(1);
	}
	for (int i=BodyZ1; BodyZ1Min<i; i--) {
	ServoBodyZ1.writeMicroseconds(i);   // loop body z1 to min (closed)
	delay(1);
	}
	BodyZ1 = BodyZ1Min;
	BodyZ2 = BodyZ2Max;
}

void tuckInBodySlow()   // to be replaced with routine
{
    Serial.println("Tuck In");
	myDFPlayer.play(11);
	for (int i=BodyZ2; i<BodyZ2Max; i++) {
	ServoBodyZ2.writeMicroseconds(i);   // loop body z2 to to max value (closed)
	delay(5);
	}
	for (int i=BodyZ1; BodyZ1Min<i; i--) {
	ServoBodyZ1.writeMicroseconds(i);   // loop body z1 to min (closed)
	delay(5);
	}
	BodyZ1 = BodyZ1Min;
	BodyZ2 = BodyZ2Max;
}

void expandBody()   // to be replaced with routine
{
	Serial.println("Expand");
	for (int i=BodyZ1; i<BodyZ1Max; i++) {
	ServoBodyZ1.writeMicroseconds(i);   // loop body z2 to to max value (opened)
    }
	for (int i=BodyZ2; BodyZ2Min<i; i--) {
	ServoBodyZ2.writeMicroseconds(i);   // loop body z1 to min (opened)
	}
	BodyZ1 = BodyZ1Max;
	BodyZ2 = BodyZ2Min;
	myDFPlayer.play(11);    
}

void expandBodyMed()   // to be replaced with routine
{
	Serial.println("Expand");
	for (int i=BodyZ1; i<BodyZ1Max; i++) {
	ServoBodyZ1.writeMicroseconds(i);   // loop body z2 to to max value (opened)
    delay(1);
	}
	for (int i=BodyZ2; BodyZ2Min<i; i--) {
	ServoBodyZ2.writeMicroseconds(i);   // loop body z1 to min (opened)
	}delay(1);

	BodyZ1 = BodyZ1Max;
	BodyZ2 = BodyZ2Min;
	myDFPlayer.play(11);    
}

void expandBodySlow()   // to be replaced with routine
{
	Serial.println("Expand");
	for (int i=BodyZ1; i<BodyZ1Max; i++) {
	ServoBodyZ1.writeMicroseconds(i);   // loop body z2 to to max value (opened)
    delay(5);
	}
	for (int i=BodyZ2; BodyZ2Min<i; i--) {
	ServoBodyZ2.writeMicroseconds(i);   // loop body z1 to min (opened)
	delay(5);
	}
	BodyZ1 = BodyZ1Max;
	BodyZ2 = BodyZ2Min;
	myDFPlayer.play(11);    
}

void animationRelaxed()   // Move all positions to the middle of the extents
{
	Drive = 1500;
	Rotate = 1500;
	Strafe = 1500;
	HeadL = HeadLCenter;
	HeadR = HeadRCenter;
	HeadT = HeadTCenter;
	HeadZ = (HeadZMin + HeadZMax)/2;
	BodyZ1 = (BodyZ1Min + BodyZ1Max)/2;
	BodyZ2 = (BodyZ2Min + BodyZ2Max)/2;
	FLSpread = (FLSpreadMin + FLSpreadMax)/2;
	FRSpread = (FRSpreadMin + FRSpreadMax)/2;
	BRSpread = (BRSpreadMin + BRSpreadMax)/2;
	BLSpread = (BLSpreadMin + BLSpreadMax)/2;
	FLHeight = (FLHeightMin + FLHeightMax)/2;
	FRHeight = (FRHeightMin + FRHeightMax)/2;
	BRHeight = (BRHeightMin + BRHeightMax)/2;
	BLHeight = (BLHeightMin + BLHeightMax)/2;
	ServoFLHeight.attach(PinFLHeight);
	ServoFRHeight.attach(PinFRHeight);
	ServoBRHeight.attach(PinBRHeight);
	ServoBLHeight.attach(PinBLHeight);
	ServoFLSpread.attach(PinFLSpread);
	ServoFRSpread.attach(PinFRSpread);
	ServoBRSpread.attach(PinBRSpread);
	ServoBLSpread.attach(PinBLSpread);
	ServoDrive.writeMicroseconds(Drive);
	ServoRotate.writeMicroseconds(Rotate);
	ServoStrafe.writeMicroseconds(Strafe);
	ServoHeadL.writeMicroseconds(HeadL);
	ServoHeadR.writeMicroseconds(HeadR);
	ServoHeadZ.writeMicroseconds(HeadZ);
	ServoHeadT.writeMicroseconds(HeadT);
	ServoBodyZ1.writeMicroseconds(BodyZ1);
	ServoBodyZ2.writeMicroseconds(BodyZ2);
	ServoFLSpread.writeMicroseconds(FLSpread);
	ServoFRSpread.writeMicroseconds(FRSpread);
	ServoBRSpread.writeMicroseconds(BRSpread);
	ServoBLSpread.writeMicroseconds(BLSpread);
	ServoFLHeight.writeMicroseconds(FLHeight);
	ServoFRHeight.writeMicroseconds(FRHeight);
	ServoBRHeight.writeMicroseconds(BRHeight);
	ServoBLHeight.writeMicroseconds(BLHeight);
	delay(1500);
	ServoFLSpread.detach();
   	ServoFRSpread.detach();
   	ServoBRSpread.detach();
   	ServoBLSpread.detach();
	ServoFLHeight.detach();
   	ServoFRHeight.detach();
   	ServoBRHeight.detach();
   	ServoBLHeight.detach();
	footServosAttached = false;
	Serial.println("Relaxed");
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
		ServoStrafe.writeMicroseconds(Strafe);	isFootMotorStopped = true;
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
    //Serial.println("Second critical fauly section ");
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
