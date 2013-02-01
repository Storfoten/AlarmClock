#ifndef ToggleButton_h
#define ToggleButton_h
class ToggleButton
{
  public:
    ToggleButton(int pin)
     : _pin(pin),
      _isChanged(false),
      _isDown(false),
      _isPressed(false),
      _isReleased(false),
	  _inverted(false),
	  _delayUpdate(0),
	  _lastMillis(0),
	  pause(false)
	{ pinMode(_pin,INPUT); };     
    void update()
      {
		unsigned long currMillis = millis();
		boolean currState = digitalRead(_pin) ^ _inverted; 
		if(((currState && _isDown) || (!currState && !_isDown)) && !pause)
		{
			_isChanged = false;
			_isPressed = false;
			_isReleased = false;
		}else if(currState && !_isDown && !pause)
		{
			_isPressed = true;
			_isReleased = false;
			_isChanged = true;
		}else if(!currState && _isDown && !pause)
		{
			_isPressed = false;
			_isReleased = true;
			_isChanged = true;
		}

		
		if(_delayUpdate > 0 && _isPressed)
		{
			if(currMillis-_lastMillis < _delayUpdate)
				pause = true;
			else
			{
				_lastMillis = currMillis;
				pause = false;
			}
        }		
		if(!pause)
			_lastMillis = currMillis;
		_isDown = currState;		
     };
     boolean isButtonDown(){return _isDown;};
	 void setButtonPressedTime(unsigned long millis){_delayUpdate = millis;};
	 void setInvertedButton(){_inverted = true;};
     boolean isButtonChanged(){return _isChanged;};
     boolean isButtonPressed(){return _isPressed;};
     boolean isButtonReleased(){return _isReleased;};
        
  private:
    boolean _isDown;
    boolean _isChanged;
    boolean _isPressed;
    boolean _isReleased;
	boolean _inverted;
	boolean pause;
	unsigned long _delayUpdate;
	unsigned long _lastMillis;
    int _pin;
};
#endif