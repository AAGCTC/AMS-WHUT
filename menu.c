//本文件负责实现 “主菜单” 的功能，属于表示层
#include"menu.h"
#include"stdio.h"
#include<stdlib.h>
#include<string.h>
#include"service.h"

//表示层主循环，用户在此循环中完成多次交互
void Menu_Main() {
	int cmd = 0;
	do {
		Menu_PrintMenu();
		//保存数据
		DataSave();
		cmd = Menu_GetCommend();
		switch (cmd)
		{
		case 0:
			printf("%s\n", "-------退出中......");
			DataSave();
			printf("%s\n", "计费管理系统已正常退出。");
			//linklistFree();
			exit(0);
			break;
		case 1:
			CardService_newCard();
			break;
		case 2:
			CardService_checkCard();
			break;
		case 3:
			Bill_Serv_logIn();
			break;
		case 4:
			Bill_Serv_logOut();
			break;
		case 5:
			Card_Serv_rechargeCard();
			break;
		case 6:
			Card_Serv_drawMoneyFromCard();
			break;
		case 7:
			Record_Serv_check();
			break;
		case 8:
			CardService_cancelCard();
			break;
		default:
			printf("%s\n", "输入的菜单序号错误！");
			continue;
		}
	} while (cmd != 0);
}

//打印菜单
void Menu_PrintMenu() {
	printf("\n%s\n", "欢迎进入计费管理系统");
	printf("\n%s\n", "-----------菜单----------");
	printf("%s\n", "1.添加卡");
	printf("%s\n", "2.查询卡");
	printf("%s\n", "3.上机");
	printf("%s\n", "4.下机");
	printf("%s\n", "5.充值");
	printf("%s\n", "6.退费");
	printf("%s\n", "7.查询统计");
	printf("%s\n", "8.注销卡");
	printf("%s\n", "0.退出");
	printf("%s\n", "请选择菜单编号（0~8）：");
}

//读入用户输入的命令，接受非法命令后提示用户重新输入
int Menu_GetCommend() {
	int cmd = 0;
	char string[256] = { 0 };
	scanf_s("%s", string,256);
	if (strlen(string) > 1) return -1;
	else if ('0' <= string[0] && string[0] <= '9') {
		cmd = string[0] - '0';
		return cmd;
	}
	else return -1;
}
