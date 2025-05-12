/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:53:33 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/12 16:02:47 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readySetBool.hpp"

uint32_t parse_uint32_from_chars(const char* s) {
	uint32_t val = 0;
	auto first = s;
	auto last  = s + std::strlen(s);
	auto result = std::from_chars(first, last, val, 10);

	if (result.ec == std::errc::invalid_argument) {
		throw std::invalid_argument("Invalid Argument to parse to uint32_t.");
	}
	if (result.ec == std::errc::result_out_of_range) {
		throw std::out_of_range("Out of range to parse uint32_t.");
	}
	return val;
}

void	reverse_map_entrypoint(int argc, char *argv[]) {
	if (argc != 3) {
		std::cerr << "Reverse Map test entrypoint needs 2 arguments: --reverse-map mappedDouble" << std::endl;
		exit(1);
	}
	(void)argv;
	uint32_t x = 0, y = 0;

	errno = 0;
	char* endptr = nullptr;
	double mapped = std::strtod(argv[2], &endptr);
	if (errno == ERANGE) {
		throw std::out_of_range("Out of range double.");
	}
	if (endptr == argv[0]) {
		throw std::invalid_argument("Invalid Argument to parse to double.");
	}
	reverse_map(mapped, x, y);
	std::cout << "Input " << std::setprecision(std::numeric_limits<double>::max_digits10) <<  mapped
	          << " : X = " << x << " Y = " << y << std::endl;
}

void	map_entrypoint(int argc, char *argv[]) {
	if (argc != 4) {
		std::cerr << "Map test entrypoint needs 3 arguments: --map strX strY" << std::endl;
		exit(1);
	}
	(void)argv;
	uint32_t x = parse_uint32_from_chars(argv[2]);
	uint32_t y = parse_uint32_from_chars(argv[3]);
	double db = map(x, y);
	std::cout << "Input X = " << x << " Y = " << y << " Mapped: " << std::setprecision(std::numeric_limits<double>::max_digits10) << db << std::endl;

}


void	map_and_reverse_entrypoint(int argc, char *argv[]) {
	if (argc != 4) {
		std::cerr << "Map and reverse test entrypoint needs 3 arguments: --map-and-reverse strX strY" << std::endl;
		exit(1);
	}
	(void)argv;
	uint32_t x = parse_uint32_from_chars(argv[2]);
	uint32_t y = parse_uint32_from_chars(argv[3]);
	double db = map(x, y);
	uint32_t x_p = 0, y_p = 0;
	reverse_map(db, x_p, y_p);
	std::string eval;
	if (x == x_p && y == y_p)
		eval = "OK";
	else
		eval = "KO";
	std::cout << "Input X = " << x << " Y = " << y << " Mapped: "
	          << std::setprecision(std::numeric_limits<double>::max_digits10) << db
			  << " X' = " << x_p << " Y' = " << y_p << " consistency: "
			  << eval << std::endl;
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
		entrypoint["--map"] = &map_entrypoint;
		entrypoint["--reverse-map"] = &reverse_map_entrypoint;
		entrypoint["--map-and-reverse"] = &map_and_reverse_entrypoint;
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
