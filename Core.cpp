#include "Core.h"
#include <QIcon>
#include <QListIterator>


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
	setArticleName();
}

void Core::setArticleName()
{
	articleName[BASE] = "BASE";
	articleName[GLASS] = "GLASS";
	articleName[EARTH] = "EARTH";
	articleName[STONE] = "STONE";
	articleName[WOOD] = "WOOD";
	articleName[LEAF] = "LEAF";
	articleName[WATER] = "WATER";

	articleName[SWORD] = "SWORD";
	articleName[AXE] = "AXE";
	articleName[PICK] = "PICK";
	articleName[SHOVEL] = "SHOVEL";
	articleName[BOW] = "BOW";
	articleName[ARROW] = "ARROW";
	articleName[MEAT] = "MEAT";
}


Core::~Core()
{

}

void Core::resetGame()
{
	renderTimer = startTimer(1000/FPS);
	mobsMoveTimer = startTimer(MOBS_MOVE_TIMER);
	arrowMoveTimer = startTimer(50); // TODO: arrowMoveTimer
}

void Core::startGame()
{

}

void Core::quitGame()
{
	saveMapData();
	this->close();
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
	file->close();
	return true;
}

bool Core::saveMapData()
{
	if (file->open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream input(file);
		for (int row = 0; row < WORLD_ROW; row++)
		{
			for (int col = 0; col < WORLD_COL; col++)
			{
				input << board[row][col] << " ";
			}
			input << endl;
		}
		qDebug() << "successfully save map data";
		file->close();
		return true;
	}
	return false;
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
				painter.setBrush(QBrush(QColor::fromRgb(0, 255, 127), Qt::SolidPattern));
				break;
			case WATER:
				painter.setBrush(QBrush(QColor::fromRgb(0, 0, 255), Qt::SolidPattern));
				break;
			case LEAF:
				painter.setBrush(QBrush(QColor::fromRgb(0, 139, 69), Qt::SolidPattern));
				break;
			case STONE:
				painter.setBrush(QBrush(QColor::fromRgb(156, 156, 156), Qt::SolidPattern));
				break;
			case WOOD:
				painter.setBrush(QBrush(QColor::fromRgb(139, 71, 38), Qt::SolidPattern));
				break;
			case BASE:
				painter.setBrush(QBrush(QColor::fromRgb(0, 0, 0), Qt::SolidPattern));
				break;
			case EARTH:
				painter.setBrush(QBrush(QColor::fromRgb(232, 139, 0), Qt::SolidPattern));
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

	// ��Ⱦ��������ʸ��
	renderArrows();

	// ��Ⱦ״̬������Ʒ����
	renderArticleList();

}

// ��Ⱦ��Ʒ��
void Core::renderArticleList()
{
	QString text("");
	// Ŀǰ������Ʒ
	text.append("[>>> " + articleName[player->currentArticleType] + " <<<]\t");
	text.append("LEVEL<" + QString::number(player->level) + "> BLOOD<" + QString::number(player->blood)+">\t");
	// ��ҵ�λ��
	text += ("(" + QString::number(player->realPosition.row) + " ," + QString::number(player->realPosition.col) + " )\t");
	for (int i = 0; i < MAX_ARTICLE_NUM; i++)
	{
		if (articleName[i] != NULL)
		{
			text.append(articleName[i] + "<" + QString::number(player->articleList[i]) + ">\t");
		}
	}
	QPainter painter(this);
	QPen pen;
	pen.setStyle(Qt::SolidLine);
	pen.setWidth(3);
	pen.setColor(QColor::fromRgb(148, 0, 211));
	painter.setPen(pen);
	painter.drawRect(ARTICLE_LIST_RECT);
	painter.setPen(QColor::fromRgb(255, 0, 255));
	QFont font("Geogia", 12.5, 75);
	font.setCapitalization(QFont::Capitalize);
	painter.setFont(font);
	painter.drawText(ARTICLE_LIST_RECT, Qt::AlignCenter, text);
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

void Core::renderArrows()
{
	QPainter painter(this);
	QVector<Arrow*> ::iterator iter;
	for (iter = arrowList->begin(); iter != arrowList->end(); iter++)
	{
		if ((*iter) == NULL ) continue;
		if ((*iter)->isMoving == false) continue; // �Բ��ƶ��ļ�ʸ�����ش���
		updateScreenPosition(*iter);
		QRectF targetPos((*iter)->positionRelativeToScreen.col, (*iter)->positionRelativeToScreen.row, 40, 40);

		QString imageUrl;
		if ((*iter)->direction == UP)
		{
			imageUrl = ":/lancher/image/game/arrow_up.png";
		}else if ((*iter)->direction == DOWN)
		{
			imageUrl = ":/lancher/image/game/arrow_down.png";
		}
		else if ((*iter)->direction == LEFT)
		{
			imageUrl = ":/lancher/image/game/arrow_left.png";
		}
		else if ((*iter)->direction == RIGHT)
		{
			imageUrl = ":/lancher/image/game/arrow_right.png";
		}
		painter.drawImage(targetPos, QImage(imageUrl));
	}

}

void Core::removeNotMovingArrow()
{
	QVector<Arrow*> ::iterator iter;
	for (iter = arrowList->begin(); iter != arrowList->end(); iter++)
	{
		if ((*iter) == NULL || (*iter)->isMoving == false)
		{
			delete *iter;
			arrowList->erase(iter);
		}
	}
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
		quitGame();
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
	case Qt::Key_Tab:
		player->changeCurrentHold(1);
		break;
	case Qt::Key_CapsLock:
		player->changeCurrentHold(-1);
		break;
	default:
		break;
	}
}

// �Զ�����mousePoint �� mouseGridPoint
void Core::mousePressEvent(QMouseEvent * event)
{
	mousePoint.row = (event->y() / SIZE)*SIZE;
	mousePoint.col = (event->x() / SIZE)*SIZE;
	mouseGridPoint = screenPositionToScreenGridPosition(mousePoint);
	qDebug() << "mousePoint.col " << mousePoint.col << "; mousePoint.row " << mousePoint.row;
	if (event->button() == Qt::LeftButton) 
	{
		// �ж��Ƿ���Զ�̹���
		if (player->currentArticleType == BOW)
		{
			shotArrow(player);
		}
		else
		{
			playerNormalAction();
		}
	}
	else if(event->button() == Qt::RightButton) // TODO: �ж���ҵ���Ϊ(ǰ�����߷��ã�
	{
		playerCreateCube(mouseGridPoint);
	}

}

// Զ�̹���
void Core::shotArrow(Organism* shoter)
{
	int direction;
	// ����ȷ�������뷢���ߵ�λ��
	Point shoterRealGrid = pixelToGrid(shoter->realPosition);
	Point mouseRealGrid;
	mouseRealGrid.row = mouseGridPoint.row + windowStartPoint.row;
	mouseRealGrid.col = mouseGridPoint.col + windowStartPoint.col;
	Point arrowStartRealGrid;
	if (mouseRealGrid.col < shoterRealGrid.col)
	{
		direction = LEFT;
		arrowStartRealGrid.row = shoterRealGrid.row;
		arrowStartRealGrid.col = shoterRealGrid.col - 1;

	}
	else if (mouseRealGrid.col > shoterRealGrid.col)
	{
		direction = RIGHT;
		arrowStartRealGrid.row = shoterRealGrid.row;
		arrowStartRealGrid.col = shoterRealGrid.col + 1;

	}
	else if (mouseRealGrid.row < shoterRealGrid.row)
	{
		direction = UP;
		arrowStartRealGrid.row = shoterRealGrid.row - 1;
		arrowStartRealGrid.col = shoterRealGrid.col;

	}
	else if (mouseRealGrid.row > shoterRealGrid.row)
	{
		direction = DOWN;
		arrowStartRealGrid.row = shoterRealGrid.row + 1;
		arrowStartRealGrid.col = shoterRealGrid.col;

	}
	else
	{
		direction = STAY;
		return;
	}

	qDebug() << shoter->name << " shot at direction: " << direction << " row: " << arrowStartRealGrid.row << " col: " << arrowStartRealGrid.col;
	arrowList->append(new Arrow(direction, arrowStartRealGrid));
}

// �����ʸ���ƶ��Լ���ײ����ʧ
void Core::moveAllArrows()
{
	QVector<Arrow*>::iterator iter;
	for (iter = arrowList->begin(); iter != arrowList->end(); iter++)
	{
		if ((*iter) == NULL) continue;
		if (!(*iter)->isMoving) continue;

		bool isMoving = moveArrows((*iter), (*iter)->direction);
		(*iter)->isMoving = isMoving; 
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

	if (isAbleToGo(player, direction, false))
	{
		movePoint(player->realPosition, direction, player->speed); // ʵ�������Ѹ���
	}
}

// �����ƶ�
void Core::moveMobs(Organism* mob, int direction) 
{
	if (isAbleToGo(mob, direction, mob->isPenetrateAble))
	{
		movePoint(mob->realPosition, direction, mob->speed);
	}
}

// ��ʸ�ƶ�;���ɼ����ƶ���false
bool Core::moveArrows(Arrow* arrow, int direction)
{
	if (isArrowAbleToGo(arrow, direction, arrow->isPenetrateAble))
	{
		movePoint(arrow->realPosition, direction, arrow->speed);
		return true;
	}
	return false;
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
bool Core::isAbleToGo(Organism * mobs, int direction, bool isPenetrateAble)
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
	int cubeType = board[myGridPosition.row][myGridPosition.col];
	if (cubeType == LEAF || cubeType == STONE || cubeType == WOOD)
	{
		return false;
	}
	
	if (isPenetrateAble) //�������͸��������������ײ���
	{
		return true;
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
	return true;
}

bool Core::isArrowAbleToGo(Arrow* mobs, int direction, bool isPenetrateAble)
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
	int cubeType = board[myGridPosition.row][myGridPosition.col];
	if (cubeType == LEAF || cubeType == STONE || cubeType == WOOD)
	{
		mobs->isMoving = false;
		return false;
	}

	// �Ƿ����������
	updateScreenPosition(player);
	if (myGridPosition == screenPositionToScreenGridPosition(player->positionRelativeToScreen))
	{
		// ����
		player->beAttacked(mobs->attakPower);

		if (!isPenetrateAble)
		{
			mobs->isMoving = false;
			return false;
		}
	}


	QVector<Organism*>::iterator iter;
	for (iter = mobsList->begin(); iter != mobsList->end(); iter++)
	{
		if (*iter == NULL) continue;
		updateScreenPosition(*iter);
		if (myGridPosition == screenPositionToScreenGridPosition((*iter)->positionRelativeToScreen))
		{
			(*iter)->beAttacked(mobs->attakPower);
			if (!isPenetrateAble)
			{
				mobs->isMoving = false;
				if ((*iter)->isDead)
				{
					delete *iter;
					mobsList->erase(iter); // ɾ���������ǰ�ƣ�����
				}
				return false;
			}
		}
	}
	return true;
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

Point Core::screenGridToRealGrid(Point screenGrid)
{
	Point result;
	result.col = screenGrid.col + windowStartPoint.col;
	result.row = screenGrid.row + windowStartPoint.row;
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
	arrowList = new QVector<Arrow*>;
	Point generatedScreenPosition;
	for (int i = 0; i < MOBS_NUMBER; i++)
	{
		generatedScreenPosition.row = (rand() % SCREEN_ROW)*SIZE;
		generatedScreenPosition.col = (rand() % SCREEN_COL)*SIZE;
		mobsList->append(new Pig(positionConvertor(generatedScreenPosition)));
	}
}

// ÿ�β��������mouseGridPoint
bool Core::isActionValid()
{
	mouseGridPoint = screenPositionToScreenGridPosition(mousePoint);
	// �ж��Ƿ�����Ч����
	bool isValid = false; 
	// ��Ҫ��ȡ��ҺͲ��������Ļ������
	updateScreenPosition(player);
	Point playerGridPoint = screenPositionToScreenGridPosition(player->positionRelativeToScreen);
	bool condition_1 = (player->attackRange >= abs(playerGridPoint.row - mouseGridPoint.row)) && (player->attackRange >= abs(playerGridPoint.col - mouseGridPoint.col));
	isValid = condition_1;
	if (!isValid)
	{
		qDebug() << "unvalid action, player grid position: row: " << playerGridPoint.row << " col: " << playerGridPoint.col;
		return false;
	}

	return true;
}

// ����ĳ�����Ƿ���Է���ĳһ������
bool Core::isCubeCanBeCreateOn(Point position)
{
	int cubeType = board[position.row][position.col];
	bool isAble = false;
	switch (player->currentArticleType)
	{
	case WOOD:
		if (cubeType == GLASS || cubeType == WATER)
		{
			isAble = true;
		}
		break;
	case GLASS:
		if (cubeType == EARTH || cubeType == WATER)
		{
			isAble = true;
		}
		break;
	case STONE:
		if (cubeType == GLASS || cubeType == WATER)
		{
			isAble = true;
		}
		break;
	case LEAF:
		if (cubeType == WOOD || cubeType == WATER)
		{
			isAble = true;
		}
		break;
	case EARTH:
		if (cubeType == BASE || cubeType == WATER)
		{
			isAble = true;
		}
		break;
	case WATER:
		if (cubeType == GLASS || cubeType == BASE)
		{
			isAble = true;
		}
		break;
	case BASE:

	default:
		isAble = false;
		break;
	}

	if (!isAble)
	{
		qDebug() << "you cannot put " << player->currentArticleType << " on point: row: " << position.row << " col: " << position.col;
		return isAble;
	}
	return isAble;
}

// ��ҽ�����ͨ�������������ھ򣩣�λ��ΪmousePoint
void Core::playerNormalAction()
{
	qDebug() << "Player normal operation, point: row: " << mouseGridPoint.row << " col: " << mouseGridPoint.col;

	if (!isActionValid())
	{
		return;
	}

	// �ж��Ƿ��ǹ���
	bool isAttack = false;
	QVector<Organism*>::iterator iter;
	for (iter=mobsList->begin();iter!=mobsList->end();iter++)
	{
		if (*iter == NULL) continue;
		updateScreenPosition(*iter);
		Point mobGridPosition = screenPositionToScreenGridPosition((*iter)->positionRelativeToScreen);
		if (mouseGridPoint == mobGridPosition)
		{
			(*iter)->beAttacked(player->finalAttackPower);
			if ((*iter)->isDead)
			{
				delete *iter;
				mobsList->erase(iter); // ɾ���������ǰ�ƣ�����
			}
			isAttack = true;
			return; // ���ҵ��������Ķ����˳�����
		}
	}

	if (!isAttack) // �û������˺Ϸ����ھ����
	{
		playerMining(mouseGridPoint);
	}
}

void Core::playerMining(Point miningPoint)
{
	Point realMiningPoint = screenGridToRealGrid(miningPoint);
	qDebug() << "player mine at row: " << miningPoint.row << " col: " << miningPoint.col;
	unsigned short* targetCube = &board[realMiningPoint.row][realMiningPoint.col];
	player->articleList[*targetCube] += 1; // ���������Ʒ������
	switch (*targetCube)
	{
	case LEAF:
		*targetCube = WOOD;
		break;
	case STONE:
		*targetCube = GLASS;
		break;
	case WOOD:
		*targetCube = GLASS;
		break;
	case GLASS:
		*targetCube = EARTH;
		break;
	case EARTH:
		*targetCube = BASE;
		break;
	default:
		break;
	}
}

// ���ղ����ĵ�λΪ��
void Core::playerCreateCube(Point createPoint)
{

	if (!isActionValid())
	{
		return;
	}

	if (!isCubeCanBeCreateOn(createPoint))
	{
		return;
	}

	int* cubeNumber = &(player->articleList[player->currentArticleType]);
	if ((*cubeNumber) <= 0)
	{
		qDebug() << "no enough such type material: " << player->currentArticleType;
		return;
	}
	else
	{
		(*cubeNumber)--;
	}
	board[createPoint.row][createPoint.col] = player->currentArticleType;
	qDebug()<<"player create cube: "<< player->currentArticleType << " on point: row: " << createPoint.row << " col: " << createPoint.col;
}

// �����������������ң���״̬��Ѫ�����Ƿ�������
void Core::adjustAllMobsStatus()
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
		moveAllMobs(); // ��ǰ��������
		adjustAllMobsStatus(); // Ŀǰ����
	}
	if (event->timerId() == arrowMoveTimer)
	{
		// TODO: ɾ�����õļ�ʸ
		// removeNotMovingArrow();  
		moveAllArrows();
	}

}
