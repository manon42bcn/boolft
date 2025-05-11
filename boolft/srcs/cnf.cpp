/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cnf.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:23:54 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/10 17:52:42 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "boolft.hpp"

#include <string>
#include <sstream>
#include <stack>
#include <vector>
#include <cctype>

int total_cnf = 0;

/**
 * @brief Recursively collects nodes of a specific type from a CNF expression tree.
 *
 * This function traverses the given CNF expression tree rooted at @p node and collects all nodes
 * that do NOT match the specified @p type. When a node matches the given @p type, the function
 * continues to recursively traverse its left and right subtrees. When a node does not match the
 * type, it is added to the output vector @p out, and its subtree is not further explored.
 *
 * This behavior is useful for extracting the top-level nodes of a certain type (e.g., AND/OR)
 * from a logical expression tree, which is a common step in CNF (Conjunctive Normal Form)
 * transformations and analysis.
 *
 * @param[in]  node  Pointer to the root of the current (sub)tree to process. May be nullptr.
 * @param[in]  type  The node type to match for recursive traversal (e.g., AND, OR).
 * @param[out] out   Vector to store pointers to nodes that do not match @p type.
 *
 * @note The function does not modify the tree structure; it only collects pointers to nodes.
 *       The output vector @p out is appended to; it is not cleared at the start of the function.
 */
void collect_nodes(s_cnf_node* node, e_type type, std::vector<s_cnf_node*>& out) {
	if (!node)
		return;
	if (node->type == type) {
		collect_nodes(node->left,  type, out);
		collect_nodes(node->right, type, out);
	} else {
		out.push_back(node);
	}
}

/**
 * @brief Flattens nested logical operators of the same type in a CNF expression tree.
 *
 * This function restructures the given CNF expression tree rooted at @p node by merging
 * consecutive nodes of the same logical operator type (AND or OR) into a flat, right-associative
 * binary tree. The flattening process simplifies the tree structure, making it easier to
 * manipulate and analyze in subsequent CNF transformations.
 *
 * For example, a tree representing (A AND (B AND C)) will be flattened to ((A AND B) AND C).
 * This is achieved by collecting all direct sub-nodes of the same operator type using
 * @ref collect_nodes, and then reconstructing the tree as a chain of binary nodes.
 *
 * @param[in] node Pointer to the root of the (sub)tree to flatten. May be nullptr.
 * @return Pointer to the root of the flattened tree. Returns nullptr if @p node is nullptr.
 *         If @p node is not an AND or OR node, returns @p node unchanged.
 *
 * @note The function allocates new nodes for the flattened tree structure, except for the
 *       first element in the flattened list. The caller is responsible for managing memory
 *       and deleting any nodes created by this function.
 *
 */
s_cnf_node* flatten(s_cnf_node* node) {
	if (!node)
		return nullptr;
	if (node->type != AND && node->type != OR)
		return node;
	std::vector<s_cnf_node*> elems;
	collect_nodes(node, node->type, elems);
	if (elems.empty())
		return node;
	s_cnf_node* res = elems[0];
	for (size_t i = 1; i < elems.size(); ++i) {
		res = new s_cnf_node(node->type, "", res, elems[i]);
	}
	return res;
}

/**
 * @brief Recursively normalizes a CNF expression tree by flattening nested logical operators.
 *
 * This function traverses the given CNF expression tree rooted at @p root and recursively
 * normalizes all AND and OR nodes by flattening nested operators of the same type. The
 * normalization process ensures that the tree structure is as flat as possible, which
 * simplifies further processing and analysis in CNF transformations.
 *
 * The function also tracks the total number of CNF nodes processed using the global
 * variable @c total_cnf. If the number of processed nodes exceeds a predefined threshold
 * (1,000,000), the function throws a @c BoolFtException to prevent excessive memory
 * consumption or infinite recursion due to malformed input.
 *
 * @param[in] root Pointer to the root of the CNF expression tree to normalize. May be nullptr.
 * @return Pointer to the root of the normalized (flattened) CNF tree. Returns nullptr if @p root is nullptr.
 *
 * @throws BoolFtException If the total number of CNF nodes processed exceeds 1,000,000,
 *         indicating a possible runaway recursion or excessively large expression.
 *
 * @note The function modifies the input tree in place by updating its left and right pointers.
 *       It also calls @ref flatten to further simplify AND/OR nodes after recursive normalization.
 *
 * @warning The function relies on the global variable @c total_cnf, which must be properly
 *          initialized and managed outside this function. If the input tree is malformed
 *          (e.g., cyclic), this function may result in infinite recursion or resource exhaustion.
 */
s_cnf_node* normalize_cnf(s_cnf_node* root) {
	if (!root) {
		return nullptr;
	}
	total_cnf++;
	if (total_cnf > 1000000) {
		throw BoolFtException("CNF Normalization grew too much.");
	}
	if (root->type == AND || root->type == OR) {
		root->left  = normalize_cnf(root->left);
		root->right = normalize_cnf(root->right);
		root = flatten(root);
	}
	return root;
}

/**
 * @brief Recursively distributes OR over AND to produce CNF-compliant subtrees.
 *
 * This function applies the distributive law to two CNF expression subtrees, @p a and @p b,
 * ensuring that any OR operation is properly distributed over AND operations. This is a
 * fundamental step in transforming logical expressions into Conjunctive Normal Form (CNF).
 *
 * The function works recursively:
 * - If @p b is an AND node, it distributes @p a over both children of @p b.
 * - If @p a is an AND node, it distributes both children of @p a over @p b.
 * - Otherwise, it creates a new OR node combining @p a and @p b.
 *
 * This guarantees that all ORs are pushed down below ANDs, as required by CNF.
 *
 * @param[in] a Pointer to the first CNF subtree operand.
 * @param[in] b Pointer to the second CNF subtree operand.
 * @return Pointer to the root of the newly constructed CNF subtree.
 *
 * @note The function allocates new nodes for the resulting tree structure. The caller is
 *       responsible for managing and deleting these nodes to avoid memory leaks.
 *
 * @warning The function assumes that both input trees are valid and non-null. Passing
 *          nullptr as either argument will result in undefined behavior. The function
 *          does not perform cycle detection or input validation.
 */
s_cnf_node* distribute(s_cnf_node* a, s_cnf_node* b) {
	if (b->type == AND)
		return new s_cnf_node(AND, "", distribute(a, b->left), distribute(a, b->right));
	if (a->type == AND)
		return new s_cnf_node(AND, "", distribute(a->left, b), distribute(a->right, b));
	return new s_cnf_node(OR, "", a, b);
}

/**
 * @brief Recursively transforms a logical expression tree into Conjunctive Normal Form (CNF).
 *
 * This function converts a logical expression tree rooted at @p node into its equivalent
 * Conjunctive Normal Form (CNF). It assumes that any necessary preprocessing steps
 * (such as eliminating implications and pushing negations inward) have already been performed.
 *
 * The function operates recursively:
 * - If @p node is a variable or a negation, it is returned as-is.
 * - Otherwise, it recursively transforms the left and right subtrees.
 * - For OR nodes, it applies the distributive law using @ref distribute to ensure that
 *   ORs are pushed below ANDs, as required by CNF.
 * - For AND nodes, it reconstructs the AND node with its (possibly transformed) children.
 *
 * @param[in] node Pointer to the root of the logical expression tree to transform. May be nullptr.
 * @return Pointer to the root of the CNF-transformed tree. Returns nullptr if @p node is nullptr.
 *
 * @note The function allocates new nodes for AND operations resulting from the transformation.
 *       The caller is responsible for managing and deleting these nodes to avoid memory leaks.
 *
 * @warning The function assumes the input tree is well-formed and that negations are only
 *          applied directly to variables (i.e., the tree is in Negation Normal Form, NNF).
 *          Passing malformed or cyclic trees may result in infinite recursion or undefined behavior.
 */
s_cnf_node* to_cnf(s_cnf_node* node) {
	if (!node || node->type == VAR || node->type == NOT)
		return node;
	node->left  = to_cnf(node->left);
	node->right = to_cnf(node->right);

	if (node->type == OR) {
		return distribute(node->left, node->right);
	} else {
		return new s_cnf_node(AND, "", node->left, node->right);
	}
}

/**
 * @brief Constructs a logical expression tree in Negation Normal Form (NNF) from an RPN string.
 *
 * This function parses a logical expression given in Reverse Polish Notation (RPN) and builds
 * its corresponding expression tree in Negation Normal Form (NNF). The RPN string should use
 * the following characters:
 * - '&' for AND
 * - '|' for OR
 * - '!' for NOT
 * - Any other character is treated as a variable (operand)
 *
 * The function uses a stack to process the RPN string:
 * - For each binary operator ('&', '|'), it pops two operands from the stack, constructs the
 *   corresponding node, and pushes it back onto the stack.
 * - For the unary operator ('!'), it pops one operand, constructs a NOT node, and pushes it back.
 * - For operands (variables), it creates a variable node and pushes it onto the stack.
 *
 * If the RPN string is malformed (e.g., insufficient operands for an operator), the function
 * throws a @c BoolFtException.
 *
 * @param[in] rpn The RPN string representing the logical expression in NNF.
 * @return Pointer to the root of the constructed expression tree. Returns nullptr if the input is empty.
 *
 * @throws BoolFtException If the RPN string is malformed (e.g., not enough operands for an operator).
 *
 * @note The function allocates new nodes for the tree. The caller is responsible for deleting
 *       the tree to avoid memory leaks.
 *
 * @warning The function assumes that the input string is a valid RPN expression in NNF.
 *          Invalid or malformed input will result in exceptions or undefined behavior.
 */
s_cnf_node* nnf_to_tree(const std::string& rpn) {
	std::stack<s_cnf_node*> st;
	for (char c : rpn) {
		if (c == '&' || c == '|') {
			if (st.size() < 2)
				throw BoolFtException("Binary Operator Error.");
			s_cnf_node* right = st.top(); st.pop();
			s_cnf_node* left  = st.top(); st.pop();
			st.push(new s_cnf_node(c == '&' ? AND : OR, "", left, right));
		}
		else if (c == '!') {
			if (st.empty())
				throw BoolFtException("Binary Operator Error.");
			s_cnf_node* lit = st.top(); st.pop();
			st.push(new s_cnf_node(NOT, "", lit, nullptr));
		}
		else {
			std::string op(1, c);
			st.push(new s_cnf_node(VAR, op));
		}
	}
	return st.empty() ? nullptr : st.top();
}

/**
 * @brief Converts a logical expression tree into its Reverse Polish Notation (RPN) representation.
 *
 * This function traverses a logical expression tree rooted at @p node and serializes it into
 * Reverse Polish Notation (RPN), also known as postfix notation. The resulting RPN tokens are
 * appended to the output vector @p out.
 *
 * The function processes the tree recursively:
 * - For variable nodes, it appends the variable name to @p out.
 * - For NOT nodes, it processes the child node and then appends the "!" operator.
 * - For AND/OR nodes, it processes the left and right subtrees, then appends "&" or "|" respectively.
 *
 * This serialization is useful for exporting, storing, or further processing logical expressions
 * in a compact and operator-precedence-free format.
 *
 * @param[in]  node Pointer to the root of the logical expression tree. May be nullptr.
 * @param[out] out  Vector to which the RPN tokens will be appended.
 *
 * @note The function does not clear @p out before appending tokens; it is the caller's responsibility
 *       to clear the vector if needed.
 *
 * @warning The function assumes that the input tree is well-formed. If the tree is malformed
 *          (e.g., contains cycles), this function may result in infinite recursion.
 */
void tree_to_rpn(s_cnf_node* node, std::vector<std::string>& out) {
	if (!node)
		return;
	if (node->type==VAR) {
		out.push_back(node->var);
	} else if (node->type==NOT) {
		tree_to_rpn(node->left, out);
		out.push_back("!");
	} else {
		tree_to_rpn(node->left,  out);
		tree_to_rpn(node->right, out);
		out.push_back(node->type==AND ? "&" : "|");
	}
}

/**
 * @brief Converts a logical formula string into its Conjunctive Normal Form (CNF) in RPN.
 *
 * This high-level function takes a logical formula as a C-string, converts it to Negation Normal Form (NNF),
 * builds an expression tree, transforms it into Conjunctive Normal Form (CNF), normalizes the CNF tree,
 * and finally serializes the result into a Reverse Polish Notation (RPN) string.
 *
 * The conversion process involves several steps:
 * 1. Converts the input formula to NNF in RPN form using @c negation_normal_form.
 * 2. Builds an NNF expression tree from the RPN string using @ref nnf_to_tree.
 * 3. Transforms the tree into CNF using @ref to_cnf.
 * 4. Normalizes the CNF tree structure using @ref normalize_cnf.
 * 5. Serializes the normalized CNF tree back to an RPN string using @ref tree_to_rpn.
 *
 * If any step fails (e.g., due to malformed input), the function throws a @c BoolFtException
 * with detailed error information.
 *
 * @param[in] formula The input logical formula as a null-terminated C-string.
 * @return A std::string containing the CNF of the input formula, serialized in RPN.
 *
 * @throws BoolFtException If any error occurs during parsing, transformation, or serialization.
 *
 * @note The function assumes that @c negation_normal_form is available and returns
 *       a valid RPN string in NNF. The function manages memory for intermediate trees,
 *       but may leak memory if an exception is thrown after allocation and before cleanup.
 *
 * @warning The function does not validate the input formula for correctness beyond what is
 *          handled by downstream functions. Malformed or excessively complex formulas may
 *          result in exceptions or resource exhaustion.
 */

void	clear_tree(s_cnf_node* node) {
	if (!node) return;
	std::cout << "node after " << node->left << std::endl;
	std::cout << "left " << node->left << std::endl;
	clear_tree(node->left);
	std::cout << "right " << node->right << std::endl;
	clear_tree(node->right);
	std::cout << "node " << node << std::endl;
	delete node;
	node = nullptr;
}

std::string conjunctive_normal_form(char* formula) {
	std::string nnf_rpn = negation_normal_form(formula);
	try {
		s_cnf_node* nnf_root = nnf_to_tree(nnf_rpn);
		s_cnf_node* cnf_root = to_cnf(nnf_root);
		cnf_root = normalize_cnf(cnf_root);
		std::vector<std::string> cnf_tokens;
		tree_to_rpn(cnf_root, cnf_tokens);
		std::ostringstream oss;
		for (size_t i = 0; i < cnf_tokens.size(); ++i) {
			if (i) oss << "";
			oss << cnf_tokens[i];
		}
		clear_tree(nnf_root);
//		clear_tree(cnf_root);
//		delete nnf_root;
//		delete cnf_root;
		return oss.str();
	} catch (std::exception& e) {
		std::ostringstream details;
		details << "Error Building CNF function: " << e.what();
		throw BoolFtException(details.str());
	}
}
