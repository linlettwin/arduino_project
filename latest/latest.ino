#include <ezButton.h>             //button
#include <Keypad.h>
#include <LiquidCrystal_I2C.h> 
const int ROW_NUM = 4;            //four rows
const int COLUMN_NUM = 4;         //four columns
const int trigger = 12;            //ultrasonic
const int echo = 2;
float time=0,distance=10;         //for distance
int relayState;                   
char keys[ROW_NUM][COLUMN_NUM] = {
{'1','2','3', 'A'},
{'4','5','6', 'B'},
{'7','8','9', 'C'},
{'*','0','#', 'D'}
};
LiquidCrystal_I2C lcd(0x27, 20, 4);               // ligne 8 
byte pin_rows[ROW_NUM] = {11, 10, 9, 8};         //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {7, 6, 5, 4};     //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

//int sensorInterrupt = 0;        //interrupt 0
//int sensorPin       = 13;      //Digital Pin 2
//int solenoidValve = 3;        // Digital pin 5
unsigned int SetPoint = 400;   //400 milileter
String code="";
/*The hall-effect flow sensor outputs pulses per second per litre/minute of flow.*/
//float calibrationFactor = 90; //You can change according to your datasheet

//volatile byte pulseCount =0;  

float flowRate = 0.0;
unsigned int flowMilliLitres =0;
unsigned long totalMilliLitres = 0;
double volume=0.0;
double delayTime = 0;
unsigned long oldTime ;
const int relais_moteur = 3;  // the relay is connected to pin 3 of the Adruino board

ezButton toggleSwitch(13);   // create ezButton object that attach to pin 13;
void measure_distance();     

void setup()
{ 
  //switch
  toggleSwitch.setDebounceTime(50);   // set debounce time to 50 milliseconds
  totalMilliLitres = 0;

  //lcd
  lcd.init();                // display initialization
  //lcd.clear();
  //lcd.backlight();        // activate the backlight
  //lcd.setCursor(0, 0);    // stand in the front line
  //lcd.print("Enter Amount:");

  pinMode(relais_moteur , OUTPUT);
  pinMode(trigger,OUTPUT);
  pinMode(echo,INPUT);
  
  // Initialize a serial connection for reporting values to the host
  Serial.begin(9600);
  // pinMode(solenoidValve , OUTPUT);
  //digitalWrite(solenoidValve, HIGH);
  //pinMode(sensorPin, INPUT);
  //digitalWrite(sensorPin, HIGH);
  /*The Hall-effect sensor is connected to pin 2 which uses interrupt 0. Configured to trigger on a FALLING state change (transition from HIGH
  (state to LOW state)*/
  // attachInterrupt(sensorInterrupt, pulseCounter, FALLING); //you can use Rising or Falling
  delay(2000);
}

void loop() 
{

  toggleSwitch.loop(); // MUST call the loop() function first

  int state = toggleSwitch.getState();
  if (state == LOW)
  { 
     Serial.println("On");
     start:
//   lcd.clear();
     lcd.backlight();          // activate the backlight
     lcd.setCursor(0, 0);      // stand in the front line
     lcd.print("Enter Amount:");
   
        char key = keypad.getKey(); 
        //lcd.clear();

        if(key)                   // A key on the keyboard is pressed
        { 
          code+=key;
          lcd.setCursor(0, 1);    // stand on the second line
          lcd.print(code);        // show volume value
          delay(100);
        }
      
        if (key=='*' && state == LOW) 
        {    // if you press the '*' key
        
               if(code.toInt()<=1500)
               {
                  volume=code.toInt();
                  delayTime = (10000 * (volume/230));
               }
               else
               {
                  lcd.clear();
                  lcd.backlight(); 
                  lcd.setCursor(0, 0); 
                  lcd.print("Enter Amount:");  
               }
               code="";
         }
        if(key =='D')
        {
              lcd.clear();
              lcd.backlight(); 
              lcd.setCursor(0, 0); 
              lcd.print("donner volume:");
              code="";  
        }

        if (totalMilliLitres<volume)
        {
            measure_distance();
            Serial.println(delayTime);
            delay(500);
           
            if(distance <=8)
            {
              digitalWrite(relais_moteur, HIGH); // Start the water pump
            
              lcd.clear();
              lcd.backlight(); 
              lcd.setCursor(0, 0);
                 
              lcd.print("Filling ....");
      //      delay(delayTime);
              
              for(int j=1000; j<delayTime;j+=1000)
              {
                measure_distance();
                if(distance<=8 )
                {
                        lcd.clear();
                        lcd.print("Filling ....");
                        digitalWrite(relais_moteur, HIGH);
                 }
                 else
                 {
                        key = keypad.getKey();
          
                        digitalWrite(relais_moteur,LOW);
                        lcd.clear();
                        lcd.print("Place Cup in");
                        lcd.setCursor(0, 1);
                        lcd.print("front of sensor");
                        j-=1000;
                        if(key =='A')
                        {
                          
                          delayTime = 0;
                        }
                  }
                  delay(1000);
              }
              
              digitalWrite(relais_moteur, LOW); // Start the water pump
              lcd.clear();
              lcd.backlight(); 
              lcd.setCursor(0, 0); 
              lcd.print("Done");
              delay(3000);
              lcd.clear();
              lcd.print("Enter amount");
              totalMilliLitres += volume;
      
              relayState=digitalRead(relais_moteur);
              if(relayState == LOW)
               { 
                digitalWrite(relais_moteur, LOW);
               }
          }
          else
          {
              lcd.clear();
              lcd.print("Place Cup in");
              lcd.setCursor(0, 1);
              lcd.print("front of sensor");       
              digitalWrite(relais_moteur, LOW);
       
              //goto start;
         }

  
    } 
    else  
   {
        digitalWrite(relais_moteur, LOW); // Stop the water pump
        volume=0;
        totalMilliLitres = 0;
        
   }
    // delay(3000);
 }
 else if(state == HIGH)
 {
   Serial.println("Off");
   digitalWrite(relais_moteur, LOW);
   lcd.clear();
   lcd.noBacklight();
  
 }


}

 
  


//Insterrupt Service Routine
void measure_distance()
{
 digitalWrite(trigger,LOW);
 delayMicroseconds(2);
 digitalWrite(trigger,HIGH);
 delayMicroseconds(10);
 digitalWrite(trigger,LOW);
 delayMicroseconds(2);
 time=pulseIn(echo,HIGH);
 
 distance=time/29/2;
 Serial.println("distance");
 Serial.println(distance);
} 

void pulseCounter()
{
  // Increment the pulse counter
 // pulseCount++;
}

void SetSolinoidValve()
{
  //digitalWrite(solenoidValve, LOW);
  
}
