//本文件负责实现 “主菜单” 的功能，属于表示层
#pragma once

//打印菜单
extern void Menu_PrintMenu();
//读入用户输入的命令
extern int Menu_GetCommend();
//表示层主循环，用户在此循环中完成多次交互，直到用户选择退出选项
extern void Menu_Main();
//释放链表
extern void linklistFree();