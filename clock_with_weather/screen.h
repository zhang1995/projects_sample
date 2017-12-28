

extern double temperature;

void screen_Init(void);


void Parse_Buffer(char* Recvbuff);

void get_int_temp(void);

void draw_face(void);

void ST7735_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, 
                 uint16_t color);
