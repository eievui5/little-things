#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HELP_OPT 1

typedef enum Action {
	DISPLAY,
	REMOVE,
	ADD,
} Action;

typedef struct Task {
	unsigned priority;
	size_t len;
	char* str;
} Task;

typedef struct TaskList {
	size_t size;
	Task* tasks;
} TaskList;

static const char shortopts[] = "adhi:lmr";
static struct option const longopts[] = {
	{"add",     no_argument,       NULL, 'a'},
	{"display", no_argument,       NULL, 'd'},
	{"high",    no_argument,       NULL, 'h'},
	{"help",    no_argument,       NULL, HELP_OPT},
	{"input",   required_argument, NULL, 'i'},
	{"low",     no_argument,       NULL, 'l'},
	{"medium",  no_argument,       NULL, 'm'},
	{"remove",  no_argument,       NULL, 'r'},
	{NULL}
};

void delete_task(Task* task) {
	free(task->str);
	task->str = NULL;
}

TaskList read_tasks(FILE* infile) {
	// Initiallize task list.
	Task* task_list = malloc(sizeof(Task) * 16);
	size_t task_list_size = 16;
	size_t task_index = 0;

	// Read tasks from input.
	unsigned priority;
	char* task_str = NULL;
	size_t task_str_size = 0;

	while (fscanf(infile, "%u: ", &priority) == 1) {
		if (getline(&task_str, &task_str_size, infile) == -1)
			break;

		// Resize task list.
		if (task_index >= task_list_size) {
			task_list_size *= 2;
			task_list = realloc(task_list, task_list_size);
		}
		Task* task = &task_list[task_index++];

		// Create task.
		task->priority = priority;
		task->len = strlen(task_str);
		task->str = malloc(task->len);
		memcpy(task->str, task_str, task->len);
		if (task->str[task->len - 1] == '\n') {
			task->str[task->len - 1] = 0;
			task->len--;
		}
	}
	free(task_str);
	return (TaskList) {task_index, task_list};
}

void print_tasks(FILE* outfile, TaskList* task_list) {
	for (size_t i = 0; i < task_list->size; i++) {
		Task* task = &task_list->tasks[i];
		if (task->str == NULL)
			continue;
		fprintf(outfile, "%u: %s\n", task->priority, task->str);
	}
}

// Iterate through a list of tasks in order of priority. Return -1 when finished.
int iterate_tasks(Task* task_list, size_t list_cnt, unsigned* p) {
	while (1) {
		size_t null_cnt = 0;
		for (size_t i = 0; i < list_cnt; i += 1) {
			if (task_list[i].str != NULL) {
				if (task_list[i].priority == *p)
					return i;
			} else {
				null_cnt++;
			}
		}
		if (null_cnt == list_cnt)
			return -1;
		*p += 1;
	}
}

void print_help(char* name) {
	fprintf(stderr,
		"todo v0.0.0\n"
		"Terminal-based todo list tracker.\n"
		"Usage:\n"
		"To display the current directory's .todo file:\n"
		"\t%1$s\n"
		"To use a file other than .todo:\n"
		"\t%1$s -i [infile]\n"
		"To add a high-priority task:\n"
		"\t%1$s -h [task description]\n"
		"Options:\n"
		"\t-a --add     Add a task with a custom priority.\n"
		"\t-d --display Display a task file. This is the default behavior.\n"
		"\t   --help    Display this text.\n"
		"\t-h --high    Add a task with high priority.\n"
		"\t-i --input   Use a .todo file other than the default \".todo\".\n"
		"\t-l --low     Add a task with low priority.\n"
		"\t-h --medium  Add a task with medium priority.\n"
		"\t-r --remove  Add a task with high priority.\n", name);
}

int main(int argc, char* argv[]) {
	Action action = DISPLAY;
	int args_index = -1;
	FILE* infile = NULL;
	const char* infile_path = ".todo";
	int master_priority = -1;

    for (char option_char; (option_char = getopt_long_only(argc, argv, shortopts, longopts, NULL)) != -1;) {
    	switch (option_char) {
		case 'a':
			action = ADD;
			goto escape;
    	case 'd':
			action = DISPLAY;
			goto escape;
    	case 'h':
			master_priority = 0;
			action = ADD;
			goto escape;
    	case HELP_OPT:
    		print_help(argv[0]);
    		exit(0);
    	case 'i':
    		infile_path = optarg;
    		break;
    	case 'l':
			master_priority = 2;
			action = ADD;
			goto escape;
    	case 'm':
			master_priority = 1;
			action = ADD;
			goto escape;
    	case 'r':
			action = REMOVE;
			goto escape;
    	}
    }
escape:


	// Open input file.
	if (infile_path != NULL)
		infile = fopen(infile_path, "r");

	// Read tasks from input.
	TaskList task_list = (infile != NULL) ? read_tasks(infile) : (TaskList) {0, NULL};

	switch (action) {
	case DISPLAY: {
		bool has_printed = false;
		unsigned p = 0;
		unsigned prev_p = 0;
		int i = 0;
		fputs("\033[31m", stdout);
		while ((i = iterate_tasks(task_list.tasks, task_list.size, &p)) != -1) {
			fprintf(stdout, "\033[%im", 31 + p % 6);
			if (has_printed) {
				if (p != prev_p) {
					fputc('\n', stdout);
					if (p == 1)
						fputs("Medium Priority:\n", stdout);
					else if (prev_p < 2)
						fputs("Low Priority:\n", stdout);
				}
			} else {
				if (p == 0)
					fputs("High Priority:\n", stdout);
			}
			has_printed = true;
			prev_p = p;

			fprintf(stdout, "[%i] %s\n", i, task_list.tasks[i].str);
			delete_task(&task_list.tasks[i]);
		}
		fputs("\033[0m", stdout);
	} break;
	case REMOVE: {
		for (char** input = &argv[optind]; *input != NULL; input++) {
			size_t i = strtoul(*input, NULL, 0);
			if (i > task_list.size - 1) {
				fprintf(stderr, "ERROR: Task index of %lu is invalid, maximum index is %lu.\n",
				        i, task_list.size - 1);
				for (size_t i = 0; i < task_list.size; i++)
					free(task_list.tasks[i].str);
				free(task_list.tasks);
				fclose(infile);
				exit(1);
			}
			delete_task(&task_list.tasks[i]);
			infile = freopen(infile_path, "w", infile);
			print_tasks(infile, &task_list);
		}
	} break;
	case ADD: {
		char* task_str = NULL;
		size_t task_len = 0;
		if (argc > optind) {
			for (int i = optind; i < argc; i++) {
				size_t arg_len = strlen(argv[i]) + 1;
				if (task_str != NULL)
					task_str[task_len - 1] = ' ';
				task_str = realloc(task_str, task_len + arg_len);
				memcpy(&task_str[task_len], argv[i], arg_len);
				task_len += arg_len;
			}
		} else {
			fputs("Enter a task:\n", stdout);
			if (getline(&task_str, &task_len, stdin) == -1)
				fputs("ERROR: Fatal error occured while reading input.", stderr);
		}
		// Resize task list.
		task_list.size += 1;
		task_list.tasks = realloc(task_list.tasks, task_list.size * sizeof(Task));

		Task* task = &task_list.tasks[task_list.size - 1];

		// Create task.
		if (master_priority != -1) {
			task->priority = master_priority;
		} else {
			fputs("Enter task priority (0 is highest priority): ", stdout);
			if (scanf("%u", &task->priority) != 1)
				task->priority = 0;
		}
		task->len = task_len;
		task->str = task_str;
		if (infile != NULL) {
			fclose(infile);
			infile = NULL;
		}
		FILE* outfile = fopen(infile_path, "w");
		print_tasks(outfile, &task_list);
		fclose(outfile);
	} break;
	}

	for (size_t i = 0; i < task_list.size; i++)
		free(task_list.tasks[i].str);
	free(task_list.tasks);

	if (infile != NULL)
		fclose(infile);
}
