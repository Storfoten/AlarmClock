
#include "Arduino.h"
#include <LiquidCrystal.h>
#include <ToggleButton.h>
LiquidCrystal lcd(7,8,9,10,11,12);
#include <Clock.h>
#include <Alarm.h>
// initialize the library with the numbers of the interface pins
	
String menuList[] = {"CLOCK","ALARM","DATE", "OTHER"};
int cursorStyle[] = {126,127};
int cursorLocation = 0;
String pageList[] =	{ "CLOCK","ALARM","DATE", "OTHER"};
String showPage = pageList[0];
boolean updateScreen = true;
boolean blockMenuChange = false;

int clockCursor = 0;
unsigned long lastMi = 0;

//int _seconds = 0;
int masterClock = 0; 

int _lcdBrightness = 120;
int _lcdBacklightPin = 5;
int _summerPin = 6;
int _lightSensorPin = A0;
int _lcdBrightnessAuto = 0;
int _lightSensorArray[100] = {0};
int _arrayNr = 0;
boolean _useAutoBrightness = true;
double _correction = 0.000391389+0.0000075;
double _removeOneSec = 0;


ToggleButton button1(A5); //blÃ¤ddra nerÃ¥t i menyn
ToggleButton button2(A4); //vÃ¤lja meny sida, eller minska vÃ¤rden
ToggleButton button3(A3); // Ã¶ka vÃ¤rden
ToggleButton button4(A2); // hoppa till nÃ¤sta vÃ¤rde som ska Ã¤ndas 
ToggleButton button5(3); //alarm on/off
ToggleButton button6(0); // shutdown alarm
ToggleButton button7(4); // increase light for a short period of time
Clock clock(5,1);
Alarm alarm(4,3);
void setup() 
{
	attachInterrupt(1, clockCounter, RISING);
	analogWrite(13, 127); 
	
	lcd.begin(20, 4);
	Serial.begin(9600);
	button7.setInvertedButton();
	button7.setButtonPressedTime(3000);
	
	
}

void clockCounter()      // called by interrupt
{
  masterClock++;        // with each clock rise add 1 to masterclock count
  if(masterClock == 490) // 490Hz reached     
  {                         
	_removeOneSec += _correction;
	if(_removeOneSec < 1)
		clock.update();
	else
	{
		_removeOneSec -= 1;
	}
    masterClock = 0;     // Reset after 1 second is reached
   }
}

void loop() 
{
	updateButtons(); 
	updateAutoBrightness();
	if(showPage == "CLOCK" && updateScreen)
	{
		displayClockPage();
		displayMenu();
	}
	else if(showPage == "ALARM"	&& updateScreen)
	{
		displayAlarmPage();
		displayMenu();
	}
	else if(showPage == "DATE"	&& updateScreen)
	{
		displayDatePage();
	}	
	else if(showPage == "OTHER")
	{
		displayOtherPage();
		displayMenu();
	}
	 
//	clock.update();
	alarm.update(clock.getCurrentTime("hour"),clock.getCurrentTime("minute"));
	if(alarm.isAlarmActive())
	{
		analogWrite(_summerPin,220);
		analogWrite(_lcdBacklightPin, 250);
	}
	else	if(button7.isButtonPressed() || button7.isButtonDown())
	{
		analogWrite(_lcdBacklightPin, 220);
		analogWrite(_summerPin,0);
	}
	else if(_useAutoBrightness)
	{
		analogWrite(_summerPin,0);
		analogWrite(_lcdBacklightPin, _lcdBrightnessAuto);	
	}
	else
	{
		analogWrite(_summerPin,0);
		analogWrite(_lcdBacklightPin, _lcdBrightness);
	}
}
void updateAutoBrightness()
{
	_lightSensorArray[_arrayNr] = analogRead(_lightSensorPin);
	_arrayNr++;
	if(_arrayNr>99)
		_arrayNr = 0;
	unsigned long sum = 0;
	for(int i = 0; i < 100; i++)
	{
		sum += _lightSensorArray[i];
	}
	sum = sum / 100;
	if(sum < 600)
		_lcdBrightnessAuto = 220;
	else if(sum > 600 && sum < 700)
		_lcdBrightnessAuto = 50;
	else if(sum > 700)
		_lcdBrightnessAuto = 3;
		
}

void updateButtons()
{
	button1.update();
	button2.update();
	button3.update();
	button4.update();
	button5.update();
	button6.update();
	button7.update();
				
	if(button1.isButtonPressed() && !blockMenuChange)
	{
		lcd.clear();
		updateScreen = true;
		cursorLocation++;
		if(cursorLocation>3)
			cursorLocation = 0;
	}
	else if(button2.isButtonPressed() && !blockMenuChange)
	{
		lcd.clear();
		updateScreen = true;
		showPage = pageList[cursorLocation];
	}
	if(button5.isButtonPressed() || button5.isButtonReleased())
	{
		lcd.clear();
		alarm.activateAlarm(button5.isButtonDown());
	}
	if(button6.isButtonPressed())
	{
		lcd.clear();
		alarm.shutDownAlarm();
	}
}

void displayOtherPage()
{
	lcd.setCursor(0,0);
	lcd.print("Set Display"); 
	lcd.setCursor(0,1);
	lcd.print("Brightness");
	if(button3.isButtonPressed())
	{
		if(_lcdBrightness<10)
			_lcdBrightness += 1;
		else
			_lcdBrightness += 10;
		if(_lcdBrightness>250)
			 _lcdBrightness = 250;
	}
	else if(button2.isButtonPressed())
{
		if(_lcdBrightness<=10)
			_lcdBrightness -= 1;
		else		
			_lcdBrightness -=10;
		 if(_lcdBrightness<0)
			 _lcdBrightness = 0;
	} 
	
	lcd.setCursor(0,3);
	lcd.print("Value:");
	lcd.print(_lcdBrightness);
}

void displayMenu()
{
	for(int i = 0;i<4;i++)
	{
		lcd.setCursor(14, i);
		lcd.print(menuList[i]);
		
	}
	if(true)
	{
		lcd.setCursor(13,cursorLocation);
		lcd.write(cursorStyle[0]);
		lcd.setCursor(19,cursorLocation);
		lcd.write(cursorStyle[1]);
	}
}
void displayDatePage()
{
	clock.drawBigClock(true);
	clock.drawClock();
	alarm.setAlarmPos(3,6);
	if(alarm.isAlarmOn())
		 alarm.drawAlarm();	
}
void displayClockPage()
{
	clock.drawBigClock(false);
	if(button4.isButtonPressed() && clock.isIncrementEnabled())
	{
		clock.moveCursor();
	}
	
	if(!clock.isIncrementEnabled())
	{
		blockMenuChange = false;
	}
	else if(button3.isButtonPressed())
	{
		 clock.increment(1); 
	}
	else if(button2.isButtonPressed())
	{
		 clock.increment(-1);
	}
	
	if(button3.isButtonDown() && button4.isButtonDown())
	{
			clock.enableIncrement(true);
			blockMenuChange = true;
	}

	
	 clock.drawClock(); 
	 alarm.setAlarmPos(3,4);
	 if(alarm.isAlarmOn())
		 alarm.drawAlarm();
}
void displayAlarmPage()
{
	if(button4.isButtonPressed() && alarm.isIncrementEnabled())
	{
		alarm.moveCursor();
	}
	
	if(!alarm.isIncrementEnabled())
	{
		blockMenuChange = false;
	}
	else if(button3.isButtonPressed())
	{
		 alarm.increment(1); 
	}
	else if(button2.isButtonPressed())
	{
		 alarm.increment(-1);
	}
	
	if(button3.isButtonDown() && button4.isButtonDown())
	{
			alarm.enableIncrement(true);
			blockMenuChange = true;
	}

	
	 clock.drawClock(); 
	 alarm.setAlarmPos(3,4);
	 alarm.drawAlarm();
}


 /* for(int i = 0;i<20;i++) //^ 14+80	 //deg 3+60+80+80 //alla pixlar 15+80+80+80 
	{
		lcd.setCursor(i,0);
		lcd.write(i+80+80+80+80); 
		
		lcd.setCursor(i,1);
		lcd.write(i+20 +80+80+80+80);
		
		lcd.setCursor(i,2);
		lcd.write(i+40+80+80+80+80);
		
		lcd.setCursor(i,3);
		lcd.write(i+60+80+80+80+80);
	}
	while(true)
	{
		
	}*/
	
