#include "Organism.h"



Organism::Organism()
{
	name = "Organism";

}

// �ṩ������ʵ�����꣬��λΪ����
Organism::Organism(Point givenRealPosition)
{
	Organism();
	realPosition = givenRealPosition;
}

Organism::~Organism()
{

}

int Organism::desiredDirection()
{
	return rand() % 5;
}

int Organism::getRealMove(int speed)
{
	return (int)((double)(speed*SPEED_SCALAR) / (double)(FPS));
}