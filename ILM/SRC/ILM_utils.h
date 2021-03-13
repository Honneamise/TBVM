#ifndef ILM_UTILS_H
#define ILM_UTILS_H

void ILM_load_file(ILM *IL, char *file);

void ILM_error(ILM *IL, char *str);

void ILM_clear_line(char *line);

int ILM_skip_spaces(ILM *IL);

int ILM_read_until_blank(ILM *IL);

int ILM_read_until_is_digit(ILM *IL);

#endif