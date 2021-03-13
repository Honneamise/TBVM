#include "ILM_headers.h"

int main(int argc, char **argv)
{
    
    EnableDebug();
	
    ILM IL = {0};
    
    ILM_load_file(&IL,"intp.o");

    ILM_execute(&IL);

    return EXIT_SUCCESS;
}
