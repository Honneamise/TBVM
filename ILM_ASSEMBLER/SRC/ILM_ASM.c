#include "ILM_ASM_headers.h"

/**********/
int ILM_get_label_address(ILM_ASM ilm, char *label_name)
{
    for(int i=0; i< ilm.labels_count;i++)
    {
        if( strcmp(ilm.labels[i].name, label_name)==0 ){ return ilm.labels[i].address ;}
    }

    return -1;
}

/**********/
ILM_ASM ILM_assemble(char *buf)
{
    int ADC = 0x00;//address counter

    ILM_ASM ilm = {0};    

    char *start = buf;

    while(start!=NULL)
    {
        start = SkipBlanks(start);
        if(start==NULL){ break;}

        char *end = TokenEnd(start);
        if(end==NULL){ ExitError("[%s]: Error reading token at : %s\n", __func__, start); }

        char *token = Calloc(end-start+1,sizeof(char));
        strncpy(token,start,end-start);

        if(token[0]==';')//comment
        { 
            start = SkipLine(start); 
        }
        else if (token[strlen(token)-1]==':')//label
        { 
            ILM_LABEL label = {0};
            strncpy(label.name, token, strlen(token)-1);
            label.address = ADC;

            ilm.labels[ilm.labels_count] = label;
            ilm.labels_count++;

            start = end;
        }
        else if (InsCode(token)>=0)//instruction
        { 
            ADC++;
            start = end;

            //ins + label + string
            if(strcmp(token,"TST")==0)
            {
                ADC += 2;

                start = SkipBlanks(start);

                start = ReadUntil(start,'\'');
                start++;
                end = ReadUntil(start,'\'');

                ADC += 1 + end-start;

                start = SkipLine(start);
                 
            }

            //ins + Uint8
            else if(strcmp(token,"LIT")==0 ||
            strcmp(token,"MODE")==0)
            {
                ADC += 1;
                start = SkipLine(start);
            }
            //ins + label
            else if(strcmp(token,"CALL")==0 ||
            strcmp(token,"JMP")==0 ||
            strcmp(token,"TSTV")==0 ||
            strcmp(token,"TSTN")==0 ||
            strcmp(token,"TSTL")==0
            )
            {
                ADC += 2;
                start = SkipLine(start);
            }

        }
        else//error 
        {
            ExitError("[%s]: Unable to handle token : %s\n", __func__, token); exit(EXIT_FAILURE);
        }

        Free(token);
  
    }

    return ilm;
}

/**********/
void ILM_compile(ILM_ASM ilm, char *buf , char *file)
{
    int ins = -1;

    FILE *fp = fopen(file,"wb");
    if(fp==NULL){ ExitError("[%s]:Unable to open file \"%s\" ",__func__, file); }

    char *start = buf;
    
    while(start!=NULL)
    {
        start = SkipBlanks(start);
        if(start==NULL){ break;}

        char *end = TokenEnd(start);
        if(end==NULL){ ExitError("[%s]: Error reading token at : %s\n", __func__, start); }

        char *token = Calloc(end-start+1,sizeof(char));
        strncpy(token,start,end-start);

        if(token[0]==';')//comment
        { 
            start = SkipLine(start); 
        }
        else if (token[strlen(token)-1]==':')//label
        { 
            start = end;
        }
        else if ( (ins = InsCode(token))>=0 )//instruction
        { 
            start = end;

            fwrite( (Uint8[]){(Uint8)ins}, sizeof(Uint8), 1, fp);
            
            //ins + label + string
            if(strcmp(token,"TST")==0)
            {
                start = SkipBlanks(start);

                end = ReadUntil(start,',');

                char *label_name = Calloc(end-start+1,sizeof(char));
                strncpy(label_name,start,end-start);

                int address = ILM_get_label_address(ilm, label_name);

                if(address<0){ ExitError("[%s]: Address for label \"%s\" not found\n",__func__, label_name); }

                fwrite( (Uint8[]){ (Uint8)(address>>8), (Uint8)(address & 0xFF) }, sizeof(Uint8), 2, fp);

                Free(label_name);

                start = end + 2;
                end = end = ReadUntil(start,'\'');

                char *str = Calloc(end-start+1,sizeof(char));
                strncpy(str,start,end-start);

                fwrite( (Uint8[]){ (Uint8)(strlen(str)) }, sizeof(Uint8), 1, fp);

                for(size_t i=0;i<strlen(str);i++)
                {
                    fwrite( (Uint8[]){ (Uint8)(str[i]) }, sizeof(Uint8), 1, fp);
                }

                Free(str);

                start = SkipLine(start);
            }

            //ins + Uint8
            if(strcmp(token,"LIT")==0 || 
            strcmp(token,"MODE")==0 )
            {
                start = SkipBlanks(start);

                end = TokenEnd(start);

                char *string_val = Calloc(end-start+1,sizeof(char));
                strncpy(string_val,start,end-start);

                int val = atoi(string_val);

                fwrite( (Uint8[]){ (Uint8)(val & 0xFF) }, sizeof(Uint8), 1, fp);

                Free(string_val);

                start = SkipLine(start);
                
            }

            //1 elements label
            if(strcmp(token,"CALL")==0 ||
                strcmp(token,"JMP")==0 ||
                strcmp(token,"TSTV")==0 ||
                strcmp(token,"TSTN")==0 ||
                strcmp(token,"TSTL")==0
            )
            {
                start = SkipBlanks(start);

                end = TokenEnd(start);

                char *label_name = Calloc(end-start+1,sizeof(char));
                strncpy(label_name,start,end-start);

                int address = ILM_get_label_address(ilm, label_name);

                if(address<0){ ExitError("[%s]: Address for label \"%s\" not found\n",__func__, label_name); }

                fwrite( (Uint8[]){ (Uint8)(address>>8), (Uint8)(address & 0xFF) }, sizeof(Uint8), 2, fp);

                Free(label_name);

                start = SkipLine(start);

            }

        }
        else//error 
        {
            ExitError("[%s]: Unable to handle token : %s\n", __func__, token); exit(EXIT_FAILURE);
        }

        Free(token);

        
    }

    fclose(fp);
}