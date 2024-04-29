#include <ezButton.h>
#define trigger 9
#define echo 8
#define Relay 6
float time=0,distance=0;

ezButton toggleSwitch(3);  // create ezButton object that attach to pin 7;

void setup() {
  Serial.begin(9600);
  toggleSwitch.setDebounceTime(50); // set debounce time to 50 milliseconds
  pinMode(trigger,OUTPUT);
  pinMode(echo,INPUT);
  pinMode(Relay,OUTPUT);

  delay(2000);
}

void loop() {
  
  toggleSwitch.loop(); // MUST call the loop() function first

  int state = toggleSwitch.getState();
  if (state == LOW)
  {
    
         digitalWrite(trigger,LOW);
         delayMicroseconds(2);
         digitalWrite(trigger,HIGH);
         delayMicroseconds(10);
         digitalWrite(trigger,LOW);
         delayMicroseconds(2);
         time=pulseIn(echo,HIGH);
        
         
         distance=time/29/2;
         Serial.println(distance);
  
     if(distance<=8)
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
 
