/* Universidad del Valle de Guatemala
 * Electrónica Digital 2
 * Proyecto Final - TivaWare
 * Autor: Luis Pablo Carranza
 * Fecha de creación: 12/5/23
 * Última modificación: 12/5/23
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "grlib/grlib.h"
#include "driverlib/pin_map.h"
#include "driverlib/adc.h"
#include "driverlib/systick.h"

//Variables y constantes para el código principal
#define COMP 2000           // Valor para comparar el ADC de los LDR
#define DELTIME 500000      // Delay entre cada lectura ADC
float ValADC0;


void initUART(void);
void UARTstring(char *string);

// Variables para las lecturas de ADC
uint32_t ADCBuffer0;
uint32_t ADCBuffer1;
uint32_t ADCBuffer2;
uint32_t ADCBuffer3;


int main(void)
{

    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);


    initUART();

    // Habilitación de puertos
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    // Declaración de tipo de puertos
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4);

    // Encender los leds rojos al inicio
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0x00);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0x00);

    // Configuración de los pines para ADC
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1);
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);

    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_3);
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);

    // Configuración de los canales ADC
    SysCtlPeripheralDisable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));

    ADCSequenceDisable(ADC0_BASE, 3);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH1 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_CH2 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH4 | ADC_CTL_IE | ADC_CTL_END);

    // Habilitar las secuencias de lectura para los 4 canales analógicos
    ADCSequenceEnable(ADC0_BASE, 0);
    ADCSequenceEnable(ADC0_BASE, 1);
    ADCSequenceEnable(ADC0_BASE, 2);
    ADCSequenceEnable(ADC0_BASE, 3);

    while(1){

        ADCProcessorTrigger(ADC0_BASE, 0);
        ADCSequenceDataGet(ADC0_BASE, 0, &ADCBuffer0);

        ValADC0 = (float)ADCBuffer0;

        if(ValADC0 < COMP){
            UARTCharPutNonBlocking(UART1_BASE, '1');
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4);
        }
        else{
            UARTCharPutNonBlocking(UART1_BASE, '2');
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0x00);
        }

        SysCtlDelay(DELTIME);


        ADCProcessorTrigger(ADC0_BASE, 1);
        ADCSequenceDataGet(ADC0_BASE, 1, &ADCBuffer1);

        ValADC0 = (float)ADCBuffer1;

        if(ValADC0 < COMP){
            UARTCharPutNonBlocking(UART1_BASE, '3');
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_PIN_7);
        }
        else{
            UARTCharPutNonBlocking(UART1_BASE, '4');
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0x00);
        }

        SysCtlDelay(DELTIME);


        ADCProcessorTrigger(ADC0_BASE, 2);
        ADCSequenceDataGet(ADC0_BASE, 2, &ADCBuffer2);

        ValADC0 = (float)ADCBuffer2;

        if(ValADC0 < COMP){
            UARTCharPutNonBlocking(UART1_BASE, '5');
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_PIN_6);
        }
        else{
            UARTCharPutNonBlocking(UART1_BASE, '6');
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0x00);
        }

        SysCtlDelay(DELTIME);


        ADCProcessorTrigger(ADC0_BASE, 3);
        ADCSequenceDataGet(ADC0_BASE, 3, &ADCBuffer3);

        ValADC0 = (float)ADCBuffer3;

        if(ValADC0 < COMP){
            UARTCharPutNonBlocking(UART1_BASE, '7');
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_5);
        }
        else{
            UARTCharPutNonBlocking(UART1_BASE, '8');
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0x00);
        }

        SysCtlDelay(DELTIME);


    }
}

void initUART(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1);
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE|UART_CONFIG_PAR_NONE);
    UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);
}

void UARTstring(char *string){
    int i;
    for (i = 0; string[i] != '\0'; i++) {
        UARTCharPut(UART1_BASE, string[i]);
    }
}



