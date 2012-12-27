#include <linux/kernel.h>
#include <linux/module.h>

int (*__cputime_register_idx)(void) = NULL;
void (*__cputime_start_idx)(unsigned int id) = NULL;
void (*__cputime_stop_idx)(unsigned int id) = NULL;
void (*__collect_cputime)(void) = NULL;
void (*__init_cputime_current_all)(void) = NULL;
EXPORT_SYMBOL(__cputime_register_idx);
EXPORT_SYMBOL(__cputime_start_idx);
EXPORT_SYMBOL(__cputime_stop_idx);
EXPORT_SYMBOL(__collect_cputime);
EXPORT_SYMBOL(__init_cputime_current_all);

int cputime_register_idx() 
{
	if (__cputime_register_idx != NULL) 
		return __cputime_register_idx();
	else
		return -1;
}
EXPORT_SYMBOL(cputime_register_idx);

void cputime_start_idx(unsigned int id) 
{
	if (__cputime_start_idx != NULL)
		__cputime_start_idx(id);
}
EXPORT_SYMBOL(cputime_start_idx);

void cputime_stop_idx(unsigned int id) 
{
	if (__cputime_stop_idx != NULL)
		__cputime_stop_idx(id);
}
EXPORT_SYMBOL(cputime_stop_idx);

void collect_cputime() 
{
	if (__collect_cputime != NULL)
		__collect_cputime();
}
EXPORT_SYMBOL(collect_cputime);

void init_cputime_current_all() 
{
	if (__init_cputime_current_all != NULL)
		__init_cputime_current_all();
}
EXPORT_SYMBOL(init_cputime_current_all);

