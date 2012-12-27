#ifndef __MTK_CPUTIMES_STATS__
#define __MTK_CPUTIMES_STATS__

#define MTK_CPUTIME_ENTRIES	10

struct cputime_record {
	unsigned long long start;
	unsigned int numtimes;
	unsigned long long total_time;
};

int cputime_register_idx(void);
void cputime_start_idx(unsigned int id);
void cputime_stop_idx(unsigned int id);
void collect_cputime(void);
void init_cputime_current_all(void);

#endif

