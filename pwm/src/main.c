#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

static void init(void){
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO0);

    rcc_periph_clock_enable(RCC_TIM2);
    timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM2, 0);
    timer_set_period(TIM2, 1023); 
    timer_disable_preload(TIM2);
    timer_continuous_mode(TIM2);
    timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_PWM1);
    timer_enable_oc_output(TIM2, TIM_OC1);
    timer_enable_counter(TIM2);
}

int main(void){
    init();
    timer_set_oc_value(TIM2, TIM_OC1, 512);
    while(1);
}