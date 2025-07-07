//本文件负责实现 “计费标准管理” 的 业务逻辑层 的服务
#include"service.h"

float costCalculator(time_t howMuchTime) {
	float cost = 0;
	if (howMuchTime < 60) {
		printf("\n%s\n", "上机时间小于1分钟，不收费");
		howMuchTime = 0;
	}
	cost = 1.0 * howMuchTime / HOUR * RATE_PER_HOUR_1 * DISCOUNT;
	//阶梯网费，上机越久越便宜
	if (howMuchTime > 3600) {
		cost = 1.0 * 3600 / HOUR * RATE_PER_HOUR_1 * DISCOUNT;
		cost += 1.0 * (howMuchTime - 3600) / HOUR * RATE_PER_HOUR_2 * DISCOUNT;
	}
	return cost;
}