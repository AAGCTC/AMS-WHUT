//本文件负责实现 “卡管理”、“费用管理” 的 数据存储层 的服务
#include"service.h"
#include"card_file.h"
#include"billing_file.h"
#include"model.h"
#include"tool.h"
#include<stdlib.h>
#include<string.h>

//初始化
void DataInit() {
	cardDataInit();//将卡信息加载到链表AllCards
	//cardFile_dataInit_B();//读取二进制文件
	LoginDataInit();//读取上机信息
}

//在主菜单循环中自动保存
void DataSave() {
	cardFile_writeInfo();//将卡信息写入文件card.txt
	cardFile_writeInfoBinary();//将信息写入二进制文件
	billFile_writeLogin();//这个是和卡信息一样覆盖写的，所以自动保存
	//billFile_writeBill();//计费信息是追加写的，不涉及删除，所以只在有新增内容时才调用。
}

//将卡信息加载到AllCards[]
void cardDataInit(){
	FILE* fp = fopen("./Data/card.txt", "r");
	if (fp == NULL) {
		printf("%s\n", "加载失败！请尝试重启程序！");
		return;
	}
	char temp[512] = { 0 };
	char* str=NULL;
	char time_str[3][32] = { 0 };
	Card *cur, *pre=NULL;
	AllCards = (Card*)malloc(sizeof(Card));
	if (AllCards == NULL) {
		printf("内存分配失败！\n");
		return;
	}
	*AllCards = StdCard;
	cur = AllCards;
	//逐行读取card.txt中的信息
	for (int j=0;;) {
		str = fgets(temp, 512, fp);
		if (NULL != str) {
			cur = endPush(cur);
			if (cur == NULL) {
				printf("内存分配失败！\n");
				return;
			}
			j = sscanf_s(temp, "%[^#]##%[^#]##%d##%[^#]##%[^#]##%f##%[^#]##%d##%f##%d",
				cur->aName, 23, cur->aPwd, 15, &cur->nStatus,
				time_str[0], 31, time_str[1], 31, &cur->fTotalUse,
				time_str[2], 31, &cur->nUseCount, &cur->fBalance, &cur->nDel);
			if (j < 10) {
				printf("%s\n", "加载时读取错误！");
			}
			else {
				global.cardAmount++;
				global.cardID++;
				global.havewrite++;
			}
		}
		else break;//读取结束
	}
	fclose(fp);
	fp = NULL;
	cardFile_readTS();
	return;
}

//将卡信息写入文件card.txt
void cardFile_writeInfo() {
	//需要覆盖旧的信息，故用"w+"
	FILE* fp = fopen("./Data/card.txt", "w+");
	if (fp == NULL) {
		printf("%s\n", "数据保存失败！请重试！");
		return;
	}
	cardFile_writeTS();
	char time[3][32] = { 0 };
	Card* cur = AllCards->next;
	for (; cur!=NULL; cur = cur->next){
		
		fprintf(fp, "%s##%s##%d", cur->aName, cur->aPwd, cur->nStatus);
		fprintf(fp, "##%s##%s##%.3f", timePrintToStr(time[0], &cur->tStart), 
			timePrintToStr(time[1], &cur->tEnd), cur->fTotalUse);
		fprintf(fp, "##%s##%d##%.3f", timePrintToStr(time[2], &cur->tLast),
			cur->nUseCount, cur->fBalance);
		fprintf(fp, "##%d\n", cur->nDel);
	}
	fclose(fp);
	fp = NULL;
}

//将时间用time_t格式另存一份
void cardFile_writeTS() {
	FILE* fp = fopen("./Data/cardTS.txt", "w");
	if (fp == NULL) {
		printf("%s\n", "数据保存失败！请重试！");
		return;
	}
	Card* cur = AllCards->next;
	for (; cur!=NULL; cur = cur->next) {
		fprintf(fp, "%s##%lld##%lld##%lld\n", cur->aName, cur->tStart, cur->tEnd, cur->tLast);
	}
	fclose(fp);
	fp = NULL;
	return;
}

//读入timeStamp
void cardFile_readTS() {
	FILE* fp = fopen("./Data/cardTS.txt", "r");
	if (fp == NULL) {
		printf("%s\n", "未找到文件\". / Data / cardTS\"！请尝试重启程序！");
		return;
	}
	Card* cur = AllCards;
	time_t now = time(NULL);
	for (int i=0 ,temp=0;i<global.cardAmount; i++) {
		cur = cur->next;
		temp = fscanf_s(fp, "%*[^#]##%lld##%lld##%lld", &cur->tStart, &cur->tEnd, &cur->tLast);
		if (temp < 0) break;
		if (cur->tEnd < now) {
			char temp[32] = { 0 };
			printf("\n卡号为%s的卡有效期至%s ，已过有效期，已经失效！\n",
				cur->aName, timePrintToStr(temp, cur->tEnd));
			cur->nStatus = 3;
		}
	}
	fclose(fp);
	fp = NULL;
	return;
}

//将信息写入二进制文件
void cardFile_writeInfoBinary() {
	FILE* fp = fopen("./Data/card-B.bin", "wb");
	if (fp == NULL) {
		printf("%s\n", "文件card-B.bin打开失败！请尝试重启程序！");
		return;
	}
	Card* cur = AllCards;
	if (cur->next == NULL) {
		return;
	}
	else cur = cur->next;
	size_t count = 0;
	for (; cur != NULL; cur = cur->next) {
		count = fwrite(cur, sizeof(Card), 1, fp);
		if (count < 1) {
			printf("%s\n", "二进制文件写入发生错误！");
			return;
		}
	}
	fclose(fp);
	fp = NULL;
}

//从二进制文件读入信息
void cardFile_dataInit_B() {
	FILE* fp = fopen("./Data/card-B.bin", "rb");
	if (fp == NULL) {
		printf("%s\n", "文件card-B.bin打开失败！请尝试重启程序！");
		return;
	}
	AllCards = (Card*)malloc(sizeof(Card));
	if (AllCards == NULL) {
		printf("内存分配失败！\n");
		return;
	}
	*AllCards = StdCard;
	Card* cur = AllCards;
	cur = endPush(AllCards);
	size_t count = 0;
	for (; cur != NULL; cur = cur->next) {
		count = fread(cur, sizeof(Card), 1, fp);
		if (count < 1) {
			printf("%s\n", "二进制文件读取发生错误！");
			free(cur);
			fclose(fp);
			fp = NULL;
			return;
		}
		else {
			global.cardAmount++;
			global.cardID++;
			global.havewrite++;
			cur = endPush(AllCards);
		}
	}
	fclose(fp);
	fp = NULL;
}