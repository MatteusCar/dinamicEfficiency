#include "stm32f4xx.h"
#include "arm_math.h"
#include "signals.h"
#include "systick.h"

#define GPIOAEN  (1U<<0)
#define PIN5     (1U<<5)
#define LED_PIN  PIN5
#define sysClock 16000000

__IO uint32_t rccRegister;

const float32_t oneTickTime = 1/(float32_t)(sysClock);

double teste;
double sum=0;

static float32_t signalMean(float32_t *sampleArray, uint32_t length);
static float32_t signalVariance(float32_t *sampleArray, float32_t signalMean, uint32_t length);

extern float32_t input_signal_f32_1kHz_15kHz[KHZ1_15_SIG_LEN];
extern float _5hz_signal[HZ_5_SIG_LEN];

volatile uint32_t begin,end, numTicks;

float32_t mean;
float32_t timeTaken_sec, timeTaken_ms, timeTaken_us;

float32_t mean_5hz_signal, mean_1kz_15kz_signal;
float32_t signalVariance_5hz_signal, signalVariance_1kz_15kz_signal;
float32_t stdDeviation_5Hz_CMSIS, stdDeviation_15kHz_CMSIS;

int main(){

	/*Programa que utiliza o CMSIS DSP Library para calcular Desvio Padrão*/
	/*Para acessar a documentação do CMSIS só digitar no google CMSIS DSP*/

	/*Verificando se o clock source está habilitado para clock externo corretamente*/
	/*Precisei dar um shift 15 pois o registrador inteiro é de 32 bits*/
	rccRegister = (RCC->CR << 16);

	/*Habilita floating point nos algoritmos*/
	SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));

	/*Inicializa contagem de bordas de subida do System Clock*/
	systick_counter_init();

	//LEMBRAR QUE O SYSTICK É UM DOWNCOUNTER TIMER, ENTÃO TEM QUE SER ÍNICIO - FIM

	begin = SysTick->VAL;

	mean_5hz_signal = signalMean((float32_t*)_5hz_signal, HZ_5_SIG_LEN);
	mean_1kz_15kz_signal = signalMean((float32_t*)input_signal_f32_1kHz_15kHz, KHZ1_15_SIG_LEN);

	signalVariance_5hz_signal = signalVariance((float32_t*)_5hz_signal, mean_5hz_signal, HZ_5_SIG_LEN);
	signalVariance_1kz_15kz_signal = signalVariance((float32_t*)input_signal_f32_1kHz_15kHz, mean_1kz_15kz_signal, KHZ1_15_SIG_LEN);

	arm_std_f32(_5hz_signal, HZ_5_SIG_LEN, &stdDeviation_5Hz_CMSIS);
	arm_std_f32(input_signal_f32_1kHz_15kHz, KHZ1_15_SIG_LEN, &stdDeviation_15kHz_CMSIS);

	end = SysTick->VAL;
	numTicks = (begin-end);
	timeTaken_sec = numTicks*oneTickTime;
	timeTaken_ms = 1000*timeTaken_sec;
	timeTaken_us = 1000000*timeTaken_sec;

	/*while(1){
		GPIOA->ODR ^= LED_PIN;
		for(int i=0; i<1000000;i++){};
		teste = (double)5/(double)3;

	}*/

}


static float32_t signalMean(float32_t *sampleArray, uint32_t length){

	float32_t meanValue=0.0;
	uint32_t i;

	for(i=0; i<length;i++){
		meanValue += sampleArray[i];
	}
	return (meanValue/(float32_t)length);

}

static float32_t signalVariance(float32_t *sampleArray, float32_t signalMean, uint32_t length){

	float32_t _variance = 0.0;
	uint32_t i;

	for(i=0;i<length;i++){
		_variance += (sampleArray[i] - signalMean)*(sampleArray[i] - signalMean);
	}
	_variance = _variance/(length-1);
	return _variance;

}






