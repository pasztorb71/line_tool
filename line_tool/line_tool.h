#include <windows.h>
#include <stdio.h>

#define FILENAME_MAXLEN 260
#define ARRAY_SIZE(a)       (sizeof(a) / sizeof(a[0]))

// -------------------------------------------------------------------------------------------------------------- variables

enum file_type {
	input = 0,
	output = 1
};

struct s_files {
	char fname[FILENAME_MAXLEN];
	file_type ftype;
	bool mandatory;
	char open_mode[5];
	FILE* fhandler;
	char errmsg[100];
	int err;
};

struct s_params {
	int mode;
	int sort;
	int empty;
} params;

enum mode {
	count = 0,
	unique = 1,
	not_unique = 2,
};

struct s_line {
	char* line;
	unsigned long cnt;
};

struct s_line* lines = NULL;
long lines_cnt = 0;
static const char* pb_line_prefix = "[PBTOOL - lines]";

// -------------------------------------------------------------------------------------------------------------- functions

FILE* _file_open(char* fname_i, const char* open_mode_i, char* errmsg_io, int errmsg_maxlen, int* err);
int fill_new_record(char* line);
long find_record(char* line);
void free_records(void);
void print_records(int o_mode, int to_sort, FILE* ofh);
int cmpfunc_sort(const void* a, const void* b);
void print_params(struct s_params par,struct s_files* files);
int open_io(struct s_files* files, unsigned int asize);
void close_io(struct s_files* files, unsigned int asize);
int get_params(int argc, char** argv, struct s_files* files);
int main_task(struct s_files* files);
void print_helper(void);
void set_defaults(struct s_files* files, unsigned int asize);
bool remove_newline(char* str);
