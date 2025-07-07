#pragma once
//���ļ�����ʵ�� ���������������ù��� �� ҵ���߼��� �ķ���
#include"model.h"
//��ӿ�����ӿ�
extern void CardService_newCard();
//��ѯ������ӿ�
extern void CardService_checkCard();
//ע��������ӿ�
extern void CardService_cancelCard();


//----------��ӿ���������----------
//ת��tools.c����ȥ��

//----------��ѯ����������----------
void printCardInfo(Card *card);
Card* findCard1(char* cName);
Card* findCard2(Card* head, char* cName);

//----------ע������������----------
int cardStatus_cancel(Card* theCard);
void printCancelInfo(Card* theCard, float temp_money);