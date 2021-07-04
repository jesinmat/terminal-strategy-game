#include "dailyAction.hpp"

CWorldAction::CWorldAction(unsigned short people, unsigned short metal, unsigned short powerDrop, unsigned short destructionChance, std::string name)
: name(name), metal(metal), people(people), powerDrop(powerDrop), destructionChance(destructionChance) {
}

