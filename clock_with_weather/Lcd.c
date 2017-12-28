#include "inc\tm4c123gh6pm.h"

#include "ST7735.h"
#include "Lcd.h"








void Lcd_Init(void) {
	
	ST7735_InitR(INITR_REDTAB);

}


int abs(int a){
	if(a<0)
		return a*-1;
	return a;
}

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






