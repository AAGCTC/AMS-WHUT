//���ļ�����ʵ�� ���Ʒѹ��� �� ҵ���߼��� �ķ���
#include"billing_service.h"
#include"billing_file.h"
#include"rate_service.h"
#include"card_file.h"
#include<stdio.h>
#include"model.h"
#include"tool.h"
#include<string.h>

//���õ�����Դ�ļ��ĺ���:---------------------------
//��AllCards��,�����ž�ȷ��ѯ
extern Card* findCard1(char* str2);
//-----------------------------------------------


//�ϻ�����ӿ�
void Bill_Serv_logIn() {
	printf("%s\n", "----------�ϻ�----------");
	printf("%s\n", "������Ҫ�ϻ��Ŀ��Ŀ��ţ�");
	char name[32] = { 0 };
	Card* theCard;
	do
	{
		inputNameOfCard(name);
		theCard = findCard1(name);
		if (theCard == NULL) {
			printf("%s\n", "δ�ҵ���Ӧ�������ٴ����룡");
		}
		else break;
	} while (1);
	printf("%s\n", "���������룺");
	char keyofCard[128];
	int flag = 0;
	do {
		while (flag < 1)
		{
			scanf_s("%s", keyofCard, 128);
			if (!isLenthLegal(keyofCard, 8)) {
				printf("%s\n", "�������������");
				printf("%s", "(����������)");
				continue;
			}
			else {
				flag++;
			}
		}
		if (strcmp(theCard->aPwd, keyofCard) == 0) break;
		else {
			printf("%s\n", "�������");
			return;
		}
	} while (1);
	switch (theCard->nStatus)
	{
	case 0:break;
	case 1:printf("%s\n", "�ϻ�ʧ�ܣ����ϻ���"); return;
	case 2:printf("%s\n", "�ϻ�ʧ�ܣ��˿���ע����"); return;
	case 3:printf("%s\n", "�ϻ�ʧ�ܣ���״̬�쳣���˿���ʧЧ��"); return;
	default:
		return;
	}
	if (theCard->fBalance <= 0) {
		printf("%s\n", "�ϻ�ʧ�ܣ����㣡");
		return;
	}
	theCard->nStatus = 1;
	theCard->tLast = time(NULL);
	LoginInfo* temp = (LoginInfo*)malloc(sizeof(LoginInfo));
	if (temp == NULL) {
		printf("%s\n", "���������ڴ治�㣬�ϻ�ʧ�ܣ�");
		return;
	}
	strcpy_s(temp->aCardName, 18, theCard->aName);
	temp->fBalance = theCard->fBalance;
	temp->tLogin = theCard->tLast;
	theCard->Login = temp;
	temp->settle = NULL;
	printf("\n%s\n", "�ϻ��ɹ���");
	billFile_writeLogin();
	printLoginInfo(temp);
	return;
}
//�»�����ӿ�;  �»����̣��ж��Ƿ������ϻ����ҵ��ϻ���Ϣ������Ʒѱ�׼������۷ѡ�д��Ʒ���Ϣ�ṹ���ͷ��ϻ��»��ṹ��
void Bill_Serv_logOut() {
	printf("%s\n", "----------�»�----------");
	Card* theCard = inputNameKey(cardStatus_settle);
	if (theCard == NULL) return;
	SettleInfo* p = (SettleInfo*)malloc(sizeof(SettleInfo));
	if (p == NULL) {
		printf("%s\n", "���������ڴ治�㣬�»�ʧ�ܣ�");
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
		printf("\n�»�ʧ�ܣ����㣡����%.3fԪ�����%.3fԪ�����ȳ�ֵ��\n", cost, lastBalance);
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
	printf("\n%s\n", "�»��ɹ���");
	printBillInfo(theCard);
	free(theCard->Login->settle);
	free(theCard->Login);
	theCard->Login = NULL;
	billFile_writeLogin();

}


//�����Ѽ�¼���Ʒ���Ϣ��д��bills������,�����ú���д���ļ�
void billservice_writeNewBill(Card* thecard) {
	BillNode* cur = endPush_Bills(&All_Bills);
	if (cur == NULL) {
		printf("%s\n", "���������ڴ治�㣬�»�ʧ�ܣ�");
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
//��ӡ�ϻ���Ϣ
void printLoginInfo(LoginInfo* temp) {
	char time[32] = { 0 };
	printf("%s\n", "---------�ϻ���Ϣ����--------");
	printf("%-18s%-12s%-12s\n\n", "����", "���", "�ϻ�ʱ��");
	printf("%-18s%-12.3f%-12s\n", temp->aCardName, temp->fBalance,
		timePrintToStr(time, &temp->tLogin));
	printf("\n%s\n", "----------�ָ���----------");
}

//��ӡ�Ʒ���Ϣ
void printBillInfo(Card* temp) {
	char time[2][32] = { 0 };
	printf("%s\n", "---------�»���Ϣ����--------");
	printf("%-18s%-10s%-12s%-27s%-27s\n\n", "����", "����", "���", "�ϻ�ʱ��","�»�ʱ��");
	printf("%-18s%-10.3f%-12.3f%-27s%-27s\n", temp->aName, temp->Login->settle->fAmount,
		temp->fBalance,timePrintToStr(time[0], &temp->Login->tLogin),
		timePrintToStr(time[1], &temp->Login->settle->tSettle));
	printf("\n%s\n", "----------�ָ���----------");
}

//��ӡ��ֵ��Ϣ
void printChargeInfo(Card* temp,float temp_money) {
	printf("%s\n", "--------��ֵ��Ϣ����--------");
	printf("%-18s%-12s%-10s\n\n", "����", "��ֵ���", "���");
	printf("%-18s%-12.3f%-10.3f\n", temp->aName, temp_money,
		temp->fBalance);
	printf("\n%s\n", "----------�ָ���----------");
}

//��ӡ�˷���Ϣ
void printDrawInfo(Card* temp, float temp_money) {
	printf("%s\n", "--------�˷���Ϣ����--------");
	printf("%-18s%-12s%-10s\n\n", "����", "�˷ѽ��", "���");
	printf("%-18s%-12.3f%-10.3f\n", temp->aName, temp_money,
		temp->fBalance);
	printf("\n%s\n", "----------�ָ���----------");
}
//------------------------------------


//----------inputNameKey�Ӻ���------------------
//�жϿ�״̬�Ƿ�����»�,1��ʾ����,0��ʾ�쳣
int cardStatus_settle(Card* theCard) {
	switch (theCard->nStatus)
	{
	case 0:printf("%s\n", "�»�ʧ�ܣ�δ�ϻ���"); return 0;
	case 1:break;
	case 2:printf("%s\n", "�»�ʧ�ܣ��˿���ע����"); return 0;
	case 3:printf("%s\n", "�»�ʧ�ܣ���״̬�쳣���˿���ʧЧ��"); return 0;
	default:
		return 0;
	}
	if (theCard->fBalance <= 0) {
		printf("%s\n", "�»�ʧ�ܣ������С��0�����ȳ�ֵ��");
		return 0;
	}
	return 1;
}

//�жϿ�״̬�Ƿ���Գ�ֵ,1��ʾ����,0��ʾ�쳣
int cardStatus_charge(Card* theCard) {
	switch (theCard->nStatus)
	{
	case 0:
	case 1:return 1;
	case 2:printf("%s\n", "��ֵʧ�ܣ��˿���ע����"); return 0;
	case 3:printf("%s\n", "��ֵʧ�ܣ���״̬�쳣���˿���ʧЧ��"); return 0;
	default:
		return 0;
	}
}

//�жϿ�״̬�Ƿ�����˷�,1��ʾ����,0��ʾ�쳣
int cardStatus_draw(Card* theCard) {
	switch (theCard->nStatus)
	{
	case 0:break;
	case 1:printf("%s\n", "�˷�ʧ�ܣ��˿������ϻ���"); return 0;
	case 2:printf("%s\n", "�˷�ʧ�ܣ��˿���ע����"); return 0;
	case 3:printf("%s\n", "�˷�ʧ�ܣ��˿���ʧЧ����ֱ��ע����"); return 0;
	default:
		return 0;
	}
	if (theCard->fBalance <= 0) {
		printf("\n%s\n", "�˷�ʧ�ܣ����㣡");
		return 0;
	}
	return 1;
}
//---------------------------------------------
 

//��ֵ����ӿ�
void Card_Serv_rechargeCard() {
	printf("%s\n", "----------��ֵ----------");
	Card* theCard = inputNameKey(cardStatus_charge);
	if (theCard == NULL) return;
	Money* p = (Money*)malloc(sizeof(Money));
	if (p == NULL) {
		printf("%s\n", "���������ڴ治�㣬��ֵʧ�ܣ�");
		return;
	}
	float temp_money = 0;
	while (1) {
		printf("%s", "�������ֵ���(RMB)��");
		scanf_s("%f", &temp_money);
		if (temp_money < 0) { printf("%s\n", "�������븺����"); continue; }
		else if (temp_money > 1000000) { printf("%s\n", "������������һ�������ڵ�������"); continue; }
		else { theCard->fBalance += temp_money; break; }
	}
	if (theCard->nStatus == 1)theCard->Login->fBalance = theCard->fBalance;
	printf("\n%s\n", "��ֵ�ɹ���");
	printChargeInfo(theCard, temp_money);
	cardFile_writeInfo();//�������
	billFile_writeLogin();
	strcpy_s(p->aCardName, 18, theCard->aName);
	p->fMoney = temp_money;
	p->nDel = 0;
	p->nStatus = 0;
	p->tTime = time(NULL);
	billFile_writeChrage(*p);
	free(p);
}
//�˷ѷ���ӿ�
void Card_Serv_drawMoneyFromCard() {
	printf("%s\n", "----------�˷�----------");
	Card* theCard = inputNameKey(cardStatus_draw);//�˴�������������Ƿ������
	if (theCard == NULL) return;
	Money* p = (Money*)malloc(sizeof(Money));
	if (p == NULL) {
		printf("%s\n", "���������ڴ治�㣬�˷�ʧ�ܣ�");
		return;
	}
	float temp_money = theCard->fBalance;
	theCard->fBalance = 0;
	printf("\n%s\n", "�˷ѳɹ���");
	printDrawInfo(theCard, temp_money);
	cardFile_writeInfo();//�������
	strcpy_s(p->aCardName, 18, theCard->aName);
	p->fMoney = temp_money;
	p->nDel = 0;
	p->nStatus = 1;
	p->tTime = time(NULL);
	billFile_writeDraw(*p);
	free(p);
}