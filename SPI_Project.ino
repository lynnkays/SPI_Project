//Include SPI library
#include <SPI.h>

//declare DisplayRow and DisplayColumn
int DisplayRow;
int DisplayColumn;

//declare timer variable
unsigned long Timer;

//Code for handling LED display
//MAX7219 SPI LED Driver
#define MAX7219_TEST   0x0f00 //Display in test mode
#define MAX7219_BRIGHTNESS  0x0a00 //Set brightness of the display
#define MAX7219_SCAN_LIMIT  0x0b00 //Set Scan limit
#define MAX7219_DECODE_MODE 0x0900 //Sets chip to accept bit patterns
#define MAX7219_SHUTDOWN    0x0C00 //Code for shutdown chip


//Initialize the settings for SPI 
SPISettings settingsA(800000, MSBFIRST, SPI_MODE0);

//Method that tells the CS what to do 
void SPI_16 (int data) {
  //Set CS to Low
  digitalWrite(10, LOW);

  //transfers data through SPI
  SPI.transfer16(data);

  //Set CS to HIGH
  digitalWrite(10, HIGH); 
}//end of SPI_16 method

//The following code will only loop once
void setup () {
  //Set CS as an output
 pinMode (10, OUTPUT);

  //Configure SPI Hardware 
  SPI.begin();
  
  //Set up display 
  SPI.beginTransaction(settingsA);
  SPI_16(MAX7219_TEST + 0x01); //Turn on all the LEDS
  delay (100);   //delay 100 ms  
  SPI_16(MAX7219_TEST + 0x00); // Turn the LEDS off
  SPI_16(MAX7219_DECODE_MODE + 0x00); //Disable BCD mode
  SPI_16(MAX7219_BRIGHTNESS + 0x03); //Use lower intensity
  SPI_16(MAX7219_SCAN_LIMIT + 0x0f); //Scan all digits
  SPI_16(MAX7219_SHUTDOWN + 0x01);  //Turn on chip
  SPI.endTransaction();

  DisplayColumn = 0; //initilize column to 0
  DisplayRow = 0xc5; //Change the initialization value from 0 to 0xc5

  Timer = millis(); //Have the timer variable equal millis() function
}//end of setup

//The following code will loop continously
void loop() {
  //if 500 ms have passed
  if (millis() - Timer >= 500) {
    DisplayColumn ++; //add 1 to the column variable

  //if DisplayColumn is greater than 8
  if (DisplayColumn > 8) {
    DisplayColumn = 1; //change the value of the variable to 1
   
  } //end of if statement

  
  //if bit 7 of DisplayRow is set
  if (DisplayRow & 0x80) {
    //Shift Display row 1 bit to the left
    DisplayRow = DisplayRow << 1;
    
  } else {
    //Shift display 1 bit to the left 
    DisplayRow = DisplayRow <<1;
    //Set bottom bit (|1)
    DisplayRow = DisplayRow |= 1;
    
  }
  //Make sure the DisplayRow only affects the lower 8 or data bits
  DisplayRow &= 0x00ff;

  //Start the SPI 
  SPI.beginTransaction(settingsA);
  //Have the SPI place column number in bits 11 - 8 and display the pattern in the bottom 8 bits
  SPI_16((DisplayColumn << 8) + DisplayRow);
  //end of SPI
  SPI.endTransaction();
//Reset the timer
  Timer += 500;
    
  }//end of if statement
}//end of loop



