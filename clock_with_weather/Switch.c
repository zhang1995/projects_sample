
#include "../inc/tm4c123gh6pm.h"
#include "Switch.h"
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#define GPIO_PORTB0	(*((volatile uint32_t *)0x40005004))
#define GPIO_PORTB1	(*((volatile uint32_t *)0x40005008))
#define GPIO_PORTB2	(*((volatile uint32_t *)0x40005010))
#define GPIO_PORTB3	(*((volatile uint32_t *)0x40005020))
#define PF2             (*((volatile uint32_t *)0x40025010))
	
void Switch_Init(void) {
	volatile uint32_t delay;
	SYSCTL_RCGC2_R |= 0x0A; /*set port B, D, F, active*/
	delay = SYSCTL_RCGC2_R; //allow time for clock to start
	
	/*Tactile switch for port B*/
	/*set pb 0, 1,2,3 as input*/
	//GPIO_PORTB_DEN_R |= 0x0F;
	//GPIO_PORTB_DIR_R &= ~ 0x0F;
	/*disable analog*/
	//GPIO_PORTB_AMSEL_R &= ~0x0F;
	//GPIO_PORTB_AFSEL_R &= ~0x0F;
	/* // Interrupt setuü
    GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_4);        // Disable interrupt for PF4 (in case it was enabled)
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);      // Clear pending interrupts for PF4
    GPIOIntRegister(GPIO_PORTF_BASE, onButtonDown);     // Register our handler function for port F
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4,
        GPIO_FALLING_EDGE);             // Configure PF4 for falling edge trigger
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4);     // Configure PF4 for falling edge trigger

*/
	
	
	
	/*set to pull down interface*/
	GPIO_PORTB_PDR_R |= 0x0F;
	// Disable interrupt for PF4 (in case it was enabled)	
	/*GPIO_PORTB_IM_R &= ~0x0F;
	
	// Clear pending interrupts for PF4
	GPIO_PORTB_ICR_R |= 0x0F;

	// Register our handler function for port B
	//GPIOIntRegister(*GPIO_PORTB_DATA_BITS_R, onButton_press);
	//GPIOIntRegister(0x40005000, onButton_press);
	// Configure PF4 for falling edge trigger
	GPIO_PORTB_IEV_R &= ~0x0F;
	
	//Enable the interupt
	GPIO_PORTB_IM_R |= 0x0F;*/
	
	/*set pd1 for output*/
	GPIO_PORTD_DEN_R |= 0x02;
	GPIO_PORTD_DIR_R |= 0x02;
	GPIO_PORTD_AMSEL_R &= ~0x02;
	GPIO_PORTD_AFSEL_R &= ~0x02;
	
	/*Set PF1 PF2 for (implement later)*/
	/*GPIO_PORTF_DIR_R |= 0x04;             // make PF2, PF1 out (built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x04;          // disable alt funct on PF2, PF1
  GPIO_PORTF_DEN_R |= 0x04;             // enable digital I/O on PF2, PF1
                                        // configure PF2 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF00F)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;               // disable analog functionality on PF
  PF2 = 0;                
	*/
	//PF2=0;
}


void onButton_press(){
	
	printf("Hello World!");
	
	
}


void Timer0A_Handler() {
	TIMER0_ICR_R = TIMER_ICR_TATOCINT;
	/*inactive++;
	/*set clock*
	if(GPIO_PORTB0) {
		
		if(!flag[0]) {flag[0] = 3; }
		
		inactive = 0;
	} else {
		if(flag[0] == 3) {
			flag[0] = 1;
		}
		
	}
	
	/*set alarm*
	if(GPIO_PORTB1) {
		
		if(flag[1] == 0) {flag[1] = 3; }
		inactive = 0;
		
	}else if(flag[1] == 3) {
		
				flag[1] = 1;
	}
	
	/*change from normal and digital*
	
	if(GPIO_PORTB2) {
		if(flag[2] == 0) { flag[2] = 3; }
		inactive = 0;
	} else if(flag[2] == 3) {
		
		flag[2] = 1;

	}		
		
	if(GPIO_PORTB3) {
		if(flag[3] == 0) { flag[3] = 3; }
		inactive = 0;
	} else if(flag[3] == 3) {
		
		flag[3] = 1;
	}	
	
	if(flag[0]==2)
		flag[0]=0;		//set low
	if(flag[1]==2)
		flag[1]=0;
	if(flag[2]==2)
		flag[2]=0;
	if(flag[3]==2)
		flag[3]=0;*/

	
}
	









