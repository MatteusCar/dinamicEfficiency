#include "stm32f4xx.h"
#include "arm_math.h"
#include "signals.h"
#include "systick.h"

#define GPIOAEN  (1U<<0)
#define PIN5     (1U<<5)
#define LED_PIN  PIN5
double teste;

__IO uint32_t rccRegister;

extern float32_t input_signal_f32_1kHz_15kHz[KHZ1_15_SIG_LEN];
extern float _5hz_signal[HZ_5_SIG_LEN];
double mean=0;
double sum=0;
volatile uint32_t begin,end,period;

int main(){

	/*Verificando se o clock source está habilitado para clock externo corretamente*/
	/*Precisei dar um shift 15 pois o registrador inteiro é de 32 bits*/
	rccRegister = (RCC->CR << 16);
	/*Habilita floating point nos algoritmos*/
	SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));

	systick_counter_init();

	/*while(1){
		GPIOA->ODR ^= LED_PIN;
		for(int i=0; i<1000000;i++){};
		teste = (double)5/(double)3;

	}*/

	//LEMBRAR QUE O SYSTICK É UM DOWNCOUNTER TIMER, ENTÃO TEM QUE SER ÍNICIO - FIM

	begin = SysTick->VAL;

	for(int i=0; i<HZ_5_SIG_LEN;i++){
		sum += _5hz_signal[i];
	}
	mean = sum/HZ_5_SIG_LEN;

	end = SysTick->VAL;
	period = (begin-end);


}
