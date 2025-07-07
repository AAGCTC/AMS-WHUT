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

//���ܿ��Ų���鳤���Ƿ�Ϸ����Ϸ��󱣴��ڴ�����ַ�����
int inputNameOfCard(char* cardName) {
	char cName[128] = { 0 };//������ʱ��������Ŀ���
	int flag = 0;
	while (flag == 0)
	{
		printf("%s\n", "�����뿨�ţ�");
		scanf_s("%s", cName, 128);
		if (!isLenthLegal(cName, 18)) {
			printf("%s\n", "���󣡿��Ź�����");
			printf("%s", "(���������)");
			continue;
		}
		else {
			flag = 1;
			cName[18] = 0;//���Ѿ���֤����<=18����£��ضϺ���Ĳ���
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
		printf("%s\n", "����������(����1~8)��");
		scanf_s("%s", cKey, 128);
		if (!isLenthLegal(cKey, 8)) {
			printf("%s\n", "�������������");
			printf("%s", "(����������)");
			continue;
		}
		else {
			flag++;
		}
	}
	char cKeyCopy[128] = { 0 };
	while (flag < 2)
	{
		printf("%s\n", "��ȷ���������룺");
		scanf_s("%s", cKeyCopy, 128);
		if (!isLenthLegal(cKeyCopy, 8)) {
			printf("%s\n", "�������������");
			printf("%s", "(����������)");
			continue;
		}
		else if (strcmp(cKey, cKeyCopy) == 0) {
			cKey[8] = 0;//���Ѿ���֤����<=8����£��ضϺ���Ĳ���
			strcpy_s(cardKey, 8, cKey);
			flag++;
			return 1;
		}
		else
		{
			printf("%s\n", "�����������벻һ�£�");
			printf("%s", "(����������)");
			continue;
		}
	}
	return 1;
}

//��time_t���͵ĳ�Աת��Ϊ����ʱ�䲢����ַ���
char* timePrintToStr(char* str, time_t* time) {
	struct tm buffer;//struct tm�ĸ�����Ա�ֱ𱣴�������ʱ�������ڵȵ�
	localtime_s(&buffer, time);//�Ƚ�time_t����ת��Ϊstruct tm����
	asctime_s(str, 32 * sizeof(char), &buffer);//�ٰѽṹ���е���Ϣ�ϲ���һ���ַ���
	str[strlen(str)-1] = 0;
	return str;//��ȡ����ֵ��ʽ���Ǵ������ָ������޸ģ���Ϊ�˱���Ƕ��printf�ȵĲ����б��С�
}

//���뿨�����룬�ж��Ƿ����»�����ֵ���˷�
Card* inputNameKey(int (*cardStatus)(Card*)) {
	char temp_name[128] = { 0 };
	Card* theCard;
	do
	{
		inputNameOfCard(temp_name);
		theCard = findCard1(temp_name);
		if (theCard == NULL) {
			printf("%s\n", "δ�ҵ���Ӧ�������ٴ����룡");
		}
		else break;
	} while (1);
	printf("%s\n", "���������룺");
	char* keyofCard = (char*)malloc(128 * sizeof(char));
	int flag = 0;
	do {
		while (flag < 1)
		{
			scanf_s("%s", keyofCard, 128);
			if (!isLenthLegal(keyofCard, 8)) {
				printf("%s\n", "�������������");
				printf("%s", "(����������)");
				continue;
			}
			else {
				flag++;
			}
		}
		if (keyofCard != NULL) {
			if (strcmp(theCard->aPwd, keyofCard) == 0) break;
			else {
				printf("%s\n", "�������");
				return NULL;
			}
		}
		else {
			printf("%s\n", "�ڴ����ʧ�ܣ�");
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


//��time_t���ͳ�Աת��Ϊ����ʱ�䲢�������Ļ
void timePrint(time_t* time) {
	char temp[32] = { 0 };
	printf("%s",timePrintToStr(temp, time));
}
//���ַ�����ʽ�ı���ʱ��ת��time_t����##(��ʱ����)##
time_t strToTime_t(char* str) {
	struct tm time1 = { 0 };
	char temp[32] = { 0 };
	return 0;
}

//����β��(��������������ڵ㼴�ɣ�,�����²���Ľڵ�ָ��
Card* endPush(Card* head) {
	if (head == NULL) {
		printf("%s", "�������");
		return NULL;
	}
	Card* cur, * pre;
	for (pre = head; pre->next != NULL; pre = pre->next) {
		;
	}
	
	cur = (Card*)malloc(sizeof(Card));
	if (cur == NULL) {
		printf("�ڴ����ʧ�ܣ�\n");
		return NULL;
	}
	*cur = StdCard;
	pre->next = cur;
	return cur;
}

//�����ͷ�
void linklistFree() {
	Card* temp = NULL;
	for (Card* cur = AllCards; cur != NULL;) {
		temp = cur;
		cur = cur->next;
		free(temp);
	}
	AllCards = NULL;
}

//Bills����β��
BillNode* endPush_Bills(Bills* head) {
	BillNode* cur, * pre;
	if (head == NULL) {
		printf("%s", "bills����β�����");
		return;
	}
	pre = *head;
	cur = (BillNode*)malloc(sizeof(BillNode));
	if (cur == NULL) {
		printf("�ڴ����ʧ�ܣ�\n");
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

//����βɾ
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