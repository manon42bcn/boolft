/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readySetBool.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 09:39:49 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/05 11:23:06 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../boolft/inc/boolft.h"

typedef void (*t_entry)(int argc, char* argv[]);
void	ft_putstr_fd(char *s, int fd);
t_bool	ft_match_cmp(char *s1, char *s2);
char	*ft_strtrim_clean(char *s1, char const *set);
// Entrypoints
void	gray_code_entrypoint(int argc, char *argv[]);
void	multiplier_entrypoint(int argc, char *argv[]);
void	adder_entrypoint(int argc, char *argv[]);
void	eval_function_entrypoint(int argc, char *argv[]);
void	truth_table_entrypoint(int argc, char *argv[]);
