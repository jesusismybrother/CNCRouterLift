#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <BasicStepperDriver.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
#define MOTOR_STEPS 200
#define RPM 230
#define MM_PER_REVOLUTION 1
#define BOOT_ON_STARTUP 0

#define MICROSTEPS 16
#define DIR 12
#define STEP 14
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 4);

static int zero = 5;
static int park = 19;
static int change = 18;
static int unit = 4;
static int down = 15;
static int up = 27;
static int limit_switch = 23;
static int stepp_enable = 32;

float abs_pos = 0;
float abs_limit = 73;
float curr_pos = 0;
int scale_selector = 0;

char RcvMsg[16];
char TxMsg[16] = "Test";

char currChar;
int charcounter = 0;
long BTdelay = millis();
bool connected;

bool limit_switch_read()
{
	int read = 0;
	for (int i = 0; i < 5; i++)
	{
		read = read + (digitalRead(limit_switch));
	}
	//	Serial.println(read);
	read = read / 5;
	return (!read);
}

void write_position()
{

	lcd.setCursor(10, 1);
	lcd.print("     ");

	float display_pos = curr_pos;
	if (display_pos < 0)
	{
		display_pos = display_pos * -1;
		lcd.setCursor(10, 1);
		lcd.print("-");
	}
	if (display_pos >= 100)
	{
		lcd.setCursor(11, 1);
		lcd.print(display_pos);
	}
	if (display_pos >= 10 && display_pos < 100)
	{
		lcd.setCursor(12, 1);
		lcd.print(display_pos);
	}
	if (display_pos < 10)
	{
		lcd.setCursor(13, 1);
		lcd.print(display_pos);
	}

}

void write_absolute()
{

	lcd.setCursor(10, 2);
	lcd.print("     ");

	float display_pos = abs_pos;
	if (display_pos < 0)
	{
		display_pos = display_pos * -1;
		lcd.setCursor(10, 2);
		lcd.print("-");
	}
	if (display_pos >= 100)
	{
		lcd.setCursor(11, 2);
		lcd.print(display_pos);
	}
	if (display_pos >= 10 && display_pos < 100)
	{
		lcd.setCursor(12, 2);
		lcd.print(display_pos);
	}
	if (display_pos < 10)
	{
		lcd.setCursor(13, 2);
		lcd.print(display_pos);
	}
}

void sendBTdata()
{
	String TxStr = String(abs_pos)+","+String(curr_pos);
	SerialBT.println(TxStr);
	delay(250);
}
boolean move_motor(float distance)
{

	bool answere = false;

	float req_distance = distance * MM_PER_REVOLUTION * MOTOR_STEPS * MICROSTEPS;
	lcd.setCursor(0, 3);
	if (abs_pos + distance >= 0 && abs_pos + distance <= abs_limit)
	{
		digitalWrite(stepp_enable, LOW);
		delay(50);
		if (req_distance > 0)
		{
			lcd.print("Moving up");
		}
		else
		{
			lcd.print("Moving down");
		}

		Serial.print("Move motor by: ");
		Serial.println(distance);
		//stepper.move(req_distance);
		stepper.startMove(req_distance);

		while (stepper.nextAction() != 0)
		{
		}

		curr_pos = curr_pos + distance;
		abs_pos = abs_pos + distance;

		answere = true;
		digitalWrite(stepp_enable, HIGH);
		delay(50);
	}

	else
	{
		if (req_distance > 0)
		{
			lcd.print("Error: Max 73mm");
		}
		else
		{
			lcd.print("Error: <1 mm");
		}

		Serial.println("Out of range!");

		delay(1000);
		lcd.print("                 ");
		answere = false;
	}
	lcd.setCursor(0, 3);
	lcd.print("                ");
	write_position();
	write_absolute();
	sendBTdata();

	return (answere);
}

void tool_change(float distance)
{
	digitalWrite(stepp_enable, LOW);
	delay(50);
	float req_distance = distance * MM_PER_REVOLUTION * MOTOR_STEPS * MICROSTEPS;
	lcd.setCursor(0, 3);
	if (abs_pos + distance >= 0 && abs_pos + distance <= abs_limit)
	{
		if (req_distance > 0)
		{
			lcd.print("Move to Change");
		}
		Serial.print("Move motor by: ");
		Serial.println(distance);
		//stepper.move(req_distance);
		stepper.startMove(req_distance);

		while (stepper.nextAction() != 0)
		{
		}

		curr_pos = curr_pos + distance;
		abs_pos = abs_pos + distance;
	}

	lcd.setCursor(0, 3);
	lcd.print("                ");
	write_position();
	write_absolute();
	digitalWrite(stepp_enable, HIGH);
	delay(50);
}

void homing()
{
	digitalWrite(stepp_enable, LOW);
	delay(50);
	lcd.setCursor(0, 3);
	lcd.print("Homing");

	long i = 0;
	float moved_distance = 0;
	//move once towards stop
	stepper.startRotate(-75 * 360);

	while (limit_switch_read())
	{
		stepper.nextAction();
		i++;
		//	Serial.println(limit_switch_read());
	}
	stepper.stop();

	//move back 1mm
	stepper.startRotate(1 * 360);

	while (stepper.nextAction() != 0)
	{
		i--;
	}
	//trigger again but slowly
	stepper.begin(10, MICROSTEPS);
	stepper.startRotate(-75 * 360);

	while (limit_switch_read())
	{
		stepper.nextAction();
		i++;
	}
	stepper.stop();
	stepper.begin(RPM, MICROSTEPS);
	//calculate moved distance
	moved_distance = float(i) * MM_PER_REVOLUTION / MOTOR_STEPS / MICROSTEPS;
	// Serial.println(i);
	// Serial.println(moved_distance);
	//change curr_pos with now position
	curr_pos = curr_pos - moved_distance;
	//zero abs_position
	abs_pos = 0;

	//Delete "homing" from lcd
	lcd.setCursor(0, 3);
	lcd.print("                 ");
	write_position();
	write_absolute();
	sendBTdata();
	digitalWrite(stepp_enable, HIGH);
	delay(50);
}

void zero_tool()
{
	digitalWrite(stepp_enable, LOW);
	delay(50);
	lcd.setCursor(0, 3);
	lcd.print("Zero Tool");

	long i = 0;
	float moved_distance = 0;
	//move once towards stop
	stepper.startRotate(-75 * -360);

	while (limit_switch_read())
	{
		stepper.nextAction();
		i++;
		//	Serial.println(limit_switch_read());
	}
	stepper.stop();

	//move back 1mm
	stepper.startRotate(-1 * 360);

	while (stepper.nextAction() != 0)
	{
		i--;
	}
	//trigger again but slowly
	stepper.begin(10, MICROSTEPS);
	stepper.startRotate(-75 * -360);

	while (limit_switch_read())
	{
		stepper.nextAction();
		i++;
	}
	stepper.stop();
	stepper.begin(RPM, MICROSTEPS);
	//calculate moved distance
	moved_distance = float(i) * MM_PER_REVOLUTION / MOTOR_STEPS / MICROSTEPS;
	// Serial.println(i);
	// Serial.println(moved_distance);
	//change curr_pos with now position
	curr_pos = 0;
	//zero abs_position
	abs_pos = abs_pos + moved_distance;

	//Delete "zero tool" from lcd
	lcd.setCursor(0, 3);
	lcd.print("                 ");
	write_position();
	write_absolute();
	sendBTdata();
	digitalWrite(stepp_enable, HIGH);
	delay(50);
}

void BluetoothCommand(char command[15])
{
	if (String(command) == "home")
	{
		Serial.println("Homing");
		if (!limit_switch_read())
		{
			move_motor(2);
		}
		if (limit_switch_read())
		{
			homing();
		}
	}
	else if (String(command) == "zero")
	{
		Serial.println("Zero tool");
		zero_tool();
	}
	else if (String(command) == "park")
	{
		Serial.println("Parking");
		tool_change((-1*abs_pos)+5);
	}
	else if (String(command) == "change")
	{
		Serial.println("Change tool");
		tool_change(abs_limit - abs_pos);
	}

	else
	{
		Serial.println(command);
	}
}

void setup()
{
	// initialize the LCD
	Serial.begin(9600);
	lcd.begin();
	pinMode(zero, INPUT_PULLUP);
	pinMode(unit, INPUT_PULLUP);
	pinMode(up, INPUT_PULLUP);
	pinMode(down, INPUT_PULLUP);
	pinMode(limit_switch, INPUT_PULLDOWN);
	pinMode(park, INPUT_PULLUP);
	pinMode(change, INPUT_PULLUP);
	pinMode(stepp_enable, OUTPUT);

	digitalWrite(stepp_enable, HIGH);
	stepper.begin(RPM, MICROSTEPS);
	stepper.enable();

	// Turn on the blacklight and print a message.
	lcd.backlight();
	lcd.setCursor(0, 0);
	lcd.print("Step     =  10.00mm");
	lcd.setCursor(0, 1);
	lcd.print("Position =   0.00mm");
	lcd.setCursor(0, 2);
	lcd.print("Absolut  =   0.00mm");

	Serial.println("Hello world!");
	if (!SerialBT.begin("Routerlift"))
	{
		Serial.println("An error occurred initializing Bluetooth");
	}

	//homing
	if (BOOT_ON_STARTUP)
	{
		if (!limit_switch_read())
		{
			move_motor(2);
		}
		if (limit_switch_read())
		{
			homing();
		}
		Serial.println("homing done");
	}
	write_position();
	write_absolute();
}

void loop()
{

	//	Serial.println(limit_switch_read());

	//zero Tool
	if (!digitalRead(zero))
	{
		Serial.println("zero");
		zero_tool();
	}

	//Tool Change
	if (!digitalRead(change))
	{
		Serial.println("tool change");
		tool_change(abs_limit - abs_pos);
	}

	//select scale
	if (!digitalRead(unit))
	{
		delay(10);
		Serial.println("unit");
		while (!digitalRead(unit))
			delay(10);
		if (scale_selector != 3)
			scale_selector++;
		else
			scale_selector = 0;
		Serial.println(scale_selector);

		lcd.setCursor(12, 0);
		switch (scale_selector)
		{
		case 0:
			lcd.print("10.00mm");
			break;
		case 1:
			lcd.print(" 1.00mm");
			break;
		case 2:
			lcd.print(" 0.10mm");
			break;
		case 3:
			lcd.print(" 0.05mm");
			break;
		default:
			break;
		}
	}

	//Up
	if (!digitalRead(up))
	{
		Serial.println("up");
		delay(10);
		while (!digitalRead(up))
			delay(10);
		Serial.println("up released");

		if (scale_selector == 3)
		//move motor
		{
			move_motor(0.05);
		}

		if (scale_selector == 2)
		//move motor
		{
			move_motor(0.1);
		}

		if (scale_selector == 1)
		//move motor
		{
			move_motor(1);
		}

		if (scale_selector == 0)
		//move motor
		{
			move_motor(10);
		}
	}

	//Down
	if (!digitalRead(down))
	{
		Serial.println("down");
		delay(10);
		while (!digitalRead(down))
			delay(10);

		if (scale_selector == 3)
		//move motor
		{
			move_motor(-0.05);
		}

		if (scale_selector == 2)
		//move motor
		{
			move_motor(-0.1);
		}

		if (scale_selector == 1)
		//move motor
		{
			move_motor(-1);
		}

		if (scale_selector == 0)
		//move motor
		{
			move_motor(-10);
		}
	}

	if (millis() - BTdelay >= 50)
	{
		while (SerialBT.available())
		{
			currChar = SerialBT.read();
			//Serial.write(SerialBT.read());
			if (currChar == '\\')
			{
				BluetoothCommand(RcvMsg);
				charcounter = 0;
				memset(RcvMsg, 0, 16);
			}
			else
			{
				RcvMsg[charcounter] = currChar;
				charcounter++;
			}
			Serial.println(RcvMsg);
		}

		//		for (int j; j <= 15; j++)
		//		{
		//			SerialBT.write(15);

		//		}

		BTdelay = millis();
	}
}