#ifndef Clock_h
#define Clock_h
static int _daysInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
class Clock
{
  public:
    Clock(int clockPosCol, int clockPosRow)
     : _year(2013),_setClockCursor(0),
      _month(1),
      _day(1),
      _hour(0),
      _minute(0),
	  _second(0),
	  _setClock(false),
	  _drawSecond(false),
	  _clockPosCol(clockPosCol),
	  _clockPosRow(clockPosRow),
	  _drawBigClock(false)
      { _lastMillis = 0; }; 

    void update()
	{ 
		if(!_setClock)
		{
			//unsigned long currentMillis = millis();  
			
			//if(currentMillis-_lastMillis > 1000)
			//{
			//	_lastMillis = currentMillis;
				_second += 1;
			//}
			if(_second > 59)
			{
				_second = 0;
				_minute += 1; 
			}
			if(_minute > 59)
			{
				_minute = 0;
				_hour += 1; 
			}
			if(_hour > 23)
			{
				_hour = 0;
				_day += 1;
			}
			if(_month != 2 && _day > _daysInMonth[_month-1])
			{
				_day = 1;
				_month += 1;
			}else if(_month == 2)
			{
				int daysInCurrMonth = _daysInMonth[_month-1];
				if(_year%4 == 0)
					daysInCurrMonth++;
				if(_day > daysInCurrMonth)
				{
					_day = 0;
					_month += 1;
				}
			}
			if(_month > 12)
			{
				_month = 1;
				_year += 1;
			}
		}
		else
		{
			unsigned long currentMillis = millis();  
			if(currentMillis-_lastMillis > 1000)
			{
				_lastMillis = currentMillis;
			}
			_second = 0;
		}
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
			else if(_setClockCursor == 2 && _drawSecond)
				 type = "second";

			if(type == "year")
			{
				_year += var;
			}
			else if(type == "month")
			{
				_month += var;
				if(_month>12)
					_month = 1;
				else if(_month<1)
					_month = 12;
			}
			else if(type == "day")
			{
				int daysInMonth =  _daysInMonth[_month-1];
				if(_month == 2 && _year%4 == 0)
				{
					daysInMonth += 1;
				}
				_day += var;
				if(_day>daysInMonth)
					_day = 1;
				else if(_day<1)
					_day = daysInMonth;
			}
			else if(type == "hour")
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
			else if(type == "second")
			{
				_second += var;
				if(_second>59)
					_second = 0;
				else if(_second<0)
					_second = 59;
			}				
		}
	};
	void drawNumber(int val, int col)
	{
		switch(val)
		{
			case 0 :
				{				
					//NOLL
					lcd.setCursor(col, 0);  
					lcd.print(" _ ");
					lcd.setCursor(col, 1);
					lcd.print("| |");
					lcd.setCursor(col, 2);
					lcd.print("|_|");
					break;
				}					
			case 1 : 
				{					
					//ETT
					lcd.setCursor(col, 0);
					lcd.print("   ");					
					lcd.setCursor(col, 1);
					lcd.print("  |");
					lcd.setCursor(col, 2);
					lcd.print("  |");
					break;
				}
			case 2 :
				{
					//TVÅ
					lcd.setCursor(col, 0);  
					lcd.print(" _ ");
					lcd.setCursor(col, 1);
					lcd.print(" _|");
					lcd.setCursor(col, 2);
					lcd.print("|_ ");
					break;
				}					
			case 3 :
				{
					//TRE
					lcd.setCursor(col, 0);  
					lcd.print("__");
					lcd.setCursor(col, 1);
					lcd.print("__|");
					lcd.setCursor(col, 2);
					lcd.print("__|");
					break;
				}					
			case 4 :
				{
					//FYRA    
					lcd.setCursor(col, 0);
					lcd.print("   ");
					lcd.setCursor(col, 1);
					lcd.print("|_|");
					lcd.setCursor(col, 2);
					lcd.print("  |");
					break;
				}					
			case 5 :
				{				
					// FEM
					lcd.setCursor(col,0);
					lcd.print(" _ ");
					lcd.setCursor(col,1);
					lcd.print("|_ ");
					lcd.setCursor(col,2);
					lcd.print(" _|");
					break;
				}					
			case 6 :
				{
					//SEX
					lcd.setCursor(col, 0);  
					lcd.print(" _ ");
					lcd.setCursor(col, 1);
					lcd.print("|_ ");
					lcd.setCursor(col, 2);
					lcd.print("|_|");
					break;
				}					
			case 7 :
				{
					//SJU
					lcd.setCursor(col, 0);  
					lcd.print("___");
					lcd.setCursor(col, 1);
					lcd.print("  /");
					lcd.setCursor(col, 2);
					lcd.print(" / ");
					break;
				}					
			case 8 :
				{
					//ÅTTA
					lcd.setCursor(col, 0);  
					lcd.print(" _ ");
					lcd.setCursor(col, 1);
					lcd.print("|_|");
					lcd.setCursor(col, 2);
					lcd.print("|_|");
					break;
				}					
			case 9 :
				{
					//NIO
					lcd.setCursor(col, 0);  
					lcd.print(" _ ");
					lcd.setCursor(col, 1);
					lcd.print("|_|");
					lcd.setCursor(col, 2);
					lcd.print("  |");
					break;
				}
		}
	}
	void drawClock()
	{
		unsigned long currentMillis = millis(); 
		int seconds_1 = _second % 10;
		int seconds_10 = (_second - seconds_1)/10; 
		int minutes_1 = _minute % 10;
		int minutes_10 = (_minute - minutes_1)/10;
		int hours_1 = _hour % 10;
		int hours_10 = (_hour - hours_1)/10;
		if(_drawBigClock)
		{
			//KOLON
			lcd.setCursor(9, 1);
			lcd.write(255);
			lcd.setCursor(9, 2);
			lcd.write(255);
			drawNumber(hours_10, 1);
			drawNumber(hours_1, 5);
			drawNumber(minutes_10, 11);
			drawNumber(minutes_1, 15);
			
		}
		else 
		{
			lcd.setCursor(_clockPosCol, _clockPosRow);
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
			if(_drawSecond)
			{
				lcd.print(':');
				lcd.print(seconds_10);
				lcd.print(seconds_1);
			}
		}
	};
	void drawDate(){};
	void enableIncrement(boolean bol){ _setClock = bol; };
	void drawBigClock(boolean bol){ _drawBigClock = bol; };
	boolean isIncrementEnabled() { return _setClock; };
	int getClockRowPos(){ return _clockPosRow; };
	int getClockColPos(){ return _clockPosCol; };
	int getCurrentTime(String val)
	{ 
		if(val == "hour")
			return _hour;
		else if(val == "minute")
			return _minute;
		else
			return -1;
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
	
        
  private:
  
	unsigned long _lastMillis;
	int _year;
	int _month;
	int _day;
	int _hour;
	int _minute;
	int _second;
	int _clockPosCol, _clockPosRow;
	int _setClockCursor;
	boolean _setClock,_drawSecond, _drawBigClock;
};
#endif
/*
    int months_1 = month % 10;
  int months_10 = (month - months_1)/10; 
  int days_1 = day % 10;
  int days_10 = (day - days_1)/10;
  lcd.setCursor(0, 0);
  lcd.print(year);
  lcd.print("-");
  lcd.print(months_10);
  lcd.print(months_1);
  lcd.print('-'); 
  lcd.print(days_10);
  lcd.print(days_1);
*/