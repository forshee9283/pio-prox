
#include <stdio.h>
#include "pico.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "prox.pio.h"

#define PROX_PIO pio0 //PIO block for proximity sensor
#define PROX_SM 0 //state machine number for proximity sensor
#define PROX_PIN 7 //GPIO number for the proximity sensor

int prox_setup(PIO pio_prox, int start_pin, int sm, const float clk_div){
    uint offset_prox = pio_add_program(pio_prox, &prox_program);
    pio_sm_claim(pio_prox, sm);//Panic if unavailible
    prox_init(pio_prox, sm, offset_prox, start_pin, clk_div);
    pio_sm_set_enabled(pio_prox, sm, true);
}

int main(){
    stdio_init_all();
    static const float pio_clk_div = 1; //This should be tuned for the size of the sensors keep as low as possible

    prox_setup(PROX_PIO, PROX_PIN, PROX_SM, pio_clk_div);

    while (true){
    int i;
    int prox_a =0;
    int prox_temp =0;
    for (i=0; i<8001; i++){
        prox_temp =pio_sm_get(PROX_PIO,0);
        if (prox_temp>prox_a){
            prox_a = prox_temp;
        }
    }
    printf("touch_state: %8u \n", (prox_a));
    //printf("touch_state: %32b \n", pio_sm_get(PROX_PIO,0));
    }
    return 0;
}