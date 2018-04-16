#include "Bullet.h"


CBullet::CBullet(CMaps* pMaps)
{
	m_pMaps = pMaps;
}


CBullet::~CBullet()
{
}


//�˺����� ��ֲ��ʱ��ͬ������ֲ�����ݣ�
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
			//����Ҫ�����ӵ��� ����ʾҲ������
		}
		else if (isShow) {
			if ((m_pMaps->m_nMap[posY][posX] == MAP_GRASS) || (m_pMaps->m_nMap[posY][posX] == MAP_ICE) || (m_pMaps->m_nMap[posY][posX] == MAP_RIVER)) {
				//��ʾ�ӵ�������������ӵ�λ�ã�ֲ�ﱻ�ƻ��� ����Ҫ�ָ����ƻ���ֲ�
				m_pMaps->m_vecDamagedPlant.push_back({ posX, posY }); //ͬ����ͼ
			}
			m_pMaps->printChar(posX, posY, "��", color);
		}
		else {
			if ((m_pMaps->m_nMap[posY][posX] == MAP_GRASS) || (m_pMaps->m_nMap[posY][posX] == MAP_ICE) || (m_pMaps->m_nMap[posY][posX] == MAP_RIVER)) {//����λ����ֲ��
				m_pMaps->m_vecDamagedPlant.push_back({ posX, posY });  //ͬ����ͼ
			}
			m_pMaps->printChar(posX, posY, "  ", color);
		}
	}
}

//�˺���0. ����̬��ͼ����
//      1. ����  m_pMaps->m_pBulletMap
//      2. ���m_pMaps->m_pTankMap �Ƿ������̹��

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

		//̹�˵�ͼ���
		if (m_pMaps->m_pTankMap[posY][posX] != NULL) {//������̹��
			BULLET* pBullet = m_vecBullet[i];
			TANK* pTank = m_pMaps->m_pTankMap[posY][posX];
			if (pBullet->isNPC != pTank->isNPC) { //���ˣ� ��Ѫ
				pTank->curHP -= pBullet->power;
			}
			//�ӵ�ʧЧ
			m_vecBullet[i]->isValid = false;
			m_pMaps->m_pBulletMap[posY][posX] = NULL;
			continue;
		}// ��ʼ��̬��ͼ���
		else if (m_pMaps->m_nMap[posY][posX] == MAP_STONE) { //stone: �ӵ�ʧЧ
			m_vecBullet[i]->isValid = false;
			m_pMaps->m_pBulletMap[posY][posX] = NULL;
			continue;
		}
		else if (m_pMaps->m_nMap[posY][posX] == MAP_SPACE) { //�����򣺲����� 
													//nothing
		}
		else if (m_pMaps->m_nMap[posY][posX] == MAP_BRICK) {//ש�飺 ����ʧ
			m_pMaps->m_nMap[posY][posX] = MAP_SPACE;
			m_pMaps->reDrawMapPoint(posY, posX);
			m_vecBullet[i]->isValid = false;
			m_pMaps->m_pBulletMap[posY][posX] = NULL;
			continue;
		}
		else if (m_pMaps->m_nMap[posY][posX] == MAP_TREE) {//����
												  //nothing 
		}
		else if (m_pMaps->m_nMap[posY][posX] == MAP_ICE || m_pMaps->m_nMap[posY][posX] == MAP_GRASS || m_pMaps->m_nMap[posY][posX] == MAP_RIVER) { //�ݵغͱ���
																														//nothing
		}
		else if (posX < 0 || posX >= MAPWIDTH || posY < 0 || posY >= MAPHEIGHT) {
			//never happen
			printf("�ӵ������쳣����%d, %d��\n", posX, posY);

		}
		else {
			printf(" ��δ����ĵ�ͼ��̬���� removeInvalidBullet()\n");
			int map = m_pMaps->m_nMap[posY][posX];
			printf(" x = %d y = %d  %d\n", posX, posY, map);
		}
	}
	//crontab: ɾ����Ч�ӵ�
	for (vector<BULLET*>::iterator itr = m_vecBullet.begin(); itr != m_vecBullet.end(); ++itr) {
		if (!(*itr)->isValid) {
			m_vecBullet.erase(itr);
			break;
		}
	}

}

bool CBullet::runBullet()
{
	//���������ӵ�
	drawBullet(false);
	for (unsigned int i = 0; i < m_vecBullet.size(); i++)
	{
		if (!m_vecBullet[i]->isValid) {
			continue;
		}
		//�����ٶ�����
		clock_t start_time = m_vecBullet[i]->last_fly_time;
		clock_t end_time = clock();
		if (end_time - start_time < 50) {
			continue;
		}
		m_vecBullet[i]->last_fly_time = end_time;

		//�ӵ�����
		int posX = m_vecBullet[i]->posX;
		int posY = m_vecBullet[i]->posY;
		//�޸�����
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
			printf("������ӵ��������ԣ�drawbullet\n");
			return false;
			break;
		}
		m_vecBullet[i]->posX = posX;
		m_vecBullet[i]->posY = posY;
	}
	//�Ƴ���Ч�ӵ�
	removeInvalidBullet();
	//��ʾ
	drawBullet(true);
	return true;
}



void CBullet::shootBullet(vector<TANK *>& m_vecTank, int nTankIndex)
{
	//��������
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
		printf("������ӵ���ʼ����shootBullet\n");
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
	//�����Ƴ���Ч�ӵ�
	removeInvalidBullet();
	//��ʾ
	drawBullet(true);
}