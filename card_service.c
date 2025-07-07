//本文件负责实现 “卡管理”、“费用管理” 的 业务逻辑层 的服务
#include"service.h"
#include"tool.h"
#include"model.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"billing_file.h"

//卡的默认有效期（三年是94608000秒）
#define CARD_VALID_TIME 94608000

//添加卡服务接口
void CardService_newCard() {
	Card *newCard = endPush(AllCards);
	if (newCard == NULL) {
		printf("%s\n", "电脑运行内存不足，添加卡失败！");
		return;
	}
	global.cardID++;
	global.cardAmount++;
	*newCard = StdCard;//用一个Card型常量对newCard进行初始化
	printf("%s\n", "----------添加卡----------");
	printf("%s\n", "正在办理一张新卡，请按提示输入信息：");
	char cName[128] = { 0 };//用于临时保存输入的卡号
	int flag = 0;
	while (flag == 0)
	{
		printf("%s\n", "请您为新卡指定一个卡号(长度1~18)：");
		scanf_s("%s", cName, 128);
		if (!isLenthLegal(cName, 18)) {
			printf("%s\n", "错误！卡号过长！");
			printf("%s", "(请从新输入)");
			continue;
		}
		//不能使用已被占用的卡号
		else if (findCard1(cName)!=NULL) {
			printf("%s\n", "不能使用已被占用的卡号");
			printf("%s", "(请从新输入)");
			continue;
		}
		else {
			flag = 1;
			cName[18] = 0;//在已经保证长度<=18情况下，截断后面的部分
			strcpy_s(newCard->aName, 18, cName);
		}
	}
	inputKeyOfCard(newCard->aPwd);
	float initMoney;
	while (1) {
		printf("%s\n", "请输入开卡金额(万元以内)(请不要输入数字以外的其他字符)：");
		scanf_s("%f", &initMoney);
		if (initMoney < 0) {
			printf("%s\n", "开卡金额不能为负数！");
			printf("%s", "(请从新输入)");
			continue;
		}
		else if (initMoney > 10000) {
			printf("%s\n", "开卡金额过大！");
			printf("%s", "(请从新输入)");
			continue;
		}
		else {
			newCard->fBalance = initMoney;
			break;
		}
	}
	newCard->tStart = time(NULL);
	newCard->tEnd = newCard->tStart + CARD_VALID_TIME;//默认三年后到期
	newCard->tLast = newCard->tStart;
	cardFile_writeInfo();
	cardFile_writeInfoBinary();
	printf("%s\n", "您新建的卡的信息如下：");
	printCardInfo(newCard);
	printf("\n%s\n", "办卡成功！添加卡服务结束！");
}
//查询卡服务接口
void CardService_checkCard() {
	printf("%s\n", "----------查询卡----------");
	char cName[24] = { 0 };
	inputNameOfCard(cName);
	Card* obj = AllCards;//从头节点开始遍历
	for (int count = 0;;count++) {
		obj = findCard2(obj ,cName);
		if (obj == NULL) {
			if(count==0) printf("%s\n", "没有该卡的信息！");
			break;
		}
		else {
			if(count == 0) printf("%s\n", "----以下是找到的卡的信息----");
			printCardInfo(obj);
		}
		if (obj->next != NULL) obj = obj->next;
		else break;
	}
	printf("\n%s\n", "-------输出完毕-------");
	return;
	/*
	需求分析：
	检查输入卡号、密码的长度等是否规范的功能需要频繁调用，应当封装成函数放入tool中；
	在查询卡中调用数据层函数拉取卡信息，未找到卡要报错；（应该不用了，有AllCards[]了）
	*/
}
//注销卡服务接口
void CardService_cancelCard() {
	printf("%s\n", "----------注销卡----------");
	Card* theCard = inputNameKey(cardStatus_cancel);//失效的卡不能使用但可以注销
	if (theCard == NULL) return;
	float temp_money = theCard->fBalance;
	theCard->nStatus = 2;
	theCard->fBalance = 0;//注销的同时退还卡内余额
	theCard->tLast = time(NULL);
	printf("\n%s\n", "注销成功！");
	printCancelInfo(theCard, temp_money);
	cardFile_writeInfo();//保存更改
	Money* p = (Money*)malloc(sizeof(Money));
	if (p == NULL) {
		printf("%s\n", "电脑运行内存不足，退费失败！");
		return;
	}
	strcpy_s(p->aCardName, 18, theCard->aName);
	p->fMoney = temp_money;
	p->nDel = 0;
	p->nStatus = 1;
	p->tTime = time(NULL);
	billFile_writeDraw(*p);
}



//----------添加卡附属函数----------


//----------查询卡附属函数----------
//打印卡信息
void printCardInfo(Card* card) {
	char time[3][32] = { 0 };
	//printf("%s\n", "----以下是这张卡的信息----");
	printf("%-18s%-10s%-10s%-10s%-10s%-10s%-10s\n\n", "卡号", "密码", "余额",
		"累计金额", "使用次数", "卡状态", "删除标志");
	printf("%-18s%-10s%-10.3f%-10.3f%-10d", card->aName, card->aPwd, card->fBalance,
		card->fTotalUse, card->nUseCount);
	switch (card->nStatus)
	{
	case 0:
		printf("%-10s", "未上机");
		break;
	case 1:
		printf("%-10s", "正在上机");
		break;
	case 2:
		printf("%-10s", "已注销");
		break;
	default:
		printf("%-10s", "失效");
		break;
	}
	switch (card->nDel)
	{
	case 0:
		printf("%-10s", "未删除");
		break;
	case 1:
		printf("%-10s", "已删除");
		break;
	}
	printf("\n\n%-27s%-27s%-27s\n\n", "开卡时间", "有效期至", "最后使用时间");
	printf("%-27s%-27s%-27s", timePrintToStr(time[0], &card->tStart), timePrintToStr(time[1], &card->tEnd),
		timePrintToStr(time[2], &card->tLast));

	printf("\n%s\n", "----------分割线----------");
}

//在AllCards中,按卡号精确查询
Card* findCard1(char* str2) {
	//先对参数判空
	if (str2 == NULL) {
		return NULL;
	}
	Card* cur = AllCards->next;
	for (; cur != NULL; cur = cur->next) {
		if (strcmp(cur->aName,str2)==0) {
			return cur;
		}
	}
	return NULL;//没找到
}

//在AllCards中,按卡号模糊查询,从head所指向的节点开始遍历
Card* findCard2(Card* head, char* str2) {
	//先对参数判空
	if (str2 == NULL) {
		return NULL;
	}
	Card* cur = head;
	int len2 = strlen(str2);
	for (; cur != NULL; cur = cur->next) {
		//检查str2是否为str1的子串,str2为查询卡模块输入的卡号子串
		int len1 = strlen(cur->aName);
		if (len1< len2) {
			continue;
		}
		// 使用strstr库函数简化子串查找
		if (strstr(cur->aName, str2) != NULL) {
			return cur;
		}
	}
	return NULL;//没找到
}


//----------注销卡附属函数----------
int cardStatus_cancel(Card* theCard) {
	switch (theCard->nStatus)
	{
	case 0:
	case 3:return 1;//失效的卡不能使用但可以注销和退费
	case 1:printf("%s\n", "注销失败，此卡正在上机！"); return 0;
	case 2:printf("%s\n", "注销失败，此卡已注销！"); return 0;
	default:
		return 0;
	}
}

void printCancelInfo(Card* theCard, float temp_money) {
	printf("\n注销时卡内余额还剩%.3f，已全部退还，现余额为：0.000\n", temp_money);
	printf("%s\n", "--------注销信息如下--------");
	printCardInfo(theCard);
}