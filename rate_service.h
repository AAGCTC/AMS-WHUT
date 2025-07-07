#pragma once
//本文件负责实现“计费标准管理”的 业务逻辑层 的服务

#define RATE_PER_HOUR_1 8
#define RATE_PER_HOUR_2 5
#define DISCOUNT 0.98
#define HOUR 3600

//计算网费
extern float costCalculator(time_t howMuchTime);