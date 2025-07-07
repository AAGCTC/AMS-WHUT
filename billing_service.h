#pragma once
//本文件负责实现 “计费管理” 的 业务逻辑层 的服务
#include"model.h"
//上机服务接口
extern void Bill_Serv_logIn();
//下机服务接口
extern void Bill_Serv_logOut();
//打印上机信息
void printLoginInfo(LoginInfo* temp);
//打印计费信息
extern void printBillInfo(Card* temp);

//将消费记录（计费信息）写入bills链表中,并调用函数写入文件
void billservice_writeNewBill(Card* thecard);
//充值服务接口
extern void Card_Serv_rechargeCard();
//退费服务接口
extern void Card_Serv_drawMoneyFromCard();

//-------------（inputNameKey附属函数）-------------
// inputNameKey函数定义在tool.c内，声明在tool.h内
//判断卡状态是否可以下机,1表示正常,0表示异常
int cardStatus_settle(Card* theCard);
//判断卡状态是否可以充值,1表示正常,0表示异常
int cardStatus_charge(Card* theCard);
//判断卡状态是否可以退费,1表示正常,0表示异常
int cardStatus_draw(Card* theCard);