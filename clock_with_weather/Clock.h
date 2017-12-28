
#define MAX_RECV_BUFF_SIZE  1024
extern int minutes;
extern int hours;
extern int seconds;
extern double temperature;
extern char Recvbuff[MAX_RECV_BUFF_SIZE];

void Clock_wait(void);
void Clock_Init(void);

void drawTemp(double temp);
