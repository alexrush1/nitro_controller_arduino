#define ON 1                                      //turned on
#define OFF 0                                     //turned off
#define VELOCITY_GAUGE_PIN 3                      //tachometer pin
#define RELE_PIN  8                  //rele pin
#define HIGH_RPM_MODE 7               //pin for 4500 min rpm
#define CALCULATION_METHOD 6            //pin for 1/3 or 1/6 coefficient 
short cc = 0;
double gz = 0;
int minGZ = 67;
int ready = 0;
double coeff = 3;
unsigned long old_time = 0;
unsigned long new_time = 0;
unsigned long counter = 0;

void setup() {
  pinMode(RELE_PIN, OUTPUT);                      //setting rele pin in output mode
  pinMode(VELOCITY_GAUGE_PIN, INPUT);             //setting tachometer pin in input mode
  attachInterrupt(1, Gz, CHANGE);
  
  digitalWrite(RELE_PIN, OFF);                    //rele turned off when arduino starts

  pinMode(10, INPUT_PULLUP);
  digitalWrite(10, HIGH);
  
  //pinMode(CALCULATION_METHOD, INPUT_PULLUP);
  
  //debug only
  pinMode(13, OUTPUT);
  pinMode(4, OUTPUT);
  tone(4, 30);
  //old_time = -3500;
}

void loop() {
  Serial.begin(115200);
  
  if (digitalRead(CALCULATION_METHOD)) {
    coeff = 6;
  } else {
    coeff = 3;
  }
  
  if (!digitalRead(HIGH_RPM_MODE)) {
    minGZ = 75;
  } else {
    minGZ = 67;
  }
  Serial.println(digitalRead(10));
  if (ready) {
    if ((gz >= minGZ)&&(gz <= 101)) {
      digitalWrite(13, ON);
        digitalWrite(RELE_PIN, ON);
      //Serial.println("HIGH PERFORMANCE!");
    } else if ((gz < minGZ-5)||(gz > 101.5)) {
      digitalWrite(13, OFF);
        digitalWrite(RELE_PIN, OFF);
      //Serial.println("EMERGENCY SHUTDOWN!");
    }
  }
  delay(500);
}

void Gz() {
  if (cc == 1) {
    counter++;
    cc = 0;
  } else {
    cc++;
  } 
  if (counter == 15) {
    new_time = millis();
    gz = ((15.0 * 1000.0)/ (double)((new_time - old_time)* coeff));
    old_time = new_time;
    //Serial.println(gz);
    counter = 0;
    ready = 1;
  }
}
