
#ifndef __MTK_CCCI_HELPER_H
#define __MTK_CCCI_HELPER_H



#define MAX_REGISTER_API 20

//error code
#define E_NO_EXIST			-1
#define E_PARAM				-2




typedef enum {
	ID_GET_MD_WAKEUP_SRC = 0,
}FUNC_ID;
typedef int (*ccci_kern_cb_func_t)(char *buf, unsigned int len);

typedef struct{
	FUNC_ID id;
	ccci_kern_cb_func_t func;
}ccci_kern_func_info;




extern int register_ccci_kern_func(unsigned int id, ccci_kern_cb_func_t func);
extern int exec_ccci_kern_func(unsigned int id, char *buf, unsigned int len);

extern int get_dram_info(int *clk, int *type);
extern int get_dram_type_clk(int *clk, int *type);


#endif
