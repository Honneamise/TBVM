#include "ILM_ASM_headers.h"

int main(int argc, char **argv)
{
    EnableDebug();
	
    char *buf = FileToString("intp.asm");

    ILM_ASM ilm = ILM_assemble(buf);

    for(int i=0; i< ilm.labels_count;i++)
    {
        printf("%s : %04X\n", ilm.labels[i].name,  ilm.labels[i].address);
    }

    ILM_compile(ilm, buf, "intp.o");

    Free(buf);
    
	return EXIT_SUCCESS;
}
