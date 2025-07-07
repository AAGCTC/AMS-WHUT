//���ļ�����ʵ�֡���ѯͳ�ơ��� ҵ���߼��� �ķ���
#include"service.h"
#include<stdio.h>

//��ӡ�Ʒ���Ϣ
void printRecord(BillNode* temp);

//��ѯͳ�Ʒ���ӿ�
void Record_Serv_check() {
	printf("%s\n", "----------��ѯͳ��----------");
	int cmd = 0;
	do {
		Menu_PrintMenu_rec();
		cmd = Menu_GetCommend_rec();
		switch (cmd)
		{
		case 0:
			return;
		case 1:
			record_Card();
			break;
		case 2:
			record_Total();
			break;
		case 3:
			record_Total_thisMonth();
			break;
		default:
			printf("%s\n", "����Ĳ˵���Ŵ���");
			continue;
		}
	} while (cmd != 0);
}

//��ӡ�Ӳ˵�
void Menu_PrintMenu_rec() {
	printf("\n%s\n", "��ӭ�����ѯͳ�ƹ���");
	printf("\n%s\n", "-----�Ӳ˵�-----");
	printf("%s\n", "1.������ͳ��");
	printf("%s\n", "2.��Ӫҵ��");
	printf("%s\n", "3.��Ӫҵ��");
	printf("%s\n", "0.�������˵�");
	printf("%s\n", "��ѡ��˵���ţ�0~3����");
}

//�����û����������
int Menu_GetCommend_rec() {
	int cmd = 0;
	char string[256] = { 0 };
	scanf_s("%s", string, 256);
	if (strlen(string) > 1) return -1;
	else if ('0' <= string[0] && string[0] <= '3') {
		cmd = string[0] - '0';
		return cmd;
	}
	else return -1;
}

//����inputNameKey�������һ������ָ�룬���˴�����Ҫ�жϿ�״̬���ʶ���һ���պ���
int cardStatus_rec(Card* theCard) {
	;
}

//�����Ų�ѯ���Ѽ�¼
void record_Card() {

	Card* theCard = inputNameKey(cardStatus_rec);
	Bills All_rec = (BillNode*)malloc(sizeof(BillNode));
	if (All_rec == NULL) {
		printf("%s\n", "ͷ������ʧ��");
		return;
	}
	All_rec->next = NULL;
	All_rec = readAllRecs_Card(All_rec,theCard);
	
	printf("%-18s%-10s\n", "����", "�����ѽ��");
	printf("%-18s%-10.3f\n", theCard->aName, theCard->fTotalUse);
	printf("%s\n", "---------���Ѽ�¼--------");
	for (BillNode* cur = All_rec->next; cur != NULL; cur = cur->next) {
		printRecord(cur);
	}
}

//��Ӫҵ��
void record_Total() {
	float sum = 0;
	for (Card* cur = AllCards->next; cur != NULL; cur = cur->next) {
		sum += cur->fTotalUse;
	}
	printf("�����ѽ�%.3f\n", sum);
}

//��Ӫҵ��
void record_Total_thisMonth() {
	float sum = 0;
	;
}

//��ӡ�Ʒ���Ϣ
void printRecord(BillNode* temp)
{
	char time[2][32] = { 0 };
	printf("%-18s%-10s%-10s%-27s%-27s\n\n", "����", "���ѽ��", "����״̬", "�ϻ�ʱ��", "�»�ʱ��");
	printf("%-18s%-10.3f", temp->data.aCardName, temp->data.fAmount);
	switch (temp->data.nStatus)
	{
	case 0:
		printf("%-10s", "δ����");
		break;
	case 1:
		printf("%-10s", "�ѽ���");
		break;
	}
	printf("%-27s%-27s\n",timePrintToStr(time[0], &temp->data.tSignIn),
		timePrintToStr(time[1], &temp->data.tSettle));
	printf("%s\n", "----------�ָ���----------");
}