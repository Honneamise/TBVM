#include "ILM_headers.h"

/**********/
void ILM_reset(ILM *IL)
{
    IL->PC = 0x00;
    
    IL->SP = 0x00;

    IL->LINES_SP = 0x00;

    IL->AESTK_SP = 0x00;

    IL->CURSOR = 0x00;

    IL->CURSOR_LINE = 0x00;
    
    for(int i=0;i<STACK_SIZE;i++)
    {
        IL->STACK[i] = 0x00;
    }
	
    ILM_clear_line(IL->LBUF);

    for(int i=0;i<MAX_LINES;i++)
    {
        ILM_clear_line(IL->LINES[i]);
    }

    for(int i=0;i<LINES_STACK_SIZE;i++)
    {
        IL->LINES_STACK[i] = 0x00;
    }

    for(int i=0;i<AESTK_SIZE;i++)
    {
        IL->AESTK[i] = 0x00;
    }

    for(int i=0;i<VARIABLES_SIZE;i++)
    {
        IL->VARIABLES[i] = 0x00;
    }
}

/**********/
void ILM_execute(ILM *IL)
{
    while(IL->VMODE!=2)
    {
        //printf("[%04X] %02X %s\n", IL->PC, IL->MEM[IL->PC], InsName(IL->MEM[IL->PC]));

        ILM_execute_ins(IL);
    }
}

/**********/
void ILM_execute_ins(ILM *IL)
{
    switch(IL->MEM[IL->PC])
    {
        case NOP: 
            ILM_NOP(IL);
            break;
        case TST: 
            ILM_TST(IL);
            break;     
        case CALL:
            ILM_CALL(IL); 
            break;
        case RTN: 
            ILM_RTN(IL);
            break;
        case DONE: 
            ILM_DONE(IL);
            break;
        case JMP: 
            ILM_JMP(IL);
            break;
        case PRS: 
            ILM_PRS(IL);
            break;
        case PRN: 
            ILM_PRN(IL);
            break;
        case SPC: 
            ILM_SPC(IL);
            break;
        case NLINE: 
            ILM_NLINE(IL);
            break;
        case NXT: 
            ILM_NXT(IL);
            break;
        case XFER:
            ILM_XFER(IL);
            break;
        case SAV: 
            ILM_SAV(IL);
            break;
        case RSTR: 
            ILM_RSTR(IL);
            break;
        case CMPR: 
            ILM_CMPR(IL);
            break;
        case LIT: 
            ILM_LIT(IL);
            break;
        case INNUM: 
            ILM_INNUM(IL);
            break;
        case FIN: 
            ILM_FIN(IL);
            break;
        case ERR: 
            ILM_ERR(IL);
            break;
        case ADD: 
            ILM_ADD(IL);
            break;
        case SUB: 
            ILM_SUB(IL);
            break;
        case NEG: 
            ILM_NEG(IL);
            break;
        case MUL: 
            ILM_MUL(IL);
            break;
        case DIV: 
            ILM_DIV(IL);
            break;
        case STORE: 
            ILM_STORE(IL);
            break;
        case TSTV: 
            ILM_TSTV(IL);
            break;
        case TSTN: 
            ILM_TSTN(IL);
            break;
        case IND: 
            ILM_IND(IL);
            break;
        case LST: 
            ILM_LST(IL);
            break;
        case INIT: 
            ILM_INIT(IL);
            break;
        case GETLINE: 
            ILM_GETLINE(IL);
            break;
        case TSTL: 
            ILM_TSTL(IL);
            break;
        case INSRT: 
            ILM_INSRT(IL);
            break;
        case XINIT: 
            ILM_XINIT(IL);
            break;
        case MODE: 
            ILM_MODE(IL);
            break;

        default:
            ExitError("[%s]: Uknow instruction : %02X\n",__func__, IL->MEM[IL->PC]);
            exit(EXIT_FAILURE);
            break;
    }
}

/**********/
void ILM_NOP(ILM *IL)
{
    // no op 
    printf("\n[!!!] REACHED NOP INSTRUCTION, PROGRAM HALTED !!!\n");

    DisableDebug();

    exit(EXIT_FAILURE);
}

/**********/
void ILM_TST(ILM *IL)
{
    //delete leading blanks
    //If string matches the BASIC line, advance cursor over the matched string and execute the next IL instruction.
    //If a match fails, execute the IL instruction at the labled lbl.

    //retrieve keyword from memory
    int tp = IL->PC + 3;

    int len = IL->MEM[tp];

    char *str = Calloc(len+1,sizeof(char));

    tp++;

    for(int i=0;i<len;i++)
    {
        str[i] = (char)IL->MEM[tp+i];        
    }

    //token
    if(ILM_skip_spaces(IL)<0) { ILM_error(IL, "Line error"); return; }

    int start = IL->CURSOR;

    int end = start + len;

    char *token = Calloc(end-start+1,sizeof(char));
    strncpy(token,IL->LINES[IL->CURSOR_LINE] + start, end-start);

    if(strcmp(token,str)==0)
    {
        IL->PC += 2 + 1 + len + 1;

        IL->CURSOR = end;
    }
    else//match fail, go to label
    {
        IL->PC = (IL->MEM[IL->PC+1] << 8 ) + IL->MEM[IL->PC+2];
    }

    Free(token);
    Free(str);
}

/**********/
void ILM_CALL(ILM *IL)
{
    //Execute the IL subroutine starting at lbl. 
    //Save the IL address following the CALL on the control stack.

    IL->STACK[IL->SP] = IL->PC + 3;

    IL->SP++;

    IL->PC = (IL->MEM[IL->PC+1] << 8 ) + IL->MEM[IL->PC+2];
}

/**********/
void ILM_RTN(ILM *IL)
{
    //Return to the IL location specified by the top of the control stack.

    IL->SP--;
    
    IL->PC = IL->STACK[IL->SP];  
}

/**********/
void ILM_DONE(ILM *IL)
{
    //Report a syntax error if after deletion leading blanks the cursor is not positioned to road a carriage return.

    ILM_skip_spaces(IL);

    if(IL->LINES[IL->CURSOR_LINE][IL->CURSOR]!='\n') { ILM_error(IL, "Line error"); return; }

    IL->PC++;
}

/**********/
void ILM_JMP(ILM *IL)
{
    //Continue execution of IL at the line specified.

    IL->PC = (IL->MEM[IL->PC+1] << 8 ) + IL->MEM[IL->PC+2];
}

/**********/
void ILM_PRS(ILM *IL)
{
    //Print characters from the BASIC text up to but not including the closing quote mark. 
    //If a cr is found in the program text, report an error. 
    //Move the cursor to the point following the closing quote.

    while(IL->LINES[IL->CURSOR_LINE][IL->CURSOR]!='"')
    {
        if(IL->LINES[IL->CURSOR_LINE][IL->CURSOR]=='\n')
        {
            ILM_error(IL,"Missing \"");
            return;
        }
        printf("%c",IL->LINES[IL->CURSOR_LINE][IL->CURSOR]);
        IL->CURSOR++;
    }

    IL->CURSOR++;

    IL->PC++;

}

/**********/
void ILM_PRN(ILM *IL)
{
    //Print number obtained by popping the top of the expression stack.

    IL->AESTK_SP--;
    
    int val = IL->AESTK[IL->AESTK_SP];

    printf("%d",val);

    IL->PC++;
}

/**********/
void ILM_SPC(ILM *IL)
{
    //Insert spaces, to move the print head to next zone.

    printf(" ");

    IL->PC++;
}

/**********/
void ILM_NLINE(ILM *IL)
{
    //Output CRLF to Printer.

    printf("\n");

    IL->PC++;
}

/**********/
void ILM_NXT(ILM *IL)
{
    //If the present mode is direct (line number zero), then return to line collection. 
    //Otherwise, select the next line and begin interpretation.

    if(IL->VMODE==0){ IL->PC = ILM_COLLECTOR_POINT; IL->CURSOR=0;}
    else if(IL->VMODE==1)
    { 
        int next_line = 0;

        for(int i=IL->CURSOR_LINE+1;i<MAX_LINES;i++)
        {
            if( !IsEmpty(IL->LINES[i])) { next_line = i; break; }
        }

        if(next_line==0)
        {
            IL->VMODE=0;
            IL->PC = ILM_COLLECTOR_POINT; 
            IL->CURSOR=0;
        }
        else
        {
            IL->CURSOR_LINE = next_line;
            IL->CURSOR = 0;
            IL->PC = ILM_EXECUTOR_POINT; 
        }
    }
}

/**********/
void ILM_XFER(ILM *IL)
{
    //Test valiue at the top of the AE stack to be within range. ( SHOULD I POP IT ??? )
    //If not, report an error. 
    //If so, attempt to position cursor at that line. 
    //If it exists, begin interpretation there; if not report an error.

    IL->AESTK_SP--;
    int val = IL->AESTK[IL->AESTK_SP];

    if(val>0 && val<AESTK_SIZE)
    {
        if(!IsEmpty(IL->LINES[val]))
        {
            IL->CURSOR = 0;
            IL->CURSOR_LINE = val;
            IL->PC = ILM_EXECUTOR_POINT;
        }
        else
        {
            ILM_error(IL,"Empty line"); return;
        }
    }
    else
    {
        ILM_error(IL,"Invalid line number"); return;
    }
}

/**********/
void ILM_SAV(ILM *IL)
{
    //Push present line number on SBRSTK. Report overflow as error.

    IL->LINES_STACK[IL->LINES_SP] = IL->CURSOR_LINE;

    IL->LINES_SP++;

    IL->PC++;
}

/**********/
void ILM_RSTR(ILM *IL)
{
    //Replace current line number with value on SBRSTK. 
    //If stack is empty, report error.

    IL->LINES_SP--;

    IL->CURSOR_LINE = IL->LINES_STACK[IL->LINES_SP];

    IL->PC++;
}

/**********/
void ILM_CMPR(ILM *IL)
{
    //Compare AESTK(SP), the top of the stack, with AESTK(SP-2) as per the relations indicated by AESTK(SP-1). 
    //Delete all from stack. 
    //If the condition specified did not match, then perform NXT action.

    //NUMBER      <-- TOP
    //RELATION    <-- TOP - 1
    //NUMBER      <-- TOP - 2

    // RELATION : 
    // 0 =
    // 4 >
    // 1 <
    // 5 >=
    // 2 <=
    // 3 <>

    IL->AESTK_SP--;
    int b = IL->AESTK[IL->AESTK_SP];

    IL->AESTK_SP--;
    int rel = IL->AESTK[IL->AESTK_SP];

    IL->AESTK_SP--;
    int a = IL->AESTK[IL->AESTK_SP];

    switch(rel)
    {
        case 0:
            if(a==b) { IL->PC++; }
            else { ILM_NXT(IL); }
            break;

        case 4:
            if(a>b) { IL->PC++; }
            else { ILM_NXT(IL); }
            break;

        case 1:
            if(a<b) { IL->PC++; }
            else { ILM_NXT(IL); }
            break;

        case 5:
            if(a>=b) { IL->PC++; }
            else { ILM_NXT(IL); }
            break;

        case 3:
            if(a!=b) { IL->PC++; }
            else { ILM_NXT(IL); }
            break;

        case 2:
            if(a<=b) { IL->PC++; }
            else { ILM_NXT(IL); }
            break;
        
        default :
            char msg[LEN_64];
            sprintf(msg,"Unknow relation code : %d",rel);
            ILM_error(IL, msg);
            break;
    }

}

/**********/
void ILM_LIT(ILM *IL)
{
    //Push the number num onto the AESTK (Originally omitted)
    
    IL->AESTK[IL->AESTK_SP] = IL->MEM[IL->PC+1];
    
    IL->AESTK_SP++;

    IL->PC += 2;
}

/**********/
void ILM_INNUM(ILM *IL)
{
    //Read a number from the terminal and push its value onto the AESTK. 

    int val = 0;

    printf(">");

    scanf("%d",&val);

    IL->AESTK[IL->AESTK_SP] = val;

    IL->AESTK_SP++;

    IL->PC++;
}

/**********/
void ILM_FIN(ILM *IL)
{
    //Return to the line collect routine.
    IL->CURSOR = 0;

    IL->CURSOR_LINE = 0;

    IL->PC = ILM_COLLECTOR_POINT;

    IL->VMODE = 0;
}

/**********/
void ILM_ERR(ILM *IL)
{
    //Report syntax error am return to line collect routine.

    ILM_error(IL,"Syntax error");

    IL->PC = ILM_COLLECTOR_POINT;
}

/**********/
void ILM_ADD(ILM *IL)
{
    //Replace top two elements of AESTK by their sum.

    IL->AESTK_SP--;
    int a = IL->AESTK[IL->AESTK_SP];

    IL->AESTK_SP--;
    int b = IL->AESTK[IL->AESTK_SP];

    IL->AESTK[IL->AESTK_SP] = a + b;

    IL->AESTK_SP++;

    IL->PC++;

}

/**********/
void ILM_SUB(ILM *IL)
{
    //Replace top two elements of AESTK by their difference.

    IL->AESTK_SP--;
    int a = IL->AESTK[IL->AESTK_SP];

    IL->AESTK_SP--;
    int b = IL->AESTK[IL->AESTK_SP];

    IL->AESTK[IL->AESTK_SP] = b - a;

    IL->AESTK_SP++;

    IL->PC++;
}

/**********/
void ILM_NEG(ILM *IL)
{
    //Replace top of AESTK with its neqative.

    IL->AESTK[IL->AESTK_SP-1] *= -1;

    IL->PC++; 
}

/**********/
void ILM_MUL(ILM *IL)
{
    //Replace top two elements of AESTK by their product.
    IL->AESTK_SP--;
    int a = IL->AESTK[IL->AESTK_SP];

    IL->AESTK_SP--;
    int b = IL->AESTK[IL->AESTK_SP];

    IL->AESTK[IL->AESTK_SP] = a * b;

    IL->AESTK_SP++;

    IL->PC++;
}

/**********/
void ILM_DIV(ILM *IL)
{
    //Replace top two elements of AESTK by their quotient.

    IL->AESTK_SP--;
    int a = IL->AESTK[IL->AESTK_SP];

    IL->AESTK_SP--;
    int b = IL->AESTK[IL->AESTK_SP];

    IL->AESTK[IL->AESTK_SP] = b / a;

    IL->AESTK_SP++;

    IL->PC++;
}

/**********/
void ILM_STORE(ILM *IL)
{
    //Place the value at the top of the AESTK into the variable designated by the index specified by the value immediately below it. 
    //Delete both from the stack.

    IL->AESTK_SP--;
    int val = IL->AESTK[IL->AESTK_SP];

    IL->AESTK_SP--;
    int var_index = IL->AESTK[IL->AESTK_SP];

    IL->VARIABLES[var_index] = val ;

    //remove me
    /*for(int i=0;i<VARIABLES_SIZE;i++)
    {
        printf("%c : %d\n", (char)(i+VARIABLE_OFFSET), IL->VARIABLES[i]);
    }*/

    IL->PC++;
}

/**********/
void ILM_TSTV(ILM *IL)
{
    //Test for variable (i.e letter) if present.
    //Place its index value onto the AESTK and continue execution at next suggested location. 
    //Otherwise continue at lbl.

    if(ILM_skip_spaces(IL)<0) { ILM_error(IL, "Line error"); return; }

    if( (int)IL->LINES[IL->CURSOR_LINE][IL->CURSOR]>=65 &&  (int)IL->LINES[IL->CURSOR_LINE][IL->CURSOR]<=90 )
    {     
        IL->AESTK[IL->AESTK_SP] = (int)IL->LINES[IL->CURSOR_LINE][IL->CURSOR]-VARIABLE_OFFSET;

        IL->CURSOR++;

        IL->AESTK_SP++;

        IL->PC += 3;
    }
    else
    {
        IL->PC = (IL->MEM[IL->PC+1] << 8 ) + IL->MEM[IL->PC+2];
    }
}

/**********/
void ILM_TSTN(ILM *IL)
{
    //Test for number. 
    //If present, place its value onto the AESTK and continue execution at next suggested location. 
    //Otherwise continue at lbl.

    if(ILM_skip_spaces(IL)<0) { ILM_error(IL, "Line error"); return; }
    int start = IL->CURSOR;

    if(ILM_read_until_is_digit(IL)<0) { ILM_error(IL, "Line error"); return; }
    int end = IL->CURSOR;

    char *token = Calloc(end-start+1,sizeof(char));
    strncpy(token,IL->LINES[IL->CURSOR_LINE] + start, end-start);

    if(IsNumber(token))
    {
        IL->AESTK[IL->AESTK_SP] = atoi(token);

        IL->AESTK_SP++;

        IL->PC += 3;
    }
    else
    {
        IL->CURSOR = start;

        IL->PC = (IL->MEM[IL->PC+1] << 8 ) + IL->MEM[IL->PC+2];
    }

    Free(token);
    
}

/**********/
void ILM_IND(ILM *IL)
{
    //Replace top of AEstack by variable value it indexes.

    IL->AESTK[IL->AESTK_SP-1] = IL->VARIABLES[IL->AESTK[IL->AESTK_SP-1]];

    IL->PC++;
}

/**********/
void ILM_LST(ILM *IL)
{
    //List the contents of the program area.

    for(int i=1;i<MAX_LINES;i++)
    {
        if(!IsEmpty(IL->LINES[i])) { printf("%02d %s", i, IL->LINES[i]); }
    }

    IL->PC++;
}

/**********/
void ILM_INIT(ILM *IL)
{
    //Perform global initilization
    //Clears program area, empties GOSUB stack, etc.

    ILM_reset(IL);

    IL->PC++;
}

/**********/
void ILM_GETLINE(ILM *IL)
{
    //Input a line to LBUF.

    ILM_clear_line(IL->LBUF);

    fgets(IL->LBUF, LINE_SIZE, stdin); 

    if( IsEmpty(IL->LBUF) ) { return; } //trick to avoid empty lines

    IL->CURSOR_LINE = 0;

    IL->CURSOR = 0;

    IL->PC++;
}

/**********/
void ILM_TSTL(ILM *IL)
{
    //After editing leading blanks, look for a line number. 
    //Report error if invalid; transfer to lbl if not present.

    char *start = SkipSpaces(IL->LBUF);
    if(start==NULL){ ILM_error(IL,"Error reading LBUF"); return; }

    char *end = TokenEnd(start);
    if(end==NULL){ ILM_error(IL,"Error reading LBUF"); return;}

    char *token = Calloc(end-start+1,sizeof(char));
    strncpy(token,start,end-start);
    
    if(IsNumber(token))
    {
        int val = atoi(token);

        if(val>0 && val<MAX_LINES)//is line number valid ?
        {
            IL->PC += 3;
        }
        else
        {
            Free(token);
            ILM_error(IL,"Invalid line number"); 
            return;
        }
    }
    else //no line number, PUT LBUF IT IN LINE[0],SET CURSOR LINE 0 and go to label
    {
        strcpy(IL->LINES[0], IL->LBUF);

        IL->CURSOR_LINE = 0;

        IL->CURSOR = 0;        

        IL->PC = (IL->MEM[IL->PC+1] << 8 ) + IL->MEM[IL->PC+2];
    }

    Free(token);
}

/**********/
void ILM_INSRT(ILM *IL)
{
    //Insert line after deleting any line with same line number.

    char *start = SkipSpaces(IL->LBUF);
    if(start==NULL){ ILM_error(IL,"Error reading LBUF"); return; }

    char *end = TokenEnd(start);
    if(end==NULL){ ILM_error(IL,"Error reading LBUF"); return; }

    char *token = Calloc(end-start+1,sizeof(char));
    strncpy(token,start,end-start);
    
    if(strlen(token)==0){ Free(token); ILM_error(IL,NULL); return; }

    if(IsNumber(token))
    {
        int val = atoi(token);

        if(val>=1 && val < MAX_LINES)//is line number valid ?
        {
            ILM_clear_line(IL->LINES[val]);

            start = SkipBlanks(end);

            if(start!=NULL)
            {
                strcpy(IL->LINES[val], start);
            }

            IL->PC++;
        }
        else
        {
            Free(token);
            ILM_error(IL,"Invalid line number");
            return;
        }
    }
    else //no line number
    {
        Free(token);
        ILM_error(IL,"Missing line number");
        return;
    }

    free(token);
}

/**********/
void ILM_XINIT(ILM *IL)
{
    //Perform initialization for each stated execution. Empties AEXP stack.

    IL->AESTK_SP = 0x00;
    
    for(int i=0;i<AESTK_SIZE;i++)
    {
        IL->AESTK[i] = 0x00;
    } 

    IL->PC++;
}

/**********/
void ILM_MODE(ILM *IL)
{
    //ADDED BY ME :
    //read the next byte and set it to the VMODE (virtual machine mode)
    //MODE : 0 = direct, 1 = run, 2 = exit

    if(IL->VMODE==0 && IL->MEM[IL->PC+1]==1)
    {
        for(int i=0;i<VARIABLES_SIZE;i++)
        {
            IL->VARIABLES[i] = 0x00;
        } 
    }

    IL->VMODE = IL->MEM[IL->PC+1];    

    IL->PC += 2;
}
