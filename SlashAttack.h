//
// SlashAttack.h
//

// Engine includes
#include "Object.h"

const std::string SLASHATTACK_STRING = "SlashAttack";

class SlashAttack : public df::Object {
private:
	int time_to_live;
	bool is_electric;

public:
	SlashAttack(df::Vector location, bool electric);
	int draw();
	int eventHandler(const df::Event* p_e);
	bool getIsElectric();
};