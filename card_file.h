#pragma once
//���ļ�����ʵ�� ���������������ù��� �� ���ݴ洢�� �ķ���
#include"tool.h"
#include"model.h"

//������Ϣ���ص�AllCards
extern void cardDataInit();
//���ļ��е���Ϣ���ص��ڴ�
extern void DataInit();
//����
extern void DataSave();
//��ʱ����time_t��ʽ���һ��
extern void cardFile_writeTS();
//������Ϣд���ļ�card.txt
extern void cardFile_writeInfo();
//����timeStamp
extern void cardFile_readTS();
//����Ϣд��������ļ�
extern void cardFile_writeInfoBinary();
//�Ӷ������ļ�������Ϣ
extern void cardFile_dataInit_B();