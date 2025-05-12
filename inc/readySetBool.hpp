/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readySetBool.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:53:33 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/12 20:03:47 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <stack>
#include <map>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <charconv>
#include "../boolft/inc/boolft.hpp"

typedef void (*t_entry)(int argc, char* argv[]);
uint32_t	parse_uint32_from_chars(const char* s);
// ----- Entrypoints
void		adder_entrypoint(int argc, char *argv[]);
void		multiplier_entrypoint(int argc, char *argv[]);
void		gray_code_entrypoint(int argc, char *argv[]);
void		eval_function_entrypoint(int argc, char *argv[]);
void		truth_table_entrypoint(int argc, char *argv[]);
void		sat_entrypoint(int argc, char *argv[]);
void		nnf_entrypoint(int argc, char *argv[]);
void		nnf_only_entrypoint(int argc, char *argv[]);
void		powerset_entrypoint(int argc, char *argv[]);
void		powerset_size_entrypoint(int argc, char *argv[]);
void		eval_set_entrypoint(int argc, char* argv[]);
void		cnf_entrypoint(int argc, char *argv[]);
void		cnf_only_entrypoint(int argc, char *argv[]);
void		map_entrypoint(int argc, char *argv[]);
void		reverse_map_entrypoint(int argc, char *argv[]);
void		map_and_reverse_entrypoint(int argc, char *argv[]);
