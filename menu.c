//���ļ�����ʵ�� �����˵��� �Ĺ��ܣ����ڱ�ʾ��
#include"menu.h"
#include"stdio.h"
#include<stdlib.h>
#include<string.h>
#include"service.h"

//��ʾ����ѭ�����û��ڴ�ѭ������ɶ�ν���
void Menu_Main() {
	int cmd = 0;
	do {
		Menu_PrintMenu();
		//��������
		DataSave();
		cmd = Menu_GetCommend();
		switch (cmd)
		{
		case 0:
			printf("%s\n", "-------�˳���......");
			DataSave();
			printf("%s\n", "�Ʒѹ���ϵͳ�������˳���");
			//linklistFree();
			exit(0);
			break;
		case 1:
			CardService_newCard();
			break;
		case 2:
			CardService_checkCard();
			break;
		case 3:
			Bill_Serv_logIn();
			break;
		case 4:
			Bill_Serv_logOut();
			break;
		case 5:
			Card_Serv_rechargeCard();
			break;
		case 6:
			Card_Serv_drawMoneyFromCard();
			break;
		case 7:
			Record_Serv_check();
			break;
		case 8:
			CardService_cancelCard();
			break;
		default:
			printf("%s\n", "����Ĳ˵���Ŵ���");
			continue;
		}
	} while (cmd != 0);
}

//��ӡ�˵�
void Menu_PrintMenu() {
	printf("\n%s\n", "��ӭ����Ʒѹ���ϵͳ");
	printf("\n%s\n", "-----------�˵�----------");
	printf("%s\n", "1.��ӿ�");
	printf("%s\n", "2.��ѯ��");
	printf("%s\n", "3.�ϻ�");
	printf("%s\n", "4.�»�");
	printf("%s\n", "5.��ֵ");
	printf("%s\n", "6.�˷�");
	printf("%s\n", "7.��ѯͳ��");
	printf("%s\n", "8.ע����");
	printf("%s\n", "0.�˳�");
	printf("%s\n", "��ѡ��˵���ţ�0~8����");
}

//�����û������������ܷǷ��������ʾ�û���������
int Menu_GetCommend() {
	int cmd = 0;
	char string[256] = { 0 };
	scanf_s("%s", string,256);
	if (strlen(string) > 1) return -1;
	else if ('0' <= string[0] && string[0] <= '9') {
		cmd = string[0] - '0';
		return cmd;
	}
	else return -1;
}
