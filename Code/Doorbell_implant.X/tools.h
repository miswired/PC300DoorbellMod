/* 
 * File: tools.h
 * Author: Miswired
 * 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TOOLS_H
#define	TOOLS_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include <stdint.h> //include C basic type definitions

/* Type Notes for stdint.h
 * int8_t	Signed	8	1	?27 which equals ?128	27 ? 1 which is equal to 127
 * uint8_t	Unsigned	8	1	0	28 ? 1 which equals 255
 * int16_t	Signed	16	2	?215 which equals ?32,768	215 ? 1 which equals 32,767
 * uint16_t	Unsigned	16	2	0	216 ? 1 which equals 65,535
 * int32_t	Signed	32	4	?231 which equals ?2,147,483,648	231 ? 1 which equals 2,147,483,647
 * uint32_t	Unsigned	32	4	0	232 ? 1 which equals 4,294,967,295
 * int64_t	Signed	64	8	?263 which equals ?9,223,372,036,854,775,808	263 ? 1 which equals 9,223,372,036,854,775,807
 * uint64_t	Unsigned	64	8	0	264 ? 1 which equals 18,446,744,073,709,551,615
 */


//Macros for splitting int to bytes
#define GET_LOW_BYTE(a)      *((uint8_t *)&a)
#define GET_HI_BYTE(a)       *(((uint8_t *)&a) + 1)


#endif	/* TOOLS_H */

