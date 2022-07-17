#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/systick.h>

volatile uint32_t ticks;

static void delay(uint32_t delta_ms){
    uint32_t final_ms = ticks + delta_ms;
    while (ticks < final_ms);
}

static void init(void){
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_set_reload(71999);
    systick_interrupt_enable();
    systick_counter_enable();

    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
}

static void motorInit(void){
    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO9 | GPIO8);

    rcc_periph_clock_enable(RCC_TIM4);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO7);

    timer_set_mode(TIM4, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM4, 0);
    timer_set_period(TIM4, 1023);
    timer_disable_preload(TIM4);
    timer_continuous_mode(TIM4);
    timer_set_oc_mode(TIM4, TIM_OC2, TIM_OCM_PWM1);
    timer_enable_oc_output(TIM4, TIM_OC2);
    timer_enable_counter(TIM4);
}

static void motorSetSpeed(uint16_t speed){
    if (speed > 1023)
        speed = 1023;
    timer_set_oc_value(TIM4, TIM_OC2, speed);
}

static void motorSetDirection(bool direction){
    if (direction){
        gpio_set(GPIOB, GPIO8);
        gpio_clear(GPIOB, GPIO9);
    }
    else
    {
        gpio_clear(GPIOB, GPIO8);
        gpio_set(GPIOB, GPIO9);
    }
}

static void motorBrake(uint16_t brake){
    gpio_set(GPIOB, GPIO8 | GPIO9);
    if (brake > 1023)
        brake = 1023;
    timer_set_oc_value(TIM4, TIM_OC2, brake);
}

static void motorFree(void){
    gpio_clear(GPIOB, GPIO8);
    gpio_clear(GPIOB, GPIO9);
}

int main(void){
    init();
    motorInit();
    motorSetDirection(0);
    motorSetSpeed(300);

    while (1){
        gpio_toggle(GPIOC, GPIO13);
        delay(100);
    }
}

void sys_tick_handler(void){
    ticks++;
}