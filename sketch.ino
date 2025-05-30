#include <LiquidCrystal_I2C.h> 
#include <DHT.h>
#include <LowPower.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <MD_MAX72xx.h>
#include <Servo.h>
#include <TM1637.h>

#define	CLK_PIN		13
#define	DATA_PIN	11
#define	CS_PIN		10
MD_MAX72XX mx = MD_MAX72XX(MD_MAX72XX::PAROLA_HW, CS_PIN, 2);

// Pin for DHT22 sensor
#define DHTPIN 7
#define DHTTYPE DHT22

Adafruit_MPU6050 mpu;

Servo myservo;  // create servo object to control a servo
int pos = 90;    // variable to store the servo position

// Variables for stepper controlling
const int DIR = 9;
const int STEP = 8;
const int steps_per_rev = 200;
int CW = 0;
int randNumber;
int windDir;
int steps;

int potpin = 0;  // analog pin used to connect the potentiometer
int windSpeed;    // variable to read the value from the analog pin

float tempInterior = 15;
float tempConsigna = 25;
float tempMargin = 3;
float Max; 
float Min;

int x = 0;
int y = 0;

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 20, 4);

TM1637 tm(2, 4);
int tempDisplay;

bool heating = false;
bool cooling = false;
bool winter = false;
bool summer = false;


void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // configure the digital pins connected with STEP and DIR as output pins.
  pinMode(STEP, OUTPUT); 
  pinMode(DIR, OUTPUT);
  
  // Initialize DHT sensor
  dht.begin();

  // Initialiye accelerometer
  if(!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while(1) {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);

  // Initialize LCD screen
  lcd.init();
  lcd.backlight();
  lcd.clear();

  pinMode(5, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(6, OUTPUT);

  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, MAX_INTENSITY / 2);
  mx.clear();

  myservo.attach(3);  // attaches the servo on pin 3 to the servo object

  tm.init();
  tm.set(BRIGHT_TYPICAL);
  
}


void loop() {
  
  digitalWrite(5, LOW);
  digitalWrite(12, LOW);
  digitalWrite(6, LOW);
  
  
  // Read temperature and humidity data from DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  // Display temperature and humidity on serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("ºC");
  Serial.print("Inside temperature: ");
  Serial.println(tempInterior);
  
  // Temperatura exterior se transmite al interior
  if (tempInterior > temperature){
    tempInterior = tempInterior - 0.5;
  } else {
    tempInterior = tempInterior + 0.5;
  }
  
  if (temperature<tempConsigna){
    winter=true;
    summer=false;
    myservo.write(90); 
  } else{
    winter=false;
    summer=true;
    mx.clear();
  }
  if ((tempInterior<(tempConsigna-tempMargin))&&winter){
    for(int x = 0; x < 8 ; x++){
    for(int y = 0; y < 8 ; y++){
      mx.setPoint(y, x, true);
      mx.update();
    }
  }
    tempInterior = tempInterior + 3.0;
  }
  if (tempInterior>(tempConsigna+tempMargin)){
    mx.clear();
  }
  if ((tempInterior>(tempConsigna+tempMargin))&&summer){
    myservo.write(180);
    tempInterior = tempInterior - 2.0;
  }
  if (tempInterior<(tempConsigna-tempMargin)){
   myservo.write(90); 
  }
  

  


  if(temperature>0){
  // Read wind speed value
  windSpeed = analogRead(potpin); // Values between 0 and 1023
  // scale the value for wind speed range
  windSpeed = map(windSpeed, 0, 1023, 0, 180);
  Serial.print("Wind speed: ");
  Serial.print(windSpeed);
  Serial.println(" km/h");

  // Controlling the step motor  
  randNumber = random(30);
  CW = random(0,2);

  if (CW == 1) {
    digitalWrite(DIR, HIGH);
        steps = steps + randNumber;
      } else {
        digitalWrite(DIR, LOW);
        steps = steps - randNumber;
      }
      if (steps>=200){
        steps = steps - 200;
      }
      if (steps<0){
        steps = 200 + steps;
      }
      windDir = (float)steps/200*360;
      Serial.print("Wind direction: ");
      Serial.print(windDir);
      Serial.println("º");

      for(int i = 0; i < randNumber ; i++){
        digitalWrite(STEP, HIGH);
        delayMicroseconds(2000);
        digitalWrite(STEP, LOW);
        delayMicroseconds(2000);
      }
      delay(1000);
  }
  

  
  
  

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  Serial.print("Acceleration: ");
  Serial.print(a.acceleration.x);
  Serial.print(",");
  Serial.print(a.acceleration.y);
  Serial.print(",");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");
  
  // Display measurements on LCD screen and sea state LED
  if (temperature>0){
    
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Temperature: ");
    lcd.setCursor(15, 0);
    lcd.print(temperature);
    lcd.setCursor(0, 1);
    lcd.print("Wind direction:");
    lcd.setCursor(17,1);
    lcd.print("___");
    lcd.setCursor(17,1);
    lcd.print(windDir);
    lcd.setCursor(0,2);
    lcd.print("WS__________________");
    lcd.setCursor(2,2);
    for(int i=1;i<(windSpeed/10+1);i++){
      lcd.print(char(255));
    }
    lcd.setCursor(0,3);
    lcd.print("WH__________________");
    lcd.setCursor(2,3);
    if (a.acceleration.z<0){
      for(int i=1;i<(int)(a.acceleration.z*(-1));i++){
      lcd.print(char(255));
      }
    }
    // Sea state according to wind speed
    if ((windSpeed>80)||(a.acceleration.z<-10)){
      digitalWrite(5, HIGH);
    } else if ((windSpeed>50)||(a.acceleration.z<-5)){
      digitalWrite(12, HIGH);
    } else {
      digitalWrite(6, HIGH);
    }
  }
  

  // Hibernating when temperature is below 0ºC    
  if (temperature<0){
    Serial.print("Taking a nap...");
    lcd.clear();
    lcd.noBacklight();
    lcd.setCursor(0, 0);
    lcd.println("Hibernating...");
    LowPower.powerDown(SLEEP_4S,ADC_OFF,BOD_OFF);
  }
  tempDisplay = tempInterior*100;
  tm.display(0, (tempDisplay / 1000) % 10);
  tm.display(1, (tempDisplay / 100) % 10);
  tm.display(2, (tempDisplay / 10) % 10);
  tm.display(3, tempDisplay % 10);

  delay(500);
}





void cooling_on(){
  myservo.write(180);
  tempInterior = tempInterior - 1.5;
}

void cooling_off(){
  myservo.write(90);
}
