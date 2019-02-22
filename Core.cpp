#include "Core.h"
#include <QIcon>
#include <QListIterator>

// TODO: �ƻ�����
// TODO: �ŷŷ���
// TODO: �л�����
// TODO: ���Զ�̹���
// TODO: ������ͨ����
// TODO: ����Զ�̹���
// TODO: ��Ч
// TODO: Setting
// TODO: ���µ�ͼ
// TODO: �������������λ��

Core::Core(QString archivePath)
{
	this->showFullScreen();
	this->setWindowIcon(QIcon(":lancher/image/icon.png"));
	path = archivePath;
	file = new QFile(path+"/map.txt");
	loadMapData();
	resetGame();
	windowStartPoint.row = 0;
	windowStartPoint.col = 0;
	player = new Player();
	player->realPosition = positionConvertor(player->positionRelativeToScreen);
	generateMobs();
	mousePoint.col = WORLD_COL*SIZE;
	mousePoint.row = WORLD_ROW*SIZE;
}


Core::~Core()
{

}

void Core::resetGame()
{
	renderTimer = startTimer(1000/FPS);
	mobsMoveTimer = startTimer(MOBS_MOVE_TIMER);
}

void Core::startGame()
{

}

bool Core::loadMapData()
{
	if (!file->open(QIODevice::ReadWrite | QIODevice::Text)) {
		return false;
	}
	QTextStream output(file);
	QStringList line;
	for (int row = 0; row < WORLD_ROW; row++)
	{
		line = output.readLine().split(" ");
		QListIterator<QString> listIterator(line);
		for (int col = 0; col < WORLD_COL; col++)
		{
			board[row][col] = (unsigned short int)listIterator.next().toInt();
		}
	}
	return true;
}

void Core::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	// ��Ⱦ����
	for (int row = 0; row < SCREEN_ROW; ++row)
	{
		for (int col = 0; col < SCREEN_COL; ++col) // ��Ⱦʱ���ݾ�������
		{
			switch (board[windowStartPoint.row + row][windowStartPoint.col + col])
			{
			case GLASS:
				painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));

				break;
			case WATER:
				painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));

				break;
			case LEAF:
				painter.setBrush(QBrush(Qt::darkGreen, Qt::SolidPattern));

				break;
			case STONE:
				painter.setBrush(QBrush(Qt::gray, Qt::SolidPattern));

				break;
			default:
				break;
			}
			painter.drawRect(col*SIZE, row*SIZE, SIZE, SIZE);
		}
	}
	// ��Ⱦ�����ָ��
	painter.setBrush(Qt::NoBrush);
	QPen pen;
	pen.setStyle(Qt::DotLine);
	pen.setWidth(3);
	pen.setColor(Qt::red);
	painter.setPen(pen);
	painter.drawRect(mousePoint.col, mousePoint.row, SIZE, SIZE);
	// ��Ⱦ���������ң�
	renderMobs();
}

// ������Ӧ�� ʵ�� ������Ⱦ��������ÿ����Ⱦǰ�Ÿ��������Ļ����
void Core::renderMobs()
{
	QPainter painter(this);
	// ��Ⱦ����
	QVector<Organism*> ::iterator iter;
	for (iter = mobsList->begin(); iter != mobsList->end(); iter++)
	{
		if ((*iter) == NULL) continue;
		updateScreenPosition(*iter);
		QRectF targetPos((*iter)->positionRelativeToScreen.col, (*iter)->positionRelativeToScreen.row, 40, 40);
		painter.drawImage(targetPos, (*iter)->image);
	}


	// ��Ⱦ���;��ұ��������Ⱦ
	updateScreenPosition(player);
	QRectF targetPosition(player->positionRelativeToScreen.col, player->positionRelativeToScreen.row, 40, 40);
	painter.drawImage(targetPosition, player->image);
}

void Core::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Up:
		moveWindow(UP);
		break;
	case Qt::Key_Down:
		moveWindow(DOWN);
		break;
	case Qt::Key_Left:
		moveWindow(LEFT);
		break;
	case Qt::Key_Right:
		moveWindow(RIGHT);
		break;
	case Qt::Key_Escape:
		this->close();
		break;
	case Qt::Key_W:
		movePlayer(UP);
		break;
	case Qt::Key_S:
		movePlayer(DOWN);
		break;
	case Qt::Key_A:
		movePlayer(LEFT);
		break;
	case Qt::Key_D:
		movePlayer(RIGHT);
		break;

	default:
		break;
	}
}

void Core::mousePressEvent(QMouseEvent * event)
{
	mousePoint.row = (event->y() / SIZE)*SIZE;
	mousePoint.col = (event->x() / SIZE)*SIZE;
	mouseGridPoint = screenPositionToScreenGridPosition(mousePoint);
	qDebug() << "mousePoint.col " << mousePoint.col << "; mousePoint.row " << mousePoint.row;
	if (event->button() == Qt::LeftButton) // TODO: �ж���ҵ���Ϊ
	{
		playerNormalAttack();
	}

}

// ���ݷ������screen��ʾ�Ĳ�λ����ֹԽ�磻�˺��������� windowStartPoint
void Core::moveWindow(int direction, int moveStep)
{
	switch (direction)
	{
	case UP:
		windowStartPoint.row -= moveStep;
		break;
	case DOWN:
		windowStartPoint.row += moveStep;
		break;
	case LEFT:
		windowStartPoint.col -= moveStep;
		break;
	case RIGHT:
		windowStartPoint.col += moveStep;
		break;
	default:
		break;
	}
	if (windowStartPoint.col < 0) windowStartPoint.col = 0;
	if (windowStartPoint.row < 0) windowStartPoint.row = 0;
	if (windowStartPoint.col >= WORLD_COL - SCREEN_COL) windowStartPoint.col = WORLD_COL - SCREEN_COL;
	if (windowStartPoint.row >= WORLD_ROW - SCREEN_ROW) windowStartPoint.row = WORLD_ROW - SCREEN_ROW;
}

void Core::moveWindow(int direction)
{
	moveWindow(direction, WINDOW_MOVE_STEP);
}

// ����ƶ�
void Core::movePlayer(int direction) 
{
	if (isMobNearScreenBorder(player, direction))
	{
		moveWindow(direction, SCREEN_MOVE_DISTANCE);
	}

	if (isAbleToGo(player, direction))
	{
		movePoint(player->realPosition, direction, player->speed); // ʵ�������Ѹ���
	}
}

// �����ƶ�
void Core::moveMobs(Organism* mob, int direction) 
{
	if (isAbleToGo(mob, direction))
	{
		movePoint(mob->realPosition, direction, mob->speed);
	}
}

// ÿ�μ�鶼���������Ļ����; ��������ڿ���������true
bool Core::isMobNearScreenBorder(Organism * mobs, int direction)
{
	// ȷ����Ļ������ȷ
	updateScreenPosition(mobs);
	bool colMin = mobs->positionRelativeToScreen.col <= DISTANCE_TO_SCREEN_BORDER;
	bool colMax = mobs->positionRelativeToScreen.col + DISTANCE_TO_SCREEN_BORDER >= SCREEN_COL * SIZE;
	bool rowMin = mobs->positionRelativeToScreen.row <= DISTANCE_TO_SCREEN_BORDER;
	bool rowMax = mobs->positionRelativeToScreen.row + DISTANCE_TO_SCREEN_BORDER >= SCREEN_ROW * SIZE;

	if ((direction == UP && rowMin) || (direction == DOWN && rowMax) || (direction == LEFT && colMin) || direction == RIGHT && colMax)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// ��Ҫ׼ȷ��ʵ������
bool Core::isAbleToGo(Organism * mobs, int direction)
{
	Point myGridPosition = pixelToGrid(mobs->realPosition);
	switch (direction)
	{
	case UP:
		myGridPosition.row -= 1;
		break;
	case DOWN:
		myGridPosition.row += 1;
		break;
	case LEFT:
		myGridPosition.col -= 1;
		break;
	case RIGHT:
		myGridPosition.col += 1;
		break;
	default:
		break;
	}
	// ����Ƿ�Խ��
	if (myGridPosition.row < 0 || myGridPosition.col < 0 || myGridPosition.col >= WORLD_COL || myGridPosition.row >= WORLD_ROW)
	{
		return false;
	}

	// ����Ƿ��ǿ���ͨ���ķ���
	if (board[myGridPosition.row][myGridPosition.col] == LEAF || board[myGridPosition.row][myGridPosition.col] == STONE)
	{
		return false;
	}
	
	// ����Ƿ�������赲
	if (mobs != player)
	{
		updateScreenPosition(player);
		if (myGridPosition == screenPositionToScreenGridPosition(player->positionRelativeToScreen))
		{
			return false;
		}
	}

	// ����Ƿ����������赲
	if (ENABLE_CHECK_ALL)
	{
		QVector<Organism*>::iterator iter;
		for (iter = mobsList->begin(); iter != mobsList->end(); iter++)
		{
			if (*iter == NULL) continue;
			if (mobs != (*iter))
			{
				updateScreenPosition(*iter);
				if (myGridPosition == screenPositionToScreenGridPosition((*iter)->positionRelativeToScreen))
				{
					return false;
				}
			}
		}
	}
	return true;;
}

Point Core::pixelToGrid(Point inPixel)
{
	Point inGrid;
	inGrid.col = inPixel.col / SIZE;
	inGrid.row = inPixel.row / SIZE;
	return inGrid;
}

Point Core::screenPositionToScreenGridPosition(Point screenPosition)
{
	Point result;
	result.col = (screenPosition.col / SIZE);
	result.row = (screenPosition.row / SIZE);
	return result;
}

int Core::min(int a, int b)
{
	if (a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

void Core::moveAllMobs()  // ��Mobs�����ö�ʱ����ʵ���ٶȿ��Ʋ����ո����ж�
{
	QVector<Organism*> ::iterator iter;
	for (iter = mobsList->begin(); iter != mobsList->end(); iter++)
	{
		if ((*iter) == NULL) continue;
		moveMobs(*iter, (*iter)->desiredDirection());
	}
}

// ȷ����������ʵ��ͼ�߽磻��λ������
void Core::movePoint(Point& point, int direction, int moveDistance) 
{
	switch (direction)
	{
	case UP:
		point.row -= moveDistance;
		break;
	case DOWN:
		point.row += moveDistance;
		break;
	case LEFT:
		point.col -= moveDistance;
		break;
	case RIGHT:
		point.col += moveDistance;
		break;
	case STAY:
		break;
	default:
		break;
	}
	if (point.col < 0) point.col = 0;
	if (point.row < 0) point.row = 0;
	if (point.col >= (WORLD_COL)*SIZE) point.col = (WORLD_COL - 1)*SIZE;
	if (point.row >= (WORLD_ROW)*SIZE) point.row = (WORLD_ROW - 1)*SIZE;
}

// ���ɹ���
void Core::generateMobs()
{
	srand(static_cast<unsigned int>(time(0)));
	mobsList = new QVector<Organism*>; 
	Point generatedScreenPosition;
	for (int i = 0; i < MOBS_NUMBER; i++)
	{
		generatedScreenPosition.row = (rand() % SCREEN_ROW)*SIZE;
		generatedScreenPosition.col = (rand() % SCREEN_COL)*SIZE;
		mobsList->append(new Pig(positionConvertor(generatedScreenPosition)));
	}
}

// ��ҽ��й���������λ��ΪmousePoint
void Core::playerNormalAttack()
{
	Point mouseGridPosition = screenPositionToScreenGridPosition(mousePoint);
	qDebug() << "Player attack STH, point: row: " << mouseGridPosition.row << " col: " << mouseGridPosition.col;
	int mouseDirection;
	QVector<Organism*>::iterator iter;
	for (iter=mobsList->begin();iter!=mobsList->end();iter++)
	{
		if (*iter == NULL) continue;
		updateScreenPosition(*iter);
		Point mobGridPosition = screenPositionToScreenGridPosition((*iter)->positionRelativeToScreen);
		if ( mouseGridPosition == mobGridPosition) 
		{
			(*iter)->beAttacked(player->finalAttackPower);
			if ((*iter)->isDead)
			{
				delete *iter;
				mobsList->erase(iter); // ɾ���������ǰ�ƣ�����
			}
		}
	}
}

// �����������������ң���״̬��Ѫ�����Ƿ�������
void Core::adjustAllMobsStatus() // TODO: �����������������ң���״̬
{

}


// ������Ϊ��λ���Ǹ���Ļ����-->��������
Point Core::positionConvertor(Point screenPostion)
{
	Point realPostion;
	realPostion.row = screenPostion.row + windowStartPoint.row*SIZE;
	realPostion.col = screenPostion.col + windowStartPoint.col*SIZE;
	return realPostion;
}

// ��������-->��Ļ����; ע�⣺ת������ܲ�������Ļ��
Point Core::absolutePositionConvertor(Point absolutePosition)
{
	Point screenPosition;
	screenPosition.row = absolutePosition.row - windowStartPoint.row*SIZE;
	screenPosition.col = absolutePosition.col - windowStartPoint.col*SIZE;
	return screenPosition;
}

// ������ʵ�������������Ļ����
void Core::updateScreenPosition(Organism * mobs)
{
	mobs->positionRelativeToScreen = absolutePositionConvertor(mobs->realPosition);
}


void Core::timerEvent(QTimerEvent *event)
{

	//ˢ�»���
	if (event->timerId() == renderTimer)
	{
		update();
		
	}
	if (event->timerId() == mobsMoveTimer)
	{
		// �����ƶ�
		moveAllMobs();
		adjustAllMobsStatus();
	}
}
