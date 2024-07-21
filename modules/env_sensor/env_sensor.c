/**
 **************************************************
 * @file env_sensor.c
 * @author Berkay Özgür, C. Arda Sengenc
 * @version v1.0
 * @date 08.07.2023
 * @brief: env_sensor.c module for using microchip utilizing it with help of Bosch BME280 Sensor for
 * Temperature, Humidity and Pressure
 *
 @verbatim
 ==================================================
 ### Resources used ###
 GPIO: PB6: I2C1_SCL
       PB7: I2C1_SDA

 I2C: I2C1

 TIMER: Timer 2 for microseconds.

 Bosch BME280.

 ==================================================
 ### Usage ###

 (#) Call "env_sensor_init(void)" to initialize the module and all the necessary peripheries.

 (#) Call "env_sensor_get_value(env_sensor_t input)" in main-function's while-loop to read
     desired data from sensor. (For parameter, see env_sensor.h)


 @endverbatim
 **************************************************

 /*

 Environmental sensor BME280:
 The board uses maximum 3.5V as internal supply voltage. So you can connect VCC to 3.3V.

 If we should use the I2C1 then the pins: 		  PB6: I2C1_SCL
 PB7: I2C1_SDA

 CSB: (Chip Select) signal on the BME280 is an input signal used to select the chip.
 It is used to distinguish between different devices when multiple devices are connected
 to the same bus, such as when using I2C.

 *
 * 	   ADDR/SDO: Serial data output.
 *
 */

/* Includes */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include "bme280.h"
#include "utils.h"
#include "env_sensor.h"

/* Preprocessor macros */
#define DEVICE_ADDRESS 0x76

/* Module variables */
// Variable to store the temperature value obtained from the BME280 sensor.
float env_sensor_temperature;

// Variable to store the humidity value obtained from the BME280 sensor.
float env_sensor_humidity;

// Variable to store the pressure value obtained from the BME280 sensor.
float env_sensor_pressure;

// Variable to store the result or status of BME280 API functions.
int8_t env_sensor_rslt;

// Variable to store the period value used for delays or timing calculations.
uint32_t env_sensor_period;

/* Module structs  */
// Structure that represents the BME280 device.
struct bme280_dev env_sensor_dev;

// Structure to store the compensated sensor data (temperature, humidity, pressure) from the BME280.
struct bme280_data env_sensor_comp_data;

// Structure to store the BME280 sensor settings.
struct bme280_settings env_sensor_settings;

// I2C handle structure for the I2C communication interface.
I2C_HandleTypeDef env_sensor_hi2c1;

// Timer handle structure for the weather timer
TIM_HandleTypeDef env_sensor_tim_handle_struct_weather;

/* ### Static Functions ###

 (#) "env_sensor_timer_init" for Initializing the weather timer (TIM2) for precise counting.

 (#) "env_sensor_gpio_init" for Initializing the GPIO pins used for the I2C interface.

 (#) "env_sensor_i2c_init" for Initializing the I2C interface for communication.

 (#) "env_sensor_bosch_init" for Initializing the BME280 sensor.

 (#) "user_delay_us" for Delaying the execution for the specified microseconds.

 (#) "user_i2c_write" for Writing data to the sensor's registers through the I2C bus.

 (#) "user_i2c_read" for Reading data from the sensor's registers through the I2C bus.

 (#) "Error_Handler" for Error handling function.

 */
/* Static module functions */
void Error_Handler(void);
static void env_sensor_gpio_init(void);
static void env_sensor_timer_init(void);
static void user_delay_us(uint32_t period, void *intf_ptr);
static int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data,
		uint32_t len, void *intf_ptr);
static int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len,
		void *intf_ptr);
static void env_sensor_i2c_init(void);
static void env_sensor_bosch_init(void);


/**
 * @brief Initializes the environment sensor module.
 *
 * This function initializes the necessary components and configurations to use the environmental sensor.
 * It performs the following steps:
 *   1. Initializes the microcontroller HAL.
 *   2. Initializes the weather timer (TIM2) for precise counting.
 *   3. Initializes the GPIO pins used for the I2C interface.
 *   4. Initializes the I2C interface for communication.
 *   5. Initializes the BME280 sensor.
 *
 * @note This function should be called before using any other functions in the environment sensor module.
 *
 * @param none
 * @return none
 */
void env_sensor_init(void) {
	HAL_Init();

	env_sensor_timer_init();

	env_sensor_gpio_init();

	env_sensor_i2c_init();

	env_sensor_bosch_init();
}

/**
 * @brief This function sets up the I2C1 interface with the desired parameters for proper communication with external devices.
 *
 * @param none
 * @return none
 */
static void env_sensor_i2c_init(void) {
	__HAL_RCC_I2C1_CLK_ENABLE();

	env_sensor_hi2c1.Instance = I2C1;
	env_sensor_hi2c1.Init.ClockSpeed = 100000;
	env_sensor_hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	env_sensor_hi2c1.Init.OwnAddress1 = 0;
	env_sensor_hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	env_sensor_hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	env_sensor_hi2c1.Init.OwnAddress2 = 0;
	env_sensor_hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	env_sensor_hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if (HAL_I2C_Init(&env_sensor_hi2c1) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief This function initializes the BME280 sensor, configures its settings, and prepares it for
 *        continuous measurement in the recommended mode of operation.
 *
 * @param none
 * @return none
 */
static void env_sensor_bosch_init(void) {
	/*
	 * make sure to select BME280_I2C_ADDR_PRIM
	 * or BME280_I2C_ADDR_SEC as needed
	 */

	// Set the device address to BME280_I2C_ADDR_PRIM (0x76).
	uint8_t dev_addr = BME280_I2C_ADDR_PRIM;

	// Set the interface pointer to the device address.
	env_sensor_dev.intf_ptr = &dev_addr;

	//  Set the interface type to I2C.
	env_sensor_dev.intf = BME280_I2C_INTF;

	// Assign the user-defined I2C read function.
	env_sensor_dev.read = user_i2c_read;

	// Assign the user-defined I2C write function.
	env_sensor_dev.write = user_i2c_write;

	// Assign the user-defined delay function.
	env_sensor_dev.delay_us = user_delay_us;

	// Initialize the BME280 sensor with the provided device structure.
	env_sensor_rslt = bme280_init(&env_sensor_dev);

	uint8_t settings_sel;

	/* Recommended mode of operation: Indoor navigation */
	env_sensor_dev.settings.osr_h = BME280_OVERSAMPLING_1X;
	env_sensor_dev.settings.osr_p = BME280_OVERSAMPLING_16X;
	env_sensor_dev.settings.osr_t = BME280_OVERSAMPLING_2X;
	env_sensor_dev.settings.filter = BME280_FILTER_COEFF_16;
	env_sensor_dev.settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

	settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL
			| BME280_OSR_HUM_SEL | BME280_FILTER_SEL;

	// Set the sensor settings based on the selected configuration.
	env_sensor_rslt = bme280_set_sensor_settings(settings_sel, &env_sensor_dev);

	// Set the sensor mode to normal mode for continuous measurements.
	env_sensor_rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE,
			&env_sensor_dev);
}

/**
 * @brief This function initializes the GPIO pins (PB6 and PB7) for I2C communication (specifically for I2C1).
 * 		  The pins are configured as alternate function with open-drain output type, ensuring compatibility with I2C bus requirements.
 * 		  The GPIO pins are set to operate at a fast speed, and no pull-up or pull-down resistors are enabled.
 * 		  The function enables the clock for GPIOB and initializes the GPIO pins accordingly.
 *
 * @param none
 * @return none
 */
static void env_sensor_gpio_init(void) {
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init_b;
	/* If we are to use the I2C1 then the pins:		  PB6: I2C1_SCL
	 * 	   											  PB7: I2C1_SDA */
	gpio_init_b.Pin = GPIO_PIN_6;
	gpio_init_b.Mode = GPIO_MODE_AF_OD;
	gpio_init_b.Speed = GPIO_SPEED_FAST;
	gpio_init_b.Alternate = GPIO_AF4_I2C1;
	HAL_GPIO_Init(GPIOB, &gpio_init_b);

	gpio_init_b.Pin = GPIO_PIN_7;
	gpio_init_b.Mode = GPIO_MODE_AF_OD;
	gpio_init_b.Speed = GPIO_SPEED_FAST;
	gpio_init_b.Alternate = GPIO_AF4_I2C1;
	HAL_GPIO_Init(GPIOB, &gpio_init_b);
}

/**
 * @brief Retrieves the value of a specific environmental parameter from the BME280 sensor.
 *
 * @param input The type of value to be retrieved (temperature, humidity, or pressure).
 * @return The measured value of the specified environmental parameter.
 *
 */
float env_sensor_get_value(env_sensor_t input) {
	float env_sensor_output = 0;

	env_sensor_dev.delay_us(70, env_sensor_dev.intf_ptr);
	/* Get Data */
	env_sensor_rslt = bme280_get_sensor_data(BME280_ALL, &env_sensor_comp_data,
			&env_sensor_dev);
	if (env_sensor_rslt == BME280_OK) {
		switch (input) {
		case ENV_TEMPERATURE:
			env_sensor_output = env_sensor_comp_data.temperature;
			break;

		case ENV_HUMIDITY:
			env_sensor_output = env_sensor_comp_data.humidity;
			break;

		case ENV_PRESSURE:
			env_sensor_output = env_sensor_comp_data.pressure;
			break;
		}
	}
	return env_sensor_output;

}

/**
 * @brief Writes data to the sensor's registers through the I2C bus.
 * @param reg_addr The register address to write to.
 * @param  A pointer to the data buffer containing the data to be written.
 * @param len Length of the data buffer.
 * @param intf_ptr Pointer to the interface structure.
 * @return 0 on success, -1 on failure.
 *
 * The code uses the HAL library function HAL_I2C_Mem_Write to perform the I2C write operation.
 * The parameters passed to this function are:
 1) &hi2c1: The I2C handle corresponding to the I2C peripheral being used (hi2c1 in this case).
 2) (DEVICE_ADDRESS << 1): The 7-bit device address shifted left by 1 bit to make room for the R/W bit.
 The device address is determined as DEVICE_ADDRESS (0x76) shifted left by 1 bit.
 3) reg_addr: The register address to write to.
 4) 1: The memory address size in bytes, which is set to 1 byte.
 5) data: A pointer to the data buffer containing the data to be written.
 6) len: The length of the data buffer.
 7)  70: The timeout value in milliseconds for the write operation. (for normal mode)
 */
static int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data,
		uint32_t len, void *intf_ptr) {

	/* @DevAddress is 0x76, because we have connected the SDO(ADDR) pin to GND.
	 * @MemAddress is the address of internal memory of the device, we are reading from.
	 * @MemAddSize is always 1 byte-size.
	 * @pData is the address of our buffer, where the result will be stored     HAL_MAX_DELAY
	 * @Size is always 1, because the target size is also 1 byte.*/
	if (HAL_I2C_Mem_Write(&env_sensor_hi2c1, (DEVICE_ADDRESS << 1), reg_addr, 1,
			data, len, 70) != HAL_OK)
		return -1;

	return BME280_OK;
}

/**
 * @brief Reads data from the sensor's registers through the I2C bus.
 * @param reg_addr The register address to read from.
 * @param data A pointer to the data buffer where the read data will be stored.
 * @param len Length of the data buffer.
 * @param intf_ptr Pointer to the interface structure.
 * @return 0 on success, -1 on failure.
 *
 *
 *	The code uses the HAL library function HAL_I2C_Mem_Read to perform the I2C read operation.
 *	The The parameters passed to this function are:
 1) &hi2c1: The I2C handle corresponding to the I2C peripheral being used (hi2c1 in this case).
 2) (DEVICE_ADDRESS << 1) | 0x01: The 7-bit device address shifted left by 1 bit to make room for the R/W bit,
 with the LSB (R/W bit) set to 1 to indicate a read operation. The device address is determined as
 DEVICE_ADDRESS (0x76) shifted left by 1 bit, and the R/W bit is set to 1.
 3) reg_addr: The register address to read from.
 4) 1: The memory address size in bytes, which is set to 1 byte.
 5) data: A pointer to the data buffer where the read data will be stored.
 6) len: The length of the data buffer.
 7) 70: The timeout value in milliseconds for the read operation (for normal mode)
 */
static int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len,
		void *intf_ptr) {

	if (HAL_I2C_Mem_Read(&env_sensor_hi2c1, (DEVICE_ADDRESS << 1) | 0x01,
			reg_addr, 1, data, len, 70) != HAL_OK)
		return -1;

	return BME280_OK;
}

/**
 * @brief Delays the execution for the specified microseconds.
 * @param period The delay period in microseconds.
 * @param intf_ptr Pointer to the interface structure
 * @note This function introduces a delay in the execution by waiting for the specified number of microseconds.
 *
 * This function provides a microsecond delay by utilizing the TIM2 timer peripheral.
 * It starts the timer, enters a loop until the specified period in microseconds is reached, stops the timer,
 * and resets its count value. The function enables precise timing by utilizing the hardware timer.
 *
 * @return none
 */
static void user_delay_us(uint32_t period, void *intf_ptr) {
	HAL_TIM_Base_Start(&env_sensor_tim_handle_struct_weather);

	while (__HAL_TIM_GET_COUNTER(&env_sensor_tim_handle_struct_weather) < period) {
	}
	HAL_TIM_Base_Stop(&env_sensor_tim_handle_struct_weather);
	__HAL_TIM_SET_COUNTER(&env_sensor_tim_handle_struct_weather, 0);
}

/**
 * @brief Handles any error occurrence.
 * @note This function is executed in case of an error, and the user can add their own implementation if needed.
 *
 * @param none
 * @return none
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

/**
 * @brief Initializes the weather timer (TIM2) for precise counting.
 * @note This function configures and initializes the weather timer (TIM2) for precise timekeeping and counting.
 *
 * @param none
 * @return none
 */
static void env_sensor_timer_init(void) {
	__HAL_RCC_TIM2_CLK_ENABLE();

	env_sensor_tim_handle_struct_weather.Instance = TIM2;

	env_sensor_tim_handle_struct_weather.Init.Prescaler = (SystemCoreClock / 2
			/ 1000000) - 1;

	env_sensor_tim_handle_struct_weather.Init.Period = 10000000 - 1;

	env_sensor_tim_handle_struct_weather.Init.CounterMode = TIM_COUNTERMODE_UP;
	env_sensor_tim_handle_struct_weather.Init.ClockDivision =
	TIM_CLOCKDIVISION_DIV1;
	env_sensor_tim_handle_struct_weather.Init.AutoReloadPreload =
	TIM_AUTORELOAD_PRELOAD_DISABLE;

	env_sensor_tim_handle_struct_weather.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&env_sensor_tim_handle_struct_weather);
}

//void env_sensor_can_init(void) {
//	CAN_HandleTypeDef can_handle_struct;
//	can_handle_struct.Instance = CAN1;
//	can_handle_struct.Init.Prescaler = 21;
//	can_handle_struct.Init.SyncJumpWidth = CAN_SJW_1TQ;
//	can_handle_struct.Init.TimeSeg1 = CAN_BS1_13TQ;
//	can_handle_struct.Init.TimeSeg2 = CAN_BS2_2TQ;
//
//	CAN_FilterTypeDef can_filter_config;
//	can_filter_config.FilterActivation = CAN_FILTER_ENABLE;
//	can_filter_config.FilterBank = 0;
//	can_filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO0;
//	can_filter_config.FilterScale = CAN_FILTERSCALE_32BIT;
//	can_filter_config.FilterMode = CAN_FILTERMODE_IDMASK;
//	can_filter_config.FilterIdHigh = XXXX;
//	can_filter_config.FilterIdLow = XXXX;
//	can_filter_config.FilterMaskIdHigh = XXXX;
//	can_filter_config.FilterMaskIdLow = XXXX;
//
//	HAL_CAN_ConfigFilter(&can_handle_struct, &can_filter_config);
//}
