/* share/avr/xmega/dma-mem/dma-mem.h */

/*
 Copyright (C) 2010 Potrepalov I.S.  potrepalov@list.ru

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
 Операции с памятью с помощью DMA.
 Операции могут выполняться одновременно с исполнением кода
 (суфикс _bg от background, возврат из функции осуществляется до
 завершения выполнения операции).
*/



#include <avr/io.h>


#undef DMA_memset
#undef DMA_memcpy
#undef DMA_memset_bg


#define isDMAfinished()		\
	( 0 == (DMA.CH3.CTRLB & (DMA_CH_CHBUSY_bm | DMA_CH_CHPEND_bm)) )

#define DMA_wait()	do { (void)0; } while ( !isDMAfinished() )


void DMA_memcpy_bg(
	unsigned char * dst,
	const unsigned char * src,
	unsigned short len );


#define DMA_memset_bg( dst, val, len )				\
	do {	unsigned char * d = (unsigned char*)(dst);	\
		unsigned char v = (val);			\
		unsigned short l = (len) - 1;			\
		*d = v;	DMA_memcpy_bg( d+1, d, l );		\
	} while (0)


#define DMA_memcpy( dst, src, len )			\
	do { DMA_memcpy_bg( (dst), (val), (len) ); DMA_wait(); } while (0)


#define DMA_memset( dst, val, len )			\
	do { DMA_memset_bg( (dst), (val), (len) ); DMA_wait(); } while (0)


/* End of file  dma-mem.h */
