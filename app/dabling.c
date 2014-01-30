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

#include "util/util.h"
#include "adc/adc.h"
#include "leds/leds.h"
#include "timer/timer.h"
#include "dabling.h"

#include <util/delay.h>
#include <avr/interrupt.h>

//! number of pattern in the pattern array
#define NUM_OF_PATTERN 16

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
                                    SEG_NUM_9,
                                    SEG_NUM_A,
                                    SEG_NUM_B,
                                    SEG_NUM_C,
                                    SEG_NUM_D,
                                    SEG_NUM_E,
                                    SEG_NUM_F };

//! value of the adc pin
uint16_t adcVal = 0xAFFE;

//! trigger value of flasher
uint16_t flashTrigger = 1;

//! trigger matrix pattern
uint16_t matrixTrigger = 0;

//! how far to count the trigger until the pattern changes
#define WAIT_FOR_NEXT_PATTERN 15

//! pattern type for matrix
typedef struct { //! row pattern
                 uint8_t rows;
                 //! column patter
                 uint8_t cols;
               } matrix_t;

//! max. number of matrix pattern
#define NUM_OF_MATRIX_PATTERN 4

//! simple pattern for matrix
matrix_t matrixPattern[NUM_OF_MATRIX_PATTERN][MATRIX_MAX_ROW] =
   { { { 1, 2 }, { 2, 2 }, { 4, 2 } },    // oo*,o*o o*o,o*o *oo,o*o
     { { 1, 1 }, { 2, 2 }, { 4, 4 } },    // oo*,oo* o*o,o*o *oo,*oo
     { { 2, 1 }, { 2, 2 }, { 2, 4 } },    // o*o,oo* o*o,o*o o*o,*oo
     { { 1, 4 }, { 2, 2 }, { 4, 1 } } };  // oo*,*oo o*o,o*o *oo,oo*

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
   uint16_t i = 0;
   uint16_t j = 0;

   initHardware();

   while(1)
   {
      // count to 10 - two times
      for(i = 0; i < NUM_OF_PATTERN; ++i)
      {
         // j * wait time in show7Segment() = time to display pattern
         for(j = 0; j < 23; ++j)
         {
            show7Segment(pattern[i], ~(pattern[i]));
         }
      }
      _delay_ms(10);

      for(i = 0; i < NUM_OF_PATTERN; ++i)
      {
         // j * wait time in show7Segment() = time to display pattern
         for(j = 0; j < 23; ++j)
         {
            show7Segment(~(pattern[i]), pattern[i]);
         }
      }
      _delay_ms(10);

      // random adc pattern
      for(i = 0; i < 2500; ++i)
      {
         show7Segment(adcVal & 0xFF,
                      adcVal >> 8);
      }
   }
}

// === ISR ===================================================================

/**
 * \brief timer0 overflow interrupt service routine
 * Every overflow, the adc value of the unconnected port pin is read
 * to get a random value.
 */
ISR(TIMER0_OVF_vect)
{
   adcVal = adc_get();
}

/**
 * \brief timer1 compare interrupt service routine
 */
ISR(TIMER1_CAPT_vect)
{
   // (trigger/wait for pattern) counts up to select the pattern
   // and its modulo keeps it in range of the array. The row selection
   // within the pattern is the simple modulo of the trigger with the
   // number of rows.
   showMatrixPattern(((matrixTrigger / WAIT_FOR_NEXT_PATTERN) % NUM_OF_MATRIX_PATTERN),
                      (matrixTrigger % MATRIX_MAX_ROW));
   ++matrixTrigger;
}

/**
 * \brief timer2 compare interrupt service routine
 */
ISR(TIMER2_COMP_vect)
{
   flashTrigger <<= 1;

   flashLed(flashBlue1, 0x0333);
   flashLed(flashBlue2, 0x0249);
   // reset trigger
   if(flashTrigger & 0x8000)
   {
      flashTrigger = 1;
   }
}

// === HELPERS ===============================================================

/**
 * \brief initialization for hardware
 * The port and pins for the 7 segment displays are setup as
 * well as timer(s) and adc.
 */
void initHardware(void)
{
   // block select (common cathode of 7 segment blocks)
   PIN_SET_OUTPUT(SEG_CAT1);
   RESET_PIN(SEG_CAT1);

   PIN_SET_OUTPUT(SEG_CAT2);
   RESET_PIN(SEG_CAT2);

   // init port for segments (all outputs) and set to 0
   EXP_DDR(SEG_PORT) = 0xFF;
   EXP_PORT(SEG_PORT) = 0;

   // set 3x3 LED matrix rows and columns as output
   EXP_DDR(MATRIX_ROW_PORT) |= MATRIX_ROW_MASK;
   EXP_PORT(MATRIX_ROW_PORT) &= ~MATRIX_ROW_MASK;
   EXP_DDR(MATRIX_COL_PORT) |= MATRIX_COL_MASK;
   EXP_PORT(MATRIX_COL_PORT) &= ~MATRIX_COL_MASK;

   // approx. 262ms @ 1MHz
   initTimer0(/* TimerOverflow */);
   // approx. 10ms @ 1MHz
   initTimer1(TimerCompare);
   // approx. 25ms @ 1MHz
   initTimer2(TimerCompare);

   adc_init();
   led_init();

   // enable all (configured) interrupts
   sei();

   adc_enable();
   startTimer0();
   startTimer1();
   startTimer2();
}

/**
 * \brief show values on left and right 7 segment display
 * \param left  byte and left display side
 * \param right byte and right display side
 */
void show7Segment(uint8_t left, uint8_t right)
{
   SET_PIN(SEG_CAT1);
   EXP_PORT(SEG_PORT) = right;
   _delay_ms(10);
    RESET_PIN(SEG_CAT1);
   _delay_ms(1);

   SET_PIN(SEG_CAT2);
   EXP_PORT(SEG_PORT) = left;
   _delay_ms(10);
   RESET_PIN(SEG_CAT2);
   _delay_ms(1);
}

/**
 * \brief routine to flash leds using a trigger pattern
 * \param led definition in leds_config.h
 * \param trigger pattern
 */
void flashLed(eLED led, uint16_t trigger)
{
   // only flash if pattern is met
   if(flashTrigger & trigger)
   {
      led_on(led);
   }
   else
   {
      led_off(led);
   }
}

/**
 * \brief show pattern in 3x3 matrix
 * The pattern is defined by the rows and columns selected.
 * \param rows to be selected
 * \param columns to be selected
 */
void showMatrix(uint8_t rows, uint8_t columns)
{
   EXP_PORT(MATRIX_ROW_PORT) |= (rows << MATRIX_ROW_SHIFT);
   EXP_PORT(MATRIX_COL_PORT) |= (columns << MATRIX_COL_SHIFT);
}

/**
 * \brief switch 3x3 matrix off
 */
void hideMatrix(void)
{
   EXP_PORT(MATRIX_ROW_PORT) &= ~MATRIX_ROW_MASK;
   EXP_PORT(MATRIX_COL_PORT) &= ~MATRIX_COL_MASK;
}

/**
 * \brief show the pattern defined multiplexing the rows
 * \param pattern (array index) to be shown
 * \param select row within pattern
 */
void showMatrixPattern(uint8_t pattern, uint8_t select)
{
   // remove old pattern first
   hideMatrix();
   _delay_us(100);
   // show new pattern
   showMatrix(matrixPattern[pattern][select].rows,
              matrixPattern[pattern][select].cols);
}
