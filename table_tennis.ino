//**************************************************************************************************
//      Game point! (table tennis score display)                           
//		BIG FONT (4-line) LCD CHARACTERS  from Adrian Jones, February 2015
//**************************************************************************************************


#define build 1
#define revision 2

//Pins used for LCD
#define LCD_RS 12
#define LCD_EN 11
#define LCD_D4 7
#define LCD_D5 6
#define LCD_D6 5
#define LCD_D7 3

//RAW REMOTE CODES
/*WD REMOTE
#define REM_UP		0x219EA05F
#define REM_DOWN	0x219E00FF
#define REM_LEFT	0x219EE01F
#define REM_RIGHT	0x219E906F

#define REM_0	0x219E8877
#define REM_1	0x219E18E7
#define REM_2	0x219E9867
#define REM_3	0x219E708F
#define REM_4	0x219E38C7
#define REM_5	0x219EB847
#define REM_6	0x219E7A85
#define REM_7	0x219EBA45
#define REM_8	0x219E3AC5
#define REM_9	0x219EFA05

#define REM_RED		0x219EA857
#define REM_GREEN	0x219E28D7
#define REM_BLUE	0x219EE817
#define REM_YELLOW	0x219E6897
*/

//********888888888CHEAP REMOTE*/
#define REM_UP          0x00511DBB
#define REM_LEFT        0x52A3D41F
#define REM_RIGHT       0x20FE4DBB
#define REM_DOWN        0xA3C8EDDB
#define REM_OK          0xD7E84B1B

#define REM_1   0xC101E57B
#define REM_2   0x97483BFB
#define REM_3   0xF0C41643
#define REM_4   0x9716BE3F
#define REM_5   0x3D9AE3F7
#define REM_6   0x6182021B
#define REM_7   0x8C22657B
#define REM_8   0x488F3CBB
#define REM_9   0x0449E79F
#define REM_0   0x1BC0157B

#define NUM_NAME 23
//********************************************************************************************


#include <IRremote.h>		//library for reading IR remote comes from github.com/
#include "/home/chez/git/table_tennis/player/player.h"  		//library used for palyer object
#include <avr/pgmspace.h>   // for memory storage in program space
#include <LiquidCrystal.h>  // library for LCD interface

/*modes
 0       player enter id
 1       player setup
 2       game entry
 3       set view
 */




// initialize the interface pins
LiquidCrystal lcd(LCD_RS,LCD_EN,LCD_D4,LCD_D5,LCD_D6,LCD_D7); // Create the lcd object

//customer glyphs (characters)
const char custom[][8] PROGMEM = 
{
  {0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0F, 0x1F},      // char 1: bottom right triangle
  {0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F},  // char 2: bottom block
  {0x00, 0x00, 0x00, 0x00, 0x10, 0x1C, 0x1E, 0x1F},  // char 3: bottom left triangle
  {0x1F, 0x0F, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00},  // char 4: top right triangle
  {0x1F, 0x1E, 0x1C, 0x10, 0x00, 0x00, 0x00, 0x00},  // char 5: top left triangle
  {0x1F, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00},  // char 6: upper block
  {0x1F, 0x1F, 0x1E, 0x1C, 0x18, 0x18, 0x10, 0x10}  // char 7: full top right triangle
                                                                                  // room for another one!
};

//fonts for digits 0-9 setup as a 4 (rows) by 9x3 array(columns each digit consists of  4 rows and three columns of glyphs)
const char bn[][30] PROGMEM = 
{                             // organized by row
  //         0               1               2               3               4              5               6                7               8               9
  {0x01,0x02,0x03, 0x01,0x02,0xFE, 0x01,0x02,0x03, 0x01,0x02,0x03, 0x02,0xFE,0x02, 0x02,0x02,0x02, 0x01,0x02,0x03, 0x02,0x02,0x02, 0x01,0x02,0x03, 0x01,0x02,0x03},
  {0xFF,0xFE,0xFF, 0xFE,0xFF,0xFE, 0x01,0x02,0xFF, 0xFE,0x02,0xFF, 0xFF,0x02,0xFF, 0xFF,0x02,0x02, 0xFF,0x02,0x03, 0xFE,0x01,0x07, 0xFF,0x02,0xFF, 0xFF,0xFE,0xFF},
  {0xFF,0xFE,0xFF, 0xFE,0xFF,0xFE, 0xFF,0xFE,0xFE, 0xFE,0xFE,0xFF, 0xFE,0xFE,0xFF, 0xFE,0xFE,0xFF, 0xFF,0xFE,0xFF, 0xFE,0xFF,0xFE, 0xFF,0xFE,0xFF, 0x04,0x06,0xFF},
  {0x04,0x06,0x05, 0xFE,0x06,0xFE, 0x06,0x06,0x06, 0x04,0x06,0x05, 0xFE,0xFE,0x06, 0x04,0x06,0x05, 0x04,0x06,0x05, 0xFE,0x06,0xFE, 0x04,0x06,0x05, 0xFE,0xFE,0x06}
};

//built in names of players
char player_names[][8] =
{
  {'B','e','n',' ','C','h','r','n'}  ,
  {'C','h','n','g','C','h','e','n'}  ,
  {'P','e','t','e','B','f','r','d'}  ,
  {'P','h','i','l','L','a','y',' '}  ,
  {'N','r','n','g','P','e','n','n'}  ,
  {'D','a','v','e','G','r','d','n'}  ,
  {'T','o','m',' ','L','i','u',' '}  ,
  {'L','i','a','m','M','n','g','l'}  ,
  {'X','i','n',' ','S','h','i',' '}  ,
  {'Q','i','n','g','L','u',' ',' '}  ,
  {'S','t','p','n','S','k','i','l'}  ,
  {'G','s','t','v','D','o','n','g'}  ,
  {'T','a','r','a','Y','e','e',' '}  ,
  {'A','n','n','e','G','u','a','n'}  ,
  {'H','s','n',' ','M','a','k',' '}  ,
  {'E','m','m','a','C','h','i','u'}  ,
  {'J','u','l','e','K','l','t','y'}  ,
  {'C','o','l','n','S','t','s','n'}  ,
  {'S','a','m',' ','B','r','t','n'}  ,
  {'C','h','r','l','S','t','w','n'}  ,
  {'K','e','v',' ','C','h','r','n'}  ,
  {'L','u','a','t','V','u','n','g'}  ,
  {'J','o','s','h','R','o','s','s'}
};

uint8_t col,row,nb=0,bc=0;                                   // general
uint8_t bb[8];                                               // uint8_t buffer for reading from PROGMEM

long result; //Store IR result

uint8_t mode=0; //mode or state of the program
uint8_t num_sets=3; //number of sets to play

int RECV_PIN = 9; //IR receive pin
IRrecv irrecv(RECV_PIN);
decode_results results; //object to decode IR transmissions

player  player_1=player(); //players uobjects
player  player_2=player();

//*****************************************************************************************//
//                                      Initial Setup
//*****************************************************************************************//
void setup() {

  Serial.begin(9600);          //  setup serial
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Starting Setup");
  randomSeed(analogRead(0)); //initialize a random seed
  lcd.write(pgm_read_byte( &bn[row][col]) );

  lcd.begin(20, 4);
  for (nb=0; nb<7; nb++ ) 
  {  
    // create 8 custom characters and store in HD44780
    for (bc=0; bc<8; bc++) 
      bb[bc]= pgm_read_byte( &custom[nb][bc] );
    lcd.createChar ( nb+1, bb );
  }
  
// DISPLAY screen for game point
  lcd.clear();
  lcd.setCursor(4, 0); 
  lcd.print(F("GAME POINT!")); 
  lcd.setCursor(4, 1); 
  lcd.print(F("SCORE DISPLAY")); 
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
  delay(3000);
  lcd.clear();
  Serial.println("Setup Finished");
}





//*****************************************************************************************//
//                                      MAIN LOOP
//*****************************************************************************************//

void loop() 
{
  
  byte id_1=0;
  byte id_2;
  
  boolean rand_1; // variable to Set side
  boolean rand_2; //Set serve

  rand_1=random(2); //who serves first
  rand_2=random(2);// what side they are on

  
  if (irrecv.decode(&results)) 
  {
    
    Serial.println(results.value, HEX);
    result=results.value & 0xFFFFFFFF; //store the output of the remote (read in raw mode see IR  library for more detail)
    irrecv.resume(); // Receive the next value
  }

  if (mode==0) //enter player ids
  { 
    Serial.println("Mode 0");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(F("Enter id 1"));
    enter_player_id(id_1); //function to read in two digits sent by the remote andtranslate them to a two digit number
    lcd.clear();
    player_1.set_name(player_names[id_1]); 
    lcd.setCursor(0, 3); 
    print_player_name_line(player_1);
    delay(1500);
    
    id_2=id_1+1;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(F("Enter id 2"));
    enter_player_id(id_2);
    player_2.set_name(player_names[id_2]);		
    lcd.setCursor(0, 3); 
    print_player_name_line(player_2);    
    delay(1500);
    
    mode=1;

  }

  
  if (mode==1) // determine and display who is serving and who is on waht side
  { 
    Serial.println("Mode 1");
    if (rand_1)
      init_serve(player_1,player_2);   
    else
      init_serve(player_2,player_1);   
      
    if (rand_2)
      init_side(player_1,player_2);
    else
      init_side(player_2,player_1)    ;
       
    delay(5000);
    
    lcd.clear();      
    
    mode=2; 
  }

  if (mode==2) //game has begun, update game score, set score, side and serve according to input presses
  {
    
    //update game score based on button presse
    //TODO implement undo function that reverts player_1 and player_2 to their previous state by storing a copy of them, when restored player_1 and player_1_back swap values
    switch(result) // depending on waht is read by the remote
    {
    case REM_UP: //increase player one score
      {
        lcd.clear();        
        player_1.game_increment();
        if (determine_change_serve(player_1.read_game_score()+player_2.read_game_score()))
          change_serve(player_1,player_2);
        break;
      }			
    case REM_DOWN: //decrease player one score
      {
        if (player_1.read_game_score()>0)
        {      
          lcd.clear();                        
          if (determine_change_serve(player_1.read_game_score()+player_2.read_game_score()))
            change_serve(player_1,player_2);
          player_1.game_decrement();
        }          
        break;
      }
    case  REM_RIGHT:
      {
        lcd.clear();                
        player_2.game_increment();
        if (determine_change_serve(player_1.read_game_score()+player_2.read_game_score()))
          change_serve(player_1,player_2);
        break;
      }
    case REM_LEFT:
      {
        if (player_2.read_game_score()>0)
        {
          lcd.clear();                
          if (determine_change_serve(player_1.read_game_score()+player_2.read_game_score()))
            change_serve(player_1,player_2);
          player_2.game_decrement();
        }
        break;
      }
    default:
      {
      }
    }
    
	//print palyer scores, the format of the scores depend on the side, serve of the players.
    print_player_scores(player_1,player_2);
    
    if (player_1.read_side())
    {
      print_player_name(player_1,0);
      print_player_name(player_2,17);      
    }
    else    
    {
      print_player_name(player_2,0);
      print_player_name(player_1,17);      
    }

    print_set( player_1, player_2);

    // determine if a set has been won and then update set_score 
    if (game_score(player_1,player_2))
    {
      delay(1000);      
      player_1.set_increment(); 
      change_sides(player_1,player_2);
      
    }
	
    if (game_score(player_2,player_1))
    {
      delay(1000);      
      player_2.set_increment();
      change_sides(player_1,player_2);
      
    }
	
//	Once the number of sets has been reached print the winner
//TODO manually input the number of sets the palyer wants to play
    if (player_1.read_set_score()==num_sets)
    {
      delay(1000);      
      display_winner(player_1,mode);
      reset_game(player_1,player_2);
    }
    if (player_2.read_set_score()==num_sets)
    {
      delay(1000);      
      display_winner(player_2,mode);
      reset_game(player_1,player_2);
    }		
  } //end of mode 2

  
  result=0; //cleanup
  delay(5);                // not strictly necessary
}


// ********************************************************************************** //
//                                      SUBROUTINES
// ********************************************************************************** //


void enter_player_id(byte &id)
{
  lcd.setCursor(4,1);
  lcd.print(F(">"));
  long ir_result=0;
  
  for(int i=1; i<4 ;i++)
      {
        int pos;
        pos=(id+i-1)%NUM_NAME;
        lcd.setCursor(5,i);
        lcd.print(pos);
        Serial.println(i);
        lcd.print(F("   "));
        print_name_line(player_names[pos]); // TODO MAKE NEW FUNCTION print_player_name
      }
      
  while(ir_result != REM_OK)
  {
    Serial.println(ir_result, HEX);
    delay(50);
    if (ir_result==REM_DOWN)
    {
      id=(id+1)%NUM_NAME ;
    }
  
    if (ir_result==REM_UP)
    {
      id=(id-1)%NUM_NAME ;
    }
    
    if (ir_result==REM_UP || ir_result==REM_DOWN)
    {
      for(int i=1; i<4 ;i++)
      {
        int pos;
        pos=(id+i-1)%NUM_NAME;
        Serial.println(id);
        lcd.setCursor(5,i);
        lcd.print(pos);
        lcd.print(F("   "));
        print_name_line(player_names[pos]);
      }
    }
    
    ir_result=0;
    
    if (irrecv.decode(&results))
    {
      ir_result=results.value & 0xFFFFFFFF; //store the output of the remote (read in raw mode see IR  library for more detail)
      
      irrecv.resume(); // Receive the next value
    }
  }
    
}
/*
// function used to read two remote presses and ouput a two digit number

int enter_player_id()
{
//-1 indicates digit has not been set 
  int digit_1=-1; //tens digit
  int digit_0=-1; //units digit
  int remote_digit;
  int ir_result;
  

  while (digit_0==-1) //while units digit not enterd
  {
    remote_digit=-1; // read the remote while both digits not entered
    if (irrecv.decode(&results)) 
    {
      ir_result=results.value & 0xFFFFFFFF; //kepp to four bytes long		
      irrecv.resume(); // Receive the next value
      remote_digit=remote_numpad(ir_result);
      lcd.setCursor(1,3);
    }

    if (digit_1==-1 && remote_digit!=-1) //read second digit when the remote is pressed
    {
      digit_1=remote_digit;
      lcd.setCursor(1,0);
      lcd.print(digit_1);
      remote_digit=-1; //reset s next block doesnt read same IR code
    }
  
    if (digit_0==-1 && remote_digit!=-1) //read second digit when the remote is pressed
    {
      digit_0=remote_digit;
      lcd.setCursor(2,0);
      lcd.print(digit_0);
    }
  }
  return(10*digit_1+digit_0);
}
*/

//convert remote results to 0-9 number
int remote_numpad(int result)
{
    switch(result)
    {
      case REM_0:
       return (0);
      case REM_1:
       return (1);
      case REM_2:
       return (2);
      case REM_3:
       return (3);
      case REM_4:
       return (4);
      case REM_5:
       return (5);
      case REM_6:
       return (6);
      case REM_7:
       return (7);
      case REM_8:
       return (8);
      case REM_9:
       return (9);
      default:
       return -1; 
    }
}

void reset_game(player &player_a, player &player_b)
{
      player_a.reset_game_score();
      player_b.reset_game_score();
      player_a.reset_set_score();
      player_b.reset_set_score();
}

void change_sides(player &player_a, player &player_b)
{
    player_a.reset_game_score();
    player_b.reset_game_score(); 
    player_a.swap_side();
    player_b.swap_side();
	delay(1000);      
}

void change_serve(player &player_a, player &player_b)
{
  player_a.swap_serve();
  player_b.swap_serve();
}

void init_serve(player &player_a, player &player_b)
{
    boolean init=1;
    lcd.clear();							
    lcd.setCursor(0, 0); 
    print_player_name_line(player_a);
    lcd.print(F( " to serve")); 
    player_a.set_serve(init); 			//TODO manual override of side and serve i.e press remote button to swap serve or side
    player_b.set_serve(!init);   
 
}

void init_side(player &player_a, player &player_b)
{
      boolean init=1;
      lcd.setCursor(0, 1); 
      print_player_name_line(player_a);
      lcd.setCursor(0, 2);
      lcd.print(F( "to left side")); 
      player_a.set_side(init);
      player_b.set_side(!init);       
}

boolean determine_change_serve(byte sum_score)
{
  return (
  (sum_score>0 && (sum_score&B1)==0)
    ||sum_score>20
   );
}

void	display_winner(player player_a,uint8_t &mode)
{
  lcd.clear();
  print_player_name_line(player_a);
  lcd.setCursor(0,1);
  lcd.print("wins the match!");
  delay(5000);
  mode=0; //set the actual value to zero not the address
}	

//determine if a set has been won
boolean game_score(player player_a,player player_b)
{
  return 
  (
	 player_a.read_game_score()>10 && 
    (player_a.read_game_score() > player_b.read_game_score()+1)
   );
}

//print name vertically
void print_player_name(player player_a,uint8_t left_adjust) 
{
  for (uint8_t i=0 ; i<8 ; i++)
  {
    lcd.setCursor( ((i>>2)&B1)+left_adjust , (i&B11) ); //column,row
    lcd.print(player_a.read_name(i));                                
  }
}

//print player first name inital and set score below
void print_set(player player_a,player player_b) 
{
  for (uint8_t i=0 ; i<4 ; i++)
  {
    lcd.setCursor(19,i); //column,row
    if (i==0)
      lcd.print(player_a.read_name(0));                              
    if (i==1)
      lcd.print(player_a.read_set_score());                                
    if (i==2)
      lcd.print(player_b.read_name(0));                              
    if (i==3)
      lcd.print(player_b.read_set_score());                                
  }
}

void print_player_scores(player player_a,player player_b) 
{

  uint8_t player_a_score=player_a.read_game_score();
  uint8_t player_b_score=player_b.read_game_score();
  if (player_a.read_side())
  {
    printNum(player_a_score/10,2);
    printNum(player_a_score%10,5);
    printNum(player_b_score/10,11);
    printNum(player_b_score%10,14);
  }
  else
  {  
    printNum(player_b_score/10,2);
    printNum(player_b_score%10,5);
    printNum(player_a_score/10,11);
    printNum(player_a_score%10,14);
  }    

  if ( !((player_a.read_side() ) ^ (player_a.read_serve() ) ) ) //check side and serve to determine which arrow to print (arrow indicates who should serve)
      print_left_Colon(9);
  else
      print_right_Colon(9);
}

//functions used to print a 4 row digit
void printNum(uint8_t digit, uint8_t leftAdjust) 
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

void printColon(uint8_t leftAdjust) 
{
  for(int row=0; row<4; row++)
  {
    lcd.setCursor(leftAdjust,row);
    if(row == 1 || row == 2)    lcd.write(pgm_read_byte( &bn[0][1]) ); 
    else lcd.print(F(" "));
  }
}

void print_left_Colon(uint8_t leftAdjust) 
{
  for(int row=0; row<4; row++)
  {
    lcd.setCursor(leftAdjust,row);
    if(row == 1 || row == 2)    lcd.write(pgm_read_byte( &bn[-3+3*row][0]) ); 
    else lcd.print(F(" "));
  }
}

void print_right_Colon(byte leftAdjust) 
{
  for(int row=0; row<4; row++)
  {
    lcd.setCursor(leftAdjust,row);
    if(row == 1 || row == 2)    lcd.write(pgm_read_byte( &bn[-3+3*row][2]) ); 
    else lcd.print(F(" "));
  }
}

//print name horizontally iwth a space in between the first and last name
void print_player_name_line(player player_a) 
{
  for (uint8_t i=0 ; i<8 ; i++)
  {
    lcd.print(player_a.read_name(i));
    if (i==3)
      lcd.print(F(" "));
  }
}

void print_name_line(char name[8])
{
  for (uint8_t i=0 ; i<8 ; i++)
  {
      lcd.print(name[i]);
//    lcd.print(name+sizeof(name)*i);
    if (i==3)
      lcd.print(F(" "));
  }
}
// ********************************************************************************** //
//                                      OPERATION ROUTINES
// ********************************************************************************** //
// FREERAM: Returns the number of bytes currently free in RAM  

int freeRam(void) 
{
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














