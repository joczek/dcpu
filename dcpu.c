#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "instructions.h"
#include "opcodes.h"
#include "queue.h"

unsigned short *find_to(unsigned char b);
unsigned short *find_from(unsigned char b);
void execute_instruction(unsigned char opcode, unsigned short *to, unsigned short *from);

unsigned short registers[12];
unsigned short *A = registers + 0;
unsigned short *B = registers + 1;
unsigned short *C = registers + 2;
unsigned short *X = registers + 3;
unsigned short *Y = registers + 4;
unsigned short *Z = registers + 5;
unsigned short *I = registers + 6;
unsigned short *J = registers + 7;
unsigned short *SP = registers +8;
unsigned short *PC = registers + 9;
unsigned short *EX = registers + 10;
unsigned short *IA = registers + 11;
unsigned short mem[0x10000];

unsigned short literal_holder;

int cycles_total;
int cycles;
int running;
struct queue *interrupts;
int interrupt_queueing;

void init_registers(void)
{
	int i;
	for (i = 0; i < 12; i++)
		registers[i] = 0;
}

void display_registers(void)
{
	printf("     hex        dec\n");
	printf("A:  0x%04X      %d\n", *A, *A);
	printf("B:  0x%04X      %d\n", *B, *B);
	printf("C:  0x%04X      %d\n", *C, *C);
	printf("X:  0x%04X      %d\n", *X, *X);
	printf("Y:  0x%04X      %d\n", *Y, *Y);
	printf("Z:  0x%04X      %d\n", *Z, *Z);
	printf("I:  0x%04X      %d\n", *I, *I);
	printf("J:  0x%04X      %d\n", *J, *J);
	printf("SP: 0x%04X      %d\n", *SP, *SP);
	printf("PC: 0x%04X      %d\n", *PC, *PC);
	printf("EX: 0x%04X      %d\n", *EX, *EX);
}

void load_unstructions()
{
	mem[0x0000] = 0x0022;
}

int main()
{
	unsigned short *from, *to;
	unsigned short instruction;
	unsigned char opcode, a, b;

	interrupts = make_queue();
	interrupt_queueing = 0;
	init_registers();
	running = 1;
	cycles_total = 0;

	while (running) {
		cycles = 0;
		display_registers();
		instruction = mem[(*PC)++];
		opcode = instruction & 0x1F;
		b = (instruction & 0x3E0) >> 5;
		a = (instruction & 0xFC00) >> 10;
	
		if (opcode == OP_SPECIAL)
			goto OP_SPECIAL_HANDLER;

		to = find_to(b);
		from = find_from(a);

		execute_instruction(opcode, to, from);
	
	OP_SPECIAL_HANDLER:
		if (b == OP_JSR) { }
		else if (b == OP_INT) { }
		else if (b == OP_IAG) { }
		else if (b == OP_IAS) { }
		else if (b == OP_RFI) { }
		else if (b == OP_IAQ) { }
		else if (b == OP_HWN) { }
		else if (b == OP_HWQ) { }
		else if (b == OP_HWI) { } 
	
		printf("%d\n", running);
	}
	return 0;
}

void JSR(unsigned short *a)
{
	mem[(*SP)--] = *PC;
	*PC = *a;
}

void IAG(unsigned short *a)
{
	*a = *IA;
}

void IAS(unsigned short *a)
{
	*IA = *a;
}

void RFI(unsigned short *a)
{
	
	*A = mem[*(SP)++];
	*PC = mem[*(SP)++];
}

void IAQ(unsigned short *a)
{
	interrupt_queuing = *a;
}

void HWN(unsigned short *a)
{
	*a = 0;
}

void HWQ(unsigned short *a)
{
	// sets A, B, C, X, Y to info about hardware 'a'
}

void HWI(unsigned short *a)
{
	// sends an interrupt to hardware a
}

void execute_instruction(unsigned char opcode, unsigned short *to, unsigned short *from)
{
	if (opcode == OP_SET) SET(to, from);
	else if (opcode == OP_ADD) ADD(to, from);
	else if (opcode == OP_SUB) SUB(to, from);
	else if (opcode == OP_MUL) MUL(to, from);
	else if (opcode == OP_MLI) MLI(to, from);
	else if (opcode == OP_DIV) DIV(to, from);	
	else if (opcode == OP_DVI) DVI(to, from);
	else if (opcode == OP_MOD) MOD(to, from);
	else if (opcode == OP_AND) AND(to, from);
	else if (opcode == OP_BOR) BOR(to, from);
	else if (opcode == OP_XOR) XOR(to, from);
	else if (opcode == OP_SHR) SHR(to, from);
	else if (opcode == OP_ASR) ASR(to, from);
	else if (opcode == OP_SHL) SHL(to, from);
	else if (opcode == OP_IFB) IFB(to, from);
	else if (opcode == OP_IFC) IFC(to, from);
	else if (opcode == OP_IFE) IFE(to, from);
	else if (opcode == OP_IFN) IFN(to, from);
	else if (opcode == OP_IFG) IFG(to, from);
	else if (opcode == OP_IFA) IFA(to, from);
	else if (opcode == OP_IFL) IFL(to, from);
	else if (opcode == OP_IFU) IFU(to, from);
	else if (opcode == OP_ADX) ADX(to, from);
	else if (opcode == OP_SBX) SBX(to, from);
	else if (opcode == OP_STI) STI(to, from);
	else if (opcode == OP_STD) STD(to, from);
}

unsigned short *find_to(unsigned char b)
{
	unsigned short *to;
	if (b >= 0x00 && b <= 0x07) {
		/* register value (A, B, C, X, Y, Z, I, J) */
		to = registers + b;
	} else if (b >= 0x08 && b <= 0x0f) {
		/* [register] */
		to = mem + registers[b - 0x08];
	} else if (b >= 0x10 && b <= 0x17) { 
		/* [register + next word] */
		to = mem + registers[b - 0x10] + mem[(*PC)++];
		cycles++;
	} else if (b == 0x18) {
		/* PUSH / [--SP] */
		to = mem + --(*SP);
	} else if (b == 0x19) {
		/* PEEK / [SP] */
		to = mem + *SP;
	} else if (b == 0x1A) {
		/* [SP + next word] / PICK */
		to = mem + *SP + mem[(*PC)++];
		cycles++;
	} else if (b == 0x1B) {
		/* SP */
		to = SP;
	} else if (b == 0x1C) {
		/* PC */
		to = PC;
	} else if (b == 0x1D) {
		/* EX */
		to = EX;
	} else if (b == 0x1E) {
		/* [next word] */
		to = mem + mem[(*PC)++];
		cycles++;
	} else if (b == 0x1F) {
		/* next word - literal */
		literal_holder = mem[(*PC)++];
		to = &literal_holder; 
		cycles++;
	} else if (b >= 0x20 && b <= 0x3f) {
		printf("Abort: constant issue in b \n");	
		exit(EXIT_FAILURE);
	}
	return to;
}

unsigned short *find_from(unsigned char a)
{
	unsigned short *from;
	if (a >= 0x00 && a <= 0x07) {
		/* register value (A, B, C, X, Y, Z, I, J) */
		from = registers + a;
	} else if (a >= 0x08 && a <= 0x0f) {
		/* [register] */
		from = mem + registers[a - 0x08];
	} else if (a >= 0x10 && a <= 0x17) { 
		/* [register + next word] */
		from = mem + registers[a - 0x10] + mem[(*PC)++];
		cycles++;
	} else if (a == 0x18) {
		/* POP / [SP++] */
		from = mem + ++(*SP);
	} else if (a == 0x19) {
		/* PEEK / [SP] */
		from = mem + *SP;
	} else if (a == 0x1A) {
		/* [SP + next word] / PICK */
		from = mem + *SP + mem[(*PC)++];
		cycles++;
	} else if (a == 0x1B) {
		/* SP */
		from = SP;
	} else if (a == 0x1C) {
		/* PC */
		from = PC;
	} else if (a == 0x1D) {
		/* EX */
		from = EX;
	} else if (a == 0x1E) {
		/* [next word] */
		from = mem + mem[(*PC)++];
		cycles++;
	} else if (a == 0x1F) {
		/* next word - literal */
		from = mem + *(PC)++; 
		cycles++;
	} else if (a >= 0x20 && a <= 0x3f) {
		literal_holder = a - 0x21;
		from = &literal_holder;
	}
	return from;
}

void skip_instruction()
{
	int word_count = 1;
	unsigned short instruction = mem[*PC];
	unsigned char opcode = instruction & 0x1F;
	unsigned char b = (instruction & 0x3E0) >> 5;
	unsigned char a = (instruction & 0xFC00) >> 10;
	if (opcode) {
		if ((b >= 0x10 && b <= 0x17) || b == 0x1e || b == 0x1f)
			word_count++;
	
		if ((a >= 0x10 && a <= 0x17) || a == 0x1e || a == 0x1f)
			word_count++;
	}
	*PC += word_count;
}

void SET(unsigned short * const to, unsigned short * const from)
{
	*to = *from;
}

void ADD(unsigned short * const to, unsigned short * const from)
{
	*to += *from;
	*EX = (*to < *from) ? 0x0001 : 0x0000;
}

void SUB(unsigned short * const to, unsigned short * const from)
{
	*EX = (*from > *to) ? 0xFFFF : 0x0000;
	*to -= *from;
}

void MUL(unsigned short * const to, unsigned short * const from)
{
	unsigned int res;
	res = (unsigned int)(*to) * (unsigned int)(*from);
	*to = 0xFFFF & res;
	*EX = res >> 16;
}

void MLI(unsigned short * const to, unsigned short * const from)
{
	int res;
	res = (short)(*to) * (short)(*from);
	*to =  res;
	*EX = (res >> 16);
}

void DIV(unsigned short * const to, unsigned short * const from)
{
	if (*from == 0)
		*to = *EX = 0x0000;
	else {
		*EX = ((unsigned int)(*to) << 16) / *from;
		*to /= *from;
	}	
}	

void DVI(unsigned short * const to, unsigned short * const from)
{
	int res;
	if (*from == 0)
		*to = *EX = 0x0000;
	else {
		*EX = ((int)(*to) << 16) / (short)(*from);
		*to = (short)(*to) / (short)(*from);
	}
}

void MOD(unsigned short * const to, unsigned short * const from)
{
	*to = (*from) ? (*to % *from) : 0;
}

void AND(unsigned short * const to, unsigned short * const from)
{
	*to &= *from;
}

void BOR(unsigned short * const to, unsigned short * const from)
{
	*to |= *from;
}

void XOR(unsigned short * const to, unsigned short * const from)
{
	*to ^= *from;
}

void SHR(unsigned short * const to, unsigned short * const from)
{
	*EX = (((unsigned int)(*to) << 16) >> *from) & 0xFFFF; 	
	*to = *to >> *from;
}

void ASR(unsigned short * const to, unsigned short * const from)
{
	*EX = (((unsigned int)(*to) << 16) >> *from) & 0xFFFF;
	*to = (short)(*to) >> *from;
}

void SHL(unsigned short * const to, unsigned short * const from)
{
	*EX = (((unsigned int)(*to) << *from) >> 16) & 0xFFFF;
	*to = *to << *from;
}

void IFB(unsigned short * const to, unsigned short * const from)
{
	if (!(*to & *from))
		skip_instruction();
}

void IFC(unsigned short * const to, unsigned short * const from)
{
	if (*to & *from)
		skip_instruction();
}

void IFE(unsigned short * const to, unsigned short * const from)
{
	if (*to != *from)
		skip_instruction();
}

void IFN(unsigned short * const to, unsigned short * const from)
{
	if (*to == *from)
		skip_instruction();
}

void IFG(unsigned short * const to, unsigned short * const from)
{
	if (*to <= *from)
		skip_instruction();
}

void IFA(unsigned short * const to, unsigned short * const from)
{
	if ((signed short)(*to) <= (signed short)(*from))
		skip_instruction();
}

void IFL(unsigned short * const to, unsigned short * const from)
{
	if (*to >= *from)
		skip_instruction();
}

void IFU(unsigned short * const to, unsigned short * const from)
{
	if ((signed short)(*to) >= (signed short)(*from))
		skip_instruction();
}

void ADX(unsigned short * const to, unsigned short * const from)
{
	unsigned short res;
	res = *to + *from;
	if (res < *to) {
		*EX = 0x0001;
		*to = res + *EX;
	} else {
		res += *EX;
		*EX = (res < *EX) ? 0x0001 : 0x0000;
		*to = res;
	}
}

void SBX(unsigned short * const to, unsigned short * const from)
{
	unsigned short res;

	res = *to - *from;
	if (res > *to)
		*EX = 0xFFFF;
	else
		*EX = (*EX > res) ? 0xFFFF : 0x0000;

	*to = res - *EX;
}


void STI(unsigned short * const to, unsigned short * const from)
{
	*to = *from;
	(*I)++;
	(*J)++;
}

void STD(unsigned short * const to, unsigned short * const from)
{
	*to = *from;
	(*I)--;
	(*J)--;
}
