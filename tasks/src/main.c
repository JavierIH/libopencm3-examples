#include<libopencm3/stm32/rcc.h>
#include<libopencm3/stm32/gpio.h>
#include<libopencm3/cm3/systick.h>
#include<libopencm3/cm3/nvic.h>


static void init(void){
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO12|GPIO13|GPIO14);

    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_set_reload(72000*100-1); // tick every 100ms
    systick_interrupt_enable();
    systick_counter_enable();
}


int main(void){
    init();
    while(true);
}


void sys_tick_handler(void){
    static uint16_t task_tick = 0;
    if(!(task_tick%2)) // task 1, every 200ms, highest priority
        gpio_toggle(GPIOC,GPIO13);
    if (task_tick==0) // task 2, only once at init, high priority
        gpio_toggle(GPIOB, GPIO12);
    if (!(task_tick%5)) // task 3, every 500ms, medium priority
        gpio_toggle(GPIOB, GPIO13);
    if (task_tick==3 || task_tick==17) // task 4, two times, low priority
        gpio_toggle(GPIOB, GPIO14);

    if(task_tick++>=19)
        task_tick=0; 
}
