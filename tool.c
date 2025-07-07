#include"tool.h"
extern Card* findCard1(char* str2);

//Is the Lenth of string Legal?(Only Check the Lenth)
int isLenthLegal(char* str, int lenMax) {
	int len = strlen(str);
	if (len > lenMax) {
		return 0;
	}
	else
	{
		return 1;
	}
}

//接受卡号并检查长度是否合法，合法后保存在传入的字符串中
int inputNameOfCard(char* cardName) {
	char cName[128] = { 0 };//用于临时保存输入的卡号
	int flag = 0;
	while (flag == 0)
	{
		printf("%s\n", "请输入卡号：");
		scanf_s("%s", cName, 128);
		if (!isLenthLegal(cName, 18)) {
			printf("%s\n", "错误！卡号过长！");
			printf("%s", "(请从新输入)");
			continue;
		}
		else {
			flag = 1;
			cName[18] = 0;//在已经保证长度<=18情况下，截断后面的部分
			strcpy_s(cardName, 18, cName);
		}
	}
	return 1;
}

//Input Key of a NEW card and check
int inputKeyOfCard(char* cardKey) {
	char cKey[128] = { 0 };
	int flag = 0;
	while (flag < 1)
	{
		printf("%s\n", "请设置密码(长度1~8)：");
		scanf_s("%s", cKey, 128);
		if (!isLenthLegal(cKey, 8)) {
			printf("%s\n", "错误！密码过长！");
			printf("%s", "(请重新输入)");
			continue;
		}
		else {
			flag++;
		}
	}
	char cKeyCopy[128] = { 0 };
	while (flag < 2)
	{
		printf("%s\n", "请确认您的密码：");
		scanf_s("%s", cKeyCopy, 128);
		if (!isLenthLegal(cKeyCopy, 8)) {
			printf("%s\n", "错误！密码过长！");
			printf("%s", "(请重新输入)");
			continue;
		}
		else if (strcmp(cKey, cKeyCopy) == 0) {
			cKey[8] = 0;//在已经保证长度<=8情况下，截断后面的部分
			strcpy_s(cardKey, 8, cKey);
			flag++;
			return 1;
		}
		else
		{
			printf("%s\n", "两次输入密码不一致！");
			printf("%s", "(请重新输入)");
			continue;
		}
	}
	return 1;
}

//将time_t类型的成员转换为本地时间并输出字符串
char* timePrintToStr(char* str, time_t* time) {
	struct tm buffer;//struct tm的各个成员分别保存年月日时分秒星期等等
	localtime_s(&buffer, time);//先将time_t类型转换为struct tm类型
	asctime_s(str, 32 * sizeof(char), &buffer);//再把结构体中的信息合并成一个字符串
	str[strlen(str)-1] = 0;
	return str;//采取返回值形式而非传入二级指针进行修改，是为了便于嵌入printf等的参数列表中。
}

//输入卡号密码，判断是否能下机、充值、退费
Card* inputNameKey(int (*cardStatus)(Card*)) {
	char temp_name[128] = { 0 };
	Card* theCard;
	do
	{
		inputNameOfCard(temp_name);
		theCard = findCard1(temp_name);
		if (theCard == NULL) {
			printf("%s\n", "未找到对应卡！请再次输入！");
		}
		else break;
	} while (1);
	printf("%s\n", "请输入密码：");
	char* keyofCard = (char*)malloc(128 * sizeof(char));
	int flag = 0;
	do {
		while (flag < 1)
		{
			scanf_s("%s", keyofCard, 128);
			if (!isLenthLegal(keyofCard, 8)) {
				printf("%s\n", "错误！密码过长！");
				printf("%s", "(请重新输入)");
				continue;
			}
			else {
				flag++;
			}
		}
		if (keyofCard != NULL) {
			if (strcmp(theCard->aPwd, keyofCard) == 0) break;
			else {
				printf("%s\n", "密码错误！");
				return NULL;
			}
		}
		else {
			printf("%s\n", "内存访问失败！");
			return NULL;
		}
	} while (1);
	switch (cardStatus(theCard))
	{
	case 0: return NULL;
	case 1:break;
	default:
		return NULL;
	}
	return theCard;
}


//将time_t类型成员转换为本地时间并输出到屏幕
void timePrint(time_t* time) {
	char temp[32] = { 0 };
	printf("%s",timePrintToStr(temp, time));
}
//将字符串形式的本地时间转回time_t类型##(暂时弃用)##
time_t strToTime_t(char* str) {
	struct tm time1 = { 0 };
	char temp[32] = { 0 };
	return 0;
}

//链表尾插(传入链表中任意节点即可）,返回新插入的节点指针
Card* endPush(Card* head) {
	if (head == NULL) {
		printf("%s", "链表错误！");
		return NULL;
	}
	Card* cur, * pre;
	for (pre = head; pre->next != NULL; pre = pre->next) {
		;
	}
	
	cur = (Card*)malloc(sizeof(Card));
	if (cur == NULL) {
		printf("内存分配失败！\n");
		return NULL;
	}
	*cur = StdCard;
	pre->next = cur;
	return cur;
}

//链表释放
void linklistFree() {
	Card* temp = NULL;
	for (Card* cur = AllCards; cur != NULL;) {
		temp = cur;
		cur = cur->next;
		free(temp);
	}
	AllCards = NULL;
}

//Bills链表尾插
BillNode* endPush_Bills(Bills* head) {
	BillNode* cur, * pre;
	if (head == NULL) {
		printf("%s", "bills链表尾插错误！");
		return;
	}
	pre = *head;
	cur = (BillNode*)malloc(sizeof(BillNode));
	if (cur == NULL) {
		printf("内存分配失败！\n");
		return NULL;
	}
	if (*head != NULL) {
		for (; pre->next != NULL; pre = pre->next) {
			;
		}
		cur->next = NULL;
		pre->next = cur;
	}
	else {
		cur->next = NULL;
		*head = cur;
	}
	return cur;
}

//链表尾删
Bills endDelete(Bills* head) {
	if (*head == NULL) {
		return *head;
	}
	BillNode* p = *head;
	if (p->next == NULL) {
		*head = NULL;
		free(p);
		return *head;
	}
	BillNode* pre = p;
	for (; p->next != NULL; p = p->next) {
		pre=p;
	}
	pre->next = NULL;
	free(p);
	return *head;
}