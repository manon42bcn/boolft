/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:53:33 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/06 22:13:30 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readySetBool.hpp"

void	nnf_entrypoint(int argc, char *argv[]) {
	if (argc != 3) {
		std::cerr << "NNF needs 2 arguments: --nnf funcString" << std::endl;
		exit(1);
	}
	negation_normal_form(argv[2]);
}

void	sat_entrypoint(int argc, char *argv[]) {
	if (argc != 3) {
		std::cerr << "SAT needs 2 arguments: --sat funcString" << std::endl;
		exit(1);
	}
	std::cout << BOOL_TO_STRING(sat(argv[2])) << std::endl;
}

void	entrypoint(int argc, char *argv[]) {
	static std::map<std::string, t_entry> entrypoint;
	std::string flag(argv[1]);
	if (entrypoint.empty()) {
		entrypoint["--adder"] =  &adder_entrypoint;
		entrypoint["--multiplier"] = &multiplier_entrypoint;
		entrypoint["--gray-code"] = &gray_code_entrypoint;
		entrypoint["--function"] = &eval_function_entrypoint;
		entrypoint["--truth-table"] = &truth_table_entrypoint;
		entrypoint["--nnf"] = &nnf_entrypoint;
		entrypoint["--sat"] = &sat_entrypoint;
	}
	auto it = entrypoint.find(flag);
	if (it != entrypoint.end()) {
		it->second(argc, argv);
		return;
	}
	std::cerr << "Not available flag: " << flag << std::endl;
	exit(1);
}

int main (int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "Not enough arguments to run" << std::endl;
		exit (1);
	}
	try {
		entrypoint(argc, argv);
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
