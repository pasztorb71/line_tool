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

int _file_open(char* fname_i, const char* open_mode_i, FILE** fhandler_o, char* errmsg_io, int errmsg_maxlen);
FILE* _file_open(char* fname_i, const char* open_mode_i, char* errmsg_io, int errmsg_maxlen, int* err);
int fill_new_record(char* line);
long find_record(char* line);
void free_records(void);
void print_records(int o_mode, int to_sort, FILE* ofh);
int cmpfunc_sort(const void* a, const void* b);
int parse_parameters(int argc, char* argv[]);
char* set_mode_string(int mode);
int print_header(char* fname_i, char* fname_o, char* mode_str, int empty);
int open_files(char* fname_i, char* fname_o, FILE** fp_input, FILE** fp_output);
void print_info(struct s_params par,struct s_files* files);
void open_io(struct s_files* files, unsigned int asize);
void parse_params(int argc, char** argv, struct s_files* files);
void main_task(struct s_files* files);

