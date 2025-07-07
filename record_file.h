#pragma once
//本文件负责实现“查询统计”的 数据存储层 的服务

#include"model.h"
//按卡号在文件中查找消费记录并构建链表
extern Bills readAllRecs_Card(Bills All_rec, Card* theCard);
//消费记录链表尾插
BillNode* append_rec(Bills head);
