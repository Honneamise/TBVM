#ifndef ILM_ASM_H
#define ILM_ASM_H

typedef struct ILM_LABEL
{
    char name[LEN_32];
    int address;

} ILM_LABEL;

typedef struct ILM_ASM
{
    int labels_count;
    ILM_LABEL labels[MAX_LEN];

} ILM_ASM;

/**********/

int ILM_get_label_address(ILM_ASM ilm, char *label_name);

ILM_ASM ILM_assemble(char *buf);

void ILM_compile(ILM_ASM ilm, char *buf , char *file);

#endif