/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nnf.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 18:01:11 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/07 00:33:06 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "boolft.hpp"

static std::pair<std::string,std::string> split_rpn(const std::string& expr) {
	int n = expr.size();
	if (n < 3)
		throw BoolFtException("split_rpn: expresión demasiado corta");
	char root = expr[n-1];
	if (root!='&' && root!='|')
		throw BoolFtException("split_rpn: no es operator binario");
	// Encontrar el punto donde la pila virtual vuelve a 1
	int depth = 0;
	int splitPos = -1;
	for (int i = 0; i < n-1; ++i) {
		char c = expr[i];
		if (std::isupper(c)) {
			depth += 1;
		} else if (c == '!') {
			// neto 0: pop1 push1
		} else {
			// &,| => pop2 push1  → neto -1
			depth -= 1;
		}
		if (depth == 1) {
			splitPos = i+1;
			break;
		}
	}
	if (splitPos < 1)
		throw BoolFtException("split_rpn: no pudo partir");
	return {
			expr.substr(0, splitPos),
			expr.substr(splitPos, (n-1) - splitPos)
	};
}

// Aplica NNF a ¬(expr), devolviendo un RPN válido en {&,|,!}
static std::string nnf_apply_neg(const std::string& expr) {
	int n = expr.size();
	// caso atómico: A → A!
	if (n == 1 && std::isupper(expr[0])) {
		return expr + "!";
	}
	char root = expr[n-1];
	std::cout << root << " here? " << expr << std::endl;
	if (root == '!') {
		// doble negación: (X!)!  → X
		return expr.substr(0, n-1);
	}
	if (root=='&' || root=='|') {
		// De Morgan: ¬(E1 ∧ E2) = ¬E1 ∨ ¬E2, ¬(E1 ∨ E2) = ¬E1 ∧ ¬E2
		auto [e1, e2] = split_rpn(expr);
		char dual = (root=='&' ? '|' : '&');
		std::string ne1 = nnf_apply_neg(e1);
		std::string ne2 = nnf_apply_neg(e2);
		return ne1 + ne2 + dual;
	}
	// debería haber convertido ya toda implicación/equiv/etc
	std::string e("nnf_apply_neg: token inesperado" + expr);
	throw BoolFtException(e);
}

// fnc: stack de subfórmulas RPN ya en NNF (o variables sueltas)
// op: operador actual extraído de la entrada original
void operate(std::vector<std::string>& fnc, char op) {
	using std::string;
	if (op == '!') {
		if (fnc.empty()) throw std::runtime_error("¡Pila vacía al negar!");
		string s = fnc.back(); fnc.pop_back();
		fnc.push_back( nnf_apply_neg(s) );
		return;
	}
	// para binarios, primero pop orden inverso: b = top, a = siguiente
	if (fnc.size() < 2)
		throw std::runtime_error("¡Pila con <2 operandos para binario!");
	string b = fnc.back(); fnc.pop_back();
	string a = fnc.back(); fnc.pop_back();

	string res;
	switch(op) {
		case '&': // conjunción, ya NNF
		case '|': // disyunción, ya NNF
			res = a + b + op;
			break;
		case '>': {
			// A → B  ≡  ¬A ∨ B
			string na = nnf_apply_neg(a);
			res = na + b + "|";
			break;
		}
		case '=': {
			// A ≡ B  ≡  (A ∧ B) ∨ (¬A ∧ ¬B)
			string na = nnf_apply_neg(a);
			string nb = nnf_apply_neg(b);
			// construye: A B &   na nb &   |
			res = a + b + "&"
				  + na + nb + "&"
				  + "|";
			break;
		}
		case '^': {
			// A ⊕ B  ≡  (A ∧ ¬B) ∨ (¬A ∧ B)
			string nb = nnf_apply_neg(b);
			string na = nnf_apply_neg(a);
			// sub1 = A nb &, sub2 = na B &
			res = a + nb + "&"
				  + na + b  + "&"
				  + "|";
			break;
		}
		default:
			throw BoolFtException("Operador desconocido");
	}
	fnc.push_back(res);
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