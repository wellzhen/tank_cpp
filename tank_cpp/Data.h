#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include <vector>
using namespace std;


#define MAPWIDTH  60
#define MAPHEIGHT  40

//g_nMap�ڵľ�̬����
#define MAP_SPACE 0
#define MAP_STONE 2
#define MAP_BRICK 4
#define MAP_GRASS 8
#define MAP_ICE   16
#define MAP_TREE  32
#define MAP_RIVER 64
#define MAP_HEART  128

//����ṹ��
typedef struct _POS {
	int posX;
	int posY;
} POS;

//�ڵ�
typedef struct _BULLET {
	int posX;
	int posY;
	int dir;
	bool isNPC;
	int tankId;
	bool isValid;
	int   power; //������
	clock_t last_fly_time;
	int color; //color of bullet
} BULLET;

//̹�˽ṹ��
typedef struct _TANK {
	int posX;
	int posY;
	int initPosX;
	int initPosY;
	bool isNPC;
	int dir;
	int color;

	int maxHP;
	int curHP;

	int maxSpeed;
	int curSpeed;
	int oldSpeed;

	int nKill;
	int nDie;
	int nlife;

	int nlevel;
	int nShape;
	bool ready;

	clock_t last_move_time;
	clock_t last_shoot_time;

	bool isAlive;
} TANK;

//̹�˷��򣨶�Ӧ̹����״�����±꣬�����޸ģ�
#define DIR_UP 0
#define DIR_RIGHT 1
#define DIR_DOWN  2
#define DIR_LEFT 3

//����̨��ɫ
#define	COLOR_BLACK   0
#define	COLOR_PINK   13
#define	COLOR_RED   12 //�ʺ�
#define	COLOR_RED_LIGHT   4 //����
#define	COLOR_YELLOW   14
#define	COLOR_YELLOW_LIGHT   6
#define	COLOR_BLUE   9
#define	COLOR_BLUE_LIGHT   1
#define	COLOR_GREEN   10
#define	COLOR_GREEN_LIGHT   2
#define	COLOR_WHITE   15
#define	COLOR_GRAY   8  //��ɫ

// �� ����  ���
#define TANK_SHAPE1   1//"��"
#define TANK_SHAPE2   2 //"��"
#define TANK_SHAPE3   3 // "��"
#define TANK_SHAPE4    4 //"��"
#define TANK_SHAPE5    5  //"��"
//�ǽڵ�

typedef struct _STARNODE {
	int nG; //�ƶ����
	int nH; //��̾���
	int nF; // nG + nH
	int nPosX;
	int nPosY;
	int nFromX;
	int nFromY;
	void setH_F(_STARNODE* star) 
	{
		nH = abs(nPosX - star->nPosX) + abs(nPosY - star->nPosY);
		nF = nH + nG;
	}
}STARNODE;
