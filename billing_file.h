#pragma once
//本文件负责实现 “计费管理” 的 数据存储层 的服务

extern void billFile_writeLogin();
extern void LoginDataInit();
//追加写入新的计费信息
extern void billFile_writeBill(BillNode* cur);
//读入历史计费信息（已下机的）
void billingDataInit();
//将充值信息保存至文件
extern void billFile_writeChrage(Money cur);
//将退费信息保存至文件
extern void billFile_writeDraw(Money cur);