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
	// �������� ���� ������ � ������� DMA (����� 3)
	// ������������ � ����������� ����.
	// ��� ��������� �������� �� ������������� ������ �������,
	// �������� len ������ ���� ���������� �������.
	// dst -- ����� ���� ����������
	// src -- ����� ������ ����������
	// len -- ���-�� ������ ��� �����������
	// ����������� ���������� � ������� ������� � ����������� ��
	// ������ ����� �� ���.

	// ��� ���������� ����������
	DMA_wait();

	// ��������� ������ ������
	DMA.CH3.CTRLA = 0;
	while ( DMA.CH3.CTRLA & DMA_CH_ENABLE_bm ) (void)0;

	DMA.CH3.CTRLA = DMA_CH_RESET_bm;
	while ( DMA.CH3.CTRLA & DMA_CH_RESET_bm ) (void)0;

	// ��������� ������ ������, �������� �� ������ �����
	DMA.CH3.CTRLA = DMA_CH_ENABLE_bm | DMA_CH_BURSTLEN_1BYTE_gc;
	// ��������� ���������� ������
//	DMA.CH3.CTRLB = DMA_CH_ERRINTLVL_OFF_gc | DMA_CH_TRNINTLVL_OFF_gc;
	// ����������� ������
	// �������: dst, ���������������
	// ��������: src, ���������������
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

	// �������� ���������
	DMA.CH3.CTRLA = DMA_CH_ENABLE_bm | DMA_CH_BURSTLEN_1BYTE_gc
		| DMA_CH_TRFREQ_bm;
} // DMA_memcpy_bg


/* End of file  dma-mem.c */
