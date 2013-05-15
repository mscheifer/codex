#include "PowerUp.h"

PowerUp::PowerUp(void)
{
	Respown_Counter = sf::Clock();
	active = true;
}

void PowerUp::onCollision(Player* player) {
	//if it's not active, dont do anything

	if(active == false )
		return ;
	
	Player luckyGuy = *player;
	luckyGuy.setHealth(luckyGuy.getHealth() + healthMutiplyer);
	luckyGuy.setSpeed(luckyGuy.getSpeed() + speedMutiplyer);
	luckyGuy.setMana(luckyGuy.getMana() + manaMutiplyer);
	active = false;
	
	Respown_Counter.restart();
}

void PowerUp::update() {
	if ((Respown_Counter.getElapsedTime().asMilliseconds() >= Respown_Time) && active == false) {
		active = true;
	}
}
