#pragma once
#include <qwidget.h>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <cstdlib>
#include <ctime>
#include <QImage>
#include <QPainter>
#include <QRectF>
#include <QVector>
#include <QColor>
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
	   
	QString articleName[MAX_ARTICLE_NUM];
	void resetGame();
	void startGame();
	void quitGame();
	bool loadMapData();
	bool saveMapData();
	virtual void paintEvent(QPaintEvent *event); //����ˢ��
	virtual void timerEvent(QTimerEvent *event); //��ʱ���¼�
	virtual void keyPressEvent(QKeyEvent *event); //������Ӧ
	void mousePressEvent(QMouseEvent *event);
	// virtual void keyReleaseEvent(QKeyEvent *event); //������Ӧ

private:
	int renderTimer;
	int mobsMoveTimer;
	Point mousePoint; // ��λ����
	Point mouseGridPoint; // ָ�����λ�ã���λ����
	Player *player;
	QVector<Organism*> *mobsList; 
	Point windowStartPoint; // ��λ����
	void setArticleName();
	void renderMobs(); 
	void renderArticleList();
	void moveWindow(int direction, int moveStep); 
	void moveWindow(int direction);
	void movePlayer(int direction);
	void moveMobs(Organism* mob, int direction);
	void moveAllMobs();
	void movePoint(Point& point, int direction, int moveDistance);
	void generateMobs();
	void playerNormalAction();
	void playerMining(Point miningPoint);
	void playerCreateCube(Point createPoint);
	void adjustAllMobsStatus();
	Point positionConvertor(Point screenPostion);
	Point absolutePositionConvertor(Point absolutePosition);
	void updateScreenPosition(Organism* mobs);
	bool isMobNearScreenBorder(Organism* mobs, int direction);
	bool isAbleToGo(Organism* mobs, int direction);
	bool isActionValid();
	bool isCubeCanBeCreateOn(Point position);
	Point pixelToGrid(Point inPixel);
	Point screenPositionToScreenGridPosition(Point screenPosition);
	Point screenGridToRealGrid(Point screenGrid);
	int min(int a, int b);
};

