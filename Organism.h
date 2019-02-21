#pragma once
#include "Constant.h"
#include <Qstring>


class Organism
{
public:
	Organism();
	Organism(Point givenRealPosition);
	~Organism();
	QString name;

	int speed; //ʵ���ƶ��������������Ϊ����
	int blood;
	Point positionRelativeToScreen; // ��λ����
	Point realPosition; // ��λ�����أ���������Ļλ��;ע�⣺һ��Ҫ*SIZE

	int attakPower;
	int attackRange;
	int attackInterval;
	int armor;
	int desiredDirection();
	int getRealMove(int speed);
};

