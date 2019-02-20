#pragma once
#include <qwidget.h>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QPaintEvent>
#include <QKeyEvent>
#include <cstdlib>
#include <ctime>
#include <QPainter>

const int WORLD_ROW = 270;
const int WORLD_COL = 480;
const int SIZE = 40;
const int SCREEN_ROW = 27;
const int SCREEN_COL = 48;


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
private:
	int renderTimer;
};

