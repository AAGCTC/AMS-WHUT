//本文件负责实现 “计费管理” 的 数据存储层 的服务
#include<stdio.h>
#include"model.h"
#include<stdlib.h>
#include<string.h>
#include"tool.h"
//将上机信息写入文件（下机信息不需要存入文件，因为下机后，
// 上下机记录会被保存在计费信息中，同时上下机结构体会被释放
void billFile_writeLogin() {
	//需要覆盖旧的信息，故用"w+"
	FILE* fp = fopen("./Data/Login.txt", "w+");
	if (fp == NULL) {
		printf("%s\n", "数据保存失败！请重试！");
		return;
	}
	char time[32] = { 0 };
	Card* cur = AllCards->next;
	for (; cur != NULL; cur = cur->next) {
		if (cur->nStatus == 1) {
			fprintf(fp, "%s", cur->aName);
			fprintf(fp, "##%lld##%.3f\n", cur->Login->tLogin,
				cur->Login->fBalance);
		}
	}
	fclose(fp);
	fp = NULL;
}

//读取上机信息
void LoginDataInit() {
	FILE* fp = fopen("./Data/Login.txt", "r");
	if (fp == NULL) {
		printf("%s\n", "上机信息读取失败！");
		return;
	}
	Card* cur = AllCards->next;
	for (int j=0; cur != NULL; cur = cur->next) {
		if (cur->nStatus == 1) {
			LoginInfo* newNode = (LoginInfo*)malloc(sizeof(LoginInfo));
			if (newNode == NULL) {
				printf("内存分配失败！\n");
				return;
			}
			cur->Login = newNode;
			j = fscanf_s(fp, "%[^#]##%lld##%f\n", newNode->aCardName, 18,
				&newNode->tLogin, &newNode->fBalance);
			if (j < 3) {
				printf("%s", "上机信息读取错误！");
			}
			if (strcmp(newNode->aCardName, cur->aName) != 0) {
				printf("%s", "上机信息读取错误！");
			}
		}
	}
	fclose(fp);
	fp = NULL;
}

//追加写入新的计费信息
void billFile_writeBill(BillNode* cur) {
	FILE* fp = fopen("./Data/Bills.txt", "a");
	fprintf(fp, "%s##%.3f##%d##%d##%lld##%lld\n", cur->data.aCardName,
		cur->data.fAmount, cur->data.nDel, cur->data.nStatus,
		cur->data.tSignIn, cur->data.tSettle);
	fclose(fp);
	fp = NULL;
}


//读入历史计费信息（已下机的）
void billingDataInit() {
	;
}

//将充值信息保存至文件
void billFile_writeChrage(Money cur) {
	FILE* fp = fopen("./Data/Charge.txt", "a");
	if (fp == NULL) {
		printf("%s\n", "充值信息保存失败！");
		return;	
	}
	char str[32] = { 0 };
	fprintf(fp, "%s##%d##%.3f##%d##%lld##%s\n", cur.aCardName, 
		cur.nStatus, cur.fMoney, cur.nDel, cur.tTime, 
		timePrintToStr(str, &cur.tTime));
	fclose(fp);
	fp = NULL;
}

//将退费信息保存至文件
void billFile_writeDraw(Money cur) {
	FILE* fp = fopen("./Data/Draw.txt", "a");
	if (fp == NULL) {
		printf("%s\n", "充值信息保存失败！");
		return;	
	}
	char str[32] = { 0 };
	fprintf(fp, "%s##%d##%.3f##%d##%lld##%s\n", cur.aCardName, 
		cur.nStatus, cur.fMoney, cur.nDel, cur.tTime, 
		timePrintToStr(str, &cur.tTime));
	fclose(fp);
	fp = NULL;
}