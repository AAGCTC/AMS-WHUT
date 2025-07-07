//本文件负责实现“查询统计”的 业务逻辑层 的服务
#include"service.h"
#include<stdio.h>

//打印计费信息
void printRecord(BillNode* temp);

//查询统计服务接口
void Record_Serv_check() {
	printf("%s\n", "----------查询统计----------");
	int cmd = 0;
	do {
		Menu_PrintMenu_rec();
		cmd = Menu_GetCommend_rec();
		switch (cmd)
		{
		case 0:
			return;
		case 1:
			record_Card();
			break;
		case 2:
			record_Total();
			break;
		case 3:
			record_Total_thisMonth();
			break;
		default:
			printf("%s\n", "输入的菜单序号错误！");
			continue;
		}
	} while (cmd != 0);
}

//打印子菜单
void Menu_PrintMenu_rec() {
	printf("\n%s\n", "欢迎进入查询统计功能");
	printf("\n%s\n", "-----子菜单-----");
	printf("%s\n", "1.按卡号统计");
	printf("%s\n", "2.总营业额");
	printf("%s\n", "3.月营业额");
	printf("%s\n", "0.返回主菜单");
	printf("%s\n", "请选择菜单编号（0~3）：");
}

//读入用户输入的命令
int Menu_GetCommend_rec() {
	int cmd = 0;
	char string[256] = { 0 };
	scanf_s("%s", string, 256);
	if (strlen(string) > 1) return -1;
	else if ('0' <= string[0] && string[0] <= '3') {
		cmd = string[0] - '0';
		return cmd;
	}
	else return -1;
}

//由于inputNameKey必须接受一个函数指针，但此处不需要判断卡状态，故定义一个空函数
int cardStatus_rec(Card* theCard) {
	;
}

//按卡号查询消费记录
void record_Card() {

	Card* theCard = inputNameKey(cardStatus_rec);
	Bills All_rec = (BillNode*)malloc(sizeof(BillNode));
	if (All_rec == NULL) {
		printf("%s\n", "头结点添加失败");
		return;
	}
	All_rec->next = NULL;
	All_rec = readAllRecs_Card(All_rec,theCard);
	
	printf("%-18s%-10s\n", "卡号", "总消费金额");
	printf("%-18s%-10.3f\n", theCard->aName, theCard->fTotalUse);
	printf("%s\n", "---------消费记录--------");
	for (BillNode* cur = All_rec->next; cur != NULL; cur = cur->next) {
		printRecord(cur);
	}
}

//总营业额
void record_Total() {
	float sum = 0;
	for (Card* cur = AllCards->next; cur != NULL; cur = cur->next) {
		sum += cur->fTotalUse;
	}
	printf("总消费金额：%.3f\n", sum);
}

//月营业额
void record_Total_thisMonth() {
	float sum = 0;
	;
}

//打印计费信息
void printRecord(BillNode* temp)
{
	char time[2][32] = { 0 };
	printf("%-18s%-10s%-10s%-27s%-27s\n\n", "卡号", "消费金额", "消费状态", "上机时间", "下机时间");
	printf("%-18s%-10.3f", temp->data.aCardName, temp->data.fAmount);
	switch (temp->data.nStatus)
	{
	case 0:
		printf("%-10s", "未结算");
		break;
	case 1:
		printf("%-10s", "已结算");
		break;
	}
	printf("%-27s%-27s\n",timePrintToStr(time[0], &temp->data.tSignIn),
		timePrintToStr(time[1], &temp->data.tSettle));
	printf("%s\n", "----------分割线----------");
}