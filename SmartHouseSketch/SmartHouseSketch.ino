#include <DHT.h>                    //TEMPERATURE AND HUMIDITY
#include <NewPing.h>
#include <Wire.h>                   //DISPLAY
#include <LiquidCrystal_I2C.h>      //DISPLAY

#define TRIGGER_PIN 16
#define ECHO_PIN 10
#define MAX_DISTANCE 400
#define DHTPIN 5                    //DHT11 output pin
#define LIGHT_PIN 7
#define REED_SWITCH_LIGHT_PIN 9
#define DHT_UPDATE_TIME 3000
#define LCD_UPDATE_TIME 5000
#define PHOTOCELL_UPDATE_TIME 100
#define LIGHT_TIME 100

bool isLight = false;
bool isReedSwitchConnected = false;
bool isDHT = true;

const int photocellPin = 1;
const int reedSwitchPin = 8;

int photoCellResult;
int rangeFinderResult;

unsigned long LCD_timing = 0;
unsigned long DHT_timing = 0;
unsigned long PHOTOCELL_timing = 0;

float temperature;
float humidity;

DHT dht(DHTPIN,DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2); //DISPLAY
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(REED_SWITCH_LIGHT_PIN, OUTPUT);
  pinMode(DHTPIN, INPUT);
  pinMode(A1, INPUT);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  if(millis() - PHOTOCELL_timing > PHOTOCELL_UPDATE_TIME) {
    PHOTOCELL_timing = millis();
    photoCellResult = analogRead(photocellPin);
    //reedSwitchResult = digitalRead(reedSwitchPin);
    rangeFinderResult = sonar.ping_cm();
    
    if(photoCellResult > LIGHT_TIME) {
      isLight = true;
    } else {
      isLight = false;
    }

    if(reedSwitchResult) {
      isReedSwitchConnected = true;
    } else {
      isReedSwitchConnected = false;
    }
    
//    Serial.print("Photo cell: ");
//    Serial.println(photoCellResult);
//    Serial.print("Reed switcher: ");
//    Serial.println(rangeFinderResult);
  }

  if(millis() - DHT_timing > DHT_UPDATE_TIME) {
    DHT_timing = millis();

    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
  }

  if(millis() - LCD_timing > LCD_UPDATE_TIME) {
    LCD_timing = millis();

    if(isDHT) {
      lcd.clear();
      lcd.print("Temperature:");
      lcd.print(temperature);
      lcd.setCursor(0,1);
      lcd.print("   Humidity:");
      lcd.print(humidity);
  
//      Serial.print(temperature);
//      Serial.print("\t");
//      Serial.println(humidity);

      isDHT = false;
    } else {
      lcd.clear();
      lcd.print("Photocell:");
      lcd.print(photoCellResult);
      lcd.setCursor(0,1);
      lcd.print("RangeFinder:");
      lcd.print(rangeFinderResult);
      
      isDHT = true;
    }

  }
  
//  if(isnan(humidity) || isnan(temperature)) {
//    Serial.println("Cannot read the unit(DH11)");
//  }

  if(!isLight) {
    digitalWrite(LIGHT_PIN, HIGH);
  } else {
    digitalWrite(LIGHT_PIN, LOW);
  }

  if(!isReedSwitchConnected) {
    digitalWrite(REED_SWITCH_LIGHT_PIN, HIGH);
  } else {
    digitalWrite(REED_SWITCH_LIGHT_PIN, LOW);
  }
}
