#include "Skeleton.h"



Skeleton::Skeleton(Point givenRealPosition)
	:Organism(givenRealPosition)
{
	image = QImage(":/lancher/image/game/skeleton2.png");
	blood = SKELETON_BLOOD;
	armor = SKELETON_ARMOR;
	name = "Skeleton";
	hurtSound = "sound/minecraft/sketon_hurt.mp3";
	deadSound = "sound/minecraft/skeleton_death.mp3";
	dropItemNum = 5;
	dropItemType = ARROW;
	experience = 0.4;
}


Skeleton::~Skeleton()
{
}

//// �������õ�ǰ���빥��
//int Skeleton::desiredDirection()
//{
//	return 0;
//}

