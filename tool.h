#pragma once
#include<time.h>
#include<stdio.h>
#include<string.h>
#include"model.h"
#include<stdlib.h>

//���뿨�����룬�ж��Ƿ����»�
extern Card* inputNameKey(int (*cardStatus)(Card*));

//Is the Lenth of string Legal?(Only Check the Lenth)
extern int isLenthLegal(char* str, int lenMax);
//���ܿ��Ų���鳤���Ƿ�Ϸ����Ϸ��󱣴��ڴ�����ַ�����
extern int inputNameOfCard(char* cardName);
//Input Key of a NEW card and check
extern int inputKeyOfCard(char* cardKey);
extern char* timePrintToStr(char* str, time_t* time);
extern void timePrint(time_t* time);
//����β��(��������������ڵ㼴�ɣ�
extern Card* endPush(Card* head);
//Bills����β��
extern BillNode* endPush_Bills(Bills* head);
//�����ͷ�
extern void linklistFree();
//����βɾ
extern Bills endDelete(Bills* head);