#pragma once
class CCtrl
{
public:
	CCtrl();
	~CCtrl();
public:
	//开始玩坦克游戏
	void playTank(int gameType);

	//方向键转方向宏
	int dirKey2DirNum(char keyWord);

	//设置控制台大小
	void initConsoleWindow();

	void shootBullet(int nTankIndex);

	//初始化输入法
	void initInputShift();

	//显示欢迎墙
	void  showWelcomeWall();
	//欢迎语
	void  showWelcomeWords();
	//显示主页菜单
	void showMenu();
	//获取用户的菜单选项
	int getMenuChoice();
	//存档
	void saveArchive();
	//读档
	void readArchive();
	//清空全局变量
	void clearGlobalData();
};

