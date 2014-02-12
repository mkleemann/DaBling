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
 * \file dabling.h
 *
 * \date Created: 22.01.2014 22:30:45
 * \author Matthias Kleemann
 *
 * ----------------------------------------------------------------------------
 * \section dabling_vqe23 VQE23 Pinout
 * For showing patterns on a dual 7 segment display, an old GDR made VQE23
 * (green, common cathode) was used. The pinout can be seen below and how
 * they match to the LEDs themselves.
 *
 * The display was connected with 330 Ohm resistors to the atmega port pins
 * and each block is selected by their cathode, connected via an npn transistor
 * and 1k resistor between base and select pin of the atmega, to ground.
 *
 * \code
 * As seen from below (pin side up):
 *
 * +-------------------\
 * | * * * * * * * * * |
 * | C E D K K D E C H |
 * | 1 1 1 1 2 2 2 2 2 |
 * |                   |
 * | H G A F B B F A G |
 * | 1 1 1 1 1 2 2 2 2 |
 * | * * * * * * * * * |
 * +-------------------+
 *
 * As seen from the top (flipped over):
 * /-------------------+
 * | *  --    *  --    |  H   D
 * |   |  |     |  |   |    C   E
 * |    --       --    |      G
 * |   |  |     |  |   |    B   F
 * |    --  *    --  * |      A
 * +-------------------+
 * \endcode
 *
 **/

#ifndef __DABLING_H__
#define __DABLING_H__

/**
 * \def SEG_A
 * \brief segment A of the VQE23 2x7 segment block
 * \def SEG_B
 * \brief segment B of the VQE23 2x7 segment block
 * \def SEG_C
 * \brief segment C of the VQE23 2x7 segment block
 * \def SEG_D
 * \brief segment D of the VQE23 2x7 segment block
 * \def SEG_E
 * \brief segment E of the VQE23 2x7 segment block
 * \def SEG_F
 * \brief segment F of the VQE23 2x7 segment block
 * \def SEG_G
 * \brief segment G of the VQE23 2x7 segment block
 * \def SEG_H
 * \brief segment H of the VQE23 2x7 segment block
 */
#define SEG_A D,0
#define SEG_B D,1
#define SEG_C D,2
#define SEG_D D,3
#define SEG_E D,4
#define SEG_F D,5
#define SEG_G D,6
#define SEG_H D,7

/**
 * \def SEG_PORT
 * \brief port for segments of the 7 segment block
 */
#define SEG_PORT D

/**
 * \def SEG_CAT1
 * \brief common cathode (select) for 1st 7 segment block
 * \def SEG_CAT2
 * \brief common cathode (select) for 2nd 7 segment block
 */
#define SEG_CAT1 B,6
#define SEG_CAT2 B,7

/**
 * \def SEG_NUM_0
 * \brief pattern for number 0
 * \def SEG_NUM_1
 * \brief pattern for number 1
 * \def SEG_NUM_2
 * \brief pattern for number 2
 * \def SEG_NUM_3
 * \brief pattern for number 3
 * \def SEG_NUM_4
 * \brief pattern for number 4
 * \def SEG_NUM_5
 * \brief pattern for number 5
 * \def SEG_NUM_6
 * \brief pattern for number 6
 * \def SEG_NUM_7
 * \brief pattern for number 7
 * \def SEG_NUM_8
 * \brief pattern for number 8
 * \def SEG_NUM_9
 * \brief pattern for number 9
 * \def SEG_NUM_A
 * \brief pattern for character A
 * \def SEG_NUM_B
 * \brief pattern for character B
 * \def SEG_NUM_C
 * \brief pattern for character C
 * \def SEG_NUM_D
 * \brief pattern for character D
 * \def SEG_NUM_E
 * \brief pattern for character E
 * \def SEG_NUM_F
 * \brief pattern for character F
 */
#define SEG_NUM_0 0x3F // 0b00111111
#define SEG_NUM_1 0x06 // 0b00000110
#define SEG_NUM_2 0x5B // 0b01011011
#define SEG_NUM_3 0x4F // 0b01001111
#define SEG_NUM_4 0x66 // 0b01100110
#define SEG_NUM_5 0x6D // 0b01101101
#define SEG_NUM_6 0x7D // 0b01111101
#define SEG_NUM_7 0x07 // 0b00000111
#define SEG_NUM_8 0x7F // 0b01111111
#define SEG_NUM_9 0x6F // 0b01101111
#define SEG_NUM_A 0x77 // 0b01110111
#define SEG_NUM_B 0x4F // 0b01001111
#define SEG_NUM_C 0x39 // 0b00111001
#define SEG_NUM_D 0x0F // 0b00001111
#define SEG_NUM_E 0x79 // 0b01111001
#define SEG_NUM_F 0x71 // 0b01110001

/**
 * \def SEG_DP
 * \brief digital point of 7 segment block
 */
#define SEG_DP    0x80 // 0b10000000

/**
 * \def MATRIX_ROW_PORT
 * \brief port for the matrix' row pins
 * \def MATRIX_COL_PORT
 * \brief port for the matrix' column pins
 */
#define MATRIX_ROW_PORT B
#define MATRIX_COL_PORT C

/**
 * \def MATRIX_ROW_MASK
 * \brief mask for the matrix' row pins
 * \def MATRIX_COL_MASK
 * \brief mask for the matrix' column pins
 */
#define MATRIX_ROW_MASK ((1 << PINB5) | (1 << PINB4) | (1 << PINB3))
#define MATRIX_COL_MASK ((1 << PINC3) | (1 << PINC2) | (1 << PINC1))

/**
 * \def MATRIX_ROW_SHIFT
 * \brief shift width to easily access the matrix' row pins
 * \def MATRIX_COL_SHIFT
 * \brief shift width to easily access the matrix' columns pins
 */
#define MATRIX_ROW_SHIFT 3
#define MATRIX_COL_SHIFT 1

/**
 * \def MATRIX_MAX_ROW
 * \brief maximum number of row pins
 * \def MATRIX_MAX_COLUMN
 * \brief maximum number of column pins
 */
#define MATRIX_MAX_ROW    3
#define MATRIX_MAX_COLUMN 3

/**
 * \brief initialization for hardware
 * The port and pins for the 7 segment displays are setup as
 * well as timer(s) and adc.
 */
void initHardware(void);

/**
 * \brief show values on left and right 7 segment display
 * \param left  byte and left display side
 * \param right byte and right display side
 */
void show7Segment(uint8_t left, uint8_t right);

/**
 * \brief routine to flash leds using a trigger pattern
 * \param led definition in leds_config.h
 * \param trigger pattern
 */
void flashLed(eLED led, uint16_t trigger);

/**
 * \brief show pattern in 3x3 matrix
 * The pattern is defined by the rows and columns selected.
 * \param rows to be selected
 * \param columns to be selected
 */
void showMatrix(uint8_t rows, uint8_t columns);

/**
 * \brief switch 3x3 matrix off
 */
void hideMatrix(void);

/**
 * \brief show the pattern defined multiplexing the rows
 * \param pattern (array index) to be shown
 * \param select row within pattern
 */
void showMatrixPattern(uint8_t pattern, uint8_t select);

/**
 * \brief change matrix pattern to next
 */
void nextMatrixPattern(void);

/**
 * \brief set AVR to sleep
 *
 * In this special case, only the idle mode is possible. We don't use an external
 * clock, so we need the internal clock source for timer 2 wakeup call. External
 * interrupts are also blocked.
 *
 * \code
 * SM2 SM1 SM0 Sleep Mode
 *   0   0   0 Idle
 *   0   0   1 ADC Noise Reduction
 *   0   1   0 Power-down
 *   0   1   1 Power-save
 *   1   1   0 Standby
 * \endcode
 *
 * AVR enters sleep mode and also wakes up in this state, so some intial
 * steps to set wakeup interrupt need to be done here.
 *
 * The three \c _NOP(); instructions are a safety, since older AVRs may
 * skip the next couple of instructions after sleep mode.
 */
void sleepNow(void);

#endif

