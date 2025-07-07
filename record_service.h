#pragma once
//本文件负责实现“查询统计”的 业务逻辑层 的服务

//查询统计服务接口
extern void Record_Serv_check();

//打印子菜单
void Menu_PrintMenu_rec();

//读入用户输入的命令
int Menu_GetCommend_rec();

//由于inputNameKey必须接受一个函数指针，但此处不需要判断卡状态，故定义一个空函数
int cardStatus_rec(Card* theCard);

//按卡号查询统计
void record_Card();

//总营业额
void record_Total();

//月营业额
void record_Total_thisMonth();

//打印计费信息
void printRecord(BillNode* temp);