/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entrypoints.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:53:33 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/08 21:54:23 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readySetBool.hpp"

void	powerset_entrypoint(int argc, char *argv[]) {
	if (argc < 3) {
		std::cerr << "Powerset needs more than arguments: --powerset n0 n1.." << std::endl;
		exit(1);
	}
	(void)argv;
	size_t token_pos;
	std::vector<int> set;
	for (int i = 2; i < argc; i++) {
		try {
			int v = std::stoi(argv[i], &token_pos);
			if (std::find(set.begin(), set.end(), v) != set.end()) {
				std::cerr << "Powerset duplicated element." << std::endl;
				exit(1);
			}
			set.push_back(v);
		} catch (std::exception& e) {
			std::ostringstream detail;
			detail << "Error parsing argument: " << e.what();
			std::cerr << detail.str() << std::endl;
			exit(1);
		}
	}
	t_pwr_set rst = powerset(set);
	std::cout << "result size: " << rst.size() << ". expected: " << ((1 << (argc - 2))) << std::endl;
	for (t_set elem : rst) {
		if (elem.empty()) {
			std::cout << "[ø]" << std::endl;
			continue;
		}
		std::cout << "[" ;
		std::string sep = "";
		for (int v : elem) {
			std::cout << sep << v;
			sep = ",";
		}
		std::cout << "]" << std::endl;
	}
}

void	powerset_size_entrypoint(int argc, char *argv[]) {
	if (argc < 3) {
		std::cerr << "Powerset size needs more than arguments: --powerset-size n0 n1.." << std::endl;
		exit(1);
	}
	size_t token_pos;
	std::vector<int> set;
	for (int i = 2; i < argc; i++) {
		try {
			int v = std::stoi(argv[i], &token_pos);
			if (std::find(set.begin(), set.end(), v) != set.end()) {
				std::cerr << "Powerset duplicated element." << std::endl;
				exit(1);
			}
			set.push_back(v);
		} catch (std::exception& e) {
			std::ostringstream detail;
			detail << "Error parsing argument: " << e.what();
			std::cerr << detail.str() << std::endl;
			exit(1);
		}
	}
	t_pwr_set rst = powerset(set);
	std::cout << "result size: " << rst.size() << ". expected: " << ((1 << (argc - 2))) << std::endl;
}

void	nnf_entrypoint(int argc, char *argv[]) {
	if (argc != 3) {
		std::cerr << "NNF needs 2 arguments: --nnf funcString" << std::endl;
		exit(1);
	}
	std::string rst = negation_normal_form(argv[2]);
	std::string orig(argv[2]);
	std::cout << "original: " << + "\"" + orig + "\"" << std::endl;
	print_truth_table(argv[2]);
	std::cout << "converted: " << + "\"" + rst + "\"" << std::endl;
	print_truth_table((char *)rst.c_str());
}

void	nnf_only_entrypoint(int argc, char *argv[]) {
	if (argc != 3) {
		std::cerr << "NNF only needs 2 arguments: --nnf funcString" << std::endl;
		exit(1);
	}
	std::cout << negation_normal_form(argv[2]) << std::endl;
}

void	sat_entrypoint(int argc, char *argv[]) {
	if (argc != 3) {
		std::cerr << "SAT needs 2 arguments: --sat funcString" << std::endl;
		exit(1);
	}
	std::cout << BOOL_TO_STRING(sat(argv[2])) << std::endl;
}

void	truth_table_entrypoint(int argc, char *argv[]) {
	if (argc != 3) {
		std::cerr << "Truth Table eval needs 2 arguments: --truth-table funcString" << std::endl;
		exit(1);
	}
	print_truth_table(argv[2]);
}

void	eval_function_entrypoint(int argc, char *argv[]) {
	if (argc != 3) {
		std::cerr << "Function eval needs 2 arguments: --function funcString" << std::endl;
		exit(1);
	}
	std::cout << BOOL_TO_STRING(eval_formula(argv[2])) << std::endl;
}

void	gray_code_entrypoint(int argc, char *argv[]) {
	if (argc != 3) {
		std::cerr << "Gray code needs 2 arguments: --gray-code nNatural" << std::endl;
		exit (1);
	}
	int a;
	size_t	token_pos;
	try {
		a = std::stoi(argv[2], &token_pos);
	} catch (std::exception& e) {
		std::ostringstream detail;
		detail << "Error parsing argument: " << e.what();
		std::cerr << detail.str() << std::endl;
		exit(1);
	}
	std::cout << gray_code(a) << std::endl;
}

void	multiplier_entrypoint(int argc, char *argv[]) {
	if (argc != 4) {
		std::cerr << "Multiplier needs 3 arguments: --multiplier nNatural nNatural" << std::endl;
		exit (1);
	}
	size_t	token_pos = 0;
	int a, b;
	try {
		a = std::stoi(argv[2], &token_pos);
		b = std::stoi(argv[3], &token_pos);
	} catch (std::exception& e) {
		std::ostringstream detail;
		detail << "Error parsing argument: " << e.what();
		std::cerr << detail.str() << std::endl;
		exit(1);
	}
	if (a < 0 || b < 0) {
		std::cerr << "Only natural numbers are valid as input" << std::endl;
		exit(1);
	}
	std::cout << multiplier(a, b) << std::endl;
}

void	adder_entrypoint(int argc, char *argv[]) {
	if (argc != 4) {
		std::cerr << "Adder needs 3 arguments: --adder nNatural nNatural" << std::endl;
		exit (1);
	}
	size_t	token_pos = 0;
	int a, b;
	try {
		a = std::stoi(argv[2], &token_pos);
		b = std::stoi(argv[3], &token_pos);
	} catch (std::exception& e) {
		std::ostringstream detail;
		detail << "Error parsing argument: " << e.what();
		std::cerr << detail.str() << std::endl;
		exit(1);
	}
	if (a < 0 || b < 0) {
		std::cerr << "Only natural numbers are valid as input" << std::endl;
		exit(1);
	}
	std::cout << adder(a, b) << std::endl;
}