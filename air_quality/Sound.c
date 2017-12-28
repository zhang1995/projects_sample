#include "inc\tm4c123gh6pm.h"
#include "Sound.h"
#include <stdint.h>


#define GPIO_PORTD1 (*((volatile uint32_t *)0x40007008))

int cycle;

void Sound_Init() {
	
	volatile uint32_t delay;
	SYSCTL_RCGC2_R |= 0x08; /*set portD active*/
	delay = SYSCTL_RCGC2_R; //allow time for clock to start
	
	/*set pd1 for output*/
	GPIO_PORTD_DEN_R |= 0x02;
	GPIO_PORTD_DIR_R |= 0x02;
	GPIO_PORTD_AMSEL_R &= ~0x02;
	GPIO_PORTD_AFSEL_R &= ~0x02;
	
	Timer3_Init();
}

void startSound() {
	TIMER3_CTL_R = 0x00000001;
	
}

void stopSound() {
	TIMER3_CTL_R = 0x00000000;
	
}

void Timer3_Init() {
	SYSCTL_RCGCTIMER_R |= 0x08;   // 0) activate TIMER3
  TIMER3_CTL_R = 0x00000000;    // 1) disable TIMER3A during setup
  TIMER3_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER3_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER3_TAILR_R = 45453;    // 4) reload value 880Hz 2times (1136.36us/2*25-1)
  TIMER3_TAPR_R = 0;            // 5) bus clock resolution
  TIMER3_ICR_R = 0x00000001;    // 6) clear TIMER3A timeout flag
  TIMER3_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|0x40000000; // 8) priority 2
// interrupts enabled in the main program after all devices initialized
// vector number 51, interrupt number 35
  NVIC_EN1_R = 1<<(35-32);      // 9) enable IRQ 35 in NVIC
  TIMER3_CTL_R = 0x00000000;    // 10) enable TIMER3A
	
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


