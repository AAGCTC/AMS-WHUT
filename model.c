#include"model.h"
//һ�����ڳ�ʼ����Card����
Card StdCard = {
	{0},
	{0},
	0,0,0,0,0,
	0,0,0,
	NULL,NULL
};

//Card�ṹ�����飬ʹ��ָ������+��̬����ṹ����������ٿռ��˷�
//Card* AllCards[1024];


/*����AllCards���汣�����еĿ�*/
LinkList AllCards = NULL;
//���й������е�ȫ�ֱ���
struct Global global = { 0,0,0 };

//�Ʒ���Ϣ����
Bills* All_Bills = NULL;