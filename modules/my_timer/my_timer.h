/**
**************************************************
* @file my_timer.h
* @author Berkay Özgür, C. Arda Sengenc
* @version v1.0
* @date 19.06.2023
* @brief: Header file for the custom timer module.
**************************************************
*/
#ifndef MY_TIMER_MY_TIMER_H_
#define MY_TIMER_MY_TIMER_H_

/* In order to meet the requirements given on the paper, we are required to create an Interval Value between 1 to 10 */
#define MAX_INTERVAL(x) (x*(10-1))
#define CORE_CLOCK(x) ((SystemCoreClock / (10000*x)) - 1)


/* Public enums as shortcuts for instances */
typedef enum {
	TIMER_INSTANCE_1,
	TIMER_INSTANCE_2,
	TIMER_INSTANCE_3,
} timer_instance;

/* Public enums as shortcuts for mode bases */
typedef enum {
	TIMER_MODE_BASE,
	TIMER_MODE_PWM,
} timer_mode;

/* Public enums as shortcuts for output compare */
typedef enum {
	INSTANCE_PWM_1,
	INSTANCE_PWM_2,
} timer_oc_instance;

/* Public enums as shortcuts for output compare modes */
typedef enum {
	MODE_OC,
	MODE_PWM,
} timer_oc_mode;

/* Public functions (prototypes) */
void my_timer_init(timer_instance instance, timer_mode mode, uint32_t prescaler, uint32_t period);
void my_timer_oc_init(timer_oc_instance instance, timer_oc_mode mode, uint32_t pulse, uint32_t channel);
void my_timer_start(timer_oc_mode mode, uint32_t channel);
void my_timer_set_compare(uint32_t channel, uint32_t value);
void my_timer_set_prescaler(uint32_t value);
TIM_HandleTypeDef my_timer_get_struct();
TIM_OC_InitTypeDef my_timer_get_oc_struct();

#endif /* MY_TIMER_MY_TIMER_H_ */
