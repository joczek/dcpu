#include <stdio.h>

void MUL(unsigned short to, unsigned short from);
void ADD(unsigned short to, unsigned short from);
void SUB(unsigned short to, unsigned short from);
void MLI(unsigned short to, unsigned short from);
void DIV(unsigned short to, unsigned short from);
void DVI(unsigned short to, unsigned short from);
void SHR(unsigned short to, unsigned short from);
void ASR(unsigned short to, unsigned short from);
int main()
{
	unsigned short to;
	unsigned short from;
	to = -13; 
	from = 2;
	ASR(to, from);
//	SHR(to, from);
//	DIV(to, from);
//	MUL(to, from);
//	ADD(to, from);
//	SUB(to, from);
}

void MUL(unsigned short to, unsigned short from)
{
	unsigned int ret;
	unsigned short low, high;
	ret = to * from;
	
	low = ret;
	high = ret >> 16;

	printf("low: %X   high: %X\n", low, high);
}

void ADD(unsigned short to, unsigned short from)
{
	unsigned short overflow;
	to += from;
	overflow = (to < from) ? 0x1 : 0;
	printf("result: %X   overflow %X", to, overflow);

}

void SUB(unsigned short to, unsigned short from)
{
	unsigned short underflow;
	underflow = (from > to) ? 0xFFFF : 0x0;
	to -= from;
	printf("result: %X   underflow %X", to, underflow);
}

void MLI(unsigned short to, unsigned short from)
{
	int ret;
	unsigned short low, high;
	ret = (short)to * (short)from;
	
	low = ret;
	high = ret >> 16;

	printf("low: %X   high: %X\n", low, high);
}

void DIV(unsigned short to, unsigned short from)
{
	unsigned int ret;
	unsigned short low, high;

	if (from == 0)
		low = high = 0;
	else {
		ret = to / from;
		low = ret;
		ret = ((unsigned int)to << 16) / from;
		high = ret & 0xFFFF;
	}
	printf("low: %X   high: %X\n", low, high);
}

void DVI(unsigned short to, unsigned short from)
{
	int ret;
	unsigned short low, high;

	if (from == 0)
		low = high = 0;
	else {
		ret = (short)to / (short)from;
		low = ret;
		ret = ((int)to << 16) / (short)from;
		high = ret & 0xFFFF;
	}
	printf("low: %X   high: %X\n", low, high);
}

void SHR(unsigned short to, unsigned short from)
{
	int ret;
	unsigned short low, high;
	
	low = (to >> from);
	high = (((unsigned int)(to) << 16) >> from) & 0xFFFF;
	printf("low: %X   high: %X\n", low, high);
}

void ASR(unsigned short to, unsigned short from)
{
	int ret;
	unsigned short low, high;
	high = 0;
	low = (short)to >> from;

	high = (((unsigned int)(to) << 16) >> from) & 0xFFFF;
	printf("low: %X   high: %X\n", low, high);
}
