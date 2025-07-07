#include"model.h"
//一个用于初始化的Card变量
Card StdCard = {
	{0},
	{0},
	0,0,0,0,0,
	0,0,0,
	NULL,NULL
};

//Card结构体数组，使用指针数组+动态分配结构体变量来减少空间浪费
//Card* AllCards[1024];


/*链表AllCards保存保存所有的卡*/
LinkList AllCards = NULL;
//集中管理所有的全局变量
struct Global global = { 0,0,0 };

//计费信息链表
Bills* All_Bills = NULL;