#include <Keypad.h>
#include <LiquidCrystal_I2C.h> 
const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns

char keys[ROW_NUM][COLUMN_NUM] = {
{'1','2','3', 'A'},
{'4','5','6', 'B'},
{'7','8','9', 'C'},
{'*','0','#', 'D'}
};
LiquidCrystal_I2C lcd(0x27, 20, 4); // ligne 8 
byte pin_rows[ROW_NUM] = {11, 10, 9, 8}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {7, 6, 5, 4}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

int sensorInterrupt = 0;  // interrupt 0
int sensorPin       = 2; //Digital Pin 2
int solenoidValve = 3; // Digital pin 5
unsigned int SetPoint = 400; //400 milileter
String code="";
/*The hall-effect flow sensor outputs pulses per second per litre/minute of flow.*/
float calibrationFactor = 90; //You can change according to your datasheet

volatile byte pulseCount =0;  

float flowRate = 0.0;
unsigned int flowMilliLitres =0;
unsigned long totalMilliLitres = 0,volume=0;

unsigned long oldTime ;
const int relais_moteur = 3; // the relay is connected to pin 3 of the Adruino board

void setup()
{ totalMilliLitres = 0;
  pinMode(relais_moteur, OUTPUT);
  lcd.init(); // display initialization
  lcd.clear();
  lcd.backlight(); // activate the backlight
  lcd.setCursor(0, 0); // stand in the front line
  lcd.print("donner volume:");

  // Initialize a serial connection for reporting values to the host
  Serial.begin(9600);
  pinMode(solenoidValve , OUTPUT);
  digitalWrite(solenoidValve, HIGH);
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);
  /*The Hall-effect sensor is connected to pin 2 which uses interrupt 0. Configured to trigger on a FALLING state change (transition from HIGH
  (state to LOW state)*/
  // attachInterrupt(sensorInterrupt, pulseCounter, FALLING); //you can use Rising or Falling
}

void loop()
{
  char key = keypad.getKey(); 
    //lcd.clear();

  if(key) // A key on the keyboard is pressed
    { 
  code+=key;
  lcd.setCursor(0, 1); // stand on the second line
  lcd.print(code);  // show volume value
  delay(100);
    }

    if (key=='D') { // if you press the 'D' key
     if(code.toInt()<=1500){
        volume=code.toInt();
     }
     else{
        lcd.clear();
        lcd.backlight(); 
        lcd.setCursor(0, 0); 
        lcd.print("donner volume:");  
     }
    code="";
    }

if (totalMilliLitres<volume) {
    digitalWrite(relais_moteur, HIGH); // Start the water pump
    delay(5000);
    digitalWrite(relais_moteur, LOW); // Start the water pump
totalMilliLitres += volume;

  //  if((millis() - oldTime) > 1000)    // Only process counters once per second
  // { 
  //   // Disable the interrupt while calculating flow rate and sending the value to the host
  //   detachInterrupt(sensorInterrupt);

  //   // Because this loop may not complete in exactly 1 second intervals we calculate the number of milliseconds that have passed since the last execution and use that to scale the output. We also apply the calibrationFactor to scale the output based on the number of pulses per second per units of measure (litres/minute in this case) coming from the sensor.
  //   flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;

  //   // Note the time this processing pass was executed. Note that because we've
  //   // disabled interrupts the millis() function won't actually be incrementing right
  //   // at this point, but it will still return the value it was set to just before
  //   // interrupts went away.
  //   oldTime = millis();

  //   // Divide the flow rate in litres/minute by 60 to determine how many litres have
  //   // passed through the sensor in this 1 second interval, then multiply by 1000 to
  //   // convert to millilitres.
  //   flowMilliLitres = (flowRate / 60) * 1000;

  //   // Add the millilitres passed in this second to the cumulative total
  //   totalMilliLitres += flowMilliLitres;

  //   unsigned int frac;

    // Print the flow rate for this second in litres / minute
            
    lcd.clear();
    lcd.backlight(); 
    lcd.setCursor(0, 0); 
    lcd.print("debit:");
    lcd.print(flowMilliLitres);  // Show the flow rate on the lcd display   
        lcd.print(" ml/s");    
    // Print the cumulative total of litres flowed since starting
  

    Serial.println("mL"); 
    Serial.print("\t");     
        lcd.setCursor(0, 1); 
        lcd.print("volume:");           
        lcd.print(totalMilliLitres);  // Show quantity filled   
        lcd.print(" ml"); 
//     if (totalMilliLitres > 40)
//     {
//       SetSolinoidValve();
//     }

// // Reset the pulse counter so we can start incrementing again
//     pulseCount = 0;

//     // Enable the interrupt again now that we've finished sending output
//     attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  
}else  {
  digitalWrite(relais_moteur, LOW); // Stop the water pump
  volume=0;
}

}

//Insterrupt Service Routine

void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}

void SetSolinoidValve()
{
  digitalWrite(solenoidValve, LOW);
}
