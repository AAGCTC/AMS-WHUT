#pragma once
//���ļ����ڶ����������ݽṹ
#include<time.h>
#define DATATYPE int

//SettleInfo == Settle Information
typedef struct SettleInfo {
	char aCardName[24];//����,����Ϊ[1,18]
	time_t tLogin;//�ϻ�ʱ��
	time_t tSettle;//�»�ʱ��
	float fAmount;//���ѽ��
	float fBalance;//�������
}SettleInfo;

//LoginInfo == Log in Information
typedef struct LoginInfo {
	char aCardName[24];//�ϻ�����,����Ϊ[1,18]
	time_t tLogin;//�ϻ�ʱ��
	float fBalance;//�ϻ�ʱ�������
	SettleInfo* settle;//�»���Ϣ�ṹ��
}LoginInfo;

typedef struct Card {
	char aName[24];//����,����Ϊ[1,18]
	char aPwd[16];//����,����Ϊ[1,8]
	int nStatus;//��״̬��0-δ�ϻ���1-�����ϻ���2-��ע����3-ʧЧ��
	float fTotalUse;//�ۼƽ��
	int nUseCount;//ʹ�ô���
	float fBalance;//���
	int nDel;//ɾ����ʶ��0-δɾ����1-��ɾ����
	time_t tStart;//����ʱ��
	time_t tEnd;//������Ч��������Ч�ڽ�ֹʱ�䣩
	time_t tLast;//���ʹ��ʱ��
	struct Card *next;//nextָ��
	LoginInfo* Login;//�ϻ���Ϣ�ṹ�壨���ϻ��Ŀ����У�
}Card, *LinkList;

/*����AllCards���汣�����еĿ��������Ѵ��ڶ�̬����Card�ṹ��Ĵ洢��ʽ��
�������ַ�ʽ���Ų�����������*/
extern LinkList AllCards;

//һ�����ڳ�ʼ����Card����
extern Card StdCard;

//�Ʒ���Ϣ�ṹ��
typedef struct Billing {
	char aCardName[24];//����,����Ϊ[1,18]
	float fAmount;//���ѽ��
	int nStatus;//����״̬��0-δ���㣬1-�ѽ��㣩
	int nDel;//ɾ����ʶ��0-δɾ����1-��ɾ����
	time_t tSignIn;//�ϻ�ʱ��
	time_t tSettle;//�»�ʱ��
}Billing;

//�Ʒ���Ϣ����ڵ�
typedef struct BillNode {
	Billing data;
	struct BillNode* next;
}BillNode, *Bills;

//�Ʒ���Ϣ����
extern Bills* All_Bills;




typedef struct Money {
	char aCardName[18];//����,����Ϊ[1,18]
	int nStatus;//״̬��0-��ֵ��1-�˷ѣ�
	float fMoney;//��ֵ�˷ѽ��
	int nDel;//ɾ����ʶ��0-δɾ����1-��ɾ����
	time_t tTime;//��ֵ�˷Ѳ���������ʱ��
}Money;


//�������������������������һ�ζ�û�б����ù���
//AllCards�������еĿ���ʹ��ָ������+��̬����ṹ����������ٿռ��˷�
//extern Card* AllCards[1024];
//����������ȫ�ֱ�������һ���ṹ�壬���ڼ��й���
struct Global
{
	int cardID;//ȫ�ֱ���cardID��ʾ��AllCards�����У���һ�ſ���Ҫд���λ��(�±�)����0������������ӿ�������˽�г�Ա�����������޸ġ�
	int cardAmount;//ȫ�ֱ���cardAmount��ʾ��ע��Ŀ��ĸ���
	int havewrite;//��д�뿨���������ڱ궨��׷��д���Ŀ�ʼλ��
};

extern struct Global global;