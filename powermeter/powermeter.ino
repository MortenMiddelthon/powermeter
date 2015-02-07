#include <Wire.h>
#include <Adafruit_INA219.h>
#include <LiquidCrystal.h>

#define RELAY 7
#define RELAY_BUTTON 0
uint8_t buttonState = 0;

Adafruit_INA219 ina219;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 6);

// Turn on/off serial debuggin output
boolean debug = true;

void setup(void) 
{
	uint32_t currentFrequency;
		
	Serial.begin(9600);
	lcd.begin(16, 2);
	Serial.println("Measuring voltage and current with INA219 ...");
	ina219.begin();
	pinMode(RELAY, OUTPUT);
	digitalWrite(RELAY, LOW);
	lcd.setCursor(0, 0);
	lcd.print("PowerMeter");
	delay(4000);
}

void loop(void) 
{
	uint16_t b;
	b = analogRead(RELAY_BUTTON);
	if(b > 500 and buttonState == 0) {
		buttonState = 1;
		relay();
	}
	else {
		buttonState = 0;
	}
	float shuntvoltage = 0;
	float busvoltage = 0;
	float current_mA = 0;
	float loadvoltage = 0;

	shuntvoltage = ina219.getShuntVoltage_mV();
	busvoltage = ina219.getBusVoltage_V();
	current_mA = ina219.getCurrent_mA();
	loadvoltage = busvoltage + (shuntvoltage / 1000);
	
	/*
	Serial.print("Bus Voltage:	 "); Serial.print(busvoltage); Serial.println(" V");
	Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
	Serial.print("Load Voltage:	"); Serial.print(loadvoltage); Serial.println(" V");
	Serial.print("Current:			 "); Serial.print(current_mA); Serial.println(" mA");
	Serial.println("");
	*/
	lcd.setCursor(0,0);
	lcd.print("L/V: ");
	lcd.print(loadvoltage);
	lcd.print("V  ");
	lcd.setCursor(0,1);
	lcd.print("C: ");
	lcd.print(current_mA);
	lcd.print("mA ");

	delay(500);
}

void relay() {
	uint8_t r;
	r = digitalRead(RELAY);
	if(r == HIGH) {
		digitalWrite(RELAY, LOW);
		lcd.setCursor(15,0);
		lcd.print("-");
	}
	else {
		digitalWrite(RELAY, HIGH);
		lcd.setCursor(15,0);
		lcd.print("*");
	}
	delay(500);
}
