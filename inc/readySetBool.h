
#include "../boolft/inc/boolft.h"

typedef void (*t_entry)(int argc, char* argv[]);
void	ft_putstr_fd(char *s, int fd);
t_bool	ft_match_cmp(char *s1, char *s2);
// Entrypoints
void	gray_code_entrypoint(int argc, char *argv[]);
void	multiplier_entrypoint(int argc, char *argv[]);
void	adder_entrypoint(int argc, char *argv[]);
void	eval_function_entrypoint(int argc, char *argv[]);