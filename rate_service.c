//���ļ�����ʵ�� ���Ʒѱ�׼���� �� ҵ���߼��� �ķ���
#include"service.h"

float costCalculator(time_t howMuchTime) {
	float cost = 0;
	if (howMuchTime < 60) {
		printf("\n%s\n", "�ϻ�ʱ��С��1���ӣ����շ�");
		howMuchTime = 0;
	}
	cost = 1.0 * howMuchTime / HOUR * RATE_PER_HOUR_1 * DISCOUNT;
	//�������ѣ��ϻ�Խ��Խ����
	if (howMuchTime > 3600) {
		cost = 1.0 * 3600 / HOUR * RATE_PER_HOUR_1 * DISCOUNT;
		cost += 1.0 * (howMuchTime - 3600) / HOUR * RATE_PER_HOUR_2 * DISCOUNT;
	}
	return cost;
}