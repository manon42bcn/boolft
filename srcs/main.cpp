/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:53:33 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/12 20:07:46 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readySetBool.hpp"

/**
 * @brief Dispatches execution to the appropriate subcommand based on the first CLI flag.
 *
 * @details
 * This function serves as the central dispatcher for all supported command-line
 * operations. It maintains a static lookup table (`std::map<std::string, t_entry>`)
 * mapping flag strings (e.g., `"--adder"`, `"--cnf-only"`) to their corresponding
 * entrypoint handler functions (`t_entry`). On the first invocation, the map is
 * populated with all available handlers. Subsequent calls reuse the initialized map
 * to avoid re-registering handlers.
 *
 * Behavior:
 *   1. Reads the flag from `argv[1]`.
 *   2. If the static map is empty, populates it with all supported flag→handler pairs.
 *   3. Looks up the flag in the map:
 *      - If found, invokes the corresponding handler function, forwarding `argc` and `argv`.
 *      - If not found, writes an error message to `std::cerr` and terminates the program
 *        with exit code `1`.
 *
 * @param[in] argc
 *   The number of command-line arguments, including the program name.
 * @param[in] argv
 *   Array of C-strings representing the command-line arguments:
 *   - `argv[0]` is the program name.
 *   - `argv[1]` is the flag selecting the operation.
 *   - `argv[2]`...`argv[argc-1]` are additional parameters passed to the handler.
 *
 * @note
 *   - Handlers are only registered on the first call to avoid reinitialization overhead.
 *   - The program will terminate via `std::exit(1)` if an unsupported flag is provided.
 *   - Handler functions are responsible for their own argument validation and error reporting.
 */
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

/**
 * @brief Program entry point that dispatches to the actual application logic.
 *
 * @details
 * The `main` function performs basic argument validation and error handling before
 * invoking the core logic via `entrypoint()`. It ensures the program is invoked with
 * at least one argument (in addition to the program name), prints an error message and
 * exits if not, and wraps the call to `entrypoint` in a try-catch block to handle
 * any exceptions gracefully.
 *
 * @param[in] argc
 *   The number of command-line arguments, including the program name.
 * @param[in] argv
 *   Array of C-strings representing the command-line arguments.
 *   - `argv[0]` is the program name.
 *   - `argv[1]` through `argv[argc-1]` are the user-supplied arguments.
 *
 * @return
 *   Returns `0` on successful execution of the program logic.
 *   Returns `1` if:
 *     - Not enough arguments were provided (`argc < 2`), or
 *     - An exception of type `std::exception` (or a subclass) was thrown by `entrypoint()`.
 *
 * @throws
 *   Does not throw exceptions itself. Catches all `std::exception` thrown by
 *   `entrypoint()` and reports the error message to `std::cerr`.
 */
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
