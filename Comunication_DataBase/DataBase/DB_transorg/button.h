#ifndef BUTTON_H
#define BUTTON_H


//---Macros---//

#define GPIO4 	4
#define GPIO5	5

#define HIGH    1
#define LOW 	0
#define INPUT   0
#define OUTPUT  1

//---Prototipos das funcoes---//
bool access_gpio(int pin);
bool export_gpio(int pin);
bool direction_gpio(int pin, int direction);
bool value_out_gpio(int pin, int value);
bool unexport_gpio(int pin);

int value_in_gpio(int pin, int value);

void delay(float time);




#endif
