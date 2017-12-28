
//#include <stdint.h>

void Lcd_Init();
void draw_landpage(void);
void draw_face(void);
int abs(int a);
void ST7735_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, 
                 uint16_t color);
