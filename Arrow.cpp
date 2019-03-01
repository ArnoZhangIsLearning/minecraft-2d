#include "Arrow.h"



Arrow::Arrow(int direction_, Point realGrid)
{
	// ����λ�����ꣻ�����������꣨realGrid, realPosition��
	realPosition.row = realGrid.row * SIZE;
	realPosition.col = realGrid.col * SIZE;
	direction = direction_;

	image = QImage(":/lancher/image/game/arrow.png");
	attakPower = 4; // TODO: �˴�������Ҫ����
	speed = 25;
	isPenetrateAble = false;  // �������⣬����򿪣����ｫһ�����ܵ�����˺�
	isMoving = true;
	isPlayer = false;
}

Arrow::~Arrow()
{

}
