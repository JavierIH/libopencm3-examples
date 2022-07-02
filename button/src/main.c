#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>


static void init(void){
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
    
    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO12);
}

int main(void){
    init();
    while(1){
        if(gpio_get(GPIOB, GPIO12)){
            gpio_clear(GPIOC, GPIO13);
            for(uint32_t i=0; i<1000000; i++) __asm__("nop");
            gpio_set(GPIOC, GPIO13);
            for(uint32_t i=0; i<1000000; i++) __asm__("nop");
        }
        else{
            gpio_set(GPIOC, GPIO13);
        }
    }
    return 0;
}