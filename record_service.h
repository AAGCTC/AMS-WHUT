#pragma once
//���ļ�����ʵ�֡���ѯͳ�ơ��� ҵ���߼��� �ķ���

//��ѯͳ�Ʒ���ӿ�
extern void Record_Serv_check();

//��ӡ�Ӳ˵�
void Menu_PrintMenu_rec();

//�����û����������
int Menu_GetCommend_rec();

//����inputNameKey�������һ������ָ�룬���˴�����Ҫ�жϿ�״̬���ʶ���һ���պ���
int cardStatus_rec(Card* theCard);

//�����Ų�ѯͳ��
void record_Card();

//��Ӫҵ��
void record_Total();

//��Ӫҵ��
void record_Total_thisMonth();

//��ӡ�Ʒ���Ϣ
void printRecord(BillNode* temp);