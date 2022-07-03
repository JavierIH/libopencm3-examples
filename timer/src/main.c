#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>

static void init(void){
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

    rcc_periph_clock_enable(RCC_TIM2);
    timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM2, 720-1); // 100kHz
    timer_set_period(TIM2, 10000-1); 
    timer_enable_irq(TIM2, TIM_DIER_UIE);
    timer_enable_counter(TIM2);

    nvic_enable_irq(NVIC_TIM2_IRQ);
}

int main(void){
    init();
    while(1);
}

void tim2_isr(void){
    if(timer_get_flag(TIM2, TIM_SR_UIF)){
        timer_clear_flag(TIM2, TIM_SR_UIF);
        gpio_toggle(GPIOC, GPIO13);
    }
}