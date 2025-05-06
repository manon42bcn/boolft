/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readySetBool.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:53:33 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/06 18:34:03 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <stack>
#include <map>
#include <sstream>
#include <cstdlib>
#include "../boolft/inc/boolft.hpp"

typedef void (*t_entry)(int argc, char* argv[]);
// ----- Entrypoints
void	adder_entrypoint(int argc, char *argv[]);
void	multiplier_entrypoint(int argc, char *argv[]);
void	gray_code_entrypoint(int argc, char *argv[]);
void	eval_function_entrypoint(int argc, char *argv[]);
void	truth_table_entrypoint(int argc, char *argv[]);

