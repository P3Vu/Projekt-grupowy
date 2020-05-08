#include "BME280.h"

extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef huart2;



void BME280_init(void)
{
	//uruchomienie interfejsu SPI czujnika
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
}

uint8_t read_BME280_ID(void)
{
	uint8_t BME280_ID_register = 0xD0;
	static uint8_t address[2] = {0x00,};
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi2, &BME280_ID_register, address, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);

	return address[1];
}

void write_to_register(uint8_t register_addr, uint8_t value)
{
	//odczyt zawartosci rejestru do ktorego bedzie zapis
	uint8_t receive[2] = {0x00,};
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi2, &register_addr, receive, 2, HAL_MAX_DELAY);
	while( hspi2.State == HAL_SPI_STATE_BUSY ) {};
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);

	uint8_t mask = 0x7F;//maska zapisywania
	register_addr &= mask;

	value |= receive[1];//wpisanie nowych danych bez zerowania wczesniej wpsanych

	uint8_t transmit[2] = {register_addr, value};
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, transmit, 2, HAL_MAX_DELAY);
	while( hspi2.State == HAL_SPI_STATE_BUSY ) {};
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
}

void read_hum_temp_press(void)
{
	uint8_t transmit[10] = {0xF7, };//0xF5 - 0xFE- rejestry przechowujace temp, cisnienie i wilgotnosc, wykorzystana autoinkrementacja
	static uint8_t address[9] = {0x00,};//elementy od 1 do 9 beda przechowywac pomiary

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi2, transmit, address, 9, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);

	HAL_UART_Transmit(&huart2, &address, 9, HAL_MAX_DELAY);
}

void read_register(uint8_t reg_addr)
{
	uint8_t transmit[2] = {reg_addr,};
	static uint8_t address[9] = {0x00,};

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi2, transmit, address, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);

	//HAL_UART_Transmit(&huart2, address, 2, HAL_MAX_DELAY);
}


