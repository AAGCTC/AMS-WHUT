#pragma once
//���ļ�����ʵ�� ���Ʒѹ��� �� ���ݴ洢�� �ķ���

extern void billFile_writeLogin();
extern void LoginDataInit();
//׷��д���µļƷ���Ϣ
extern void billFile_writeBill(BillNode* cur);
//������ʷ�Ʒ���Ϣ�����»��ģ�
void billingDataInit();
//����ֵ��Ϣ�������ļ�
extern void billFile_writeChrage(Money cur);
//���˷���Ϣ�������ļ�
extern void billFile_writeDraw(Money cur);