#include "readySetBool.h"

void	gray_code_entrypoint(int argc, char *argv[]) {
	if (argc != 3) {
		ft_putstr_fd("Gray code needs 2 arguments: --graycode nNatural", STDERR_FILENO);
		exit (1);
	}
	int a = atoi(argv[2]);
	printf("%d\n", gray_code(a));
}

void	multiplier_entrypoint(int argc, char *argv[]) {
	if (argc != 4) {
		ft_putstr_fd("Multiplier needs 3 arguments: --multiplier nNatural nNatural", STDERR_FILENO);
		exit (1);
	}
	int a = atoi(argv[2]);
	int b = atoi(argv[3]);
	if (a < 0 || b < 0) {
		ft_putstr_fd("Only natural numbers are valid as input\n", STDERR_FILENO);
		exit(1);
	}
	printf("%d\n", multiplier(a, b));
}

void	adder_entrypoint(int argc, char *argv[]) {
	if (argc != 4) {
		ft_putstr_fd("Adder needs 3 arguments: --adder nNatural nNatural", STDERR_FILENO);
		exit (1);
	}
	int a = atoi(argv[2]);
	int b = atoi(argv[3]);
	if (a < 0 || b < 0) {
		ft_putstr_fd("Only natural numbers are valid as input\n", STDERR_FILENO);
		exit(1);
	}
	printf("%d\n", adder(a, b));
}
