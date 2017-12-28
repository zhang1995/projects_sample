

#include "Temp_Sensor.h"
#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>

#define GPIO_PORTE5 (*((volatile uint32_t *)0x40024080))


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
//int data[40];
long data = 0;
uint32_t i = 0;
uint32_t bit;
void Temp_Sensor_Init() {
	/*init a timer and port for this sensor*/
	//this will make pe5 input
	Temp_Setup_port();
	
}

void Temp_Setup_port() {
	/*Set PE5 as output at init*/
	SYSCTL_RCGCGPIO_R |= 0x10;
	GPIO_PORTE_DIR_R |= 0x20;      // 2) make PE5 output
  GPIO_PORTE_DEN_R |= 0x20;      // 4) disable digital I/O on PE4
	GPIO_PORTE_AMSEL_R &= ~0x20;
	GPIO_PORTE_AFSEL_R &= ~0x20;
	GPIO_PORTE5 = 0x20;
}

//MCU has received 40 bits data from AM2302 as
void Temp_get_result() {
	/*once enter this function, must finsihed this without any interrupts*/
	DisableInterrupts();
	data = 0;
	send_request();
	
	/*while(GPIO_PORTE5 == 0x00) {
		
	}
	
	while(GPIO_PORTE5 == 0x20) {}*/
	while(i <= 39) {
		//data[i] =() recieve_data();
		data = (data << 1) + recieve_data();
		i += 1;
    int delay = i;		
	}
	Temp_Setup_port();
	EnableInterrupts();
}

/*Step 1: MCU send out start signal to AM2302 
	and AM2302 send response signal to MCU*/
void send_request() {
	
	/*pull low data-bus and this process
	must beyond at least 1~10ms*/
	//in this case 5 ms
	uint32_t count = 57803;
	GPIO_PORTE5 = 0x00;
	/*5ms count*/
	while(count > 0) {
		count -= 1;
	}
	
	/*pull up 20~40us*/
	
	count = 439;
	GPIO_PORTE5 = 0x20;
	while(count > 0) {
		count -= 1;	
	}
	GPIO_PORTE_DIR_R &= ~0x20;
}



/*Step 2: AM2302 send data to MCU
When AM2302 is sending data to MCU, 
every bit's transmission begin with 
low-voltage-level that last 50us, the
following high-voltage-level signal's 
length decide the bit is "1" or "0".*/
uint32_t recieve_data() {
	/*data is 40 bit*/
	int count = 0;
	while(GPIO_PORTE5 != 0x20) {
				
	}
	
	while(GPIO_PORTE5 == 0x20) {
		count += 1;
	}
	if(count > 173) {
		return 1;
	}
		
	return 0;		

}

