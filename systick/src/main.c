#include<libopencm3/stm32/rcc.h>
#include<libopencm3/stm32/gpio.h>
#include<libopencm3/cm3/systick.h>
#include<libopencm3/cm3/nvic.h>


volatile uint32_t ticks;

static void delay(uint32_t delta_ms){
    uint32_t final_ms = ticks + delta_ms;
    while(ticks < final_ms);
}

static void init(void){
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
 
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_set_reload(71999);
    systick_interrupt_enable();
    systick_counter_enable();
}


int main(void){
    init();
    while(true){
        gpio_toggle(GPIOC,GPIO13);
        delay(1000);
    }
}

void sys_tick_handler(void){
    ticks++;
}