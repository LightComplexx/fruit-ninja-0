//
// InputDisplayEntry.h
//

#ifndef INPUTDISPLAYENTRY_H
#define INPUTDISPLAYENTRY_H


class InputDisplayEntry {

private:
	int direction;			// Direction that was pressed
	int button;				// Button that was pressed
	int framesHeld;			// Number of frames the button was held

public:
	int getDirection();		// Returns direction that was pressed
	int getButton();		// Returns button that was pressed
	int getFramesHeld();	// Returns number of frames all inputs were held

	void setDirection(int direction);		// Sets direction that was pressed
	void setButton(int button);		// Sets button that was pressed
	void setFramesHeld(int framesHeld);	// Sets number of frames all inputs were held
};

#endif // INPUTDISPLAYENTRY_H
