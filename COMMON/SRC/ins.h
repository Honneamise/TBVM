#ifndef INS_H
#define INS_H

#define NOP     0x00
#define TST     0x01
#define CALL    0x02
#define RTN     0x03
#define DONE    0x04
#define JMP     0x05
#define PRS     0x06
#define PRN     0x07
#define SPC     0x08
#define NLINE   0x09
#define NXT     0x0A
#define XFER    0x0B
#define SAV     0x0C
#define RSTR    0x0D
#define CMPR    0x0E
#define LIT     0x0F
#define INNUM   0x10
#define FIN     0x11
#define ERR     0x12
#define ADD     0x13
#define SUB     0x14
#define NEG     0x15
#define MUL     0x16
#define DIV     0x17
#define STORE   0x18
#define TSTV    0x19
#define TSTN    0x1A
#define IND     0x1B
#define LST     0x1C
#define INIT    0x1D
#define GETLINE 0x1E
#define TSTL    0x1F
#define INSRT   0x20
#define XINIT   0x21
#define MODE    0x22 //MODE : 0 = direct, 1 = run, 2 = exit

char *InsName(Uint8 code);

int InsCode(char *token);

#endif