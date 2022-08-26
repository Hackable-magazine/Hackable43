#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/sleep.h>
#include <hardware/rtc.h>
#include <hardware/pll.h>
#include <hardware/clocks.h>

#include "hardware/rosc.h"
#include "hardware/structs/scb.h"

#define LED  25

void measure_freqs() {
	uint f_pll_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY);
	uint f_pll_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_USB_CLKSRC_PRIMARY);
	uint f_rosc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC);
	uint f_clk_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
	uint f_clk_peri = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_PERI);
	uint f_clk_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_USB);
	uint f_clk_adc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_ADC);
	uint f_clk_rtc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_RTC);

	printf("pll_sys  = %dkHz\n", f_pll_sys);
	printf("pll_usb  = %dkHz\n", f_pll_usb);
	printf("rosc     = %dkHz\n", f_rosc);
	printf("clk_sys  = %dkHz\n", f_clk_sys);
	printf("clk_peri = %dkHz\n", f_clk_peri);
	printf("clk_usb  = %dkHz\n", f_clk_usb);
	printf("clk_adc  = %dkHz\n", f_clk_adc);
	printf("clk_rtc  = %dkHz\n", f_clk_rtc);
}

void recover_from_sleep(uint scb_orig, uint clock0_orig, uint clock1_orig){

	//Re-enable ring Oscillator control
	rosc_write(&rosc_hw->ctrl, ROSC_CTRL_ENABLE_BITS);

	//reset procs back to default
	scb_hw->scr = scb_orig;
	clocks_hw->sleep_en0 = clock0_orig;
	clocks_hw->sleep_en1 = clock1_orig;

	//reset clocks
	clocks_init();
	stdio_init_all();

	return;
}

static bool awake;

static void sleep_callback(void) {
	printf("RTC woke us up\n");
	uart_default_tx_wait_blocking();
	awake = true;
}

static void rtc_sleep(void) {
	// Start on Friday 5th of June 2020 15:45:00
	datetime_t t = {
		.year  = 2020,
		.month = 06,
		.day   = 05,
		.dotw  = 5, // 0 is Sunday, so 5 is Friday
		.hour  = 15,
		.min   = 45,
		.sec   = 00
	};

	// Alarm 10 seconds later
	datetime_t t_alarm = {
		.year  = 2020,
		.month = 06,
		.day   = 05,
		.dotw  = 5, // 0 is Sunday, so 5 is Friday
		.hour  = 15,
		.min   = 45,
		.sec   = 10
	};

	// Start the RTC
	rtc_init();
	rtc_set_datetime(&t);

	printf("Sleeping for 10 seconds\n");
	uart_default_tx_wait_blocking();

	sleep_goto_sleep_until(&t_alarm, &sleep_callback);
}

int main() {
	stdio_init_all();
	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);

	//save values for later
	uint scb_orig = scb_hw->scr;
	uint clock0_orig = clocks_hw->sleep_en0;
	uint clock1_orig = clocks_hw->sleep_en1;

	// measure_freqs();

	printf("Hello Sleep!\n");

	printf("Switching to XOSC\n");

	// Wait for the fifo to be drained so we get reliable output
	uart_default_tx_wait_blocking();

	// UART will be reconfigured by sleep_run_from_xosc
	sleep_run_from_xosc();
	//sleep_run_from_rosc();

	printf("Switched to XOSC\n");
	uart_default_tx_wait_blocking();
	sleep_ms(500);

	awake = false;

	printf("Fais dodo !\n");
	uart_default_tx_wait_blocking();
	rtc_sleep();

	// Make sure we don't wake
	while (!awake) {
		printf("Should be sleeping\n");
	}

	// measure_freqs();

	// reset processor and clocks back to defaults
	recover_from_sleep(scb_orig, clock0_orig, clock1_orig);

	printf("clignote LED\n");
	uart_default_tx_wait_blocking();

	while(1) {
		gpio_put(LED, 1);
		sleep_ms(100);
		gpio_put(LED, 0);
		sleep_ms(500);
	}


	return 0;
}
