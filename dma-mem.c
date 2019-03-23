/* share/avr/xmega/dma-mem/dma-mem.c */

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


#include "dma-mem.h"


void
DMA_memcpy_bg(
	unsigned char * dst,
	const unsigned char * src,
	unsigned short len )
{
	//  опирует блок пам€ти с помощью DMA (канал 3)
	// одновременно с исполнением кода.
	// ƒл€ получени€ выигрыша от использовани€ данной функции,
	// значение len должно быть достаточно большим.
	// dst -- адрес куда копировать
	// src -- адрес откуда копировать
	// len -- кол-во байтов дл€ копировани€
	//  опирование начинаетс€ с младших адресов и выполн€етс€ по
	// одному байту за раз.

	// ждЄм завершени€ транзакции
	DMA_wait();

	// запрещаем работу канала
	DMA.CH3.CTRLA = 0;
	while ( DMA.CH3.CTRLA & DMA_CH_ENABLE_bm ) (void)0;

	DMA.CH3.CTRLA = DMA_CH_RESET_bm;
	while ( DMA.CH3.CTRLA & DMA_CH_RESET_bm ) (void)0;

	// разрешаем работу канала, передача по одному байту
	DMA.CH3.CTRLA = DMA_CH_ENABLE_bm | DMA_CH_BURSTLEN_1BYTE_gc;
	// запрещаем прерывани€ канала
//	DMA.CH3.CTRLB = DMA_CH_ERRINTLVL_OFF_gc | DMA_CH_TRNINTLVL_OFF_gc;
	// настраиваем адреса
	// приЄмник: dst, инкрементальный
	// источник: src, инкрементальный
	DMA.CH3.ADDRCTRL = DMA_CH_SRCRELOAD_NONE_gc | DMA_CH_SRCDIR_INC_gc
		| DMA_CH_DESTRELOAD_NONE_gc | DMA_CH_DESTDIR_INC_gc;

	DMA.CH3.TRFCNT = len;
	DMA.CH3.REPCNT = 1;
	DMA.CH3.SRCADDR0 = (unsigned char) ((unsigned short)src);
	DMA.CH3.SRCADDR1 = ((unsigned short)src) >> 8;
	DMA.CH3.SRCADDR2 = 0;
	DMA.CH3.DESTADDR0 = (unsigned char) ((unsigned short)dst);
	DMA.CH3.DESTADDR1 = ((unsigned short)dst) >> 8;
	DMA.CH3.DESTADDR2 = 0;

	// начинаем пересылку
	DMA.CH3.CTRLA = DMA_CH_ENABLE_bm | DMA_CH_BURSTLEN_1BYTE_gc
		| DMA_CH_TRFREQ_bm;
} // DMA_memcpy_bg


/* End of file  dma-mem.c */
