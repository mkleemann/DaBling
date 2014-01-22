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
 * \def SEG_CAT1
 * \brief common cathode (select) for 1st 7 segment block
 * \def SEG_CAT2
 * \brief common cathode (select) for 2nd 7 segment block
 */
#define SEG_CAT1 B,7
#define SEG_CAT2 B,6

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
 */
#define SEG_NUM_0 0b00111111
#define SEG_NUM_1 0b00000110
#define SEG_NUM_2 0b01011011
#define SEG_NUM_3 0b01001111
#define SEG_NUM_4 0b01100110
#define SEG_NUM_5 0b01101101
#define SEG_NUM_6 0b01111101
#define SEG_NUM_7 0b00000111
#define SEG_NUM_8 0b01111111
#define SEG_NUM_9 0b01101111

/**
 * \def SEG_DP
 * \brief digital point of 7 segment block
 */
#define SEG_DP    0b10000000




#endif
