//本文件负责实现 “计费管理” 的 业务逻辑层 的服务
#include"billing_service.h"
#include"billing_file.h"
#include"rate_service.h"
#include"card_file.h"
#include<stdio.h>
#include"model.h"
#include"tool.h"
#include<string.h>

//调用的其他源文件的函数:---------------------------
//在AllCards中,按卡号精确查询
extern Card* findCard1(char* str2);
//-----------------------------------------------


//上机服务接口
void Bill_Serv_logIn() {
	printf("%s\n", "----------上机----------");
	printf("%s\n", "请输入要上机的卡的卡号：");
	char name[32] = { 0 };
	Card* theCard;
	do
	{
		inputNameOfCard(name);
		theCard = findCard1(name);
		if (theCard == NULL) {
			printf("%s\n", "未找到对应卡！请再次输入！");
		}
		else break;
	} while (1);
	printf("%s\n", "请输入密码：");
	char keyofCard[128];
	int flag = 0;
	do {
		while (flag < 1)
		{
			scanf_s("%s", keyofCard, 128);
			if (!isLenthLegal(keyofCard, 8)) {
				printf("%s\n", "错误！密码过长！");
				printf("%s", "(请重新输入)");
				continue;
			}
			else {
				flag++;
			}
		}
		if (strcmp(theCard->aPwd, keyofCard) == 0) break;
		else {
			printf("%s\n", "密码错误！");
			return;
		}
	} while (1);
	switch (theCard->nStatus)
	{
	case 0:break;
	case 1:printf("%s\n", "上机失败，已上机！"); return;
	case 2:printf("%s\n", "上机失败，此卡已注销！"); return;
	case 3:printf("%s\n", "上机失败，卡状态异常！此卡已失效！"); return;
	default:
		return;
	}
	if (theCard->fBalance <= 0) {
		printf("%s\n", "上机失败，余额不足！");
		return;
	}
	theCard->nStatus = 1;
	theCard->tLast = time(NULL);
	LoginInfo* temp = (LoginInfo*)malloc(sizeof(LoginInfo));
	if (temp == NULL) {
		printf("%s\n", "电脑运行内存不足，上机失败！");
		return;
	}
	strcpy_s(temp->aCardName, 18, theCard->aName);
	temp->fBalance = theCard->fBalance;
	temp->tLogin = theCard->tLast;
	theCard->Login = temp;
	temp->settle = NULL;
	printf("\n%s\n", "上机成功！");
	billFile_writeLogin();
	printLoginInfo(temp);
	return;
}
//下机服务接口;  下机流程：判断是否正在上机、找到上机信息、读入计费标准并计算扣费、写入计费信息结构体释放上机下机结构体
void Bill_Serv_logOut() {
	printf("%s\n", "----------下机----------");
	Card* theCard = inputNameKey(cardStatus_settle);
	if (theCard == NULL) return;
	SettleInfo* p = (SettleInfo*)malloc(sizeof(SettleInfo));
	if (p == NULL) {
		printf("%s\n", "电脑运行内存不足，下机失败！");
		return;
	}
	p->tSettle = time(NULL);
	p->tLogin = theCard->Login->tLogin;
	strcpy_s(p->aCardName, 18, theCard->aName);
	time_t howMuchTime = (p->tSettle - p->tLogin);
	float cost = 0;
	cost = costCalculator(howMuchTime);
	p->fAmount = cost;
	float lastBalance = theCard->Login->fBalance;
	if (cost > lastBalance) {
		printf("\n下机失败，余额不足！消费%.3f元，余额%.3f元，请先充值！\n", cost, lastBalance);
		free(p);
		return;
	}
	p->fBalance = lastBalance - cost;
	theCard->Login->settle = p;
	theCard->fTotalUse += cost;
	theCard->fBalance = p->fBalance;
	theCard->nStatus = 0;
	theCard->nUseCount++;
	theCard->tLast = p->tSettle;
	cardFile_writeInfo();
	cardFile_writeTS();
	billservice_writeNewBill(theCard);
	printf("\n%s\n", "下机成功！");
	printBillInfo(theCard);
	free(theCard->Login->settle);
	free(theCard->Login);
	theCard->Login = NULL;
	billFile_writeLogin();

}


//将消费记录（计费信息）写入bills链表中,并调用函数写入文件
void billservice_writeNewBill(Card* thecard) {
	BillNode* cur = endPush_Bills(&All_Bills);
	if (cur == NULL) {
		printf("%s\n", "电脑运行内存不足，下机失败！");
		return;
	}
	strcpy_s(cur->data.aCardName, 18, thecard->aName);
	cur->data.fAmount = thecard->Login->settle->fAmount;
	cur->data.nDel = 0;
	cur->data.nStatus = 1;
	cur->data.tSettle = thecard->Login->settle->tSettle;
	cur->data.tSignIn = thecard->Login->tLogin;
	billFile_writeBill(cur);
}



//------------------------------------
//打印上机信息
void printLoginInfo(LoginInfo* temp) {
	char time[32] = { 0 };
	printf("%s\n", "---------上机信息如下--------");
	printf("%-18s%-12s%-12s\n\n", "卡号", "余额", "上机时间");
	printf("%-18s%-12.3f%-12s\n", temp->aCardName, temp->fBalance,
		timePrintToStr(time, &temp->tLogin));
	printf("\n%s\n", "----------分割线----------");
}

//打印计费信息
void printBillInfo(Card* temp) {
	char time[2][32] = { 0 };
	printf("%s\n", "---------下机信息如下--------");
	printf("%-18s%-10s%-12s%-27s%-27s\n\n", "卡号", "消费", "余额", "上机时间","下机时间");
	printf("%-18s%-10.3f%-12.3f%-27s%-27s\n", temp->aName, temp->Login->settle->fAmount,
		temp->fBalance,timePrintToStr(time[0], &temp->Login->tLogin),
		timePrintToStr(time[1], &temp->Login->settle->tSettle));
	printf("\n%s\n", "----------分割线----------");
}

//打印充值信息
void printChargeInfo(Card* temp,float temp_money) {
	printf("%s\n", "--------充值信息如下--------");
	printf("%-18s%-12s%-10s\n\n", "卡号", "充值金额", "余额");
	printf("%-18s%-12.3f%-10.3f\n", temp->aName, temp_money,
		temp->fBalance);
	printf("\n%s\n", "----------分割线----------");
}

//打印退费信息
void printDrawInfo(Card* temp, float temp_money) {
	printf("%s\n", "--------退费信息如下--------");
	printf("%-18s%-12s%-10s\n\n", "卡号", "退费金额", "余额");
	printf("%-18s%-12.3f%-10.3f\n", temp->aName, temp_money,
		temp->fBalance);
	printf("\n%s\n", "----------分割线----------");
}
//------------------------------------


//----------inputNameKey子函数------------------
//判断卡状态是否可以下机,1表示正常,0表示异常
int cardStatus_settle(Card* theCard) {
	switch (theCard->nStatus)
	{
	case 0:printf("%s\n", "下机失败，未上机！"); return 0;
	case 1:break;
	case 2:printf("%s\n", "下机失败，此卡已注销！"); return 0;
	case 3:printf("%s\n", "下机失败，卡状态异常！此卡已失效！"); return 0;
	default:
		return 0;
	}
	if (theCard->fBalance <= 0) {
		printf("%s\n", "下机失败，余额已小于0！请先充值！");
		return 0;
	}
	return 1;
}

//判断卡状态是否可以充值,1表示正常,0表示异常
int cardStatus_charge(Card* theCard) {
	switch (theCard->nStatus)
	{
	case 0:
	case 1:return 1;
	case 2:printf("%s\n", "充值失败，此卡已注销！"); return 0;
	case 3:printf("%s\n", "充值失败，卡状态异常！此卡已失效！"); return 0;
	default:
		return 0;
	}
}

//判断卡状态是否可以退费,1表示正常,0表示异常
int cardStatus_draw(Card* theCard) {
	switch (theCard->nStatus)
	{
	case 0:break;
	case 1:printf("%s\n", "退费失败，此卡正在上机！"); return 0;
	case 2:printf("%s\n", "退费失败，此卡已注销！"); return 0;
	case 3:printf("%s\n", "退费失败，此卡已失效！请直接注销！"); return 0;
	default:
		return 0;
	}
	if (theCard->fBalance <= 0) {
		printf("\n%s\n", "退费失败！余额不足！");
		return 0;
	}
	return 1;
}
//---------------------------------------------
 

//充值服务接口
void Card_Serv_rechargeCard() {
	printf("%s\n", "----------充值----------");
	Card* theCard = inputNameKey(cardStatus_charge);
	if (theCard == NULL) return;
	Money* p = (Money*)malloc(sizeof(Money));
	if (p == NULL) {
		printf("%s\n", "电脑运行内存不足，充值失败！");
		return;
	}
	float temp_money = 0;
	while (1) {
		printf("%s", "请输入充值金额(RMB)：");
		scanf_s("%f", &temp_money);
		if (temp_money < 0) { printf("%s\n", "不能输入负数！"); continue; }
		else if (temp_money > 1000000) { printf("%s\n", "金额过大！请输入一百万以内的正数！"); continue; }
		else { theCard->fBalance += temp_money; break; }
	}
	if (theCard->nStatus == 1)theCard->Login->fBalance = theCard->fBalance;
	printf("\n%s\n", "充值成功！");
	printChargeInfo(theCard, temp_money);
	cardFile_writeInfo();//保存更改
	billFile_writeLogin();
	strcpy_s(p->aCardName, 18, theCard->aName);
	p->fMoney = temp_money;
	p->nDel = 0;
	p->nStatus = 0;
	p->tTime = time(NULL);
	billFile_writeChrage(*p);
	free(p);
}
//退费服务接口
void Card_Serv_drawMoneyFromCard() {
	printf("%s\n", "----------退费----------");
	Card* theCard = inputNameKey(cardStatus_draw);//此处包括了判余额是否大于零
	if (theCard == NULL) return;
	Money* p = (Money*)malloc(sizeof(Money));
	if (p == NULL) {
		printf("%s\n", "电脑运行内存不足，退费失败！");
		return;
	}
	float temp_money = theCard->fBalance;
	theCard->fBalance = 0;
	printf("\n%s\n", "退费成功！");
	printDrawInfo(theCard, temp_money);
	cardFile_writeInfo();//保存更改
	strcpy_s(p->aCardName, 18, theCard->aName);
	p->fMoney = temp_money;
	p->nDel = 0;
	p->nStatus = 1;
	p->tTime = time(NULL);
	billFile_writeDraw(*p);
	free(p);
}