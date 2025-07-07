#pragma once
#include<time.h>
#include<stdio.h>
#include<string.h>
#include"model.h"
#include<stdlib.h>

//输入卡号密码，判断是否能下机
extern Card* inputNameKey(int (*cardStatus)(Card*));

//Is the Lenth of string Legal?(Only Check the Lenth)
extern int isLenthLegal(char* str, int lenMax);
//接受卡号并检查长度是否合法，合法后保存在传入的字符串中
extern int inputNameOfCard(char* cardName);
//Input Key of a NEW card and check
extern int inputKeyOfCard(char* cardKey);
extern char* timePrintToStr(char* str, time_t* time);
extern void timePrint(time_t* time);
//链表尾插(传入链表中任意节点即可）
extern Card* endPush(Card* head);
//Bills链表尾插
extern BillNode* endPush_Bills(Bills* head);
//链表释放
extern void linklistFree();
//链表尾删
extern Bills endDelete(Bills* head);