#include "Bullet.h"


CBullet::CBullet(CMaps* pMaps)
{
	m_pMaps = pMaps;
}


CBullet::~CBullet()
{
}


//此函数： 损坏植物时，同步受损植物数据；
void CBullet::drawBullet(bool isShow)
{
	int posX;
	int posY;
	int color;
	for (unsigned int i = 0; i < m_vecBullet.size(); i++)
	{
		if (!m_vecBullet[i]->isValid) {
			continue;
		}
		posX = m_vecBullet[i]->posX;
		posY = m_vecBullet[i]->posY;
		color = m_vecBullet[i]->color;
		if (m_pMaps->m_nMap[posY][posX] == MAP_TREE) {
			//树林要隐藏子弹： 不显示也不擦除
		}
		else if (isShow) {
			if ((m_pMaps->m_nMap[posY][posX] == MAP_GRASS) || (m_pMaps->m_nMap[posY][posX] == MAP_ICE) || (m_pMaps->m_nMap[posY][posX] == MAP_RIVER)) {
				//显示子弹（经过后擦除子弹位置，植物被破坏， 但是要恢复被破坏的植物）
				m_pMaps->m_vecDamagedPlant.push_back({ posX, posY }); //同步地图
			}
			m_pMaps->printChar(posX, posY, "⊙", color);
		}
		else {
			if ((m_pMaps->m_nMap[posY][posX] == MAP_GRASS) || (m_pMaps->m_nMap[posY][posX] == MAP_ICE) || (m_pMaps->m_nMap[posY][posX] == MAP_RIVER)) {//擦除位置有植物
				m_pMaps->m_vecDamagedPlant.push_back({ posX, posY });  //同步地图
			}
			m_pMaps->printChar(posX, posY, "  ", color);
		}
	}
}

//此函数0. 处理静态地图物体
//      1. 处理  m_pMaps->m_pBulletMap
//      2. 检测m_pMaps->m_pTankMap 是否击中了坦克

void CBullet::removeInvalidBullet()
{
	int posX;
	int posY;

	for (unsigned int i = 0; i < m_vecBullet.size(); i++)
	{
		if (!m_vecBullet[i]->isValid) {
			continue;
		}
		posX = m_vecBullet[i]->posX;
		posY = m_vecBullet[i]->posY;

		//坦克地图检测
		if (m_pMaps->m_pTankMap[posY][posX] != NULL) {//击中了坦克
			BULLET* pBullet = m_vecBullet[i];
			TANK* pTank = m_pMaps->m_pTankMap[posY][posX];
			if (pBullet->isNPC != pTank->isNPC) { //敌人： 减血
				pTank->curHP -= pBullet->power;
			}
			//子弹失效
			m_vecBullet[i]->isValid = false;
			m_pMaps->m_pBulletMap[posY][posX] = NULL;
			continue;
		}// 开始静态地图检测
		else if (m_pMaps->m_nMap[posY][posX] == MAP_STONE) { //stone: 子弹失效
			m_vecBullet[i]->isValid = false;
			m_pMaps->m_pBulletMap[posY][posX] = NULL;
			continue;
		}
		else if (m_pMaps->m_nMap[posY][posX] == MAP_SPACE) { //空区域：不操作 
													//nothing
		}
		else if (m_pMaps->m_nMap[posY][posX] == MAP_BRICK) {//砖块： 都消失
			m_pMaps->m_nMap[posY][posX] = MAP_SPACE;
			m_pMaps->reDrawMapPoint(posY, posX);
			m_vecBullet[i]->isValid = false;
			m_pMaps->m_pBulletMap[posY][posX] = NULL;
			continue;
		}
		else if (m_pMaps->m_nMap[posY][posX] == MAP_TREE) {//树林
												  //nothing 
		}
		else if (m_pMaps->m_nMap[posY][posX] == MAP_ICE || m_pMaps->m_nMap[posY][posX] == MAP_GRASS || m_pMaps->m_nMap[posY][posX] == MAP_RIVER) { //草地和冰块
																														//nothing
		}
		else if (posX < 0 || posX >= MAPWIDTH || posY < 0 || posY >= MAPHEIGHT) {
			//never happen
			printf("子弹坐标异常：（%d, %d）\n", posX, posY);

		}
		else {
			printf(" 尚未定义的地图静态对象： removeInvalidBullet()\n");
			int map = m_pMaps->m_nMap[posY][posX];
			printf(" x = %d y = %d  %d\n", posX, posY, map);
		}
	}
	//crontab: 删除无效子弹
	for (vector<BULLET*>::iterator itr = m_vecBullet.begin(); itr != m_vecBullet.end(); ++itr) {
		if (!(*itr)->isValid) {
			m_vecBullet.erase(itr);
			break;
		}
	}

}

bool CBullet::runBullet()
{
	//擦除所有子弹
	drawBullet(false);
	for (unsigned int i = 0; i < m_vecBullet.size(); i++)
	{
		if (!m_vecBullet[i]->isValid) {
			continue;
		}
		//飞行速度设置
		clock_t start_time = m_vecBullet[i]->last_fly_time;
		clock_t end_time = clock();
		if (end_time - start_time < 50) {
			continue;
		}
		m_vecBullet[i]->last_fly_time = end_time;

		//子弹坐标
		int posX = m_vecBullet[i]->posX;
		int posY = m_vecBullet[i]->posY;
		//修改坐标
		switch (m_vecBullet[i]->dir) {
		case DIR_UP:
			posY--;
			break;
		case DIR_DOWN:
			posY++;
			break;
		case DIR_LEFT:
			posX--;
			break;
		case DIR_RIGHT:
			posX++;
			break;
		default:
			printf("错误的子弹方向属性：drawbullet\n");
			return false;
			break;
		}
		m_vecBullet[i]->posX = posX;
		m_vecBullet[i]->posY = posY;
	}
	//移除无效子弹
	removeInvalidBullet();
	//显示
	drawBullet(true);
	return true;
}



void CBullet::shootBullet(vector<TANK *>& m_vecTank, int nTankIndex)
{
	//射速设置
	clock_t start_time = m_vecTank[nTankIndex]->last_shoot_time;
	clock_t end_time = clock();
	if (m_vecTank[nTankIndex]->isNPC && end_time - start_time < 500) {
		return;
	}
	else if (!m_vecTank[nTankIndex]->isNPC && end_time - start_time < 50) {
		return;
	}
	m_vecTank[nTankIndex]->last_shoot_time = end_time;

	int posX = m_vecTank[nTankIndex]->posX;
	int posY = m_vecTank[nTankIndex]->posY;
	int dir = m_vecTank[nTankIndex]->dir;
	switch (dir) {
	case DIR_UP:
		posY -= 2;
		break;
	case DIR_DOWN:
		posY += 2;
		break;
	case DIR_LEFT:
		posX -= 2;
		break;
	case DIR_RIGHT:
		posX += 2;
		break;
	default:
		printf("错误的子弹初始方向：shootBullet\n");
		return;
		break;
	}

	BULLET* pBullet = (BULLET*)malloc(sizeof(BULLET));
	pBullet->posX = posX;
	pBullet->posY = posY;
	pBullet->dir = dir;
	pBullet->isNPC = m_vecTank[nTankIndex]->isNPC;
	pBullet->tankId = nTankIndex;
	pBullet->last_fly_time = 0;
	pBullet->power = 10;
	pBullet->color = m_vecTank[nTankIndex]->color;

	m_vecBullet.push_back(pBullet);
	//立即移除无效子弹
	removeInvalidBullet();
	//显示
	drawBullet(true);
}