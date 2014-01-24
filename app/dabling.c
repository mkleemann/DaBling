/**
 * ----------------------------------------------------------------------------
 *
 * "THE ANY BEVERAGE-WARE LICENSE" (Revision 42 - based on beer-ware license):
 * <dev@layer128.net> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a be(ve)er(age) in return. (I don't
 * like beer much.)
 *
 * Matthias Kleemann
 *
 * ----------------------------------------------------------------------------
 *
 * \file dabling.c
 *
 * \date Created: 21.01.2014 22:55:56
 * \author Matthias Kleemann
 **/

#include "dabling.h"
#include "util/util.h"
#include "timer/timer.h"
#include "adc/adc.h"

#include <util/delay.h>
#include <avr/interrupt.h>

//! number of pattern in the pattern array
#define NUM_OF_PATTERN 10

//! pattern array for the 7 segment display
uint8_t pattern[NUM_OF_PATTERN] = { SEG_NUM_0,
                                    SEG_NUM_1,
                                    SEG_NUM_2,
                                    SEG_NUM_3,
                                    SEG_NUM_4,
                                    SEG_NUM_5,
                                    SEG_NUM_6,
                                    SEG_NUM_7,
                                    SEG_NUM_8,
                                    SEG_NUM_9 };

//! union to save random adc pattern and easier access
union {
   uint8_t  val8bit[2];
   uint16_t val16bit;
} adcValue;


// === MAIN LOOP =============================================================

/**
 * \brief main loop
 **/
#ifdef __DOXYGEN__
int main(void)
#else
int __attribute__((OS_main)) main(void)
#endif
{
   uint8_t  i = 0;

   adcValue.val16bit = 0xAFFE;

   initPorts();

   // approx. 262ms @ 1MHz
   initTimer0(/* TimerOverflow */);

   adc_init();

   // enable all (configured) interrupts
   sei();

   startTimer0();

   while(1)
   {
      // count to 10 - two times
      SET_PIN(SEG_CAT1);
      for(i = 0; i < NUM_OF_PATTERN; ++i)
      {
         EXP_PORT(SEG_PORT) = pattern[i];
         _delay_ms(500);
      }
      RESET_PIN(SEG_CAT1);
      _delay_ms(100);

      SET_PIN(SEG_CAT2);
      for(i = 0; i < NUM_OF_PATTERN; ++i)
      {
         EXP_PORT(SEG_PORT) = pattern[i];
         _delay_ms(500);
      }
      RESET_PIN(SEG_CAT2);
      _delay_ms(100);

      // random adc pattern
      for(i = 0; i < 1000; ++i)
      {
         SET_PIN(SEG_CAT1);
         EXP_PORT(SEG_PORT) = adcValue.val8bit[0] ^ adcValue.val8bit[1];
         _delay_ms(10);
         RESET_PIN(SEG_CAT1);
         _delay_ms(1);

         SET_PIN(SEG_CAT2);
         EXP_PORT(SEG_PORT) = adcValue.val8bit[1];
         _delay_ms(10);
         RESET_PIN(SEG_CAT2);
         _delay_ms(1);
      }
   }
}

// === ISR ===================================================================

/**
 * \brief timer 0 overflow interrupt service routine
 * Every overflow, the adc value of the unconnected port pin is read
 * to get a random value.
 */
ISR(TIMER0_OVF_vect)
{
   adcValue.val16bit = adc_get();
}

// === HELPERS ===============================================================

/**
 * \brief port initialization for leds
 * The port and pins for the 7 segment displays are setup.
 */
void initPorts(void)
{
   // block select (common cathode of 7 segment blocks)
   PIN_SET_OUTPUT(SEG_CAT1);
   RESET_PIN(SEG_CAT1);

   PIN_SET_OUTPUT(SEG_CAT2);
   RESET_PIN(SEG_CAT2);

   // init port for segments (all outputs) and set to 0
   EXP_DDR(SEG_PORT) = 0xFF;
   EXP_PORT(SEG_PORT) = 0;
}

