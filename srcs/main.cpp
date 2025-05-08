/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:53:33 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/08 22:18:12 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readySetBool.hpp"

void	eval_set_entrypoint(int argc, char* argv[]) {
	if (argc < 6) {
		std::cerr << "Eval sets needs more 6 than arguments: --eval-set A n0 n1.. B ... e" << std::endl;
		exit(1);
	}
	std::vector<std::vector<int>> all_sets;
	std::vector<int> set;
	set.clear();
	size_t token_pos;
	for (int i = 3; i < argc; i++) {
		if (argv[i][0] == 'e') {
			if (!set.empty()) {
				all_sets.push_back(set);
			}
			break;
		}
		if (argv[i][0] >= 'A' && argv[i][0] <= 'Z') {
			if (!set.empty()) {
				all_sets.push_back(set);
			}
			set.clear();
			continue;
		}
		try {
			int v = std::stoi(argv[i], &token_pos);
			if (std::find(set.begin(), set.end(), v) != set.end()) {
				std::cerr << "Set duplicated element." << std::endl;
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
	t_set rst = eval_set(argv[2], all_sets);
	std::cout << "[" ;
	std::string sep = "";
	for (int elem : rst) {
		std::cout << sep << elem;
		sep = ",";
	}
	std::cout << "]" << std::endl;
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
