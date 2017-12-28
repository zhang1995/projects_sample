#include "../inc/tm4c123gh6pm.h"

#include "Gas_Sensor.h"
#include "ADCSWTrigger.h"
#include "LED.h"
#include "Screen.h"
#include "Sound.h"
extern uint32_t safety_value;
void Gas_Sensor_Init() {
	/*init ADC sample*/
	ADC0_InitSWTriggerSeq3_Ch9();
	
	/*init a timer and port for this sensor*/
	Timer_Init_1(80000000);
	
	
}

void Timer_Init_1(uint32_t period) {

	SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER1_TAILR_R = period-1;    // 4) reload value
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER1A timeout flag
  TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x00008000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 37, interrupt number 21
  NVIC_EN0_R = 1<<21;           // 9) enable IRQ 21 in NVIC
	TIMER1_CTL_R = 0x00000001; // 10) enable TIMER1A
	
}

/*Timer Handler*/
void Timer1A_Handler(void){
	
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
	Gas_value = ADC0_InSeq3();
	
	//check if the value above safety
	if(Gas_value >= safety_value) {
		//play_sound
		startSound();
	}
	
	Screen_Update(Gas_value);
	LED_RedToggle();
	
}

