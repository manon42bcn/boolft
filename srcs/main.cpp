/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:53:33 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/09 01:22:10 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readySetBool.hpp"

void	cnf_entrypoint(int argc, char *argv[]) {
	if (argc < 1) {
		std::cerr << "Eval sets needs more 6 than arguments: --eval-set A n0 n1.. B ... e" << std::endl;
		exit(1);
	}
	std::string nnf = negation_normal_form(argv[2]);
	std::string cnf = conjunctive_normal_form(argv[2]);
	std::cout << "original: \"" << argv[2] << "\"" << std::endl;
	print_truth_table(argv[2]);
	std::cout << "NNF: \"" << nnf << "\"" << std::endl;
	print_truth_table((char *)nnf.c_str());
	std::cout << "CNF: \"" << cnf << "\"" << std::endl;
	print_truth_table((char *)cnf.c_str());
}

void	cnf_only_entrypoint(int argc, char *argv[]) {
	if (argc < 1) {
		std::cerr << "Eval sets needs more 6 than arguments: --eval-set A n0 n1.. B ... e" << std::endl;
		exit(1);
	}
	std::cout << conjunctive_normal_form(argv[2]) << std::endl;
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
		entrypoint["--nnf-only"] = &nnf_only_entrypoint;
		entrypoint["--sat"] = &sat_entrypoint;
		entrypoint["--powerset"] = &powerset_entrypoint;
		entrypoint["--powerset-size"] = &powerset_size_entrypoint;
		entrypoint["--eval-set"] = &eval_set_entrypoint;
		entrypoint["--cnf"] = &cnf_entrypoint;
		entrypoint["--cnf-only"] = &cnf_only_entrypoint;
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
