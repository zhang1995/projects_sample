//***********************  main.c  ***********************
// Program written by:
// - Tong Zhang
// - Kaisheng Song
//*********************************************************
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "../inc/tm4c123gh6pm.h"

#include "pll.h"
#include "UART.h"
#include "esp8266.h"
#include "LED.h"

#include "Screen.h"
#include "Gas_Sensor.h"
#include "Temp_Sensor.h"
#include "switch.h"
#include "Sound.h"
// prototypes for functions defined in startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

char Fetch[] = "GET /data/2.5/weather?q=Austin%20Texas&APPID=1234567890abcdef1234567890abcdef HTTP/1.1\r\nHost:api.openweathermap.org\r\n\r\n";
// 1) go to http://openweathermap.org/appid#use 
// 2) Register on the Sign up page
// 3) get an API key (APPID) replace the 1234567890abcdef1234567890abcdef with your APPID

uint32_t Gas_value;
uint32_t safety_value = 3000;


#define SERVER  "ee445lab4c.appspot.com"
#define REQUEST_1 "GET /query?city=Austin%2C%20Texas&id=Tong%27s%20LaunchPad&greet=Internal%20Temp%3D" 
#define REQUEST_2 " HTTP/1.1\r\nUser-Agent: Keil\r\nHost: ee445lab4c.appspot.com\r\n\r\n"

char temp[] = "GET /query?city=Austin%2C%20Texas&id=Tong%27s%20LaunchPad&greet=Internal%20Temp%3DHTTP/1.1\r\nUser-Agent: Keil\r\nHost: ee445lab4c.appspot.com\r\n\r\n";
int main(void){  
  DisableInterrupts();
  PLL_Init(Bus80MHz);
  LED_Init();
  //Output_Init();       // UART0 only used for debugging
	
	/*init the sensor*/
	Gas_Sensor_Init();
	//Temp_Sensor_Init();
	Switch_Init();
	Screen_Init();
	Sound_Init();
	ST7735_DrawChar(30, 80, 'c', 0x07FF, 0x0000, 3);
	EnableInterrupts();
  //printf("\n\r-----------\n\rSystem starting...\n\r");
  //ESP8266_Init(9600);      // connect to access point, set up as client
  //ESP8266_GetVersionNumber();
  while(1){
		WaitForInterrupt();
    /*ESP8266_GetStatus();
    if(ESP8266_MakeTCPConnection(SERVER)){ // open socket in server
      LED_GreenOn();
      ESP8266_SendTCP(temp);
    }
		
    ESP8266_CloseTCPConnection();
    while(Board_Input()==0){// wait for touch
    };*/ 
    //LED_GreenOff();
    //LED_RedToggle();
  }
}

// transparent mode for testing
void ESP8266SendCommand(char *);
/*int main2(void){  char data;
  DisableInterrupts();
  PLL_Init(Bus80MHz);
  LED_Init();  
  Output_Init();       // UART0 as a terminal
  printf("\n\r-----------\n\rSystem starting at 9600 baud...\n\r");
//  ESP8266_Init(38400);
  ESP8266_InitUART(9600,true);
  ESP8266_EnableRXInterrupt();
  EnableInterrupts();
  ESP8266SendCommand("AT+RST\r\n");
  data = UART_InChar();
//  ESP8266SendCommand("AT+UART=115200,8,1,0,3\r\n");
//  data = UART_InChar();
//  ESP8266_InitUART(115200,true);
//  data = UART_InChar();
  
  while(1){
// echo data back and forth
    data = UART_InCharNonBlock();
    if(data){
      ESP8266_PrintChar(data);
    }
  }
}*/



