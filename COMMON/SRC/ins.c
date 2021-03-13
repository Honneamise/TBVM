#include "commons.h"

/**********/
char *InsName(Uint8 code)
{
	if(code==0x00) { return "NOP"; };
	if(code==0x01) { return "TST"; };
	if(code==0x02) { return "CALL"; };
	if(code==0x03) { return "RTN"; };
	if(code==0x04) { return "DONE"; };
	if(code==0x05) { return "JMP"; };
	if(code==0x06) { return "PRS"; };
	if(code==0x07) { return "PRN"; };
	if(code==0x08) { return "SPC"; };
	if(code==0x09) { return "NLINE"; };
	if(code==0x0A) { return "NXT"; };
	if(code==0x0B) { return "XFER"; };
	if(code==0x0C) { return "SAV"; };
	if(code==0x0D) { return "RSTR"; };
	if(code==0x0E) { return "CMPR"; };
	if(code==0x0F) { return "LIT"; };
	if(code==0x10) { return "INNUM"; };
	if(code==0x11) { return "FIN"; };
	if(code==0x12) { return "ERR"; };
	if(code==0x13) { return "ADD"; };
	if(code==0x14) { return "SUB"; };
	if(code==0x15) { return "NEG"; };
	if(code==0x16) { return "MUL"; };
	if(code==0x17) { return "DIV"; };
	if(code==0x18) { return "STORE"; };
	if(code==0x19) { return "TSTV"; };
	if(code==0x1A) { return "TSTN"; };
	if(code==0x1B) { return "IND"; };
	if(code==0x1C) { return "LST"; };
	if(code==0x1D) { return "INIT"; };
	if(code==0x1E) { return "GETLINE"; };
	if(code==0x1F) { return "TSTL"; };
	if(code==0x20) { return "INSRT"; };
	if(code==0x21) { return "XINIT"; };
	if(code==0x22) { return "MODE"; };

	return NULL;
}

/**********/
int InsCode(char *token)
{
	if(strcmp(token,"NOP")==0)      { return 0x00; };
	if(strcmp(token,"TST")==0)      { return 0x01; };
	if(strcmp(token,"CALL")==0)     { return 0x02; };
	if(strcmp(token,"RTN")==0)      { return 0x03; };
	if(strcmp(token,"DONE")==0)     { return 0x04; };
	if(strcmp(token,"JMP")==0)      { return 0x05; };
	if(strcmp(token,"PRS")==0)      { return 0x06; };
	if(strcmp(token,"PRN")==0)      { return 0x07; };
	if(strcmp(token,"SPC")==0)      { return 0x08; };
	if(strcmp(token,"NLINE")==0)    { return 0x09; };
	if(strcmp(token,"NXT")==0)      { return 0x0A; };
	if(strcmp(token,"XFER")==0)     { return 0x0B; };
	if(strcmp(token,"SAV")==0)      { return 0x0C; };
	if(strcmp(token,"RSTR")==0)     { return 0x0D; };
	if(strcmp(token,"CMPR")==0)     { return 0x0E; };
	if(strcmp(token,"LIT")==0)      { return 0x0F; };
	if(strcmp(token,"INNUM")==0)    { return 0x10; };
	if(strcmp(token,"FIN")==0)      { return 0x11; };
	if(strcmp(token,"ERR")==0)      { return 0x12; };
	if(strcmp(token,"ADD")==0)      { return 0x13; };
	if(strcmp(token,"SUB")==0)      { return 0x14; };
	if(strcmp(token,"NEG")==0)      { return 0x15; };
	if(strcmp(token,"MUL")==0)      { return 0x16; };
	if(strcmp(token,"DIV")==0)      { return 0x17; };
	if(strcmp(token,"STORE")==0)    { return 0x18; };
	if(strcmp(token,"TSTV")==0)     { return 0x19; };
	if(strcmp(token,"TSTN")==0)     { return 0x1A; };
	if(strcmp(token,"IND")==0)      { return 0x1B; };
	if(strcmp(token,"LST")==0)      { return 0x1C; };
	if(strcmp(token,"INIT")==0)     { return 0x1D; };
	if(strcmp(token,"GETLINE")==0)  { return 0x1E; };
	if(strcmp(token,"TSTL")==0)     { return 0x1F; };
	if(strcmp(token,"INSRT")==0)   	{ return 0x20; };
	if(strcmp(token,"XINIT")==0)    { return 0x21; };
	if(strcmp(token,"MODE")==0)     { return 0x222; };

	return -1;
}
