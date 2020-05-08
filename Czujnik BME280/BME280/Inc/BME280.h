#ifndef __BME280_H
#define __BME280_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

//int32_t t_fine = 20;//resolution of temperature

void BME280_init(void);
uint8_t read_BME280_ID(void);
void write_to_register(uint8_t register_addr, uint8_t value);
void read_hum_temp_press(void);
void read_register(uint8_t reg_addr);
int32_t BME280_compensate_T_int32(int32_t adc_T);

#ifdef __cplusplus
}
#endif

#endif /* __BME280_H */
