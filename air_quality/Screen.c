

#include "Screen.h"

void Screen_Init(void)
{
	ST7735_InitR(INITR_REDTAB);
	ST7735_SetCursor(0, 0);
	
}
void Screen_Update(uint32_t Gas) {
	ST7735_FillScreen(0x0000);
	
	char c = (Gas/1000) + 48;
	ST7735_DrawChar(30, 80, c, 0x07FF, 0x0000, 3);
	Gas %= 1000;
	c = (Gas/100) + 48;
	ST7735_DrawChar(47, 80, c, 0x07FF, 0x0000, 3);
	Gas %= 100;
	c = (Gas/10) + 48;
	ST7735_DrawChar(64, 80, c, 0x07FF, 0x0000, 3);
	Gas %= 10;
	c = (Gas) + 48;
	ST7735_DrawChar(81, 80, c, 0x07FF, 0x0000, 3);
	
	
}

