//
// Bonus.h
//

// Engine includes
#include "ViewObject.h"

class Bonus : public df::ViewObject {
private:
	int time_to_live;

public:
	Bonus();
	~Bonus();
	int draw() override;
	int eventHandler(const df::Event* p_e) override;
};