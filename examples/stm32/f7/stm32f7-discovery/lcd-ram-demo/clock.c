/*
 * This file is part of the unicore-mx project.
 *
 * Copyright (C) 2013 Chuck McManis <cmcmanis@mcmanis.com>
 * Copyright (C) 2016 Maxime Vincent <maxime.vince@gmail.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Now this is just the clock setup code from systick-blink as it is the
 * transferrable part.
 */

#include <unicore-mx/stm32/rcc.h>
#include <unicore-mx/stm32/gpio.h>
#include <unicore-mx/cm3/nvic.h>
#include <unicore-mx/cm3/systick.h>

/* Common function descriptions */
#include "clock.h"

/* milliseconds since boot */
static volatile uint32_t system_millis;

/* Called when systick fires */
void sys_tick_handler(void)
{
	system_millis++;
}

/* simple sleep for delay milliseconds */
void msleep(uint32_t delay)
{
	uint32_t wake = system_millis + delay;
	while (wake > system_millis);
}

/* Getter function for the current time */
uint32_t mtime(void)
{
	return system_millis;
}

/*
 * clock_setup(void)
 *
 * This function sets up both the base board clock rate
 * and a 1khz "system tick" count. The SYSTICK counter is
 * a standard feature of the Cortex-M series.
 */
void clock_setup(void)
{
	/* Base board frequency, set to 216MHz */
	rcc_clock_setup_hse_3v3(&hse_25mhz_3v3);

	/* clock rate / 216000 to get 1mS interrupt rate */
	systick_set_reload(216000);
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_counter_enable();

	/* this done last */
	systick_interrupt_enable();
}


