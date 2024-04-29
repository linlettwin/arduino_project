
const int trigPin = 7;
const int echoPin = 8;


void setup() {
  // initialize serial communication:
  Serial.begin(9600);
    pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
    pinMode(5, OUTPUT);
        pinMode(10, OUTPUT);
                pinMode(6, OUTPUT);
                pinMode(11, OUTPUT);
                





}

void loop() {
  
  long duration, inches, cm;

  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
Serial.println(inches);
delay(1000);  
if(inches < 3 )
{

    tone(11,1000);

  digitalWrite(5,LOW);
    digitalWrite(10,HIGH);
  digitalWrite(6,LOW);


}
else if(inches <6 && inches > 2){
  digitalWrite(5,LOW);
      digitalWrite(10,LOW);


//     digitalWrite(4,HIGH);
  digitalWrite(6,HIGH);
            noTone(11);

}
else{
digitalWrite(5,HIGH);
      digitalWrite(10,LOW);

//     digitalWrite(4,HIGH);
  digitalWrite(6,LOW);
            noTone(11);

}

}

long microsecondsToInches(long microseconds) {
 
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
 
  return microseconds / 29 / 2;
}
