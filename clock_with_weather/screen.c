
#include <stdint.h>
#include "ST7735.h"
#include "screen.h"
#include <string.h>
#include "ADCSWTrigger.h"
#include <stdlib.h>




const int background = 0x0000; //0xFFFF;

void screen_Init() {
	ST7735_InitR(INITR_REDTAB);
	ST7735_FillScreen(0x0000);
}



void Parse_Buffer(char* Recvbuff) {
	char* temp = strstr(Recvbuff, "\"temp\":");
	
	temp+=7;
	
	temperature = atof(temp);
	
	/*char* test = temp + 5;
	*test = ' ';
	*(test+1) = 'C';
	*(test+2) = NULL;
	
	char* temper = "Temp= ";
	ST7735_DrawString(0, 3, temper, 0x7FF);
	ST7735_DrawString(5, 3, temp, 0x7FF);
	
	//printf("Temp = %.2f C\n\r", temperature);
	
	*/
}


void get_int_temp() {
	uint32_t voltage = ADC0_InSeq3();
	double temp = 147.5- ((75*3.3 * voltage) /4096); 
	printf("Temp~%.2f C\n", temp);
	
	
}

/*int abs(int a){
	if(a<0)
		return a*-1;
	return a;
}*/

void ST7735_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, 
                 uint16_t color){
				 
	if(abs(x2-x1)<=abs(y2-y1)){ //find the larger value, that way we can iterate over that one
		if(y1>y2){ //since our loop increases by 1 pixel each time we need to start with the lower value
			int swap = y1;
			y1 = y2;
			y2 = swap;
			swap = x1; //swap entire points, not just one value
			x1 = x2;
			x2 = swap;
		}
		int slope = (x2-x1)*100/(y2-y1); //calculate change of x not exactly 'slope'
		for(int i = 0; i<abs(y2-y1); i++){
			int newx = x1+slope*i/100; //for every increase of i (by 1) calculate the x that should be filled
			ST7735_DrawPixel(newx,y1+i,color);
		}
	}
	else{
		if(x1>x2){ //start with lower value
			int swap = y1;
			y1 = y2;
			y2 = swap;
			swap = x1;
			x1 = x2;
			x2 = swap;
		}
		int slope = (y2-y1)*100/(x2-x1);
		for(int i = 0; i<abs(x2-x1); i++){
			int newy = y1+slope*i/100;
			ST7735_DrawPixel(x1+i,newy,color);
		}
	}
}


