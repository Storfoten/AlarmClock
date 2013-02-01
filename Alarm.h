#ifndef Alarm_h
#define Alarm_h
class Alarm
{
  public:
    Alarm(int clockPosCol, int clockPosRow)
     : 	_alarmOn(false),
		_hour(0),
		_minute(1),
		_lastMillis(0),
		_setClockCursor(0),
		_clockPosCol(clockPosCol), 
		_clockPosRow(clockPosRow),
		_setClock(0)
	{  };     
    void update(int currHour, int currMin)
	{ 
		_currTimeHour = currHour;
		_currTimeMin = currMin;
		if(_currTimeHour == _hour && _currTimeMin >= _minute && _currTimeMin <= _minute+3 && _alarmOn)
			_soundAlarm = true;
		else
			_soundAlarm = false;
		
	};
	void drawAlarm()
	{
		unsigned long currentMillis = millis(); 
		int minutes_1 = _minute % 10;
		int minutes_10 = (_minute - minutes_1)/10;
		int hours_1 = _hour % 10;
		int hours_10 = (_hour - hours_1)/10;
		lcd.setCursor(_clockPosCol, _clockPosRow);
		if(_alarmOn)
			lcd.print("[");
		else
			lcd.print(" ");
		if(_setClock && _setClockCursor == 0 && currentMillis-_lastMillis > 800)
		{
			lcd.print("  ");
		}
		else
		{
			lcd.print(hours_10);
			lcd.print(hours_1);
		}
		lcd.print(':');
		if(_setClock && _setClockCursor == 1 && currentMillis-_lastMillis > 800)
		{
			lcd.print("  ");
		}
		else
		{		
			lcd.print(minutes_10);
			lcd.print(minutes_1);
		}
		if(_alarmOn)
			lcd.print("]");
		else
			lcd.print(" ");		
		if(currentMillis-_lastMillis > 1000)
			_lastMillis = currentMillis;
	};	
	void increment(int var)
	{
		if(_setClock)
		{
			String type = "";
			if(_setClockCursor == 0)
				 type = "hour";
			else if(_setClockCursor == 1)
				 type = "minute";


			if(type == "hour")
			{
				_hour += var;
				if(_hour>23)
					_hour = 0;
				else if(_hour<0)
					_hour = 23;
			}	
			else if(type == "minute")
			{
				_minute += var;
				if(_minute>59)
					_minute = 0;
				else if(_minute<0)
					_minute = 59;
			}						
		}
	};
	void moveCursor()
	{ 
		if(_setClock) 
		{
			_setClockCursor++;
		}
		if(_setClockCursor > 1)
		{
			_setClockCursor = 0;
			_setClock = false;
		}	
	};	
	void enableIncrement(boolean bol){ _setClock = bol; };
	boolean isIncrementEnabled() { return _setClock; };	
	boolean isAlarmOn(){return _alarmOn;};
	void setAlarmPos(int row,int col){_clockPosCol = col; _clockPosRow = row;};
	boolean isAlarmActive(){return _soundAlarm;};	
	void activateAlarm(boolean bol){ _alarmOn = bol; };
	void shutDownAlarm(){ _alarmOn = false; _soundAlarm = false;};
	
  private:
    boolean _alarmOn;
	int _hour, _currTimeHour;
	int _minute, _currTimeMin;
	unsigned long _lastMillis;
	int _clockPosCol, _clockPosRow;
	int _setClockCursor;
	boolean _setClock, _soundAlarm;
	
};
#endif