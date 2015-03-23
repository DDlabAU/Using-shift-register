//Code made to control two tpic6b959 by shifting out bytes out by using hex

//Pin connected to RCK tpic6b959
int latchPin = 2;
//Pin connected to SRCK of tpic6b959
int clockPin = 13;
//Pin connected to SER IN of tpic6b959
int dataPin = 11;
//Pin connected to SRCLR of tpic6b959
int clearPin = 4;

//holders for information you're going to pass to shifting function
byte firstRegister; //wich register are we talking to
byte seconRegister;
byte dataArray1[10]; //animation array where [10] is the numer of bytes in the array
byte dataArray2[10];
byte dataArrayTechno[2];
byte dataArrayPong[9];
//____________________________SETUP____________________________//
void setup() 
{
	//set pins to output because they are addressed in the main loop
	pinMode(latchPin, OUTPUT);
	pinMode(clearPin, OUTPUT);
	Serial.begin(9600);

	//Arduino doesn't seem to have a way to write binary straight into the code 
	//so these values are in HEX.  Decimal would have been fine, too. 
	//Use http://www.mathsisfun.com/binary-decimal-hexadecimal-converter.html to convert from binary to hex and ad 0x in front
	dataArray1[0] = 0xFF; //11111111
	dataArray1[1] = 0xFE; //11111110
	dataArray1[2] = 0xFC; //11111100
	dataArray1[3] = 0xF8; //11111000
	dataArray1[4] = 0xF0; //11110000
	dataArray1[5] = 0xE0; //11100000
	dataArray1[6] = 0xC0; //11000000
	dataArray1[7] = 0x80; //10000000
	dataArray1[8] = 0x00; //00000000
	dataArray1[9] = 0xE0; //11100000

	dataArray2[0] = 0xFF; //11111111
	dataArray2[1] = 0x7F; //01111111
	dataArray2[2] = 0x3F; //00111111
	dataArray2[3] = 0x1F; //00011111
	dataArray2[4] = 0x0F; //00001111
	dataArray2[5] = 0x07; //00000111
	dataArray2[6] = 0x03; //00000011
	dataArray2[7] = 0x01; //00000001
	dataArray2[8] = 0x00; //00000000
	dataArray2[9] = 0x07; //00000111

	dataArrayTechno[0] = 0xAA;//10101010
	dataArrayTechno[1] = 0x55;//01010101

	dataArrayPong[0] = 0x00; //00000000
	dataArrayPong[1] = 0x01; //10000000
	dataArrayPong[2] = 0x02; //01000000
	dataArrayPong[3] = 0x04; //00100000
	dataArrayPong[4] = 0x08; //00010000
	dataArrayPong[5] = 0x10; //00001000
	dataArrayPong[6] = 0x20; //00000100
	dataArrayPong[7] = 0x40; //00000010
	dataArrayPong[8] = 0x80; //00000001

	digitalWrite(clearPin, LOW); //wipes the shiftregisters
	digitalWrite(clearPin, HIGH);
	//function that blinks all the LEDs
	//gets passed the number of blinks and the pause time
	blinkAll_2Bytes(2,500); 
}


//____________________________LOOP____________________________//
void loop() {
	pingpong(50);
	pingpong(20);
	pingpong(20);
	pingpong(50);
	blinkAll_2Bytes(10,100);
	techno(50, 50);
	dataArrayAnimation();
	random_animation(20,100);
  
}

//____________________________SHIFTING BYTES____________________________//
// the heart of the program
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first, 
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights. 
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    //if the value passed to myDataOut and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else {	
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
}



//____________________________ANIMATIONS____________________________//
void dataArrayAnimation()
{
	for (int j = 0; j < 10; j++) 
  	{
	    //load the light sequence you want from array
	    firstRegister = dataArray1[j];
	    seconRegister = dataArray2[j];
	    //ground latchPin and hold low for as long as you are transmitting
	    digitalWrite(latchPin, 0);
	    //move 'em out
	    shiftOut(dataPin, clockPin, seconRegister);   
	    shiftOut(dataPin, clockPin, firstRegister);
	    //return the latch pin high to signal chip that it 
	    //no longer needs to listen for information
	    digitalWrite(latchPin, 1);
	    delay(300);
	}	
}

void pingpong(int pause)
{
	for(int i=1; i<=16; i++)
	{
	    if(i<9)
	    {
	       firstRegister = dataArrayPong[i];
	       seconRegister = dataArrayPong[0]; 
	    }
	    else
	    {
			firstRegister = dataArrayPong[0];
	    	seconRegister = dataArrayPong[i-8];
	    }
	    digitalWrite(latchPin, 0);
	    //move 'em out
	    shiftOut(dataPin, clockPin, seconRegister);   
	    shiftOut(dataPin, clockPin, firstRegister);

	    digitalWrite(latchPin, 1);
	    delay(pause);
	}
	for(int j=16; j>=1; j--)
	{
	    if(j<9)
	    {
	       firstRegister = dataArrayPong[j];
	       seconRegister = dataArrayPong[0]; 
	    }
	    else
	    {
			firstRegister = dataArrayPong[0];
	    	seconRegister = dataArrayPong[j-8];
	    }
	    digitalWrite(latchPin, 0);
	    //move 'em out
	    shiftOut(dataPin, clockPin, seconRegister);   
	    shiftOut(dataPin, clockPin, firstRegister);

	    digitalWrite(latchPin, 1);
	    delay(pause);
	}
}

void random_animation(int run_times, int pause)
{
	for(int i=0; i<run_times; i++)
	{
		firstRegister = dataArray1[random(0, 11)];
	    seconRegister = dataArray2[random(0, 11)];
	    //ground latchPin and hold low for as long as you are transmitting
	    digitalWrite(latchPin, 0);
	    //move 'em out
	    shiftOut(dataPin, clockPin, seconRegister);   
	    shiftOut(dataPin, clockPin, firstRegister);
	    //return the latch pin high to signal chip that it 
	    //no longer needs to listen for information
	    digitalWrite(latchPin, 1);
	    delay(pause);
	}
}


void techno(int run_times, int pause)
{
	for(int i=0; i<run_times; i++)
    {
		firstRegister = dataArrayTechno[0];
		seconRegister = dataArrayTechno[0];
		digitalWrite(latchPin, 0);
		shiftOut(dataPin, clockPin, seconRegister);
		shiftOut(dataPin, clockPin, firstRegister);

		digitalWrite(latchPin, 1);

		delay(pause);

		firstRegister = dataArrayTechno[1];
		seconRegister = dataArrayTechno[1];
		digitalWrite(latchPin, 0);
		shiftOut(dataPin, clockPin, seconRegister);
		shiftOut(dataPin, clockPin, firstRegister);

		digitalWrite(latchPin, 1);

		delay(pause);
    }
}

//blinks the whole register based on the number of times you want to 
//blink "n" and the pause between them "d"
//starts with a moment of darkness to make sure the first blink
//has its full visual effect.
void blinkAll_2Bytes(int n, int d) {
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, 0);
  shiftOut(dataPin, clockPin, 0);
  digitalWrite(latchPin, 1);
  delay(200);
  for (int x = 0; x < n; x++) {
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 255);
    shiftOut(dataPin, clockPin, 255);
    digitalWrite(latchPin, 1);
    delay(d);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 0);
    shiftOut(dataPin, clockPin, 0);
    digitalWrite(latchPin, 1);
    delay(d);
  }
}


/*
MODDED TO USE TPIC6B959 REGISTER DONE BY NIELS BAK
ORIGINAL FROM http://arduino.cc/en/Tutorial/ShftOut23
//**************************************************************/
//  Name    : shiftOutCode, Predefined Dual Array Style         //
//  Author  : Carlyn Maw, Tom Igoe                              //
//  Date    : 25 Oct, 2006                                      //
//  Version : 1.0                                               //
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255                            //
//****************************************************************
