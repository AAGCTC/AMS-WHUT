#pragma once
//本文件用于定义所需数据结构
#include<time.h>
#define DATATYPE int

//SettleInfo == Settle Information
typedef struct SettleInfo {
	char aCardName[24];//卡号,长度为[1,18]
	time_t tLogin;//上机时间
	time_t tSettle;//下机时间
	float fAmount;//消费金额
	float fBalance;//卡内余额
}SettleInfo;

//LoginInfo == Log in Information
typedef struct LoginInfo {
	char aCardName[24];//上机卡号,长度为[1,18]
	time_t tLogin;//上机时间
	float fBalance;//上机时卡内余额
	SettleInfo* settle;//下机信息结构体
}LoginInfo;

typedef struct Card {
	char aName[24];//卡号,长度为[1,18]
	char aPwd[16];//密码,长度为[1,8]
	int nStatus;//卡状态（0-未上机，1-正在上机，2-已注销，3-失效）
	float fTotalUse;//累计金额
	int nUseCount;//使用次数
	float fBalance;//余额
	int nDel;//删除标识（0-未删除，1-已删除）
	time_t tStart;//开卡时间
	time_t tEnd;//卡的有效期至（有效期截止时间）
	time_t tLast;//最后使用时间
	struct Card *next;//next指针
	LoginInfo* Login;//上机信息结构体（已上机的卡才有）
}Card, *LinkList;

/*链表AllCards保存保存所有的卡；由于已存在动态分配Card结构体的存储方式，
故用这种方式和遗产代码进行耦合*/
extern LinkList AllCards;

//一个用于初始化的Card变量
extern Card StdCard;

//计费信息结构体
typedef struct Billing {
	char aCardName[24];//卡号,长度为[1,18]
	float fAmount;//消费金额
	int nStatus;//消费状态（0-未结算，1-已结算）
	int nDel;//删除标识（0-未删除，1-已删除）
	time_t tSignIn;//上机时间
	time_t tSettle;//下机时间
}Billing;

//计费信息链表节点
typedef struct BillNode {
	Billing data;
	struct BillNode* next;
}BillNode, *Bills;

//计费信息链表
extern Bills* All_Bills;




typedef struct Money {
	char aCardName[18];//卡号,长度为[1,18]
	int nStatus;//状态（0-充值，1-退费）
	float fMoney;//充值退费金额
	int nDel;//删除标识（0-未删除，1-已删除）
	time_t tTime;//充值退费操作发生的时间
}Money;


//（已舍弃，在整个解决方案内一次都没有被引用过）
//AllCards保存所有的卡，使用指针数组+动态分配结构体变量来减少空间浪费
//extern Card* AllCards[1024];
//将其他所有全局变量放入一个结构体，便于集中管理
struct Global
{
	int cardID;//全局变量cardID表示在AllCards数组中，下一张卡将要写入的位置(下标)，从0递增；它是添加卡函数的私有成员，不得随意修改。
	int cardAmount;//全局变量cardAmount表示已注册的卡的个数
	int havewrite;//已写入卡数量，用于标定“追加写”的开始位置
};

extern struct Global global;