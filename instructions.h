#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H

void SET(unsigned short * const to, unsigned short * const from);
void ADD(unsigned short * const to, unsigned short * const from);
void SUB(unsigned short * const to, unsigned short * const from);
void MUL(unsigned short * const to, unsigned short * const from);
void MLI(unsigned short * const to, unsigned short * const from);
void DIV(unsigned short * const to, unsigned short * const from);
void DVI(unsigned short * const to, unsigned short * const from);
void MOD(unsigned short * const to, unsigned short * const from);
void AND(unsigned short * const to, unsigned short * const from);
void BOR(unsigned short * const to, unsigned short * const from);
void XOR(unsigned short * const to, unsigned short * const from);
void SHR(unsigned short * const to, unsigned short * const from);
void ASR(unsigned short * const to, unsigned short * const from);
void SHL(unsigned short * const to, unsigned short * const from);
void IFB(unsigned short * const to, unsigned short * const from);
void IFC(unsigned short * const to, unsigned short * const from);
void IFE(unsigned short * const to, unsigned short * const from);
void IFN(unsigned short * const to, unsigned short * const from);
void IFG(unsigned short * const to, unsigned short * const from);
void IFA(unsigned short * const to, unsigned short * const from);
void IFL(unsigned short * const to, unsigned short * const from);
void IFU(unsigned short * const to, unsigned short * const from);
void ADX(unsigned short * const to, unsigned short * const from);
void SBX(unsigned short * const to, unsigned short * const from);
void STI(unsigned short * const to, unsigned short * const from);
void STD(unsigned short * const to, unsigned short * const from);

#endif
