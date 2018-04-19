#pragma once
#include "Data.h"
#include "Maps.h"
class CAstar
{
public:
	CAstar();
	~CAstar();
public:
	bool searchPath(CMaps& maps, POS startPoint, POS endPoint);
	int  getMoveDir(POS startPos);
private:
	void __initData(CMaps& maps, POS startPoint);
	void __addToOpenTable(STARNODE * pNode);
	void __updateInvalidPoint(CMaps& maps, POS startPoint);

	void Clear();
private:
	vector<STARNODE*> m_vecOpen;
	vector<STARNODE*> m_vecClose;
	bool m_bOpenMap[MAPHEIGHT][MAPWIDTH];
	STARNODE*  m_pCloseMap[MAPHEIGHT][MAPWIDTH];;
	bool m_bValidMap[MAPHEIGHT][MAPWIDTH];
};

