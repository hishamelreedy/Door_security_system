/*
 * main.c
 *
 *  Created on: Oct 9, 2018
 *      Author: hishamelreedy
 */
#include"uart.h"
#include"lcd.h"
#include"keypad.h"
#define check_new 0x04
#define pass_pass 0x06
#define opendoor 0x17
#define changepass 0x12
#define errorgo 0x13
#define ready 0x20
#define pass_size 6
#define readyformain 0x21
#define ecuready 0x30
#define old_user 0x01
/////////functions prototypes
int compare(uint8 *,uint8 *);
void initialize_needed();
void new_user();
void main_menu();
void change_pass();
void open_door();
void error_go();

///initializing needed modules
void initialize_needed(){
LCD_init();
UART_init();
_delay_ms(500);
}
int compare(uint8 *received,uint8* reconfirm){
	for(int i=0;i<pass_size;i++){
		if(received[i]!=reconfirm[i]){
			return 0;
		}
	}
	return 1;
}
void new_user(){
	uint8 match=0;
	UART_sendByte(ready);
	UART_sendByte(check_new);
	if(check_new == UART_receiveByte()){
while(match==0){
	LCD_clearScreen();
	LCD_displayString("enter new pass");//asking user to enter pass
	LCD_goToRowColumn(1,0);
	uint8 z=0;
	uint8 pass[pass_size+1];
	uint8 repass[pass_size+1];
	uint8 i=0;
	while(z!='*'){
		z=KeyPad_getPressedKey();
		_delay_ms(500);
		LCD_displayString("*");
		pass[i]=z;
		i++;
	}
	i=0;
	LCD_clearScreen();
	LCD_displayString("re-enter pass");
	LCD_goToRowColumn(1,0);
	z=0;
		while(z!='*'){
				z=KeyPad_getPressedKey();
				_delay_ms(500);
				LCD_displayString("*");
				repass[i]=z;
				i++;
			}
	if(compare(pass,repass)==1){
		UART_sendByte(ready);
		UART_sendString(pass);
		match=1;
		main_menu();
		return;
	}else{
		LCD_clearScreen();
		LCD_displayString("password mismatch");
		match=0;
	}

	}}else if(old_user == UART_receiveByte()){
		main_menu();
	}
	}

void main_menu(void){

	uint8 mismatch=0;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"*:open_door");
	LCD_displayStringRowColumn(1,0,"#:change_pass");
	uint8 z=0;
	z=KeyPad_getPressedKey();
	_delay_ms(500);
	if(z=='*'){
		LCD_clearScreen(); /* clear the LCD display */
		LCD_displayString("enter password");
		LCD_goToRowColumn(1,0);
		uint8 pass[pass_size+1];
		uint8 i=0;
		while(mismatch!=3){
			LCD_clearScreen(); /* clear the LCD display */
			LCD_displayString("enter password");
			LCD_goToRowColumn(1,0);
			z=0;
		while(z!='*'){
			z=KeyPad_getPressedKey();
			_delay_ms(500);
			LCD_displayString("*");
			pass[i]=z;
			i++;
		}
		UART_sendByte(readyformain);
		UART_sendString(pass);
		if(pass_pass==UART_receiveByte()){
			open_door();
			return;

		}else{
			mismatch++;
			}
}
		error_go();}
	if(z=='#'){
		z=0;
		LCD_clearScreen(); /* clear the LCD display */
		LCD_displayString("enter password");
		LCD_goToRowColumn(1,0);
		uint8 pass[pass_size+1];
		uint8 i=0;
		while (mismatch!=3){
			LCD_clearScreen(); /* clear the LCD display */
					LCD_displayString("enter password");
					LCD_goToRowColumn(1,0);
			z=0;
		while(z!='*'){
			z=KeyPad_getPressedKey();
			_delay_ms(500);
			LCD_displayString("*");
			pass[i]=z;
			i++;
		}
		UART_sendByte(readyformain);
		UART_sendString(pass);
		if(pass_pass==UART_receiveByte()){
			change_pass();//changepass
		}else{
			mismatch++;
			}
}
		error_go();
}
	}

int main(){
	initialize_needed();
	new_user();


}
void open_door(void){

	if(ecuready==UART_receiveByte()){
		UART_sendByte(opendoor);
		LCD_clearScreen();
		LCD_displayString("The Door is");
		LCD_goToRowColumn(1,0);
		LCD_displayString("Unlocked");

	}
	}
void change_pass(void){
			UART_sendByte(changepass);
			//change pass code
			uint8 match=0;
				//UART_sendByte(ready);
				UART_sendByte(check_new);
				if(check_new == UART_receiveByte()){
			while(match==0){
				LCD_clearScreen();
				LCD_displayString("enter new pass");//asking user to enter pass
				LCD_goToRowColumn(1,0);
				uint8 z=0;
				uint8 pass[pass_size+1];
				uint8 repass[pass_size+1];
				uint8 i=0;
				while(z!='*'){
					z=KeyPad_getPressedKey();
					_delay_ms(500);
					LCD_displayString("*");
					pass[i]=z;
					i++;
				}
				i=0;
				LCD_clearScreen();
				LCD_displayString("re-enter pass");
				LCD_goToRowColumn(1,0);
				z=0;
					while(z!='*'){
							z=KeyPad_getPressedKey();
							_delay_ms(500);
							LCD_displayString("*");
							repass[i]=z;
							i++;
						}
				if(compare(pass,repass)==1){
					UART_sendByte(ready);
					UART_sendString(pass);
					match=1;
					main_menu();
					return;
				}else{
					LCD_clearScreen();
					LCD_displayString("password mismatch");
					match=0;
				}

				}}else if(old_user == UART_receiveByte()){
					main_menu();
				}
				}


void error_go(void){
	LCD_clearScreen();
	LCD_displayString("ERROR ERROR");
	UART_sendByte(errorgo);
	DDRC=0xFF;
	_delay_ms(90000);
	DDRC=0x00;
	main_menu();
}

