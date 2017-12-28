#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "Switch.h"
#include "timer.h"
#include "screen.h"
#include "Sound.h"
#include "LED.h"
#include <stdlib.h>
#include "clock.h"

int flag[7];
int inactive;
int alarmmin, alarmhr;

int setmin, sethr;

int prevmin, prevhr;


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
void (*Function)(void);
void update(void);
void drawDigital(int min, int hr);
void drawHands(uint16_t min, uint16_t hr, uint16_t mincolor, uint16_t hrcolor);
int getSine(int deg);
void draw_Temp(void);
/*state functions*/



/*set function*/
void setAlarm () {
	setmin = alarmmin;
	sethr = alarmhr;
	
	flag[4] = 1;
}

void setTime() {
	setmin = minutes;
	sethr = hours;
	
	flag[4] = 1;
	
}

void setHourUp() {
	sethr = (sethr+1)%24;
	
	flag[4] = 1;
	

}

void setMinuteUp() {
	setmin = (setmin+1)%60;
	flag[4] = 1;
	

}

void placeHolder(void){}

/*toogle function*/
	
void toggleAlarm() {
	
	flag[5] ^= 1;
	
	flag[4] = 1;
	
}

void toggleWeather() {
	flag[4] = 1;
	flag[6] ^= 1;
	
}

/*confirm function*/
void confirmAlarm() {
	alarmhr = sethr;
	alarmmin = setmin;
	flag[4] = 1;
}

void confirmTime() {
	hours = sethr;
	minutes = setmin;
	flag[4] = 1;
}

/*create a state*/
typedef struct {
	
	int State_num;
	int next_state[4];
	void (*actions[4]) (void); /*corresponsing fucntions*/
	

}clockStates;

clockStates FSM[] = {
	/*display clock*/
	{0x00, {4,8,1,99}, {setAlarm, setTime, toggleAlarm, toggleWeather}}, //normal watch
	{0x10, {5,9,0,99}, {setAlarm, setTime, toggleAlarm, toggleWeather}},	//noraml watch with alarm
	{0x01, {6,10,3,99}, {setAlarm, setTime, toggleAlarm, toggleWeather}},	//digital watch face
	{0x11, {7,11,2,99}, {setAlarm, setTime, toggleAlarm, toggleWeather}}, //digital watch face, alarm
	
	
	
	/*{0x00, {4,8,1,2}, {setAlarm, setTime, toggleAlarm, toggleDisplay}}, //normal watch
	{0x10, {5,9,0,3}, {setAlarm, setTime, toggleAlarm, toggleDisplay}},	//noraml watch with alarm
	{0x01, {6,10,3,0}, {setAlarm, setTime, toggleAlarm, toggleDisplay}},	//digital watch face
	{0x11, {7,11,2,1}, {setAlarm, setTime, toggleAlarm, toggleDisplay}}, //digital watch face, alarm*/
	
	
	
	
	/*set alarm
	{0x02, {99,0,99,99}, {setHourUp,confirmAlarm,placeHolder,setMinuteUp}}, //setalarm. normal watch face
	{0x12, {99,1,99,99}, {setHourUp,confirmAlarm,placeHolder,setMinuteUp}},	//setalarm. normal watch face, alarm
	{0x02, {99,2,99,99}, {setHourUp,confirmAlarm,placeHolder,setMinuteUp}}, //setalarm. digital watch face
	{0x12, {99,3,99,99}, {setHourUp,confirmAlarm,placeHolder,setMinuteUp}},	//setalarm. digital watch face, alarm*/
	
	
	{0x02, {99,2,99,99}, {setHourUp,confirmAlarm,placeHolder,setMinuteUp}}, //setalarm. normal watch face
	{0x12, {99,3,99,99}, {setHourUp,confirmAlarm,placeHolder,setMinuteUp}},	//setalarm. normal watch face, alarm
	{0x02, {99,2,99,99}, {setHourUp,confirmAlarm,placeHolder,setMinuteUp}}, //setalarm. digital watch face
	{0x12, {99,3,99,99}, {setHourUp,confirmAlarm,placeHolder,setMinuteUp}},	//setalarm. digital watch face, alarm
		
	/*set time
	{0x03, {99,0,99,99}, {setHourUp,confirmTime,placeHolder,setMinuteUp}}, //set clock, noraml watch
	{0x13, {99,1,99,99}, {setHourUp,confirmTime,placeHolder,setMinuteUp}}, //set clock, noraml watch, alarm
	{0x03, {99,2,99,99}, {setHourUp,confirmTime,placeHolder,setMinuteUp}}, //set cloack, digital
	{0x13, {99,3,99,99}, {setHourUp,confirmTime,placeHolder,setMinuteUp}}, //set clock, digital watch, alarm*/
	
	
	
	/*set time*/
	{0x03, {99,2,99,99}, {setHourUp,confirmTime,placeHolder,setMinuteUp}}, //set clock, noraml watch
	{0x13, {99,3,99,99}, {setHourUp,confirmTime,placeHolder,setMinuteUp}}, //set clock, noraml watch, alarm
	{0x03, {99,2,99,99}, {setHourUp,confirmTime,placeHolder,setMinuteUp}}, //set cloack, digital
	{0x13, {99,3,99,99}, {setHourUp,confirmTime,placeHolder,setMinuteUp}}, //set clock, digital watch, alarm
};



int currentState;
#define GPIO_PORTB0	(*(volatile uint32_t *)0x40005004)

void Clock_Init(){
	
	/*init of the port*/
	//Switch_Init();
	//Lcd_Init();
	//while(GPIO_PORTB0 == 0){}
	Sound_Init();
	//Time_Init();
	/**/
	currentState= 2;
	flag[6] = 1;
	flag[5] = 1;
}


void Clock_wait() {
	flag[4] = 1;
	while(Board_Input()==0){
		for(int i = 0; i <= 3; i+=1) {
			
			
			if(flag[i]==1) {
				//flag[5] = 2;
				
				Function = *(FSM[currentState].actions)[i];
				Function();
				
				if(FSM[currentState].next_state[i] != 99) {
					currentState = FSM[currentState].next_state[i];	
				}
				flag[i]=2;
				
			}
			
		}
		
		/*in case no button pressed refresh ui*/
		/*if(inactive >= 50*5) {
			flag[4] = 1;
			inactive = 0;
			
			if(currentState > 3) {
				currentState = FSM[currentState].next_state[1];
				
			}	
		}*/
		update();
		
	}
}


void update() {
	
	if(minutes != prevmin || hours!= prevhr || flag[4]) {
			
		int display = FSM[currentState].State_num&0x0F;
		int alarm = FSM[currentState].State_num&0x10;
		
		if(display == 1) {
			drawDigital(minutes, hours);
			prevmin = minutes;
			prevhr = hours;
			if(flag[6] == 1) {
				
				draw_Temp();
			}
			
		} else {
			drawDigital(setmin, sethr);
			
		}
		
		if(alarm) {
			char set_alarm[] = "Set Alarm";
			ST7735_DrawString(0,1, set_alarm, 0xF81F);
			
		}
		
		if(display >= 2) {
			char set_time[] = "Set  Time";
			ST7735_DrawString(0, 1, set_time, 0xF81F);
		
		}
		
		flag[4] = 0;
		
		
	}
	
	if(hours == alarmhr && minutes==alarmmin && !flag[5]) {
		startSound();
		//flag[5] = 1;
	} else {
		stopSound();
		flag[5] = 0;
		
	}
	
	
}



void drawDigital(int min, int hr) {
	ST7735_FillScreen(0x0000);
	
	char c = (hr/10) + 48;
	ST7735_DrawChar(30, 80, c, 0x07FF, 0x0000, 3);
	
	c = (hr%10) + 48;
	ST7735_DrawChar(47, 80, c, 0x07FF, 0x0000, 3);
	
	c = ':';
	ST7735_DrawChar(62, 80, c, 0x07FF, 0x0000, 3);
	
	c = (min/10)+48;
	ST7735_DrawChar(77, 80, c, 0x07FF, 0x0000, 3);
	
	c = (min%10) + 48;
	ST7735_DrawChar(94, 80, c, 0x07FF, 0x0000, 3);
	
	
}



void draw_Temp() {
	int temp = temperature * 100;
	
	int divisor = 1000;
	
	int output;
	
	if(temp <= 999) {
		
		divisor = 100;
	} 
	int pos = 10;
	while(divisor > 0) {
		
		output = temp/divisor;
		ST7735_DrawChar(pos, 20, output+48, 0x07E0, 0x0000, 2);
		pos += 12;
		if(divisor == 100) {
			ST7735_DrawChar(pos, 20, '.', 0x07E0, 0x0000, 2);
			pos+=12;
		}
		temp %= divisor; 
		divisor /= 10;	
		
	}
	
	ST7735_DrawChar(pos, 20, 'C', 0x07E0, 0x0000, 2);
	
	
	
}
/*int getSine(int deg){
	int sign = -1;
	if(deg<0 || deg > 180)
		sign = 1;
	deg+=180;
	deg %= 180;
	int num = 4*deg*(180-deg);
	num*=1000*sign;
	int den = 40500 - deg*(180-deg);
	return num/den;
}


void drawHands(uint16_t min, uint16_t hr, uint16_t mincolor, uint16_t hrcolor) {

	int cos = getSine(min*6-180);
	int sin = getSine(min*6-90);
	
	ST7735_Line(64+cos*5/1000, 96-sin*5/1000, 64+cos*30/1000, 96-sin*30/1000, mincolor);
	
	if(hr > 12) {
		hr -= 12;
	}
	cos = getSine(hr*30-180);
	sin = getSine(hr*30-90);
	ST7735_Line(64+cos*5/1000, 96-sin*5/1000, 64+cos*15/1000, 96-sin*15/1000, mincolor);
	
	prevmin = min;
	prevhr = hr;
}*/




