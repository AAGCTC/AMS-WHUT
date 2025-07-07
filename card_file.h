#pragma once
//本文件负责实现 “卡管理”、“费用管理” 的 数据存储层 的服务
#include"tool.h"
#include"model.h"

//将卡信息加载到AllCards
extern void cardDataInit();
//将文件中的信息加载到内存
extern void DataInit();
//保存
extern void DataSave();
//将时间用time_t格式另存一份
extern void cardFile_writeTS();
//将卡信息写入文件card.txt
extern void cardFile_writeInfo();
//读入timeStamp
extern void cardFile_readTS();
//将信息写入二进制文件
extern void cardFile_writeInfoBinary();
//从二进制文件读入信息
extern void cardFile_dataInit_B();