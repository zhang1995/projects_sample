

#include "..\inc\tm4c123gh6pm.h"

#include "timer.h"
#include "Switch.h"
#include <stdint.h>
#include "driverlib\timer.c"
#include "sound.h"
//#include "Switch.h"
#define PF2             (*((volatile uint32_t *)0x40025010))
#define NVIC_EN0_INT19 0x00080000
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);


void Timer0A_Init(void) {
	  volatile uint32_t delay;
  
  // **** general initialization ****
  SYSCTL_RCGCTIMER_R |= 0x01;// activate timer0
  delay = SYSCTL_RCGCTIMER_R;          // allow time to finish activating
  TIMER0_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
  TIMER0_CFG_R = 0;                // configure for 32-bit timer mode
  // **** timer0A initialization ****
                                   // configure for periodic mode
  TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
  TIMER0_TAILR_R = 799999;        // start value for 100 Hz interrupts 80Mhz*.01s = 799,999us  . we subtract one for the reload cycle so 249,999
  TIMER0_IMR_R |= TIMER_IMR_TATOIM;// enable timeout (rollover) interrupt
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// clear timer0A timeout flag
  TIMER0_CTL_R |= TIMER_CTL_TAEN;  // enable timer0A 16-b, periodic, interrupts
  // **** interrupt initialization ****
                                   // Timer0A=priority 1
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x20000000; // top 3 bits
	TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0A_Handler);
  NVIC_EN0_R = NVIC_EN0_INT19;     // enable interrupt 19 in NVIC
}

void Timer2_Init(){
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = 49999999;    // 4) reload value 80Mhz * 1/60
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 3
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
	TimerIntRegister(TIMER2_BASE, TIMER_A, Timer2A_Handler);
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  //TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
}

void Timer2A_Handler(void) {
	TIMER2_ICR_R = TIMER_ICR_TATOCINT;
	/*PF2 ^= 0x04;
	seconds +=1;
	if(seconds >= 60){
		minutes += 1;
		seconds -= 60;
	}
	if(minutes >= 60) {
		minutes -= 60;
		hours=(hours+1)%24;
	}*/
	
}

void Timer3_Init() {
	SYSCTL_RCGCTIMER_R |= 0x08;   // 0) activate TIMER3
  TIMER3_CTL_R = 0x00000000;    // 1) disable TIMER3A during setup
  TIMER3_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER3_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER3_TAILR_R = 28408;    // 4) reload value 880Hz 2times (1136.36us/2*25-1)
  TIMER3_TAPR_R = 0;            // 5) bus clock resolution
  TIMER3_ICR_R = 0x00000001;    // 6) clear TIMER3A timeout flag
  TIMER3_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|0x40000000; // 8) priority 2
//interrupts enabled in the main program after all devices initialized
// vector number 51, interrupt number 35
	
	TimerIntRegister(TIMER3_BASE, TIMER_A, Timer3A_Handler);
  NVIC_EN1_R = 1<<(35-32);      // 9) enable IRQ 35 in NVIC
  TIMER3_CTL_R = 0x00000000;    // 10) enable TIMER3A
	
}


void Time_Init(void) {

	DisableInterrupts();
	//Timer0A_Init(); //check for switch press
	Timer2_Init();	//update per minutes
	//Timer3_Init();
	EnableInterrupts();
	
}


