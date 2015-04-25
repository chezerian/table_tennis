//**************************************************************************************************
//                                 BIG FONT (4-line) LCD CHARACTERS 
//                                   Adrian Jones, February 2015
//
//**************************************************************************************************
 
// Build 1
//   r1 150214 - initial build with glyphs and big font character table in program memory
//   r2 150227 - added RTC support
//********************************************************************************************
#define build 1
#define revision 2
//********************************************************************************************
 
#include <avr/pgmspace.h>            // for memory storage in program space

#include <Wire.h> 
#include <LCD.h>                     // Standard lcd library
#include <LiquidCrystal.h>       // library for I@C interface

#define LCD_RS 12
#define LCD_EN 11
#define LCD_D4 5
#define LCD_D5 4
#define LCD_D6 3
#define LCD_D7 2
// include the library


// initialize the interface pins
LiquidCrystal lcd(LCD_RS,LCD_EN,LCD_D4,LCD_D5,LCD_D6,LCD_D7); // Create the lcd object
//Pins for the LCD are SCL A5, SDA A4
 
const char custom[][8] PROGMEM = {
     {0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0F, 0x1F},      // char 1: bottom right triangle
     {0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F},      // char 2: bottom block
     {0x00, 0x00, 0x00, 0x00, 0x10, 0x1C, 0x1E, 0x1F},      // char 3: bottom left triangle
     {0x1F, 0x0F, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00},      // char 4: top right triangle
     {0x1F, 0x1E, 0x1C, 0x10, 0x00, 0x00, 0x00, 0x00},      // char 5: top left triangle
     {0x1F, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00},      // char 6: upper block
     {0x1F, 0x1F, 0x1E, 0x1C, 0x18, 0x18, 0x10, 0x10}       // char 7: full top right triangle
                                                            // room for another one!
};
 
const char bn[][30] PROGMEM = {                             // organized by row
//         0               1               2               3               4              5               6                7               8               9
    {0x01,0x02,0x03, 0x01,0x02,0xFE, 0x01,0x02,0x03, 0x01,0x02,0x03, 0x02,0xFE,0x02, 0x02,0x02,0x02, 0x01,0x02,0x03, 0x02,0x02,0x02, 0x01,0x02,0x03, 0x01,0x02,0x03},
    {0xFF,0xFE,0xFF, 0xFE,0xFF,0xFE, 0x01,0x02,0xFF, 0xFE,0x02,0xFF, 0xFF,0x02,0xFF, 0xFF,0x02,0x02, 0xFF,0x02,0x03, 0xFE,0x01,0x07, 0xFF,0x02,0xFF, 0xFF,0xFE,0xFF},
    {0xFF,0xFE,0xFF, 0xFE,0xFF,0xFE, 0xFF,0xFE,0xFE, 0xFE,0xFE,0xFF, 0xFE,0xFE,0xFF, 0xFE,0xFE,0xFF, 0xFF,0xFE,0xFF, 0xFE,0xFF,0xFE, 0xFF,0xFE,0xFF, 0x04,0x06,0xFF},
    {0x04,0x06,0x05, 0xFE,0x06,0xFE, 0x06,0x06,0x06, 0x04,0x06,0x05, 0xFE,0xFE,0x06, 0x04,0x06,0x05, 0x04,0x06,0x05, 0xFE,0x06,0xFE, 0x04,0x06,0x05, 0xFE,0xFE,0x06}
};
 
byte col,row,nb=0,bc=0;                                   // general
byte bb[8];                                               // byte buffer for reading from PROGMEM
 
int inPin = 8;   // pushbutton connected to digital pin 7
int val = 0;           // variable to store the value read
int prev_val;

int hr,mn,se ;
 
//*****************************************************************************************//
//                                      Initial Setup
//*****************************************************************************************//
void setup() {
  
  Serial.begin(9600);          //  setup serial
  pinMode(inPin, INPUT);      // sets the digital pin 7 as input

  randomSeed(analogRead(0));
  lcd.write(pgm_read_byte( &bn[row][col]) );
  
  lcd.begin(20, 4);
  for (nb=0; nb<7; nb++ ) {                     // create 8 custom characters
    for (bc=0; bc<8; bc++) bb[bc]= pgm_read_byte( &custom[nb][bc] );
    lcd.createChar ( nb+1, bb );
  }
  lcd.clear();
  lcd.setCursor(4, 0); 
  lcd.print(F("4-Line LARGE")); 
  lcd.setCursor(4, 1); 
  lcd.print(F("TIME DISPLAY")); 
  lcd.setCursor(5, 3); 
  lcd.print(F("V")); 
  lcd.print(build); 
  lcd.print(F(".")); 
  lcd.print(revision);
  lcd.print(F(" "));  
  lcd.print(freeRam()); 
  lcd.print(F("B"));
  printNum(random(0,10),0);
  printNum(random(0,10),17);
  delay(5000);
  lcd.clear();
   hr = 12 ;
 mn = 39;
 se = 58;
 }
 
//*****************************************************************************************//
//                                      MAIN LOOP
//*****************************************************************************************//
void loop() {
 


 val = digitalRead(inPin);   // read the input pin

  if (val==0 && prev_val != 0 )
  {
    hr=hr+1;
  }   
 prev_val=val;
 
 Serial.print(val);             // debug valueyttttt
 Serial.print("\n");

 printNum(hr/10,0);
 printNum(hr%10,3);
 printColon(6);
 printNum(mn/10,7);
 printNum(mn%10,10);
 printColon(13);
 printNum(se/10,14);
 printNum(se%10,17);


 delay(50);                // not strictly necessary
}
 
 
// ********************************************************************************** //
//                                      SUBROUTINES
// ********************************************************************************** //
void printNum(byte digit, byte leftAdjust) 
{
   for(row=0; row<4; row++) 
   {
     lcd.setCursor(leftAdjust,row);               
     for(col=digit*3; col <digit*3+3; col++) 
     {
       lcd.write(pgm_read_byte( &bn[row][col]) );
     }
   }
}
 
void printColon(byte leftAdjust) 
{
   for(row=0; row<4; row++) 
   {
     lcd.setCursor(leftAdjust,row);               
     if(row == 1 || row == 2)    lcd.write(pgm_read_byte( &bn[0][1]) ); else lcd.print(F(" "));
  }
}
// ********************************************************************************** //
//                                      OPERATION ROUTINES
// ********************************************************************************** //
// FREERAM: Returns the number of bytes currently free in RAM  
int freeRam(void) {
  extern int  __bss_end, *__brkval; 
  int free_memory; 
  if((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end); 
  } 
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval); 
  }
  return free_memory; 
}

