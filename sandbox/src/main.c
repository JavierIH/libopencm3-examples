#include<libopencm3/stm32/rcc.h>
#include<libopencm3/stm32/gpio.h>
#include<libopencm3/stm32/timer.h>
#include<libopencm3/cm3/systick.h>
#include<libopencm3/cm3/nvic.h>


volatile uint32_t ticks;

static void delay(uint32_t delta_ms){
    uint32_t final_ms = ticks + delta_ms;
    while(ticks < final_ms);
}

static void init(void){
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_set_reload(71999);
    systick_interrupt_enable();
    systick_counter_enable();

    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO3|GPIO4|GPIO5|GPIO12|GPIO13|GPIO14|GPIO15);
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO8|GPIO9|GPIO10|GPIO11|GPIO12|GPIO15);
    //gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO5);
    //gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO6|GPIO7);

    //rcc_periph_clock_enable(RCC_TIM4);
    /*timer_set_mode(TIM4, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM4, 0);
    timer_set_period(TIM4, 1023); 
    timer_disable_preload(TIM4);
    timer_continuous_mode(TIM4);
    timer_set_oc_mode(TIM4, TIM_OC1, TIM_OCM_PWM1);
    timer_set_oc_mode(TIM4, TIM_OC2, TIM_OCM_PWM1);
    timer_enable_oc_output(TIM4, TIM_OC1);
    timer_enable_oc_output(TIM4, TIM_OC2);
    timer_enable_counter(TIM4);*/
}

int main(void){
    init();

    //timer_set_oc_value(TIM4, TIM_OC1, 300);
    //timer_set_oc_value(TIM4, TIM_OC2, 700);
    while(1){
        gpio_toggle(GPIOB, GPIO3|GPIO4|GPIO5|GPIO12|GPIO13|GPIO14|GPIO15);
        gpio_toggle(GPIOA, GPIO8|GPIO9|GPIO10|GPIO11|GPIO12|GPIO15);
        //gpio_toggle(GPIOB, GPIO5);
        delay(500);
    }
}


void sys_tick_handler(void){
    ticks++;
}