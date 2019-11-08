#include <DHT.h>                    //TEMPERATURE AND HUMIDITY
#include <Wire.h>                   //DISPLAY
#include <LiquidCrystal_I2C.h>      //DISPLAY
#define DHTPIN 2                    //DHT11 output pin
#define LIGHT_PIN 7
#define MOVEMENT_PIN 11
#define DHT_UPDATE_TIME 3000
#define LCD_UPDATE_TIME 5000
#define PHOTOCELL_UPDATE_TIME 500
#define LIGHT_TIME 700

bool isLight = false;
const int photocellPin = 1;
const int photoCellResult;
const int movementPin = 0;
const int movementResult = 0;
unsigned long LCD_timing = 0;
unsigned long DHT_timing = 0;
unsigned long PHOTOCELL_timing = 0;

float temperature;
float humidity;

DHT dht(DHTPIN,DHT11);
LiquidCrystal_I2C lcd(0x27, 20, 2); //DISPLAY

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(A1, INPUT);
  pinMode(movementPin, INPUT);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  movementResult = analogRead(movementPin);
      
  if(millis() - PHOTOCELL_timing > PHOTOCELL_UPDATE_TIME) {
    PHOTOCELL_timing = millis();
    photoCellResult = analogRead(photocellPin);
    
    if(photoCellResult > LIGHT_TIME) {
      isLight = true;
    } else {
      isLight = false;
    }
    
    Serial.print("Photo cell: ");
    Serial.println(photoCellResult);
    Serial.print("Movement sensor: ");
    Serial.println(movementResult);
  }

  if(millis() - DHT_timing > DHT_UPDATE_TIME) {
    DHT_timing = millis();

    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
  }

  if(millis() - LCD_timing > LCD_UPDATE_TIME) {
    LCD_timing = millis();

    lcd.clear();
    lcd.print("Temperature:");
    lcd.print(temperature);
    lcd.setCursor(0,1);
    lcd.print("Humiditiy: ");
    lcd.print(humidity);
  }
  
  if(isnan(humidity) || isnan(temperature)) {
    Serial.println("Cannot read the unit(DH22)");
    return;
  }

  if(!isLight) {
    digitalWrite(LIGHT_PIN, HIGH);
  } else {
    digitalWrite(LIGHT_PIN, LOW);
  }
}
