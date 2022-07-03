#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>


static void init(void){
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
    gpio_set(GPIOC, GPIO13);

    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO4);

    rcc_periph_clock_enable(RCC_AFIO);
    exti_select_source(EXTI4, GPIOB);
    exti_set_trigger(EXTI4, EXTI_TRIGGER_RISING);
    exti_enable_request(EXTI4);
    nvic_enable_irq(NVIC_EXTI4_IRQ);

    nvic_set_priority(NVIC_EXTI4_IRQ, 2); // Max priority = 0, Min priority = 15
}


int main(void){
    init();
    while(true);
}


void exti4_isr(void){
    exti_reset_request(EXTI4);
    gpio_clear(GPIOC, GPIO13);
    for(uint32_t i=0; i<1000000; i++)
        __asm__("nop");
    gpio_set(GPIOC, GPIO13);
    for(uint32_t i=0; i<1000000; i++)
        __asm__("nop");
}
