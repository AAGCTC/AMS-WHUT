#pragma once
//本文件负责实现 “卡管理”、“费用管理” 的 业务逻辑层 的服务
#include"model.h"
//添加卡服务接口
extern void CardService_newCard();
//查询卡服务接口
extern void CardService_checkCard();
//注销卡服务接口
extern void CardService_cancelCard();


//----------添加卡附属函数----------
//转到tools.c里面去了

//----------查询卡附属函数----------
void printCardInfo(Card *card);
Card* findCard1(char* cName);
Card* findCard2(Card* head, char* cName);

//----------注销卡附属函数----------
int cardStatus_cancel(Card* theCard);
void printCancelInfo(Card* theCard, float temp_money);