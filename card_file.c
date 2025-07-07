//���ļ�����ʵ�� ���������������ù��� �� ���ݴ洢�� �ķ���
#include"service.h"
#include"card_file.h"
#include"billing_file.h"
#include"model.h"
#include"tool.h"
#include<stdlib.h>
#include<string.h>

//��ʼ��
void DataInit() {
	cardDataInit();//������Ϣ���ص�����AllCards
	//cardFile_dataInit_B();//��ȡ�������ļ�
	LoginDataInit();//��ȡ�ϻ���Ϣ
}

//�����˵�ѭ�����Զ�����
void DataSave() {
	cardFile_writeInfo();//������Ϣд���ļ�card.txt
	cardFile_writeInfoBinary();//����Ϣд��������ļ�
	billFile_writeLogin();//����ǺͿ���Ϣһ������д�ģ������Զ�����
	//billFile_writeBill();//�Ʒ���Ϣ��׷��д�ģ����漰ɾ��������ֻ������������ʱ�ŵ��á�
}

//������Ϣ���ص�AllCards[]
void cardDataInit(){
	FILE* fp = fopen("./Data/card.txt", "r");
	if (fp == NULL) {
		printf("%s\n", "����ʧ�ܣ��볢����������");
		return;
	}
	char temp[512] = { 0 };
	char* str=NULL;
	char time_str[3][32] = { 0 };
	Card *cur, *pre=NULL;
	AllCards = (Card*)malloc(sizeof(Card));
	if (AllCards == NULL) {
		printf("�ڴ����ʧ�ܣ�\n");
		return;
	}
	*AllCards = StdCard;
	cur = AllCards;
	//���ж�ȡcard.txt�е���Ϣ
	for (int j=0;;) {
		str = fgets(temp, 512, fp);
		if (NULL != str) {
			cur = endPush(cur);
			if (cur == NULL) {
				printf("�ڴ����ʧ�ܣ�\n");
				return;
			}
			j = sscanf_s(temp, "%[^#]##%[^#]##%d##%[^#]##%[^#]##%f##%[^#]##%d##%f##%d",
				cur->aName, 23, cur->aPwd, 15, &cur->nStatus,
				time_str[0], 31, time_str[1], 31, &cur->fTotalUse,
				time_str[2], 31, &cur->nUseCount, &cur->fBalance, &cur->nDel);
			if (j < 10) {
				printf("%s\n", "����ʱ��ȡ����");
			}
			else {
				global.cardAmount++;
				global.cardID++;
				global.havewrite++;
			}
		}
		else break;//��ȡ����
	}
	fclose(fp);
	fp = NULL;
	cardFile_readTS();
	return;
}

//������Ϣд���ļ�card.txt
void cardFile_writeInfo() {
	//��Ҫ���Ǿɵ���Ϣ������"w+"
	FILE* fp = fopen("./Data/card.txt", "w+");
	if (fp == NULL) {
		printf("%s\n", "���ݱ���ʧ�ܣ������ԣ�");
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

//��ʱ����time_t��ʽ���һ��
void cardFile_writeTS() {
	FILE* fp = fopen("./Data/cardTS.txt", "w");
	if (fp == NULL) {
		printf("%s\n", "���ݱ���ʧ�ܣ������ԣ�");
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

//����timeStamp
void cardFile_readTS() {
	FILE* fp = fopen("./Data/cardTS.txt", "r");
	if (fp == NULL) {
		printf("%s\n", "δ�ҵ��ļ�\". / Data / cardTS\"���볢����������");
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
			printf("\n����Ϊ%s�Ŀ���Ч����%s ���ѹ���Ч�ڣ��Ѿ�ʧЧ��\n",
				cur->aName, timePrintToStr(temp, cur->tEnd));
			cur->nStatus = 3;
		}
	}
	fclose(fp);
	fp = NULL;
	return;
}

//����Ϣд��������ļ�
void cardFile_writeInfoBinary() {
	FILE* fp = fopen("./Data/card-B.bin", "wb");
	if (fp == NULL) {
		printf("%s\n", "�ļ�card-B.bin��ʧ�ܣ��볢����������");
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
			printf("%s\n", "�������ļ�д�뷢������");
			return;
		}
	}
	fclose(fp);
	fp = NULL;
}

//�Ӷ������ļ�������Ϣ
void cardFile_dataInit_B() {
	FILE* fp = fopen("./Data/card-B.bin", "rb");
	if (fp == NULL) {
		printf("%s\n", "�ļ�card-B.bin��ʧ�ܣ��볢����������");
		return;
	}
	AllCards = (Card*)malloc(sizeof(Card));
	if (AllCards == NULL) {
		printf("�ڴ����ʧ�ܣ�\n");
		return;
	}
	*AllCards = StdCard;
	Card* cur = AllCards;
	cur = endPush(AllCards);
	size_t count = 0;
	for (; cur != NULL; cur = cur->next) {
		count = fread(cur, sizeof(Card), 1, fp);
		if (count < 1) {
			printf("%s\n", "�������ļ���ȡ��������");
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