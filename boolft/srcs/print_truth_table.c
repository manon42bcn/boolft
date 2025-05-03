#include "boolft.h"

/**
 * @brief Prints an error message to the standard error output.
 *
 * This function writes the specified error message to the standard error (stderr)
 * using the low-level write() system call.
 *
 * @param msg Null-terminated string containing the error message to be printed.
 *
 * @note The function writes to STDERR_FILENO, which is typically file descriptor 2.
 */
void	table_error(const char *msg) {
	write(STDERR_FILENO, msg, strlen(msg));
}

/**
 * @brief Prints the header row and separator line for the truth table.
 *
 * This function generates two lines:
 * 1. Header row with variable names (A-Z) and result column (=)
 * 2. Separator line with dashes to visually separate headers from data
 *
 * @param[in] function The original formula string containing variables
 *
 * @note Formatting details:
 * - Variables are displayed as "|  X  " where X is the uppercase letter
 * - Result column is labeled "|  =  |"
 * - Separator line uses "| --- " for variables and "| --- |" for result
 * - Maintains consistent column width matching print_line() output
 *
 * @example For formula "AB&":
 * Output:
 * |  A  |  B  |  =  |
 * | --- | --- | --- |
 */
void	print_header(const char *function) {
	for (int i = 0; function[i]; i++) {
		if (function[i] >= 'A' && function[i] <= 'Z') {
			printf("|  %c  ", function[i]);
		}
	}
	printf("|  =  |\n");
	for (int i = 0; function[i]; i++) {
		if (function[i] >= 'A' && function[i] <= 'Z') {
			printf("| --- ");
		}
	}
	printf("| --- |\n");
}

/**
 * @brief Prints a single row of the truth table with variable values and formula result.
 *
 * This function:
 * 1. Generates a modified formula string with current variable values
 * 2. Prints variable values in table format
 * 3. Evaluates and displays the formula result
 *
 * @param[in] variables Array of variables with current case values
 * @param[in] func Original formula string (for value substitution)
 * @param[in] cases_info Case counter array [current_case, total_cases]
 *
 * @note Key operations:
 * - Uses BOOL_TO_CHAR macro to convert 0/1 values to '0'/'1' characters
 * - Calls print_header() on first case (case index 0)
 * - Maintains table formatting with pipe (|) separators
 * - Evaluates the formula with current variable values using eval_formula()
 *
 */
void	print_line(const t_var *variables, const char* func, const int *cases_info) {
	char *to_eval = strdup(func);
	if (cases_info[CASE_IDX] == 0)
		print_header(func);
	for (int i = 0; to_eval[i]; i++) {
		if (to_eval[i] >= 'A' && to_eval[i] <= 'Z') {
			int idx = to_eval[i] - 65;
			printf("|  %d  ", variables[idx].value);
			to_eval[i] = BOOL_TO_CHAR(variables[idx].value);
		}
	}
	const int	result = eval_formula(to_eval);
	printf("|  %d  |\n", result);
	free(to_eval);
}

/**
 * @brief Updates variable values for the current truth table case.
 *
 * This function:
 * 1. Increments the case index
 * 2. Converts the case index to binary values
 * 3. Assigns these values to variables in reverse order (MSB first)
 *
 * @param[in] operands Array of unique variables in evaluation order
 * @param[out] variables Array of all variables with updated values
 * @param[in] total Number of variables (determines bitmask size)
 * @param[in,out] idx Case counter array [current_case, total_cases]
 *
 * @note Key operations:
 * - Uses bitwise right shift (>>) and AND (&) to extract individual bits
 * - Processes variables in reverse order to maintain big-endian bit assignment
 * - Requires variables to be uppercase letters (A-Z) in t_var.var field
 *
 * @example For total=3 variables and case index 5 (binary 101):
 * Variable[0].value = 1 (bit 2), Variable[1].value = 0 (bit 1), Variable[2].value = 1 (bit 0)
 */
void	update_case(t_var *operands, t_var *variables, const int total, int *idx) {
	idx[CASE_IDX] = idx[CASE_IDX] + 1;
	int	index = total - 1;
	for (int i = 0; i < total; i++) {
		variables[operands[index - i].var - 65].value = ((idx[CASE_IDX] >> i) & 1);
		operands[index - i].value = ((idx[CASE_IDX] >> i) & 1);
	}
}

/**
 * @brief Generates and prints all rows of a truth table for a logical formula.
 *
 * @param[in] operands Array of unique variables in the formula (preprocessed)
 * @param[in] variables Array mapping each variable to its position index
 * @param[in] total Number of unique variables in the formula
 * @param[in] original Original formula string (for display purposes)
 *
 * @note Key implementation details:
 * - Uses bitwise operations to generate 2^total possible combinations
 * - cases_info array stores [current_case_index, total_cases]
 * - current_case_index starts at -1 to include 0-based iteration
 * - total_cases = (1 << total) - 1 (2^total - 1 combinations)
 *
 * @remark The loop:
 * 1. Updates variable values with update_case()
 * 2. Prints the current truth table row with print_line()
 * 3. Continues until all combinations are processed
 */
void	print_table(t_var *operands, t_var *variables, const int total, const char *original) {
	int	cases_info[2] = {-1, (1 << total) - 1};
	while (cases_info[CASE_IDX] < cases_info[TOTAL_CASES]) {
		update_case(operands, variables, total, &cases_info[0]);
		print_line(variables, original, &cases_info[0]);
	}
}

 /**
 * @brief Validates the syntax and evaluability of a logical formula.
 *
 * This function performs two key validations:
 * 1. Checks for unrecognized operator symbols
 * 2. Verifies the formula can be evaluated with dummy values
 *
 * @param[in] function The logical formula string to validate
 * @return t_bool TRUE if valid, FALSE if invalid
 *
 * @note Implementation details:
 * 1. Creates a modifiable copy of the input string
 * 2. Replaces all variables (A-Z) with '0' to test evaluability
 * 3. Uses eval_formula() to verify structural validity
 * 4. Returns memory-safe results (frees duplicated string)
 *
 * @warning The function uses strdup() which allocates memory.
 *          The duplicated string is explicitly freed before returning.
 *
 * @remark Error handling:
 * - Prints "UNRECOGNIZED SYMBOL" for invalid operators
 * - Prints "NOT SOLVABLE FUNCTION" for structurally invalid formulas
 */
t_bool	check_function(const char *function) {
	char *to_eval = strdup(function);
	for (int i = 0; to_eval[i]; i++) {
		if (to_eval[i] >= 'A' && to_eval[i] <= 'Z') {
			to_eval[i] = '0';
		} else {
			if (get_index_operator(to_eval[i]) == UNRECOGNIZED_SYMBOL) {
				table_error("UNRECOGNIZED SYMBOL\n");
				free(to_eval);
				return (FALSE);
			}
		}
	}
	const int	result = eval_formula(to_eval);
	free(to_eval);
	if (result < 0) {
		table_error("NOT SOLVABLE FUNCTION\n");
		return (FALSE);
	}
	return (TRUE);
}

/**
 * @brief Generates and prints the truth table for a logical formula.
 *
 * This function validates and processes a logical formula to:
 * 1. Identify unique propositional variables (A-Z)
 * 2. Validate operator symbols
 * 3. Delegate table creation and printing
 *
 * @param[in] function The logical formula string to analyze. Should contain:
 *                    - Uppercase letters (A-Z) as variables
 *                    - Valid operators: '!', '&', '|', '^', '>', '='
 *
 * @note The function performs these key steps:
 * 1. Initial validation with check_function()
 * 2. Variable extraction and indexing
 * 3. Delegation to print_table() for actual output
 *
 * @warning The t_var structures must have at least these fields:
 *          @code{.c}
 *          typedef struct s_var {
 *              char var;  // Stores the character (A-Z)
 *              int  idx;  // Stores the variable index
 *          } t_var;
 *          @endcode
 *
 * @remark Uses 27-element arrays to support A-Z variables (index 0-25)
 *         plus a termination element (index 26)
 */
void	print_truth_table(char *function) {
	if (!check_function(function))
		return;

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
		}
	}
	print_table(&operands[0], &variables[0], total, function);
}
