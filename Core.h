#pragma once
#include <qwidget.h>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QPaintEvent>
#include <QKeyEvent>
#include <cstdlib>
#include <ctime>
#include <QImage>
#include <QPainter>
#include <QRectF>
#include <QVector>
#include "Constant.h"
#include "Organism.h"
#include "Player.h"
#include "Pig.h"

class Core :
	public QWidget
{
public:
	Core(QString archivePath);
	~Core();
	QString path;
	QFile *file;
	unsigned short int board[WORLD_ROW][WORLD_COL]; // �У���
	   

	void resetGame();
	void startGame();
	bool loadMapData();
	virtual void paintEvent(QPaintEvent *event); //����ˢ��
	virtual void timerEvent(QTimerEvent *event); //��ʱ���¼�
	virtual void keyPressEvent(QKeyEvent *event); //������Ӧ
	// virtual void keyReleaseEvent(QKeyEvent *event); //������Ӧ

private:
	int renderTimer;
	int mobsMoveTimer;
	Player player;
	QVector<Organism*> *mobsList; 
	Point windowStartPoint;
	void renderMobs(); 
	void moveWindow(int direction, int moveStep); 
	void moveWindow(int direction);
	void movePlayer(int direction);
	void moveMobs(Organism* mob, int direction);
	void moveAllMobs();
	void movePoint(Point& point, int direction, int moveDistance);
	void generateMobs();
	Point positionConvertor(Point screenPostion);
	Point absolutePositionConvertor(Point absolutePosition);

};

