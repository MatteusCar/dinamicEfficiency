/*A biblioteca systick está disponível desde que habilitamos o stm32f4xx.h
* e o integramos ao projeto junto com a adição de símbolos
* então podemos fazer a análise de performance dinâmica dos algoritmos*/
#include "systick.h"

#define CTRL_ENABLE		(1U<<0)
#define CTRL_CLKSRC		(1U<<2)


void systick_counter_init(void)
{
	/*Disable systick before config*/
	SysTick->CTRL = 0;

	/*Load systick reload value register with the maximum value*/
	SysTick->LOAD = 0x00FFFFFF;

	/*Clear systick current value register*/
	SysTick->VAL = 0;

	/*Select internal clock source*/
	SysTick->CTRL  |= CTRL_CLKSRC;

	/*Enable systick*/
	SysTick->CTRL  |= CTRL_ENABLE;

}
