#include "Player.h"



Player::Player()
	:Organism()
{
	image = QImage(":/lancher/image/game/steven.png");
	speed = getRealMove(PLAYER_SPEED);
	blood = PLAYER_BLOOD;
	armor = PLAYER_ARMOR;
	attakPower = PLAYER_ATTACK_POWER;
	attackRange = PLAYER_ATTACK_RANGE;
	attackInterval = PLAYER_ATTACK_INTERVAL;
	level = 0;
	positionRelativeToScreen.row = 6*SIZE;
	positionRelativeToScreen.col = 10*SIZE;
}


Player::~Player()
{
}
