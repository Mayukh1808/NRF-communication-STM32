/*
 * nrf.c
 *
 *  Created on: Dec 4, 2021
 *      Author: KIIT
 */
#include "stm32f1xx_hal.h"
#include "nrf.h"
// declaring the spi here as an extern

 extern SPI_HandleTypeDef hspi1;
#define CS_pin GPIO_PIN_10
#define CE_pin GPIO_PIN_11
//selecting the cs pin
void Cs_select(){
	HAL_GPIO_WritePin(GPIOA, CS_pin, GPIO_PIN_RESET);
}
//unselecting the CS pin
void Cs_unselect(){
	HAL_GPIO_WritePin(GPIOA, CS_pin, GPIO_PIN_SET);
}
//enable the CE pin
void CE_enable(){
	HAL_GPIO_WritePin(GPIOA, CE_pin, GPIO_PIN_SET);
}
// disable the CE pin
void CE_disable(){
	HAL_GPIO_WritePin(GPIOA, CE_pin, GPIO_PIN_RESET);
}
// write single byte to a particular register
void nrf_reg_single(uint8_t Reg, uint8_t data){
	uint8_t buff[2];
	buff[0] = Reg|1<<5;
	buff[1] = data;
	//pulling the CS pin between low to select the device
	Cs_select();
	HAL_SPI_Transmit(&hspi1, buff, 2, 1000);
	// pulling the CS pin high to release the decvice
	Cs_unselect();
}
//write multiple byte to a particular register
void nrf_reg_multiple(uint8_t Reg, uint8_t *data, int size){
	uint8_t buff[2];
	buff[0] = Reg|1<<5;
	//buff[1] = data;
	//pulling the CS pin between low to select the device
	Cs_select();
	HAL_SPI_Transmit(&hspi1, buff, 2, 100);
	HAL_SPI_Transmit(&hspi1, buff, size, 1000);
	// pulling the CS pin high to release the decvice
	Cs_unselect();
}

uint8_t read_nrf_reg_single(uint8_t Reg){
	uint8_t data=0;
	//pulling the CS pin between low to select the device
	Cs_select();
	HAL_SPI_Transmit(&hspi1, &Reg, 1, 100);
	HAL_SPI_Transmit(&hspi1, &data, 1, 100);
	// pulling the CS pin high to release the decvice
	Cs_unselect();
	return data;
}
void read_nrf_reg_multiple(uint8_t Reg, uint8_t *data, int size){
	//pulling the CS pin between low to select the device
	Cs_select();
	HAL_SPI_Transmit(&hspi1, &Reg, 1, 100);
	HAL_SPI_Transmit(&hspi1, data, size, 1000);
	// pulling the CS pin high to release the device
	Cs_unselect();
}
// send command to the NRF module
void sendcmd(uint8_t cmd){
	//pulling the CS pin between low to select the device
	Cs_select();
	HAL_SPI_Transmit(&hspi1, &cmd, 1, 100);
	// pulling the CS pin high to release the decvice
	Cs_unselect();
}
// Initialize the NRF module

void init_device(){
	//disabling the chip before configuring the device
	CE_disable();
	Cs_unselect();
	nrf_reg_single(CONFIG, 0);
	nrf_reg_single(EN_AA, 0);// no auto ACK
	nrf_reg_single(EN_RXADDR, 0); // not enabling any data pipe right now
	nrf_reg_single(SETUP_AW , 0x03); // 5 byte TX/RX address
	nrf_reg_single(SETUP_RETR  , 0); // no retransmission
	nrf_reg_single(RF_CH, 0);// will be set during TX or RX
	nrf_reg_single(RF_SETUP, 0x0E); //Power=0db data rate =2mbps
	nrf_reg_single(RF_SETUP, 0x0E);

}



