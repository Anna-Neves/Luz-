// Inclusão de bibliotecas padrão para funcionamento do código
#include "stdio.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"

// Definição dos pinos dos LEDs RGB
#define VERMELHO 13
#define VERDE 11
#define AZUL 12
// Definição do pino do botão
#define BOTAOA 5

// Variáveis para controle do estado do LED
int estado_led = 0;
bool led_on = false;

// Função de callback para o timer
int64_t turn_off_callback(alarm_id_t id, void *user_data) {
    
    // Verifica o estado do LED e desliga o LED correspondente
    switch (estado_led)
    {
    // Desliga o LED vermelho
    case 0:
        gpio_put(VERDE, 0);
        printf("VERDE DESLIGADO \n");
        estado_led = 1;
        add_alarm_in_ms(3000, turn_off_callback, NULL, false);
        break;
    // Desliga o LED verde
    case 1:
        gpio_put(VERMELHO, 0);
        printf("VERMELHO DESLIGADO \n");
        estado_led = 2;
        add_alarm_in_ms(3000, turn_off_callback, NULL, false);
        break;
    // Desliga o LED azul
    case 2:
        gpio_put(AZUL, 0);
        printf("AZUL DESLIGADO \n");
        led_on = false; // Todos os LEDs foram desligados
        estado_led = 0; // Reinicia o estado do LED
        break;
    
    }
    
return 0;
   
}

// Função principal
int main() {
    // Inicialização serial
    stdio_init_all();

    // Inicialização dos pinos dos LEDs
    gpio_init(VERMELHO);
    gpio_init(AZUL);
    gpio_init(VERDE);

    gpio_set_dir(VERMELHO, GPIO_OUT);
    gpio_set_dir(VERDE, GPIO_OUT);
    gpio_set_dir(AZUL, GPIO_OUT);
     
    // Inicialização do botão
    gpio_init(BOTAOA);
    gpio_set_dir(BOTAOA, GPIO_IN);
    gpio_pull_up(BOTAOA);
 
    
    
    // Loop infinito
    while (true)
    {
        // Verifica se o botão foi pressionado
        if(gpio_get(BOTAOA) == 0 && !led_on){
            sleep_ms(50); // Debounce de 50ms

            // Verifica se o botão foi pressionado após debounce
            if (gpio_get(BOTAOA) == 0)
            {   
                // Acende os LEDs caso for verdadeiro
                gpio_put(VERDE, 1);
                gpio_put(VERMELHO, 1);
                gpio_put(AZUL, 1);

                // Atualiza o estado do LED
                led_on = true;

                // Começa pelo LED vermelho
                estado_led = 0;

                // Agenda o primeiro callback para desligar os LEDS apos 3 segundos
                add_alarm_in_ms(3000, turn_off_callback, NULL, false);
            }
            
        }
    }

    return 0;
    
}