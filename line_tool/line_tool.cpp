#include <windows.h>
#include <windows.h>
#include <stdio.h>
#include "line_tool.h"


//------------------------------------------------------------------------------------------------------------------


int main(int argc, char* argv[]) 
{
	struct s_files files[2];
	params.mode = count;
	params.sort = 0;
	params.empty = 0;

	memset(files, 0x00, sizeof(files));
	strcpy_s(files[output].open_mode, sizeof(files[output].open_mode), (char*)"w");

	if (argc < 2) {
		printf("%s Listing unique, non-unique lines, or just counting occurence of lines for a text file\n", pb_line_prefix);
		printf("%s switches (can't be concatenated!) [unique: -U] [not-unique: -NU] [sort: -S] [list empty lines: -E] \n", pb_line_prefix);
		printf("%s usage prog.exe [switches] input_file [output_file]\n", pb_line_prefix);
		printf("%s *** WARNING: otput file will be overwritten during run!\n", pb_line_prefix);
		return 0;
	}
	else {
		parse_params(argc, argv, files);
	}

	print_info(params, files);

	open_io(files, ARRAY_SIZE(files));
	files[output].fhandler = files[output].fhandler ? files[output].fhandler : stdout;

	main_task(files);

	print_records(params.mode, params.sort, files[output].fhandler);

	//closing files
	fclose(files[input].fhandler);
	if (files[output].fhandler != stdout)
		fclose(files[output].fhandler);

	free_records();
	return 0;
}

//------------------------------------------------------------------------------------------------------------------

int parse_parameters(int n_argc, char* n_argv[])
{
	return -1;
}
//------------------------------------------------------------------------------------------------------------------

char* set_mode_string(int mode)
{
	return (char *)"akarmi";
}
//------------------------------------------------------------------------------------------------------------------

int print_header(char* pb_line_prefix, char* fname_i, char* fname_o, char* mode_str, int empty)
{
	return -1;
}
//------------------------------------------------------------------------------------------------------------------

int open_files(char* fname_i, char* fname_o, FILE** fp_input, FILE** fp_output)
{
	return -1;
}
//------------------------------------------------------------------------------------------------------------------

int _file_open(char* fname_i, const char* open_mode_i, FILE** fhandler_o, char* errmsg_io, int errmsg_maxlen)
//returns err from fopen_s
//clears errmsg_io
{
	errno_t err;
	char errmsgbuf[100] = { 0 };
	int rv = 0;

	memset(errmsg_io, 0x00, errmsg_maxlen);
	err = fopen_s(fhandler_o, fname_i, open_mode_i);
	if (err) {
		if (strerror_s(errmsgbuf, sizeof(errmsgbuf), err)) {
			strcpy_s(errmsgbuf, sizeof(errmsgbuf), "unknown error (default)");
		}
		strcpy_s(errmsg_io, errmsg_maxlen - 1, errmsgbuf);
		rv = err;
	}
	return rv;
}

FILE* _file_open(char* fname_i, const char* open_mode_i, char* errmsg_io, int errmsg_maxlen, int* err)
//returns err from fopen_s
//clears errmsg_io
{
	errno_t errno;
	char errmsgbuf[100] = { 0 };
	FILE* fp_input;

	memset(errmsg_io, 0x00, errmsg_maxlen);
	errno = fopen_s(&fp_input, fname_i, open_mode_i);
	if (err) {
		if (strerror_s(errmsgbuf, sizeof(errmsgbuf), errno)) {
			strcpy_s(errmsgbuf, sizeof(errmsgbuf), "unknown error (default)");
		}
		strcpy_s(errmsg_io, errmsg_maxlen - 1, errmsgbuf);
		*err = errno;
	}
	return fp_input;
}

//------------------------------------------------------------------------------------------------------------------
int fill_new_record(char* line)
{
	int rv = 0;
	if (lines_cnt == 0) {
		lines = (s_line *) malloc(sizeof(struct s_line) * (lines_cnt + 1));
	}
	else {
		lines = (s_line *)realloc(lines, sizeof(struct s_line) * (lines_cnt + 1));
	}
	if (lines != NULL) {
		lines[lines_cnt].cnt = 1;
		lines[lines_cnt].line = (char*) malloc(strlen(line) + 1);
		if (lines[lines_cnt].line == NULL)
			rv = -1;
		else {
			strcpy_s(lines[lines_cnt].line, strlen(line) + 1, line);
			lines_cnt++;
		}
	}
	else {
		rv = -1;
	}
	return rv;
}

//------------------------------------------------------------------------------------------------------------------

long find_record(char* line)
{
	long i;
	for (i = 0; i < lines_cnt; i++) {
		if (strcmp(lines[i].line, line) == 0)
			return i;
	}
	return -1;
}

//------------------------------------------------------------------------------------------------------------------

void free_records(void)
{
	long i;
	for (i = 0; i < lines_cnt; i++) {
		if (lines[i].line)
			free(lines[i].line);
	}
	if (lines)
		free(lines);

}

//------------------------------------------------------------------------------------------------------------------

void print_records(int o_mode, int to_sort, FILE* ofh)
{
	long i;

	if (to_sort) {
		qsort(lines, lines_cnt, sizeof(struct s_line), cmpfunc_sort);
	}

	for (i = 0; i < lines_cnt; i++) {
		if (lines[i].cnt) {
			if (o_mode == count) {
				fprintf(ofh, "%5d - ", lines[i].cnt);
				fprintf(ofh, "%s\n", lines[i].line);
			}
			else if (o_mode == unique) {
				fprintf(ofh, "%s\n", lines[i].line);
			}
			else if (o_mode == not_unique) {
				if (lines[i].cnt > 1) {
					fprintf(ofh, "%s\n", lines[i].line);
				}
			}
		}

	}

}

//------------------------------------------------------------------------------------------------------------------

int cmpfunc_sort(const void* a, const void* b)
{
	/*this function belongs to qsort*/
	unsigned long cnt_a = ((struct s_line*)a)->cnt;
	unsigned long cnt_b = ((struct s_line*)b)->cnt;
	return (cnt_b - cnt_a);
}

//------------------------------------------------------------------------------------------------------------------
void print_info(struct s_params par, struct s_files *files)
{
	char mode_str[100];

	if (par.mode == count) strcpy_s(mode_str, sizeof(mode_str), "count");
	else if (par.mode == unique) strcpy_s(mode_str, sizeof(mode_str), "unique");
	else if (par.mode == not_unique) strcpy_s(mode_str, sizeof(mode_str), "not unique");
	printf("%s fname_i: %s\n", pb_line_prefix, files[input].fname);
	printf("%s fname_o: %s\n", pb_line_prefix, strlen(files[output].fname) != 0 ? files[output].fname : "(stdout)");
	printf("%s mode: %s\n", pb_line_prefix, mode_str);
	printf("%s sort: %s\n", pb_line_prefix, params.sort == 1 ? "yes" : "no");
	printf("%s deal with empty lines: %s\n", pb_line_prefix, par.empty == 1 ? "yes" : "no");
}

//------------------------------------------------------------------------------------------------------------------
/*
//open input and output files
int open_io_OLD(const char* pb_line_prefix)
{
	char errmsgbuf[100];
	int err;
	fp_input = _file_open(fname_i, "r", errmsgbuf, sizeof(errmsgbuf), &err);

	if (err != 0) {
		printf("%s *** cannot open input file '%s': %s\n", pb_line_prefix, fname_i, errmsgbuf);
		return -1;
	}

	fp_output = _file_open(fname_o, "r", errmsgbuf, sizeof(errmsgbuf), &err);

	if (fname_o[0] != 0) {
		if (err != 0) {
			printf("%s *** cannot open output file '%s': %s\n", pb_line_prefix, fname_o, errmsgbuf);
			return -1;
		}
	}
	else {
		fp_output = stdout;
	}
	return 0;
}
*/
//------------------------------------------------------------------------------------------------------------------
void open_io(struct s_files* files, unsigned int asize)
{
	unsigned int i;
	for (i = 0; i < asize; i++) {
		files[i].fhandler = _file_open(files[i].fname, files[i].open_mode, files[i].errmsg, sizeof(files[i].errmsg), &files[i].err);
		if (files[i].err) {
			printf("%s *** cannot open file \"%s\" with mode \"%s\": %s\n", pb_line_prefix, files[i].fname, files[i].open_mode, files[i].errmsg);
		}
	}
}

//------------------------------------------------------------------------------------------------------------------

void parse_params(int argc, char **argv, struct s_files* files)
{
	int i;
	int non_switch_params = 0;
	for (i = 1; i < argc; i++) {
		if (strstr(argv[i], "-U")) {
			params.mode = unique;
		}
		else if (strstr(argv[i], "-NU")) {
			params.mode = not_unique;
		}
		else if (strstr(argv[i], "-S")) {
			params.sort = 1;
		}
		else if (strstr(argv[i], "-E")) {
			params.empty = 1;
		}
		else {
			if (non_switch_params < 2) {
				if (non_switch_params == 0) {
					//strcpy_s(fname_i, sizeof(fname_i), argv[i]);
					strcpy_s(files[input].fname, sizeof(files[input].fname), argv[i]);
					strcpy_s(files[input].open_mode, sizeof(files[input].open_mode), (char*)"r");
				}
				if (non_switch_params == 1) {
					//strcpy_s(fname_o, sizeof(fname_o), argv[i]);
					strcpy_s(files[output].fname, sizeof(files[output].fname), argv[i]);
				}
			}
			non_switch_params++;
		}
	}
}

//------------------------------------------------------------------------------------------------------------------

void main_task(struct s_files* files)
{
	char input_sor[10000 + 1] = { 0 };
	int err_alloc;
	long i;
	while (!feof(files[input].fhandler)) {
		if (fgets(input_sor, sizeof(input_sor) - 1, files[input].fhandler)) {
			if (input_sor[strlen(input_sor) - 1] == '\n')
				input_sor[strlen(input_sor) - 1] = 0x00;
			if (strlen(input_sor) == 0 && params.empty == 0)
				continue;
			if ((i = find_record(input_sor)) >= 0) {
				lines[i].cnt++;
			}
			else {
				err_alloc = fill_new_record(input_sor);
				if (err_alloc) {
					printf("%s *** malloc error! Exit all!\n", pb_line_prefix);
					break;
				}
			}

		}
	}
}
