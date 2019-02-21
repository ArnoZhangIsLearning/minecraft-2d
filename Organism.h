#pragma once
#include "Constant.h"
#include <Qstring>
#include <QImage>


class Organism
{
public:
	Organism();
	Organism(Point givenRealPosition);
	~Organism();
	QString name;

	int speed; //ʵ���ƶ��������������Ϊ����
	int blood;
	Point positionRelativeToScreen; // ��λӦ����Ϊ����
	Point realPosition; // ��λ�����أ���������Ļλ��;ע�⣺һ��Ҫ*SIZE

	int attakPower;
	int attackRange;
	int attackInterval;
	int armor;
	int facing;
	int desiredDirection();
	int getRealMove(int speed);
	QImage image;
};

