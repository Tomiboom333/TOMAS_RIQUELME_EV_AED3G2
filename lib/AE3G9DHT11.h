#ifndef AE3G9DHT11
#define AE3G9DHT11
#include "stm32f103xb.h"
#include "stdbool.h"
#include "stdint.h"

typedef struct {
    // Configuración
    GPIO_TypeDef* port;       
    uint16_t      pin_num;    
    
    // Datos
    float temperature;
    float humidity;
}dht11_t;

void dht11_init(dht11_t* sensor);//Inicializa el hardware necesario.
bool dht11_read(dht11_t* sensor);//Es la función principal. Realiza una lectura completa del sensor.
//true: Si la lectura fue exitosa (incluyendo el checksum)
//false: Si algo falló (tiempo crítico superado o checksum incorrecto).


#endif