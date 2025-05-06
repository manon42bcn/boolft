/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nnf.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 18:01:11 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/06 21:51:09 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "boolft.hpp"




void	operate(std::vector<std::string>& fnc, char op) {
	if (op == '!') {
		// Negación: desapila un operando
		if (fnc.empty())
			throw BoolFtException("Stack underflow.");
		std::string a = fnc.back(); fnc.pop_back();
		// Si es una variable, simplemente niega
		if (a.size() == 1 && !is_operator(a[0])) {
			fnc.push_back(a + "!");
		} else {
			std::cout << "Morgan " << std::endl;
			// Si es una expresión compuesta, aplicar De Morgan
//			fnc.push_back(apply_de_morgan(a));
		}
	} else if (op == '&' || op == '|') {
		// Conjunción o disyunción: desapila dos operandos
		if (fnc.size() < 2)
			throw BoolFtException("Stack underflow.");
		std::string b = fnc.back(); fnc.pop_back();
		std::string a = fnc.back(); fnc.pop_back();
		fnc.push_back(a + b + op);
	}
}

std::string negation_normal_form(char *formula) {
	std::vector<std::string> fnc;
	for (size_t i = 0; formula[i]; i++) {
		if (formula[i] >= 'A' && formula[i] <= 'Z') {
			std::string operands;
			operands.assign(formula + i, 1);
			fnc.push_back(operands);
		} else {
			if (!is_operator(formula[i]))
				throw BoolFtException("Unrecognized symbol.");
			operate(fnc, formula[i]);
		}
	}
	for (auto it = fnc.begin(); it != fnc.end(); it++) {
		std::cout << *it << std::endl;
	}
	return "hello";
}