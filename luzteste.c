#include "stdio.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"

// Definição dos LEDs 
#define VERMELHO 13
#define VERDE 11
#define AZUL 12

// Definição do botão A
#define BOTAOA 5

// Variáveis para controle do estado do LED
int estado_led = 0;
bool led_on = false;

int64_t turn_off_callback(alarm_id_t id, void *user_data) {
    
    // Verifica o estado do LED e desliga o LED que estiver no gpio_put
    switch (estado_led)
    {
    // Todos ligados
            // Desliga o LED verde
    case 0:
        gpio_put(VERDE, 0);
        estado_led = 1;
        add_alarm_in_ms(3000, turn_off_callback, NULL, false);
        break;
    // Desliga o LED vermelho
    case 1:
        gpio_put(VERMELHO, 0);
        estado_led = 2;
        add_alarm_in_ms(3000, turn_off_callback, NULL, false);
        break;
    // Desliga o LED azul, fazendo todos ficarem apagados
    case 2:
        gpio_put(AZUL, 0);
        led_on = false; 
        estado_led = 0; // Reinicia o estado do LED
        break;
    
    }
    
return 0;
   
}

int main() {
    // Inicialização da placa
    stdio_init_all();

    // Inicialização dos leds e configuração como saída
    gpio_init(VERMELHO);
    gpio_init(AZUL);
    gpio_init(VERDE);

    gpio_set_dir(VERMELHO, GPIO_OUT);
    gpio_set_dir(VERDE, GPIO_OUT);
    gpio_set_dir(AZUL, GPIO_OUT);
     
    // Inicialização do botão e configuração como entrada
    gpio_init(BOTAOA);
    gpio_set_dir(BOTAOA, GPIO_IN);
    gpio_pull_up(BOTAOA);
 
    
    
    // Loop 
    while (1)
    {
        // Vê se pressionaram o botão
        if(gpio_get(BOTAOA) == 0 && !led_on){
            sleep_ms(50); 

            // Verifica se o botão foi pressionado novamente
            if (gpio_get(BOTAOA) == 0)
            {   
                // Acende os LEDs se for verdadeiro
                gpio_put(VERDE, 1);
                gpio_put(VERMELHO, 1);
                gpio_put(AZUL, 1);

                // Atualiza o estado do LED
                led_on = true;

                // Começa pelo LED verde
                estado_led = 0;

                // Programa o callback para desativar os LEDS após 3 segundos
                add_alarm_in_ms(3000, turn_off_callback, NULL, false);
            }
            
        }
    }

    return 0;
    
}
