#pragma once
//���ļ�����ʵ�� ���Ʒѹ��� �� ҵ���߼��� �ķ���
#include"model.h"
//�ϻ�����ӿ�
extern void Bill_Serv_logIn();
//�»�����ӿ�
extern void Bill_Serv_logOut();
//��ӡ�ϻ���Ϣ
void printLoginInfo(LoginInfo* temp);
//��ӡ�Ʒ���Ϣ
extern void printBillInfo(Card* temp);

//�����Ѽ�¼���Ʒ���Ϣ��д��bills������,�����ú���д���ļ�
void billservice_writeNewBill(Card* thecard);
//��ֵ����ӿ�
extern void Card_Serv_rechargeCard();
//�˷ѷ���ӿ�
extern void Card_Serv_drawMoneyFromCard();

//-------------��inputNameKey����������-------------
// inputNameKey����������tool.c�ڣ�������tool.h��
//�жϿ�״̬�Ƿ�����»�,1��ʾ����,0��ʾ�쳣
int cardStatus_settle(Card* theCard);
//�жϿ�״̬�Ƿ���Գ�ֵ,1��ʾ����,0��ʾ�쳣
int cardStatus_charge(Card* theCard);
//�жϿ�״̬�Ƿ�����˷�,1��ʾ����,0��ʾ�쳣
int cardStatus_draw(Card* theCard);