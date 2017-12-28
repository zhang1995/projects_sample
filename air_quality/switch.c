/*this file will init the 3 switch*/

#include "switch.h"
#include "..//inc//tm4c123gh6pm.h"
#include <stdint.h>
#include "Temp_Sensor.h"
#include "Sound.h"
extern uint32_t safety_value;
	

void Switch_Init() {
	volatile unsigned long delay;
	//enable portE0-2 for botton clicked
	
	SYSCTL_RCGCGPIO_R |= 0x10; 
	while((SYSCTL_PRGPIO_R&0x10) == 0) {
	} 
	
	GPIO_PORTE_DEN_R |= 0x07;
	GPIO_PORTE_DIR_R &= ~0x07;
	/*disable analog*/
	GPIO_PORTE_AMSEL_R &= ~0x07;
	GPIO_PORTE_AFSEL_R &= ~0x07;
	/*set to pull down interface*/
	GPIO_PORTE_PDR_R |= 0x07;
	
	/*PE0-1 are edge sensitive*/
	GPIO_PORTE_IS_R &= ~0x07;
	GPIO_PORTE_IBE_R &= ~0x07;
	GPIO_PORTE_IEV_R &= ~0x07;
	
	/*clear the interrupt*/
	GPIO_PORTE_ICR_R = 0x07;
	
	/*enable*/
	GPIO_PORTE_IM_R |= 0x07;
	
	
	NVIC_PRI0_R = (NVIC_PRI1_R&0xFFFFFF00)|0x00000040; //PoRT E = priority 2
	NVIC_EN0_R |= 0x00000010;// enable interrupt 3 in NVIC
	
}

void GPIOPortE_Handler(void) {
	
	/*PE0*/
	if(GPIO_PORTE_RIS_R&0x01) {
		GPIO_PORTE_ICR_R = 0x01;
		safety_value += 10;
		safety_value %= 4000;
	}
	/*PE1*/
	if(GPIO_PORTE_RIS_R&0x02) {
		GPIO_PORTE_ICR_R = 0x02;
		safety_value -= 10;
	
	}
	/*PE2*/
	if(GPIO_PORTE_RIS_R&0x04) {
		GPIO_PORTE_ICR_R = 0x04;
		stopSound();
	
	}
}	




