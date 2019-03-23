/* share/avr/xmega/dma-mem/dma-mem.c */


#include "dma-mem.h"


void
DMA_memcpy_bg(
	unsigned char * dst,
	const unsigned char * src,
	unsigned short len )
{
	// �������� ���� ������ � ������� DMA (����� 3)
	// ������������ � ���������� ����.
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
