
/*
 * $Revision: 10 $
 */

#define BTN_SW1_DOWN 1
#define BTN_SW1_UP 2
#define BTN_SW2_DOWN 3
#define BTN_SW2_UP 4
#define BTN_SW3_DOWN 5
#define BTN_SW3_UP 6
#define BTN_SW4_DOWN 7
#define BTN_SW4_UP 8

extern int led_slow_speed;

void ButtonInit(unsigned int(*button_func)());

int GetEvent(void);

void ButtonScan(void);
