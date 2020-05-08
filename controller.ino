#define ON 1                                      //turned on
#define OFF 0                                     //turned off
#define VELOCITY_GAUGE_PIN 3                      //tachometer pin
#define RELE_PIN  12                  //rele pin
#define HIGH_RPM_MODE 2               //pin for 4500 min rpm
#define CALCULATION_METHOD1 5         //pin for 1/3 or 
#define CALCULATION_METHOD2 8         //1/4 coefficient
short cc = 0;
double gz = 0;
int minGZ = 67;
int ready = 0;
double coeff = 0;
unsigned long old_time = 0;
unsigned long new_time = 0;
unsigned long counter = 0;

void setup() {
  pinMode(RELE_PIN, OUTPUT);                      //setting rele pin in output mode
  pinMode(VELOCITY_GAUGE_PIN, INPUT);             //setting tachometer pin in input mode
  attachInterrupt(1, Gz, CHANGE);
  
  digitalWrite(RELE_PIN, OFF);                    //rele turned off when arduino starts

  pinMode(CALCULATION_METHOD1, INPUT_PULLUP);
  pinMode(CALCULATION_METHOD2, INPUT_PULLUP);
  pinMode(HIGH_RPM_MODE, INPUT_PULLUP);
  
  
  //debug only
  //pinMode(4, OUTPUT);
  //tone(3, 100);
  Serial.begin(115200);
}

void loop() {
  //gz = 100;
  digitalWrite(13, ON);
  if (!digitalRead(CALCULATION_METHOD1)) {
    coeff = 3;
  } else if (!digitalRead(CALCULATION_METHOD2)) {
    coeff = 4;  
  } 
  if ((digitalRead(CALCULATION_METHOD1) && digitalRead(CALCULATION_METHOD2)) == true) {
    coeff = 1;
  }
  
  if (!digitalRead(HIGH_RPM_MODE)) {
    minGZ = 75;
  } else {
    minGZ = 67;
  }
  if (coeff != 0) {
    if (ready) {
      if ((gz >= minGZ)&&(gz <= 101)) {
        digitalWrite(RELE_PIN, ON);
        //Serial.println("HIGH PERFORMANCE!");
      } else if ((gz < minGZ-5)||(gz > 102.5)) {
        digitalWrite(RELE_PIN, OFF);
        //Serial.println("EMERGENCY SHUTDOWN!");
      }
    }
  }
  delay(400);
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
    Serial.println(gz);
    counter = 0;
    ready = 1;
    digitalWrite(13, OFF);
  }
}
