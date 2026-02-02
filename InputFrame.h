//
// InputFrame.h
//

#ifndef INPUTFRAME_H
#define INPUTFRAME_H

// Engine includes.
#include "Object.h"

const std::string INPUTFRAME_STRING = "InputFrame";

class InputFrame {

private:
	int direction;          // Direction bitmask
	int directionPressed;   // Direction pressed on current frame
	int button;				// Button bitmask
	int buttonPressed;		// Button pressed on current frame
	int frame;				// Current frame

public:
	int getDirection();			// Returns direction bimask
	int getDirectionPressed();	// Returns if direction was pressed on current frame
	int getButton();			// Returns button bitmask
	int getButtonPressed();		// Returns if button was pressed on current frame
	int getFrame();				// Returns current frame

	void setDirection(int direction);					// Sets direction bimask
	void setDirectionPressed(int directionPressed);		// Sets if direction was pressed on current frame
	void setButton(int button);							// Sets button bitmask
	void setButtonPressed(int buttonPressed);			// Sets if button was pressed on current frame
	void setFrame(int frame);							// Sets current frame
};

#endif // INPUTFRAME_H
