#ifndef FUNCS_H
#define FUNCS_H

void EnableDebug();

void DisableDebug();

void ExitError(char *str, ...);

void* Calloc(size_t count, size_t size);

void Free(void* p);

char *FileToString(char *file);

int FileBuffer(char *file, Uint8 **buffer);

int IsEmpty(char *p);

int IsNumber(char *p);

char *SkipBlanks(char *p);

char *SkipSpaces(char *p);

char *SkipLine(char *p);

char *TokenEnd(char *p);

char *ReadUntil(char *p, char symbol);

#endif
