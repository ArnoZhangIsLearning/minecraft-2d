#pragma once
#include "Organism.h"

class Player :
	public Organism
{
public:
	Player();
	~Player();
	int articleList[MAX_ARTICLE_NUM];
	int finalAttackPower;
	double level;
	void experienceAdd(double gain);
	int currentArticleType;
	void loadAticleList();
	void changeCurrentHold(int step);

	// �����ݳ�Աδ�ڹ��캯���г�ʼ����ʹ��֮ǰ��ȷ����ֵ��ȷ
	Point realGrid;
};

