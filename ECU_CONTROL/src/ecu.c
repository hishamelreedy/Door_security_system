/*
 * ecu.c
 *
 *  Created on: Oct 18, 2018
 *      Author: hishamelreedy
 */
#include"uart.h"
#include"external_eeprom.h"
#define check_new 0x04
#define old_user 0x01
#define pass_pass 0x06
#define mismatch 0x09
#define opendoor 0x17
#define changepass 0x12
#define errorgo 0x13
#define readyforcheckuser 0x20
#define readyformain 0x21
#define pass_size 6
#define ecuready 0x30
uint8 atoi1(uint8 *str)
{
	int res = 0; // Initialize result

	// Iterate through all characters of input string and
	// update result
	for (int i = 0; str[i] != '\0'; ++i)
		res = res*10 + str[i] - '0';

	// return result.
	return res;
}
void check_operation(void);
void initialize_needed(){
	UART_init();
	_delay_ms(40);
	EEPROM_init();
	for(int i=0;i<pass_size;i++){
						EEPROM_writeByte((0x0311)+i, 0);
						_delay_ms(10);
					}
	_delay_ms(20);
}
void new_user(){

if(check_new==UART_recieveByte()){
	uint8 val=0;
	EEPROM_readByte(0x0311, &val);
	if(val==0){
		UART_sendByte(check_new);//tell HMI that he is new user
		uint8 received[pass_size+1];
		UART_receiveString(received);
			for(int i=0;i<pass_size;i++){
				EEPROM_writeByte((0x0311)+i, received[i]);
				_delay_ms(15);
			}
	}else{
		UART_sendByte(old_user);//tell HMI that he is old user

	}
}


}
void main_menu(void){
	uint8 received[pass_size+1];
	uint8 checkpass_flag=0;
	uint8 mismatch1=0;
	//if(readyformain==UART_recieveByte()){
	while(mismatch1!=3){
	UART_receiveString(received);
	for(int i=1;i<pass_size;i++){//changed i
		uint8 val=0;
			EEPROM_readByte((0x0311)+i, &val);
			_delay_ms(40);
			if(received[i-1]!=val){
				checkpass_flag=0;
				break;
			}else{
				checkpass_flag=1;
			}

		}
	if(checkpass_flag==1){

		UART_sendByte(pass_pass);
		check_operation();
		return;
	}else{
		UART_sendByte(mismatch);
		mismatch1++;
	}
	}
	//}
	return;
}
void check_operation(void){
	UART_sendByte(ecuready);
	uint8 received=0;
	while (received!=opendoor && received!=changepass){
		received=0;
		received=UART_recieveByte();
	}
	EEPROM_writeByte((0x0330), received);
	_delay_ms(10);
	DDRB|=(1<<PB7);
			PORTB|=(1<<PB7);
	if(opendoor==received){
		DDRD|=(1<<PD5)|(1<<PD6)|(1<<PD7);
			SET_BIT(PORTD,PD6);///motor moves clock wise
			CLEAR_BIT(PORTD,PD7);
			SET_BIT(PORTD,PD5);
			_delay_ms(60000);
			CLEAR_BIT(PORTD,PD5);
			_delay_ms(15000);
			SET_BIT(PORTD,PD7);///reverse motion
			CLEAR_BIT(PORTD,PD6);
			SET_BIT(PORTD,PD5);
			_delay_ms(15000);
			CLEAR_BIT(PORTD,PD5);
	}
	else if(changepass==received){

	for(int i=0;i<pass_size;i++){
					EEPROM_writeByte((0x0311)+i, 0);
				}
	if(check_new==UART_recieveByte()){
			UART_sendByte(check_new);//tell HMI that he is new user
			uint8 received[pass_size+1];
			UART_receiveString(received);
				for(int i=0;i<pass_size;i++){
					EEPROM_writeByte((0x0311)+i, received[i]);
					_delay_ms(15);
				}
		}
}

return;
}
int main(){
	initialize_needed();
	while(1){
		uint8 switchfunction=UART_recieveByte();
	switch(switchfunction){
	case readyforcheckuser: new_user(); break;
	case readyformain: main_menu(); break;
	case errorgo: DDRC|=(1<<PC2);
	PORTC|=(1<<PC2);//turn buzzer on
	_delay_ms(90000);
	PORTC&=~(1<<PC2);
	main_menu();
	break;
	}

}
}
