#include <windows.h>
#include <stdio.h>

#define FILENAME_MAXLEN 260

int _file_open(char* fname_i, const char* open_mode_i, FILE** fhandler_o, char* errmsg_io, int errmsg_maxlen);
FILE* _file_open(char* fname_i, const char* open_mode_i, char* errmsg_io, int errmsg_maxlen, int* err);
int fill_new_record(char* line);
long find_record(char* line);
void free_records(void);
void print_records(int o_mode, int to_sort);
int cmpfunc_sort(const void* a, const void* b);
int parse_parameters(int argc, char* argv[]);
char* set_mode_string(int mode);
int print_header(char* pb_line_prefix, char* fname_i, char* fname_o, char* mode_str, int empty);
int open_files(char* fname_i, char* fname_o, FILE** fp_input, FILE** fp_output);

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
char fname_i[260] = { 0 };
char fname_o[260] = { 0 };
FILE* fp_input;
FILE* fp_output;
