//���ļ�����ʵ�� ���������������ù��� �� ҵ���߼��� �ķ���
#include"service.h"
#include"tool.h"
#include"model.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"billing_file.h"

//����Ĭ����Ч�ڣ�������94608000�룩
#define CARD_VALID_TIME 94608000

//��ӿ�����ӿ�
void CardService_newCard() {
	Card *newCard = endPush(AllCards);
	if (newCard == NULL) {
		printf("%s\n", "���������ڴ治�㣬��ӿ�ʧ�ܣ�");
		return;
	}
	global.cardID++;
	global.cardAmount++;
	*newCard = StdCard;//��һ��Card�ͳ�����newCard���г�ʼ��
	printf("%s\n", "----------��ӿ�----------");
	printf("%s\n", "���ڰ���һ���¿����밴��ʾ������Ϣ��");
	char cName[128] = { 0 };//������ʱ��������Ŀ���
	int flag = 0;
	while (flag == 0)
	{
		printf("%s\n", "����Ϊ�¿�ָ��һ������(����1~18)��");
		scanf_s("%s", cName, 128);
		if (!isLenthLegal(cName, 18)) {
			printf("%s\n", "���󣡿��Ź�����");
			printf("%s", "(���������)");
			continue;
		}
		//����ʹ���ѱ�ռ�õĿ���
		else if (findCard1(cName)!=NULL) {
			printf("%s\n", "����ʹ���ѱ�ռ�õĿ���");
			printf("%s", "(���������)");
			continue;
		}
		else {
			flag = 1;
			cName[18] = 0;//���Ѿ���֤����<=18����£��ضϺ���Ĳ���
			strcpy_s(newCard->aName, 18, cName);
		}
	}
	inputKeyOfCard(newCard->aPwd);
	float initMoney;
	while (1) {
		printf("%s\n", "�����뿪�����(��Ԫ����)(�벻Ҫ������������������ַ�)��");
		scanf_s("%f", &initMoney);
		if (initMoney < 0) {
			printf("%s\n", "��������Ϊ������");
			printf("%s", "(���������)");
			continue;
		}
		else if (initMoney > 10000) {
			printf("%s\n", "����������");
			printf("%s", "(���������)");
			continue;
		}
		else {
			newCard->fBalance = initMoney;
			break;
		}
	}
	newCard->tStart = time(NULL);
	newCard->tEnd = newCard->tStart + CARD_VALID_TIME;//Ĭ���������
	newCard->tLast = newCard->tStart;
	cardFile_writeInfo();
	cardFile_writeInfoBinary();
	printf("%s\n", "���½��Ŀ�����Ϣ���£�");
	printCardInfo(newCard);
	printf("\n%s\n", "�쿨�ɹ�����ӿ����������");
}
//��ѯ������ӿ�
void CardService_checkCard() {
	printf("%s\n", "----------��ѯ��----------");
	char cName[24] = { 0 };
	inputNameOfCard(cName);
	Card* obj = AllCards;//��ͷ�ڵ㿪ʼ����
	for (int count = 0;;count++) {
		obj = findCard2(obj ,cName);
		if (obj == NULL) {
			if(count==0) printf("%s\n", "û�иÿ�����Ϣ��");
			break;
		}
		else {
			if(count == 0) printf("%s\n", "----�������ҵ��Ŀ�����Ϣ----");
			printCardInfo(obj);
		}
		if (obj->next != NULL) obj = obj->next;
		else break;
	}
	printf("\n%s\n", "-------������-------");
	return;
	/*
	���������
	������뿨�š�����ĳ��ȵ��Ƿ�淶�Ĺ�����ҪƵ�����ã�Ӧ����װ�ɺ�������tool�У�
	�ڲ�ѯ���е������ݲ㺯����ȡ����Ϣ��δ�ҵ���Ҫ������Ӧ�ò����ˣ���AllCards[]�ˣ�
	*/
}
//ע��������ӿ�
void CardService_cancelCard() {
	printf("%s\n", "----------ע����----------");
	Card* theCard = inputNameKey(cardStatus_cancel);//ʧЧ�Ŀ�����ʹ�õ�����ע��
	if (theCard == NULL) return;
	float temp_money = theCard->fBalance;
	theCard->nStatus = 2;
	theCard->fBalance = 0;//ע����ͬʱ�˻��������
	theCard->tLast = time(NULL);
	printf("\n%s\n", "ע���ɹ���");
	printCancelInfo(theCard, temp_money);
	cardFile_writeInfo();//�������
	Money* p = (Money*)malloc(sizeof(Money));
	if (p == NULL) {
		printf("%s\n", "���������ڴ治�㣬�˷�ʧ�ܣ�");
		return;
	}
	strcpy_s(p->aCardName, 18, theCard->aName);
	p->fMoney = temp_money;
	p->nDel = 0;
	p->nStatus = 1;
	p->tTime = time(NULL);
	billFile_writeDraw(*p);
}



//----------��ӿ���������----------


//----------��ѯ����������----------
//��ӡ����Ϣ
void printCardInfo(Card* card) {
	char time[3][32] = { 0 };
	//printf("%s\n", "----���������ſ�����Ϣ----");
	printf("%-18s%-10s%-10s%-10s%-10s%-10s%-10s\n\n", "����", "����", "���",
		"�ۼƽ��", "ʹ�ô���", "��״̬", "ɾ����־");
	printf("%-18s%-10s%-10.3f%-10.3f%-10d", card->aName, card->aPwd, card->fBalance,
		card->fTotalUse, card->nUseCount);
	switch (card->nStatus)
	{
	case 0:
		printf("%-10s", "δ�ϻ�");
		break;
	case 1:
		printf("%-10s", "�����ϻ�");
		break;
	case 2:
		printf("%-10s", "��ע��");
		break;
	default:
		printf("%-10s", "ʧЧ");
		break;
	}
	switch (card->nDel)
	{
	case 0:
		printf("%-10s", "δɾ��");
		break;
	case 1:
		printf("%-10s", "��ɾ��");
		break;
	}
	printf("\n\n%-27s%-27s%-27s\n\n", "����ʱ��", "��Ч����", "���ʹ��ʱ��");
	printf("%-27s%-27s%-27s", timePrintToStr(time[0], &card->tStart), timePrintToStr(time[1], &card->tEnd),
		timePrintToStr(time[2], &card->tLast));

	printf("\n%s\n", "----------�ָ���----------");
}

//��AllCards��,�����ž�ȷ��ѯ
Card* findCard1(char* str2) {
	//�ȶԲ����п�
	if (str2 == NULL) {
		return NULL;
	}
	Card* cur = AllCards->next;
	for (; cur != NULL; cur = cur->next) {
		if (strcmp(cur->aName,str2)==0) {
			return cur;
		}
	}
	return NULL;//û�ҵ�
}

//��AllCards��,������ģ����ѯ,��head��ָ��Ľڵ㿪ʼ����
Card* findCard2(Card* head, char* str2) {
	//�ȶԲ����п�
	if (str2 == NULL) {
		return NULL;
	}
	Card* cur = head;
	int len2 = strlen(str2);
	for (; cur != NULL; cur = cur->next) {
		//���str2�Ƿ�Ϊstr1���Ӵ�,str2Ϊ��ѯ��ģ������Ŀ����Ӵ�
		int len1 = strlen(cur->aName);
		if (len1< len2) {
			continue;
		}
		// ʹ��strstr�⺯�����Ӵ�����
		if (strstr(cur->aName, str2) != NULL) {
			return cur;
		}
	}
	return NULL;//û�ҵ�
}


//----------ע������������----------
int cardStatus_cancel(Card* theCard) {
	switch (theCard->nStatus)
	{
	case 0:
	case 3:return 1;//ʧЧ�Ŀ�����ʹ�õ�����ע�����˷�
	case 1:printf("%s\n", "ע��ʧ�ܣ��˿������ϻ���"); return 0;
	case 2:printf("%s\n", "ע��ʧ�ܣ��˿���ע����"); return 0;
	default:
		return 0;
	}
}

void printCancelInfo(Card* theCard, float temp_money) {
	printf("\nע��ʱ������ʣ%.3f����ȫ���˻��������Ϊ��0.000\n", temp_money);
	printf("%s\n", "--------ע����Ϣ����--------");
	printCardInfo(theCard);
}