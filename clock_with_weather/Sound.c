#include "inc\tm4c123gh6pm.h"
#include "Sound.h"
#include <stdint.h>

#define GPIO_PORTD1 (*((volatile uint32_t *)0x40007008))

int cycle;

void Sound_Init() {
	
		/*port d1 has open already in switch.c*/
}

void startSound() {
	TIMER3_CTL_R = 0x00000001;
	
}

void stopSound() {
	TIMER3_CTL_R = 0x00000000;
	
}

void Timer3A_Handler(void) {
	TIMER3_ICR_R = TIMER_ICR_TATOCINT;
	cycle ^= 1;
	if(cycle) {
		GPIO_PORTD1 = 0x02;
		
	}else {
		GPIO_PORTD1 = 0x00;
	}
}

