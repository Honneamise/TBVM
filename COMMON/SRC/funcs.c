#include "commons.h"

/**********/
void EnableDebug()
{
#if defined(_MSC_VER) && defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	printf("[%s]: Memory leaks debug enabled\n",__func__);
#endif
}

/**********/
void DisableDebug()
{
#if defined(_MSC_VER) && defined(_DEBUG)
	int flag = 0;

	flag &= ~_CRTDBG_ALLOC_MEM_DF;
	_CrtSetDbgFlag(flag);

	flag &= ~_CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(flag);

	printf("[%s]: Memory leaks debug disabled\n",__func__);
#endif
}

/**********/
void ExitError(char *str, ...)
{
#if defined(_WIN32)
	_flushall();
#else
	flushall();
#endif

	if (str != NULL)
	{
		va_list argp;
		va_start(argp, str);
		vfprintf(stdout, str, argp);
		va_end(argp);
	};

	DisableDebug();
	
	exit(EXIT_FAILURE);
}

/**********/
void* Calloc(size_t count, size_t size)
{
	void* p = calloc(count, size);

	if (p == NULL) { ExitError("[%s]: Allocation failed\n", __func__); };

	return p;
}

/**********/
void Free(void* p)
{
	if (p != NULL) { free(p); };
}

/**********/
char *FileToString(char* file)
{
	Uint8 *buf = NULL;

	FILE* fp = fopen(file, "rb");

	if (fp != NULL)
	{
		if (fseek(fp, 0, SEEK_END) != 0) { ExitError("[%s]: Error seeking file \"%s\"\n", __func__, file); };

		long size = ftell(fp);

		buf = (char*)Calloc((size + 1), sizeof(char));

		if (fseek(fp, 0, SEEK_SET) != 0) { ExitError("[%s]: Error seeking file \"%s\"\n", __func__, file); };

		size_t check = fread(buf, size, 1, fp);

		if (check != 1) { ExitError("[%s]: Error reading file \"%s\"\n", __func__, file); };

		buf[size] = 0;

		fclose(fp);
	}

	else
	{
		ExitError("[%s]: Unable to open file \"%s\"\n", __func__, file);
	}

	return buf;
}

/**********/
int FileBuffer(char *file, Uint8 **buffer)
{
	int size = 0;

	Uint8 *buf = NULL;

	FILE *fp = fopen(file, "rb");

	if (fp != NULL)
	{
		if (fseek(fp, 0, SEEK_END) != 0) { ExitError("[%s]: Error seeking file \"%s\"\n", __func__, file); };

		size = (int)ftell(fp);

		buf = Calloc( size, sizeof(Uint8));

		if (fseek(fp, 0, SEEK_SET) != 0) { ExitError("[%s]: Error seeking file \"%s\"\n", __func__, file); };

		size_t check = fread(buf, size, 1, fp);

		if (check != 1) { ExitError("[%s]: Error reading file \"%s\"\n", __func__, file); };

		fclose(fp);
	}

	else
	{
		ExitError("[%s]: Unable to open file \"%s\"\n", __func__, file);
	}

	*buffer = buf;

	return size;
}

/**********/
int IsEmpty(char *p)
{
	int len = strlen(p);

	int res = 1;

	for(int i=0;i<len;i++)
	{
		if( p[i]!=0 && p[i]!=' ' && p[i]!='\t' && p[i]!='\r' && p[i]!='\n') { res=0; break; }

		else {  res=1; }
	}

	return res;
}

/**********/
int IsNumber(char *p)
{
	int len = strlen(p);

	int res = 0;

	for(int i=0;i<len;i++)
	{
		if(isdigit(p[i])) { res = 1; }
		else { res=0; break;}
	}

	return res;
}

/**********/
char *SkipBlanks(char *p)
{
	if(p==NULL){ return NULL; }

	while( (*p!=0) && ( *p==' ' || *p=='\t' || *p=='\r' || *p=='\n' ))
	{
		p++;
	}

	if(*p==0){ return NULL; }

	return p;
}

/**********/
char *SkipSpaces(char *p)
{
	if(p==NULL){ return NULL; }

	while( (*p!=0) && ( *p==' ' || *p=='\t'))
	{
		p++;
	}

	if(*p==0){ return NULL; }

	return p;
}

/**********/
char *SkipLine(char *p)
{
	if(p==NULL){ return NULL; }
	
	while(*p!=0 && *p!='\n')
	{
		p++;
	}

	if(*p==0){ return NULL; }

	p++;

	return p;
}

/**********/
char *TokenEnd(char *p)
{
	if(p==NULL){ return NULL; }
	
	while((*p!=0) && ( *p!=' ' && *p!='\t' && *p!='\r' && *p!='\n' ))
	{
		p++;
	}

	return p;
}

/**********/
char *ReadUntil(char *p, char symbol)
{
	if(p==NULL){ return NULL; }
	
	while((*p!=0) && (*p!=symbol))
	{
		p++;
	}

	return p;
}
