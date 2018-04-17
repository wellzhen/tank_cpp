#include "Tank.h"


CTank::CTank(CMaps* pMaps)
{
	__initTankShapeModel();
	m_pMaps = pMaps;
}

CTank::~CTank()
{
}

void CTank::__initTankShapeModel()
{
	int tankShape[4][3][3] = {
		{//up = 0
			{ 0, 1, 0 },
			{ 1, 1, 1 },
			{ 1, 0, 1 }
		},
		{//right = 1
			{ 1, 1, 0 },
			{ 0, 1, 1 },
			{ 1, 1, 0 }
		},
		{ //down = 2
			{ 1, 0, 1 },
			{ 1, 1, 1 },
			{ 0, 1, 0 }
		},
		{ //left
			{ 0, 1, 1 },
			{ 1, 1, 0 },
			{ 0, 1, 1 }
		}
	};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				m_tankShape[i][j][k] = tankShape[i][j][k];
			}
		}
	}
}


void CTank::initNpcTank(int Count)
{
	Count = Count > 18 ? 18 : Count;
	for (int nNum = 0; nNum < Count; nNum++) {
		TANK*  pTank = (TANK*)malloc(sizeof(TANK));
		pTank->isNPC = true;
		pTank->color = COLOR_BLUE;
		pTank->dir = DIR_DOWN;
		pTank->maxHP = 100;
		pTank->curHP = 100;
		pTank->maxSpeed = 60;
		pTank->curSpeed = 50;
		pTank->oldSpeed = 0;
		pTank->nKill = 0;
		pTank->nDie = 0;
		pTank->nlife = 0;
		pTank->nlevel = 1;
		pTank->last_move_time = 0;
		pTank->last_shoot_time = 0;
		pTank->isAlive = true;
		//坐标
		int X;
		int Y = 2;
		if (nNum % 2 == 0) { //布置在左边
			X = 2 + (nNum / 2) * 3;
		}
		else { //布置在右边
			X = MAPWIDTH - 3 - ((nNum-1)/2)*3;
		}
		pTank->posX = X;
		pTank->posY = Y;
		m_vecTank.push_back(pTank);
		drawTank(m_vecTank.size() - 1, true);
	}
}

void CTank::autoRunNpcTank(CBullet& bullets)
{
	srand((unsigned int)time(NULL));
	for (unsigned int index = 0; index < m_vecTank.size(); index++) {
		if (m_vecTank[index]->isNPC && m_vecTank[index]->isAlive) {
			int nDir = rand() % 4;
			//moveTank(nDir, index); //移动
			bullets.shootBullet(m_vecTank, index); //射击
		}
	}
}

void CTank::initPlayerTank(int Count)
{
	for (int i = 0; i < Count; i++) {
		TANK*  pTank = (TANK*)malloc(sizeof(TANK));
		pTank->isNPC = false;
		pTank->posX = MAPWIDTH / 2 - 4 + 8* i;
		pTank->posY = MAPHEIGHT - 3;
		pTank->color = COLOR_RED;
		pTank->dir = DIR_UP;
		pTank->maxHP = 100;
		pTank->curHP = 100;
		pTank->maxSpeed = 60;
		pTank->curSpeed = 50;
		pTank->oldSpeed = 0;
		pTank->nKill = 0;
		pTank->nDie = 0;
		pTank->nlife = 30;
		pTank->nlevel = 1;
		pTank->last_move_time = 0;
		pTank->last_shoot_time = 0;
		pTank->isAlive = true;

		m_vecTank.push_back(pTank);
		drawTank(m_vecTank.size() - 1, true);
	}
	

	
}

//此函数同步m_pMaps->m_pTankMap数据
void CTank::drawTank( int index, bool isShow)
{
	int posX = m_vecTank[index]->posX;
	int posY = m_vecTank[index]->posY;
	int dirNum = m_vecTank[index]->dir;
	for (int row = posY - 1; row <= posY + 1; row++) {
		for (int col = posX - 1; col <= posX + 1; col++) {
			if (m_tankShape[dirNum][row - posY + 1][col - posX + 1] == 1) {
				if (m_pMaps->m_nMap[row][col] == MAP_TREE) {
					//树林要隐藏坦克： 不显示也不擦除
					int test = 1;
				}
				else if (isShow) {
					m_pMaps->printChar(col, row, "■", m_vecTank[index]->color);
					m_pMaps->m_pTankMap[row][col] = m_vecTank[index]; // 同步坦克地图
															 //如果显示位置有植物
					if (m_pMaps->m_nMap[row][col] == MAP_GRASS || m_pMaps->m_nMap[row][col] == MAP_ICE) {//植物被破坏
						m_pMaps->m_vecDamagedPlant.push_back({ col, row });   //同步受损植物地图
					}
					else if (m_pMaps->m_nMap[row][col] == MAP_HEART) {//检测心脏是否被碾压
						m_pMaps->m_isHeartBroken = true;
					}
					
					
				}
				else {
					m_pMaps->printChar(col, row, "  ", COLOR_BLACK);
					m_pMaps->m_pTankMap[row][col] = NULL;
					if (m_pMaps->m_nMap[row][col] == MAP_GRASS || m_pMaps->m_nMap[row][col] == MAP_ICE) {//植物被破坏
						m_pMaps->m_vecDamagedPlant.push_back({ col, row });   //同步受损植物地图
					}
				}
			}
		}
	}

}

void CTank::initDrawAllTank(bool isShow)
{
	for (int i = 0; i < m_vecTank.size(); i++) {
		drawTank(i, true);
	}
}

bool CTank::moveTank(int nDirNum, int index = 0)
{
	if (m_vecTank[index]->isAlive == false) {
		return false;
	}
	//速度设置
	clock_t start_time = m_vecTank[index]->last_move_time;
	clock_t end_time = clock();
	if (end_time - start_time < 150) {
		return false;
	}
	m_vecTank[index]->last_move_time = end_time;

	//参数；
	int nDestPosX = m_vecTank[index]->posX;
	int nDestPosY = m_vecTank[index]->posY;
	int nDestDir = m_vecTank[index]->dir;
	//判断移动操作，否则坐标不变
	int  blockNumSum = MAP_STONE + MAP_BRICK + MAP_RIVER;
	int changeValue = m_vecTank[index]->dir - nDirNum;
	if (changeValue == 0) { //前进，修改坐标,方向不变
		switch (m_vecTank[index]->dir) {
		case DIR_UP:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY - 2][nDestPosX] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY - 1][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX - 1] != NULL || m_pMaps->m_pTankMap[nDestPosY - 2][nDestPosX] != NULL || m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX + 1] != NULL) {
				return false;//左中右--目的位置有墙
			}
			nDestPosY--;
			break;
		case DIR_RIGHT:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX + 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY][nDestPosX + 2] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX + 1] != NULL || m_pMaps->m_pTankMap[nDestPosY][nDestPosX + 2] != NULL || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX + 1] != NULL) {
				return false;//上中下--目的位置有墙
			}
			nDestPosX++;
			break;
		case DIR_DOWN:
			if ((m_pMaps->m_nMap[nDestPosY + 1][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 2][nDestPosX] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY + 2][nDestPosX] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX + 1] != 0) {
				return false;//左中右--目的位置有墙
			}
			nDestPosY++;
			break;
		case DIR_LEFT:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY][nDestPosX - 2] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX - 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY][nDestPosX - 2] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX - 1] != 0) {
				return false;//上中下-目的位置有墙
			}
			nDestPosX--;
			break;
		default:
			printf("坦克前进方向参数dirNum错误（0123）： moveTank\n");
			return false;
		}
	}
	else if (changeValue == -1 || changeValue == -3 || changeValue == 1 || changeValue == 3) { //左右移动: 只改变方向（左右肩和尾部不能有物体）
		switch (m_vecTank[index]->dir) {
		case DIR_UP:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY - 1][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX] != 0 || m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX + 1] != 0) {
				return false;//左下右--目的位置有墙
			}
			break;
		case DIR_RIGHT:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX + 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX + 1] != 0 || m_pMaps->m_pTankMap[nDestPosY][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX + 1] != 0) {
				return false;//上左下--目的位置有墙
			}
			break;
		case DIR_DOWN:
			if ((m_pMaps->m_nMap[nDestPosY + 1][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY - 1][nDestPosX] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX + 1] != 0) {
				return false;//左上右--目的位置有墙
			}
			break;
		case DIR_LEFT:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY][nDestPosX + 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX - 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY][nDestPosX + 1] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX - 1] != 0) {
				return false;//上右下-目的位置有墙
			}
			break;
		default:
			printf("坦克向左右方向移动参数dirNum错误（0123）： moveTank\n");
			return false;
		}
		nDestDir = nDirNum;
	}
	else if (changeValue == 2 || changeValue == -2) { //反向移动: 只改变坐标，方向不变（尾部三个方向不能有物体）
		switch (m_vecTank[index]->dir) {
		case DIR_UP:
			if ((m_pMaps->m_nMap[nDestPosY + 2][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 2][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY + 2][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX] != 0 || m_pMaps->m_pTankMap[nDestPosY + 2][nDestPosX + 1] != 0) {
				return false;//左中右--目的位置有墙
			}
			nDestPosY++;
			break;
		case DIR_RIGHT:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX - 2] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX - 2] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX - 2] != 0 || m_pMaps->m_pTankMap[nDestPosY][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX - 2] != 0) {
				return false;//上中下--目的位置有墙
			}
			nDestPosX--;
			break;
		case DIR_DOWN:
			if ((m_pMaps->m_nMap[nDestPosY - 2][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY - 1][nDestPosX] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY - 2][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 2][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX] != 0 || m_pMaps->m_pTankMap[nDestPosY - 2][nDestPosX + 1] != 0) {
				return false;//左中右--目的位置有墙
			}
			nDestPosY--;
			break;
		case DIR_LEFT:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX + 2] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY][nDestPosX + 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX + 2] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX + 2] != 0 || m_pMaps->m_pTankMap[nDestPosY][nDestPosX + 1] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX + 2] != 0) {
				return false;//上中下-目的位置有墙
			}
			nDestPosX++;
			break;
		default:
			printf("坦克向右方向参数dirNum错误（0123）： moveTank\n");
			return false;
		}
	}
	//允许移动
	drawTank(index, false); //擦除
							//修改属性
	m_vecTank[index]->posX = nDestPosX;
	m_vecTank[index]->posY = nDestPosY;
	m_vecTank[index]->dir = nDestDir;
	//绘制
	drawTank(index, true);
	//减速?
	return true;
}

int  CTank::judgeAlive()
{
	int playerNum = 0;
	int NpcNum = 0;
	for (unsigned int i = 0; i < m_vecTank.size(); i++) {
		if (m_vecTank[i]->curHP > 0 || m_vecTank[i]->isAlive == false) { //不用判断:有血或者已死
			if (m_vecTank[i]->isNPC && m_vecTank[i]->curHP > 0) {
				NpcNum++;
			}
			else if(m_vecTank[i]->curHP > 0){
				playerNum++;
			}
			continue;
		}
		if (m_vecTank[i]->nlife >= 1) { //用life换血
			m_vecTank[i]->curHP = m_vecTank[i]->maxHP;
			m_vecTank[i]->nlife = m_vecTank[i]->nlife - 1;
			m_vecTank[i]->nDie = m_vecTank[i]->nDie + 1;
			if (m_vecTank[i]->isNPC) {
				NpcNum++;
			}
			else {
				playerNum++;
			}
			continue;
		}
		//凉了
		m_vecTank[i]->isAlive = false;
		drawTank(i, false);
	}

	//判断双方存活的坦克数量
	if (playerNum == 0) { //失败
		return -1;  
	}
	else if (NpcNum == 0) { //胜利
		return 1;
	}
	else {		// 继续
		return 0;  
	}
}
