#include "ILM_headers.h"

/**********/
void ILM_load_file(ILM *IL, char *file)
{
    Uint8 *buf = NULL;

    int len = FileBuffer(file,&buf);

    for(int i=0;i<len;i++)
    {
        IL->MEM[i] = buf[i];
    }

    Free(buf);
}

/**********/
void ILM_error(ILM *IL, char *str)
{

    if(str!=NULL) { printf("\n[!!!] %s\n", str); }//mantain this line   

    if(IL->VMODE==0) { printf("%s",IL->LINES[IL->CURSOR_LINE]); }
    else { printf("%d %s",IL->CURSOR_LINE, IL->LINES[IL->CURSOR_LINE]);}

    for(int i=0;i<IL->CURSOR;i++)
    {
        printf("-");
    };

    printf("^\n"); 

    IL->CURSOR = 0;

    IL->CURSOR_LINE = 0;

    IL->PC = ILM_COLLECTOR_POINT;

    IL->VMODE = 0;
}

/**********/
void ILM_clear_line(char *line)
{
    for(int i=0;i<LINE_SIZE;i++)
    {
        line[i] = 0x00;
    }
}

/**********/
int ILM_skip_spaces(ILM *IL)
{
    while( IL->CURSOR<LINE_SIZE && IL->LINES[IL->CURSOR_LINE][IL->CURSOR]!=0 && 
        ( IL->LINES[IL->CURSOR_LINE][IL->CURSOR]==' ' ||  IL->LINES[IL->CURSOR_LINE][IL->CURSOR]=='\t' ))
    {
        IL->CURSOR++;
    }

    if(IL->CURSOR == LINE_SIZE || IL->LINES[IL->CURSOR_LINE][IL->CURSOR] == 0) { return -1; }
    
    return 0;
}

/**********/
int ILM_read_until_blank(ILM *IL)
{
    while( IL->CURSOR<LINE_SIZE && IL->LINES[IL->CURSOR_LINE][IL->CURSOR]!=0 && 
     IL->LINES[IL->CURSOR_LINE][IL->CURSOR]!=' ' &&  IL->LINES[IL->CURSOR_LINE][IL->CURSOR]!='\t'
    &&  IL->LINES[IL->CURSOR_LINE][IL->CURSOR]!='\r' &&  IL->LINES[IL->CURSOR_LINE][IL->CURSOR]!='\n' )
    {
        IL->CURSOR++;
    }

    if(IL->CURSOR == LINE_SIZE || IL->LINES[IL->CURSOR_LINE][IL->CURSOR] == 0) { return -1; }
    
    return 0;
}

/**********/
int ILM_read_until_is_digit(ILM *IL)
{
    while( IL->CURSOR<LINE_SIZE && IL->LINES[IL->CURSOR_LINE][IL->CURSOR]!=0 && 
     isdigit(IL->LINES[IL->CURSOR_LINE][IL->CURSOR]) ) 
    {
        IL->CURSOR++;
    }

    if(IL->CURSOR == LINE_SIZE || IL->LINES[IL->CURSOR_LINE][IL->CURSOR] == 0) { return -1; }
    
    return 0;
}