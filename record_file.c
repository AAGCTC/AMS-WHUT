//本文件负责实现“查询统计”的 数据存储层 的服务
#include"service.h"
#include<stdio.h>
#include"model.h"
#include"stdlib.h"

//消费记录链表尾插
BillNode* append_rec(Bills head) {
	if (head == NULL) {
		head = (BillNode*)malloc(sizeof(BillNode));
		if (head == NULL) {
			printf("%s\n", "尾插失败！");
			return;
		}
	}
	BillNode* cur = head;
	for (; cur->next != NULL; cur = cur->next) {
		;
	}
	cur->next = (BillNode*)malloc(sizeof(BillNode));
	if (cur->next == NULL) {
		printf("%s\n", "尾插失败！");
		return;
	}
	cur->next->next = NULL;
	return cur->next;
}


//按卡号在文件中查找消费记录并构建链表
Bills readAllRecs_Card(Bills All_rec, Card* theCard) {
	FILE* fp = fopen("./Data/Bills.txt", "r");
	if (fp == NULL) {
		printf("%s\n", "文件不存在！");
		return NULL;
	}
	char temp[256] = { 0 };
	char* str;
	BillNode tempNode;
	for (int j = 0;;) {
		str = fgets(temp, 256, fp);
		if (str == NULL) {
			break;
		}
		j = sscanf_s(temp, "%[^#]##%f##%d##%d##%lld##%lld", tempNode.data.aCardName,24,
			&tempNode.data.fAmount, &tempNode.data.nDel, &tempNode.data.nStatus,
			&tempNode.data.tSignIn, &tempNode.data.tSettle);
		if (j < 6) {
			printf("%s\n", "Bills.txt读取错误！");
			return;
		}
		if (strcmp(tempNode.data.aCardName, theCard->aName) == 0) {
			BillNode* cur = append_rec(All_rec);
			cur->data = tempNode.data;
			strcpy_s(cur->data.aCardName, 24, tempNode.data.aCardName);
		}
	}
	fclose(fp);
	free(fp);
	return All_rec;
}