#pragma once
#include "Constant.h"
#include <cstdlib>
#include <ctime>
#include <Qstring>


class Organism
{
public:
	Organism();
	~Organism();
	QString name;

	int speed; //ʵ���ƶ��������������Ϊ����
	int blood;
	Point positionRelativeToScreen; // ��λ����
	Point realPosition; // ��λ�����أ���������Ļλ��

	int attakPower;
	int attackRange;
	int attackInterval;
	int armor;
	int desiredDirection();
	int getRealMove(int speed);
};

