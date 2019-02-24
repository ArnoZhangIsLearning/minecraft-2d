#pragma once
#include "Constant.h"
#include <Qstring>
#include <QImage>
#include <QMediaPlayer>

class Organism
{
public:
	Organism();
	Organism(Point givenRealPosition);
	~Organism();
	QString name;
	QMediaPlayer soundPlayer;
	QString hurtSound;
	QString deadSound;

	int speed; //ʵ���ƶ��������������Ϊ����
	int blood;
	int direction;
	Point positionRelativeToScreen; // ��λӦ����Ϊ����
	Point realPosition; // ��λ�����أ���������Ļλ��
	bool isPenetrateAble;
	int attakPower;
	int attackRange;
	int attackInterval;
	int armor;
	int facing;
	int desiredDirection();
	int getRealMove(int speed); // �˺����Ĺ���������������������Ҫ�����޸�
	bool isDead;
	// �Ƿ����ڹ���
	bool attackNow;
	// ��������
	int attackDirection;
	QImage image;

	void killed();
	void beAttacked(int demage);
	void constructorTool();
	int min(int a, int b);
};

