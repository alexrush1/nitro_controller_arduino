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
int in_work = 0;
double coeff = 0;
int stopper = 0;
unsigned long alltime = 0; 
unsigned long start_time = 0;
unsigned long old_time = 0;
unsigned long new_time = 0;
unsigned long counter = 0;
unsigned long workTime = 0;

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
  alltime = millis();
  if (digitalRead(RELE_PIN)) {
    workTime = alltime - start_time;
  } else {
    workTime = 0;  
  }
  if (in_work) {
    if (workTime > 15000) {
      digitalWrite(RELE_PIN, OFF);
      stopper = 1;  
    }
  } else {
    start_time = alltime;  
  }
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
  //Serial.print("stopper = ");
  //Serial.println(stopper);
  //Serial.print("in_work = ");
  //Serial.println(in_work);
  if (!digitalRead(HIGH_RPM_MODE)) {
    minGZ = 75;
  } else {
    minGZ = 67;
  }

  
  if (coeff != 0) {
    if (ready) {
      if ((gz >= minGZ)&&(gz <= 99)) {
        if (!in_work) {
          if (!stopper) {
            start_time = millis();
            in_work = 1;
            digitalWrite(RELE_PIN, ON);
          }
        }  
        //Serial.println("HIGH PERFORMANCE!");
      } else if ((gz < minGZ-5)) {
        digitalWrite(RELE_PIN, OFF);
        stopper = 0;
        in_work = 0;
        //workTime = 0;
        //Serial.println("EMERGENCY SHUTDOWN!");
      } else if ((gz > 102.5)) {
        digitalWrite(RELE_PIN, OFF);  
      } else if ((gz >= minGZ)&&(gz <= 99)) {
        if (!stopper) {
          if (in_work) {
            digitalWrite(RELE_PIN, ON);
          }
        }
      }
    }
  }
  //delay(400);
}

void Gz() {
  if (cc == 1) {
    counter++;
    cc = 0;
  } else {
    cc++;
  } 
  if (counter == 60) {
    new_time = millis();
    gz = ((60.0 * 1000.0)/ (double)((new_time - old_time)* coeff));
    old_time = new_time;
    Serial.println(gz);
    counter = 0;
    ready = 1;
    digitalWrite(13, OFF);
  }
}
