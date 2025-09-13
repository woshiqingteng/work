/**
 ****************************************************************************************************
 * @file        tpad.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-01-01
 * @brief       电容触摸按键(TPAD) 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20240101
 * 第一次发布
 *
 ****************************************************************************************************
 */


#ifndef TPAD_TPAD_H_
#define TPAD_TPAD_H_
#include "../../SYSTEM/sys/sys.h"

#define TPAD_TIMX_CAP_CHY                       TIM_CHANNEL_2 		/* 通道Y,  1<= Y <=4 */
/******************************************************************************************/
/* 触摸的门限值, 也就是必须大于 g_tpad_default_val + TPAD_GATE_VAL
 * 才认为是有效触摸, 改大 TPAD_GATE_VAL, 可以降低灵敏度, 反之, 则可以提高灵敏度
 * 根据实际需求, 选择合适的 TPAD_GATE_VAL 即可
 */
#define TPAD_GATE_VAL       100             	/* 触摸的门限值, 也就是必须大于 g_tpad_default_val + TPAD_GATE_VAL, 才认为是有效触摸 */
#define TPAD_ARR_MAX_VAL    0XFFFF          	/* 最大的ARR值, 一般设置为定时器的ARR最大值 */
extern volatile uint16_t 	g_tpad_default_val;	/* 空载的时候(没有手按下),计数器需要的时间 */


/* 接口函数, 可以在其他.c调用 */
uint8_t tpad_init(void);    		/* TPAD 初始化 函数 */
uint8_t tpad_scan(uint8_t mode);    /* TPAD 扫描 函数 */

#endif /* TPAD_TPAD_H_ */
