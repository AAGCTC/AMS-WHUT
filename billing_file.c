//���ļ�����ʵ�� ���Ʒѹ��� �� ���ݴ洢�� �ķ���
#include<stdio.h>
#include"model.h"
#include<stdlib.h>
#include<string.h>
#include"tool.h"
//���ϻ���Ϣд���ļ����»���Ϣ����Ҫ�����ļ�����Ϊ�»���
// ���»���¼�ᱻ�����ڼƷ���Ϣ�У�ͬʱ���»��ṹ��ᱻ�ͷ�
void billFile_writeLogin() {
	//��Ҫ���Ǿɵ���Ϣ������"w+"
	FILE* fp = fopen("./Data/Login.txt", "w+");
	if (fp == NULL) {
		printf("%s\n", "���ݱ���ʧ�ܣ������ԣ�");
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

//��ȡ�ϻ���Ϣ
void LoginDataInit() {
	FILE* fp = fopen("./Data/Login.txt", "r");
	if (fp == NULL) {
		printf("%s\n", "�ϻ���Ϣ��ȡʧ�ܣ�");
		return;
	}
	Card* cur = AllCards->next;
	for (int j=0; cur != NULL; cur = cur->next) {
		if (cur->nStatus == 1) {
			LoginInfo* newNode = (LoginInfo*)malloc(sizeof(LoginInfo));
			if (newNode == NULL) {
				printf("�ڴ����ʧ�ܣ�\n");
				return;
			}
			cur->Login = newNode;
			j = fscanf_s(fp, "%[^#]##%lld##%f\n", newNode->aCardName, 18,
				&newNode->tLogin, &newNode->fBalance);
			if (j < 3) {
				printf("%s", "�ϻ���Ϣ��ȡ����");
			}
			if (strcmp(newNode->aCardName, cur->aName) != 0) {
				printf("%s", "�ϻ���Ϣ��ȡ����");
			}
		}
	}
	fclose(fp);
	fp = NULL;
}

//׷��д���µļƷ���Ϣ
void billFile_writeBill(BillNode* cur) {
	FILE* fp = fopen("./Data/Bills.txt", "a");
	fprintf(fp, "%s##%.3f##%d##%d##%lld##%lld\n", cur->data.aCardName,
		cur->data.fAmount, cur->data.nDel, cur->data.nStatus,
		cur->data.tSignIn, cur->data.tSettle);
	fclose(fp);
	fp = NULL;
}


//������ʷ�Ʒ���Ϣ�����»��ģ�
void billingDataInit() {
	;
}

//����ֵ��Ϣ�������ļ�
void billFile_writeChrage(Money cur) {
	FILE* fp = fopen("./Data/Charge.txt", "a");
	if (fp == NULL) {
		printf("%s\n", "��ֵ��Ϣ����ʧ�ܣ�");
		return;	
	}
	char str[32] = { 0 };
	fprintf(fp, "%s##%d##%.3f##%d##%lld##%s\n", cur.aCardName, 
		cur.nStatus, cur.fMoney, cur.nDel, cur.tTime, 
		timePrintToStr(str, &cur.tTime));
	fclose(fp);
	fp = NULL;
}

//���˷���Ϣ�������ļ�
void billFile_writeDraw(Money cur) {
	FILE* fp = fopen("./Data/Draw.txt", "a");
	if (fp == NULL) {
		printf("%s\n", "��ֵ��Ϣ����ʧ�ܣ�");
		return;	
	}
	char str[32] = { 0 };
	fprintf(fp, "%s##%d##%.3f##%d##%lld##%s\n", cur.aCardName, 
		cur.nStatus, cur.fMoney, cur.nDel, cur.tTime, 
		timePrintToStr(str, &cur.tTime));
	fclose(fp);
	fp = NULL;
}