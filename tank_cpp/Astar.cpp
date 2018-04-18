#include "Astar.h"



CAstar::CAstar()
{
	m_bOpenMap[MAPHEIGHT][MAPWIDTH] = { false };
	m_pCloseMap[MAPHEIGHT][MAPWIDTH] = { NULL };
	m_bValidMap[MAPHEIGHT][MAPWIDTH] = { true };
}


CAstar::~CAstar()
{
	for (unsigned int i = 0; i < m_vecClose.size(); i++) {
		if (m_vecClose[i]) {
			delete[] m_vecClose[i];
			m_vecClose[i] = NULL;
		}
	}
	m_vecClose.clear();
	m_vecClose.swap(vector<STARNODE*>());
	
	for (unsigned int i = 0; i < m_vecOpen.size(); i++) {
		if (m_vecOpen[i]) {
			delete[] m_vecOpen[i];
			m_vecOpen[i] = NULL;
		}
	}
	m_vecClose.clear();
	m_vecClose.swap(vector<STARNODE*>());
}

int  CAstar::getMoveDir(POS startPos)
{
	//��ȡ��һ���ƶ��ĵ�
	int nToX = 0; 
	int nToY = 0;
	int nFromX;
	int nFromY;
	bool hasFind = false;
	STARNODE* pNode = m_vecClose[m_vecClose.size() - 1];
	while (!hasFind) {
		nToX = pNode->nPosX;
		nToY = pNode->nPosY;
		nFromX = pNode->nFromX;
		nFromY = pNode->nFromY;
		//delete pNode;
		//��һ����
		pNode = m_pCloseMap[nFromY][nFromX];

		if (pNode->nFromX == 0 && pNode->nFromY == 0 &&pNode->nG == 0) {//�ҵ��˿�ʼ��
			hasFind = true;
		}
	}
	//�ҵ��˵�һ��Ҫ�ƶ���λ�ã�nToX, nToY��
	if (startPos.posX == nToX) { //�����ƶ�
		if (nToY - startPos.posY == -1) { //��
			return 0;
		}
		else {
			return 2;
		}
	}
	else if(startPos.posY == nToY){
		if (nToX - startPos.posX == -1) {//��
			return 3;
		}
		else {
			return 1;
		}
	}
	else {
		printf("debug: �����Astart����\n");
		return -1;
	}

}

bool  CAstar::searchPath(CMaps& maps, POS startPoint, POS endPoint)
{
	//��ʼ����Ϣ
	__initData(maps, startPoint);
	/*******��ʼ����㡢�յ�2���ڵ�********/
	//pEndNode
	STARNODE*   pEndNode = new STARNODE;
	pEndNode->nPosX = endPoint.posX;
	pEndNode->nPosY = endPoint.posY;

	//pStartNode
	STARNODE*   pStartNode = new STARNODE;
	pStartNode->nG = 0;
	pStartNode->nPosX = startPoint.posX;
	pStartNode->nPosY = startPoint.posY;
	pStartNode->nFromX = 0;
	pStartNode->nFromY = 0;
	pStartNode->setH_F(pEndNode);

	//����open��
	__addToOpenTable(pStartNode);
	//ѭ����ɢ
	while (1) {
		//�ж�open���Ƿ�Ϊ��
		if (m_vecOpen.empty()) {
			return false;
		}
		//open����ȡ��һ����СF�ĵ㣨�����ĵ�һ���㣩
		STARNODE* pCenterNode = m_vecOpen[0];
		//��ɢȡ���ĵ�
		STARNODE* pNearNode[4];//: ��������
		for (int i = 0; i < 4; i++) {
			pNearNode[i] = new STARNODE{};
		}
		int nCenterPosX = pCenterNode->nPosX;
		int nCenterPosY = pCenterNode->nPosY;
		//�����ĸ�����Ϣ
		pNearNode[0]->nPosX = nCenterPosX;
		pNearNode[0]->nPosY = nCenterPosY - 1;
		pNearNode[1]->nPosX = nCenterPosX + 1;
		pNearNode[1]->nPosY = nCenterPosY;
		pNearNode[2]->nPosX = nCenterPosX;
		pNearNode[2]->nPosY = nCenterPosY + 1;
		pNearNode[3]->nPosX = nCenterPosX - 1;
		pNearNode[3]->nPosY = nCenterPosY;

		for (int i = 0; i < 4; i++) {
			pNearNode[i]->nFromX = nCenterPosX;
			pNearNode[i]->nFromY = nCenterPosY;
			pNearNode[i]->nG = pCenterNode->nG + 1;
			pNearNode[i]->setH_F(pEndNode);
		}
		//��ɢ��ɣ� ɾ��open�����ɢ�㣬����close��
		m_vecClose.push_back(m_vecOpen[0]);
		m_pCloseMap[nCenterPosY][nCenterPosX] = m_vecOpen[0];
		m_vecOpen.erase(m_vecOpen.begin());
		//�ж��ĸ����Ƿ���Ч
		for (int i = 0; i < 4; i++) {
			int x = pNearNode[i]->nPosX;
			int y = pNearNode[i]->nPosY;
			//�ж��ǲ����յ�
			if (x == pEndNode->nPosX && y == pEndNode->nPosY) {
				m_vecClose.push_back(pNearNode[i]);
				m_pCloseMap[x][y] = pNearNode[i];
				//�ͷ�
				for (int j = i+1; j < 4; j++) {
					delete[]  pNearNode[j];
				}
				return true;
			}
			//�Ƿ��ǵ�ͼ�ϵ���Ч��
			if (!m_bValidMap[y][x]) {
				delete pNearNode[i];
				continue;
			}
			//�Ƿ���Open�� ��close����
			if (m_bOpenMap[y][x] || m_pCloseMap[y][x] != NULL) {
				continue;
			}
			//�ϸ��ˣ���ӵ�open����
			__addToOpenTable(pNearNode[i]);
		}

	}
}




void CAstar::__addToOpenTable(STARNODE * pNewNode)
{
	int x = pNewNode->nPosX;
	int y = pNewNode->nPosY;
	m_bOpenMap[y][x] = true;

	if (m_vecOpen.size() == 0) {
		m_vecOpen.push_back(pNewNode);
		return;
	}
	for (unsigned int i = 0; i < m_vecOpen.size(); i++) {
		if (pNewNode->nF < m_vecOpen[i]->nF) {
			m_vecOpen.insert(m_vecOpen.begin() + i, pNewNode);
			return;
		}
		else if (i == m_vecOpen.size() - 1) {
			m_vecOpen.push_back(pNewNode);
			return;
		}
	}
}


void CAstar::__initData(CMaps& maps, POS startPoint)
{
	m_vecClose.clear();
	m_vecOpen.clear();
	//��ʼ��
	for (int row = 0; row < MAPHEIGHT; row++) {
		for (int col = 0; col < MAPWIDTH; col++) {
			m_bOpenMap[row][col] =  false ;
			m_pCloseMap[row][col] =  NULL;
		}
	}
	__updateInvalidPoint(maps, startPoint);
}

void CAstar::__updateInvalidPoint(CMaps& maps, POS startPoint)
{
	//���ø���ʱ��
	static clock_t last_time = 0;
	clock_t now_time = clock();
	if (now_time - last_time < 5000) {
		//return;
	}
	last_time = now_time;
	//init
	for (int row = 0; row < MAPHEIGHT; row++) {
		for (int col = 0; col < MAPWIDTH; col++) {
			m_bValidMap[row][col] = true;
		}
	}
	//update
	for (int row = 0; row < MAPHEIGHT; row++) {
		for (int col = 0; col < MAPWIDTH; col++) {
			if (col == 0 || col == MAPWIDTH - 1 || row == 0 || row == MAPHEIGHT - 1) {
				m_bValidMap[row][col] = false; //Ϊ������ж����̵棬��ֹrow-1Խ��
			}
			else if (maps.m_nMap[row][col] == MAP_STONE || maps.m_nMap[row][col] == MAP_BRICK|| maps.m_nMap[row][col] == MAP_RIVER ||
				
						(
							maps.m_pTankMap[row][col] != NULL &&  maps.m_pTankMap[row][col] ->isNPC &&
							(
								(abs(maps.m_pTankMap[row][col]->posX - startPoint.posX) > 1 && abs(maps.m_pTankMap[row][col]->posX - startPoint.posX) < 5)
								//||
								//(abs(maps.m_pTankMap[row][col]->posY - startPoint.posY) > 1 && abs(maps.m_pTankMap[row][col]->posY - startPoint.posY) < 5)
							)
						)
				
				) {
				//���ܵĵ㶼����Ч��
				m_bValidMap[row - 1][col - 1] = false;
				m_bValidMap[row - 1][col] = false;
				m_bValidMap[row - 1][col + 1] = false;
				m_bValidMap[row][col - 1] = false;
				m_bValidMap[row][col] = false;
				m_bValidMap[row][col + 1] = false;
				m_bValidMap[row + 1][col - 1] = false;
				m_bValidMap[row + 1][col] = false;
				m_bValidMap[row + 1][col + 1] = false;
			}
			else if (row == MAPHEIGHT - 3 || row == MAPHEIGHT - 4) { //����λ��
				m_bValidMap[row][col] = true;
			}
			else if (col == 0 || col == 1 || col == MAPWIDTH - 1 || col == MAPWIDTH - 2 ||
				row == 0 || row == 1 || row == MAPHEIGHT - 1 || row == MAPHEIGHT - 2) {
				m_bValidMap[row][col] = false;
			}
		}
	}

}


