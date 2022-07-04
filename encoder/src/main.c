#include<libopencm3/stm32/rcc.h>
#include<libopencm3/stm32/gpio.h>
#include<libopencm3/stm32/timer.h>
#include<libopencm3/cm3/systick.h>
#include<libopencm3/cm3/nvic.h>


static void init(void){
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

    rcc_periph_clock_enable(RCC_TIM3);
    timer_set_period(TIM3, 1000);
    timer_slave_set_mode(TIM3, TIM_SMCR_SMS_EM3);
    timer_ic_set_input(TIM3, TIM_IC1, TIM_IC_IN_TI1);
    timer_ic_set_input(TIM3, TIM_IC2, TIM_IC_IN_TI2);

    timer_enable_counter(TIM3);
}

volatile uint32_t pos;

int main(void){
    init();
    while (true){
        pos = timer_get_counter(TIM3);
    }
}