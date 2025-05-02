#include "boolft.h"

void	table_error(char *msg) {
	write(STDERR_FILENO, msg, strlen(msg));
}

int		first_true(t_var *operands) {
	for (int i = 0; operands[i].var; i++) {
		if (operands[i].value == TRUE)
			return (i);
	}
	return (-1);
}

t_bool	last_case(t_var *operands) {
	for (int i = 0; operands[i].var; i++) {
		if (operands[i].value == FALSE)
			return (FALSE);
	}
	return (TRUE);
}

void	print_line(t_var *variables, char* func) {
	char *to_eval = strdup(func);
	for (int i = 0; to_eval[i]; i++) {
		if (to_eval[i] >= 'A' && to_eval[i] <= 'Z') {
			int idx = to_eval[i] - 65;
			printf("|  %d  ", variables[idx].value);
			to_eval[i] = BOOL_TO_CHAR(variables[idx].value);
		}
	}
	int	result = eval_formula(to_eval);
	printf("|  %d  |\n", result);
	free(to_eval);
}

void	next_case(t_var *operands, t_var *variables, int total, int index) {
	printf("indes %d\n", index);
	for (int i = 0; i < total; i++) {
		variables[operands[i].var - 65].value = FALSE;
		operands[i].value = FALSE;
	}
	variables[operands[index].var - 65].value = TRUE;
	operands[index].value = TRUE;
}

void	update_case(t_var *operands, t_var *variables, int total) {
	int first = first_true(variables);
	if (first == total - 1) {
		printf("%d fist \n", first);
		next_case(operands, variables, total, first - 1);
	} else {
		variables[operands[first + 1].var - 65].value = TRUE;
		operands[first + 1].value = TRUE;
	}
}

void	print_table(t_var *operands, t_var *variables, int total, char *original, int index) {
	if (first_true(operands) == -1) {
		for (int i = 0; original[i]; i++) {
			if (original[i] >= 'A' && original[i] <= 'Z') {
				printf("|  %c  ", original[i]);
			}
		}
		printf("|  =  |\n");
		for (int i = 0; original[i]; i++) {
			if (original[i] >= 'A' && original[i] <= 'Z') {
				printf("| --- ");
			}
		}
		printf("| --- |\n");
		print_line(variables, original);
		printf("index next case %d\n", index - 1);
		next_case(operands, variables, total, index - 1);
		print_line(variables, original);
		int i = 0;
		while (i < 3) {
			update_case(operands, variables, total);
			print_line(variables, original);
			i++;
		}
	}
}

void	print_truth_table(char *function) {
	t_var	variables[27];
	t_var	operands[27];
	int		total = 0;
	bzero(&variables, 27 * sizeof(t_var));
	bzero(&operands, 27 * sizeof(t_var));

	for (int i = 0; function[i]; i++) {
		if (function[i] >= 'A' && function[i] <= 'Z') {
			int pos = function[i] - 65;
			if (variables[pos].var)
				continue;
			variables[pos].var = function[i];
			variables[pos].idx = total;
			operands[total].var = function[i];
			total++;
		} else {
			if (get_index_operator(function[i]) == UNRECOGNIZED_SYMBOL) {
				table_error("UNRECOGNIZED SYMBOL\n");
				return;
			}
		}
	}
	for (int i = 0; i < 27; i++) {
		if (variables[i].var)
			printf("%c -> var char\n", variables[i].var);
	}
	printf("hell \n");
	for (int i = 0; i < total; i++) {
		if (operands[i].var)
			printf("%c -> var char\n", operands[i].var);
	}
	printf("Total %d\n", first_true(&operands[0]));
	print_table(&operands[0], &variables[0], total, function, total - 1);
	// char* original = strdup(function);
	// printf("%s %d\n", original, 90 - 65);
	// free(original);
}