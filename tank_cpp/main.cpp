#include "Data.h"
#include "CTrl.h"
#include "Maps.h"
#include "Tank.h"
#include "Bullet.h"
#include <mmsyscom.h>
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "WINMM.LIB")

int g_TankShape[4][3][3] = {
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
int main()
{
	CCtrl ctrl;
	CMaps map;
	CTank tank;
	CBullet bullet;

	//ctrl.initConsoleWindow();
	//ctrl.initInputShift();
	/************  ��ʾ��ӭ����***************/
	//ctrl.showWelcomeWall();
	//ctrl.showWelcomeWords();
	Sleep(1000);

	while (1) {
		/*�˵� ѡ��*/
		system("cls");
		//ctrl.showWelcomeWall();
		//ctrl.showMenu();
		//int optionNum = ctrl.getMenuChoice();
		int optionNum = 1;

		switch (optionNum) {
		case  1:
			//ctrl.playTank(1);//��ʼ��Ϸ
			break;
		case 2:
			//ctrl.playTank(2); //�Զ�����Ϸ
			break;
		case 3:
			//ctrl.playTank(3); //������Ϸ
			break;
		case 4:
			return 0; //�˳�
		}
	}


	return 0;
}
