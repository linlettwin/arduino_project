#include <ezButton.h>


ezButton toggleSwitch(3);  // create ezButton object that attach to pin 7;
#define trigger 5
#define echo 4
#define Relay 6

float time=0,distance=0;
 #include <Keypad.h>
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16,2);
const int trigPin = 7;
const int echoPin = 8;
const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};


byte pin_rows[ROW_NUM] = {0, 12, 13, 9}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {4, 3, 2, 1}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
unsigned long loopCount;
unsigned long startTime;
String msg;
void setup() {
  Serial.begin(9600);
  toggleSwitch.setDebounceTime(50);
  lcd.begin();
      lcd.print("Set Value : ");

// lcd.init();
  lcd.backlight();
Serial.begin(9600);
 
  pinMode(trigger,OUTPUT);
  pinMode(echo,INPUT);
  pinMode(Relay,OUTPUT);

  delay(2000);
}
long getKeypadIntegerMulti()
{
  
  long value = 0;                                // the number accumulator
  long keyvalue;                                     // the key pressed at current moment
  int isnum;
  Serial.println("Enter the digits,press any non-digit to end ");
  Serial.print("You have typed: ");
  do
  {

    keyvalue = keypad.getKey();   
                           // input the key
    isnum = (keyvalue >= '0' && keyvalue <= '9');         // is it a digit?
    if (isnum)
    {
      Serial.print(keyvalue - '0');
      value = value * 10 + keyvalue - '0';
      lcd.setCursor(12,0)     ;          // accumulate the input number
      lcd.print(value);
    }

  } while (isnum || !keyvalue);                          // until not a digit or while no key pressed
  //
  Serial.println(" "); 
  Serial.print("Returning from funtion: "); 
  Serial.println(value);
  return value;

}//getKeypadInteger
void loop() {
   
  toggleSwitch.loop(); // MUST call the loop() function first

  int state = toggleSwitch.getState();
  if (state == LOW)
  {
     long  keyvalue = keypad.getKey();   

   int val= getKeypadIntegerMulti(); 
    //  lcd.print(val);
  
    int millisecond = 10000 * (val / 100);
         digitalWrite(trigger,LOW);
         delayMicroseconds(2);
         digitalWrite(trigger,HIGH);
         delayMicroseconds(10);
         digitalWrite(trigger,LOW);
         delayMicroseconds(2);
         time=pulseIn(echo,HIGH);
        
         
         distance=time/29/2;
         Serial.println(distance);
  
     if(distance<=8 && keyvalue == "#")
     {
        digitalWrite(Relay,HIGH);
     }
     else 
     {
        digitalWrite(Relay,LOW); 
     }
     delay(500);

      
  }
  else
   digitalWrite(Relay,LOW); 
  

}