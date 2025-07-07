//���ļ�����ʵ�֡���ѯͳ�ơ��� ���ݴ洢�� �ķ���
#include"service.h"
#include<stdio.h>
#include"model.h"
#include"stdlib.h"

//���Ѽ�¼����β��
BillNode* append_rec(Bills head) {
	if (head == NULL) {
		head = (BillNode*)malloc(sizeof(BillNode));
		if (head == NULL) {
			printf("%s\n", "β��ʧ�ܣ�");
			return;
		}
	}
	BillNode* cur = head;
	for (; cur->next != NULL; cur = cur->next) {
		;
	}
	cur->next = (BillNode*)malloc(sizeof(BillNode));
	if (cur->next == NULL) {
		printf("%s\n", "β��ʧ�ܣ�");
		return;
	}
	cur->next->next = NULL;
	return cur->next;
}


//���������ļ��в������Ѽ�¼����������
Bills readAllRecs_Card(Bills All_rec, Card* theCard) {
	FILE* fp = fopen("./Data/Bills.txt", "r");
	if (fp == NULL) {
		printf("%s\n", "�ļ������ڣ�");
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
			printf("%s\n", "Bills.txt��ȡ����");
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