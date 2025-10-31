//single wire
//4 Pin Single row
//empieza con un 0 la transmision, cierra con un 1 de resistencia pull up
//(float) datoFloat = datoByte[0];

#include "AE3G9DHT11.h"
#include "stm32f103xb.h"
#include "stdint.h"


int tiempoUs=0, contBytes;
uint8_t datoByte[5];

void dht11_init(dht11_t* sensor){
    if(sensor -> port == GPIOA) RCC ->APB2ENR |= RCC_APB2ENR_IOPAEN;
    else if(sensor -> port == GPIOB) RCC ->APB2ENR |= RCC_APB2ENR_IOPBEN;
    else if(sensor -> port == GPIOB) RCC ->APB2ENR |= RCC_APB2ENR_IOPCEN;

    if(sensor ->pin_num <8){
        sensor -> port -> CRL &= ~(0xF << (sensor ->pin_num)*4);
        sensor -> port -> CRL |= (0x8 << (sensor ->pin_num)*4);
    }else{
        sensor -> port -> CRH &= ~(0xF << ((sensor ->pin_num)%8)*4);
        sensor -> port -> CRH |= (0x8 << ((sensor ->pin_num)%8)*4);
    }
    sensor -> port -> BSRR |= (1<< sensor -> pin_num);
}

bool dht11_read(dht11_t* sensor){
    
    sensor -> port -> BSRR |= (1<< sensor -> pin_num+16);
    delay_ms(18);
    sensor -> port -> BSRR |= (1<< sensor -> pin_num);

    __disable_irq();
    while(tiempoUs <60 && (sensor -> port -> IDR&(1<< sensor -> pin_num))){
        delay_us(1);
        tiempoUs ++;
    }
    if((tiempoUs >= 60)&& (sensor -> port -> IDR&(1<< sensor -> pin_num))){
        __enable_irq();
        return false;
    }

    delayUs(100);
    sensor -> port -> BSRR |= (1<< sensor -> pin_num);
    delayUs(100);

    tiempoUs = 0;

    for(int i=0;i<5;i++){
        for(int j = 0;j<8;j++){
            delay_us(50);
            while(tiempoUs <=90 && (sensor -> port -> IDR&(1<< sensor -> pin_num))){
                delay_us(1);
                tiempoUs ++;
            }
            if(tiempoUs > 90 && (sensor -> port -> IDR&(1<< sensor -> pin_num))){
                __enable_irq();
                return false;
            }
            if(tiempoUs > 48){//si hay 1
                datoByte[i] = (1<<j);
            }
            tiempoUs = 0;
        }
    }
    if(datoByte[4] != datoByte[0] + datoByte[1] + datoByte[2] + datoByte[3]){
            __enable_irq();
            return false;
    }

    sensor -> humidity = (float)datoByte[0];
    sensor -> temperature = (float)datoByte[2];
    __enable_irq();
    return true;
}