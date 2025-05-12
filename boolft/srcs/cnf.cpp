/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cnf.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras- <manon42bcn@yahoo.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:23:54 by mporras-          #+#    #+#             */
/*   Updated: 2025/05/12 12:17:27 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "boolft.hpp"

/**
 * @brief Destructor for CNF_Stack.
 *
 * @details Deletes all nodes allocated during CNF construction to prevent memory leaks.
 */
CNF_Stack::~CNF_Stack() {
	for (auto it = built.begin(); it != built.end(); it++) {
		delete *it;
	}
}

/**
 * @brief Allocates a new node in the CNF tree.
 *
 * @details
 * - Increments the internal node counter and throws if it exceeds a safe limit (15000).
 * - Creates a new s_cnf_node of the given type @p t, with string value @p v
 * 	 (used for variables),
 *   and pointers to left (@p l) and right (@p r) child subtrees.
 * - Tracks the newly built node so it can be cleaned up in the destructor.
 *
 * @param t   The type of the node (e.g., AND, OR, VAR, NOT).
 * @param v   The string payload for variable nodes; ignored for operator nodes.
 * @param l   Pointer to the left child node (nullptr if leaf or unary).
 * @param r   Pointer to the right child node (nullptr for unary or leaf).
 * @return    Pointer to the freshly allocated s_cnf_node.
 * @throws    BoolFtException if the total node count exceeds 15000 to avoid runaway growth.
 */
s_cnf_node* CNF_Stack::new_node(e_type t, const std::string &v, s_cnf_node *l, s_cnf_node *r) {
	total_nodes++;
	if (total_nodes > 15000)
		throw BoolFtException("CNF Tree grew too much.");
	s_cnf_node* node = new s_cnf_node(t, v, l, r);
	if (!node)
		throw BoolFtException("CNF Node Allocation Error.");
	built.push_back(node);
	return node;
}

/**
 * @brief Gathers all subexpressions of a tree that do not match a specific operator type.
 *
 * @details
 * This helper function performs a recursive traversal of the CNF expression tree
 * rooted at `node`, collecting every node whose operator type differs from the
 * specified `type`. It is primarily used by `flatten()` to extract the immediate
 * children (or deeper descendants) of a common AND/OR group, so they can be reassembled
 * into a flattened sequence.
 *
 * The algorithm works as follows:
 *   1. **Base Case**
 *      If `node` is null, return immediately (nothing to collect).
 *   2. **Matching Operator**
 *      If `node->type` equals the target `type`, this means `node` is an
 *      intermediate grouping node for that operator. Instead of collecting `node`
 *      itself, recurse into both its `left` and `right` subtrees to gather their
 *      constituents.
 *   3. **Non-matching Operator**
 *      If `node->type` differs from `type` (e.g., a variable, a NOT, or the other
 *      binary operator), treat `node` as an atomic element of the flattened list
 *      and append its pointer to `out`.
 *
 * After this traversal, `out` will contain pointers to all maximal subtrees that
 * should become the direct children of a flattened AND or OR node.
 *
 * @param[in]  node
 *   Pointer to the root of the (sub)expression tree to traverse. May be nullptr.
 * @param[in]  type
 *   The operator type (AND or OR) that identifies which nodes should be expanded
 *   rather than collected.
 * @param[out] out
 *   Reference to a vector that will be populated with pointers to all collected
 *   subtrees whose `type` differs from the specified operator.
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
 * @brief Flattens nested conjunctions or disjunctions into a single binary tree.
 *
 * @details
 * For a subtree rooted at an AND or OR node, this function:
 *   1. Gathers all descendant nodes of the same operator type into a flat list
 *      by calling `collect_nodes(node, node->type, elems)`.
 *      - `collect_nodes` traverses the tree and appends each leaf or subexpression
 *        whose parent operator matches `node->type` into `elems`.
 *   2. Rebuilds a right-branching binary tree from the collected elements:
 *      - Starts with the first element in `elems` as the interim result.
 *      - Iteratively creates new operator nodes combining the current result with
 *        the next element, preserving the original operator type (AND or OR).
 *   3. Returns the newly constructed flattened tree, which has no nested nodes
 *      of the same type.
 *
 * If the input `node` is null, or not an AND/OR node, it is returned unchanged.
 *
 * @param[in]      node   Pointer to the root of the (sub)expression tree to flatten.
 * @param[in,out]  built  Reference to a `CNF_Stack` used to allocate any new nodes
 *                        during the rebuild process.
 *
 * @return
 *   - A pointer to the root of the flattened subtree if `node` was an AND/OR node
 *     with two or more like-typed children.
 *   - The original `node` if it was null, a leaf (VAR or NOT), or had no like-typed
 *     descendants to flatten.
 *
 * @throws BoolFtException
 *   Propagates any allocation error thrown by `built.new_node()` when creating
 *   the rebuilt operator nodes.
 */
s_cnf_node* flatten(s_cnf_node* node, CNF_Stack& built) {
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
		res = built.new_node(node->type, "", res, elems[i]);
	}
	return res;
}

/**
 * @brief Recursively normalizes a CNF expression tree by flattening nested operators.
 *
 * @details
 * This function enforces a canonical shape on a tree representing a CNF formula by:
 *   - **Flattening**: Merging any nested `AND` or `OR` nodes of the same type into
 *     a single node with multiple children, reducing deep binary chains.
 *   - **Bottom-up Processing**: Ensuring both subtrees are normalized before flattening
 *     the current node, so that all levels of the tree satisfy the normalization rules.
 *
 * The normalization proceeds as follows:
 *   1. If the input `root` is null, return `nullptr`.
 *   2. If the node is an `AND` or `OR`:
 *      - Recursively call `normalize_cnf` on the left child.
 *      - Recursively call `normalize_cnf` on the right child.
 *      - Invoke `flatten()` to:
 *          - Coalesce adjacent nodes of the same type into a single node.
 *          - Remove duplicate literals or redundant subexpressions.
 *   3. If the node is neither `AND` nor `OR` (i.e., a `VAR` or `NOT`), return it unchanged.
 *
 * After execution, the tree rooted at the returned node will have:
 *   - No nested `AND` within `AND`, or `OR` within `OR`.
 *   - A consistent, flattened structure that simplifies further processing or serialization.
 *
 * @param[in]      root
 *   Pointer to the root of the subtree to normalize. May be `nullptr`.
 * @param[in,out]  built
 *   Reference to the `CNF_Stack` allocator used to create any new nodes during flattening.
 *
 * @return
 *   The pointer to the root of the normalized subtree, or `nullptr` if the input was null.
 *
 * @throws BoolFtException
 *   Propagates exceptions from `flatten()`, such as memory allocation failures or
 *   errors due to malformed tree structures.
 */
s_cnf_node* normalize_cnf(s_cnf_node* root, CNF_Stack& built) {
	if (!root) {
		return nullptr;
	}
	if (root->type == AND || root->type == OR) {
		root->left  = normalize_cnf(root->left, built);
		root->right = normalize_cnf(root->right, built);
		root = flatten(root, built);
	}
	return root;
}

/**
 * @brief Applies the distributive law to maintain Conjunctive Normal Form (CNF).
 *
 * @details
 * Given two subexpressions `a` and `b`, this function constructs a new subtree
 * representing the disjunction `a ∨ b` while preserving CNF structure (a conjunction
 * of disjunctions). It works by:
 *   - If `b` is an AND node `(B₁ ∧ B₂)`, distributing `a` over each conjunct:
 *       a ∨ (B₁ ∧ B₂) => (a ∨ B₁) ∧ (a ∨ B₂)
 *   - Else if `a` is an AND node `(A₁ ∧ A₂)`, distributing each conjunct over `b`:
 *       (A₁ ∧ A₂) ∨ b  => (A₁ ∨ b) ∧ (A₂ ∨ b)
 *   - Otherwise, neither operand is an AND, so a simple OR node `a ∨ b` is created.
 *
 * @param[in]     a
 *   Pointer to the left operand subtree of the disjunction.
 * @param[in]     b
 *   Pointer to the right operand subtree of the disjunction.
 * @param[in,out] built
 *   Reference to a `CNF_Stack` used to allocate and manage all newly created nodes.
 *
 * @return
 *   A pointer to the root of the newly constructed subtree, which will be:
 *   - An AND node combining distributed OR subtrees when distribution occurs, or
 *   - An OR node combining `a` and `b` directly if no distribution is necessary.
 *
 * @throws BoolFtException
 *   Propagates any exception thrown by `built.new_node()` (e.g., allocation failures).
 */
s_cnf_node* distribute(s_cnf_node* a, s_cnf_node* b, CNF_Stack& built) {
	if (b->type == AND)
		return built.new_node(AND, "", distribute(a, b->left, built), distribute(a, b->right, built));
	if (a->type == AND)
		return built.new_node(AND, "", distribute(a->left, b,built), distribute(a->right, b, built));
	return built.new_node(OR, "", a, b);
}

/**
 * @brief  Recursively transforms a logical expression tree into Conjunctive Normal Form (CNF).
 *
 * @details
 * This function applies the CNF conversion by traversing the expression tree in a
 * post-order fashion and applying the distributive law where necessary:
 *   - **Base Cases**
 *     If the node is null, a variable (VAR), or a negation (NOT), it already
 *     satisfies CNF requirements and is returned unchanged.
 *   - **Recursive Descent**
 *     Both the left and right subtrees are first converted to CNF by recursive calls.
 *   - **Disjunction Handling (OR)**
 *     After children are in CNF, any disjunction node is replaced by the result of
 *     `distribute(left, right, built)`, which applies the distributive law:
 *     A | (B & C) => (A | B) & (A | C) (and its symmetric form),
 *     ensuring the overall structure remains a conjunction of disjunctions.
 *   - **Conjunction Handling (AND)**
 *     If the node is a conjunction, a new AND node is allocated combining the CNF-converted
 *     children. Nested ANDs are flattened later in `normalize_cnf()`.
 *
 * @param[in,out] node
 *   Pointer to the root of the expression (sub)tree to convert.  Its children may be
 *   replaced or reused during conversion.
 * @param[in]     built
 *   Reference to a CNF_Stack factory that allocates all new `s_cnf_node` instances
 *   needed during distribution and node creation.
 *
 * @return
 *   Pointer to the root of the CNF-converted subtree.  This may be the original node
 *   (for VAR or NOT), the result of distribution (for OR), or a newly created AND node.
 *
 * @throws BoolFtException
 *   Propagates any exception raised by `distribute()` when applying the distributive law,
 *   such as stack errors or malformed subtree structures.
 */
s_cnf_node* to_cnf(s_cnf_node* node, CNF_Stack& built) {
	if (!node || node->type == VAR || node->type == NOT)
		return node;
	node->left  = to_cnf(node->left, built);
	node->right = to_cnf(node->right, built);

	if (node->type == OR) {
		return distribute(node->left, node->right, built);
	} else {
		return built.new_node(AND, "", node->left, node->right);
	}
}

/**
 * @brief Builds an expression tree from a Negation Normal Form (NNF) formula in RPN.
 *
 * @details
 * Parses the input string of tokens in Reverse Polish Notation (postfix) and constructs
 * a corresponding binary tree of `s_cnf_node` nodes. A temporary `std::stack` is used
 * to hold intermediate subtrees:
 *   - For each binary operator (`&` or `|`), two subtrees are popped (right then left),
 *     and a new internal node is created with those as children.
 *   - For the unary negation operator (`!`), a single subtree is popped and wrapped
 *     in a NOT node.
 *   - For any other character, a leaf variable node is created.
 * The `CNF_Stack& built` factory is used to allocate each new node and manage its lifetime.
 *
 * @param[in]  rpn    A string containing the NNF formula in postfix notation.
 *                    Supported token characters:
 *                      - `&` for conjunction (AND)
 *                      - `|` for disjunction (OR)
 *                      - `!` for negation (NOT)
 *                      - any other character is treated as a variable name
 * @param[in,out] built
 *                    Reference to a `CNF_Stack` object used to allocate and keep track
 *                    of all created `s_cnf_node` instances.
 *
 * @return
 *   Pointer to the root of the constructed expression tree. If the input was empty,
 *   returns `nullptr`.
 *
 * @throws BoolFtException
 *   Thrown if the RPN sequence is malformed:
 *     - A binary operator is encountered when fewer than two operands are on the stack.
 *     - A negation operator is encountered when the stack is empty.
 *   Propagates exception from built factory.
 */
s_cnf_node* nnf_to_tree(const std::string& rpn, CNF_Stack& built) {
	std::stack<s_cnf_node*> st;
	for (char c : rpn) {
		if (c == '&' || c == '|') {
			if (st.size() < 2)
				throw BoolFtException("Binary Operator Error.");
			s_cnf_node* right = st.top(); st.pop();
			s_cnf_node* left  = st.top(); st.pop();
			st.push(built.new_node(c == '&' ? AND : OR, "", left, right));
		}
		else if (c == '!') {
			if (st.empty())
				throw BoolFtException("Binary Operator Error.");
			s_cnf_node* lit = st.top(); st.pop();
			st.push(built.new_node(NOT, "", lit, nullptr));
		}
		else {
			std::string op(1, c);
			st.push(built.new_node(VAR, op));
		}
	}
	return st.empty() ? nullptr : st.top();
}

/**
 * @brief Serializes a CNF expression tree into Reverse Polish Notation (RPN).
 *
 * @details
 * This function performs a post-order traversal of the expression tree rooted at
 * the given node and appends the appropriate token strings to the output vector:
 *   - If the node is a variable, its name is appended directly.
 *   - If the node is a negation, the child subtree is processed first,
 *     then the "!" operator.
 *   - If the node is a binary operator (AND or OR), the left subtree is processed,
 *     then the right subtree, followed by "&" for AND or "|" for OR.
 *
 * By following post-order, operators always appear after their operand(s), yielding
 * a valid postfix (RPN) encoding of the logical expression.
 *
 * @param[in]  node  Pointer to the root of the (sub)expression tree.
 *                   May be nullptr, in which case nothing is done.
 * @param[out] out   Reference to a vector of strings to which RPN tokens will be appended.
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
 * @brief Converts a propositional logic formula into Conjunctive Normal Form (CNF).
 *
 * @details
 * This function serves as the high-level entry point for transforming a logical formula
 * (given in infix notation) into its equivalent in Conjunctive Normal Form, represented
 * in Reverse Polish Notation (RPN). The conversion proceeds through several well-defined
 * stages:
 *   1. **Negation Normal Form (NNF)**
 *      Calls `negation_normal_form()` to push negations inward and eliminate implications,
 *      producing an RPN string where negations only apply directly to variables.
 *   2. **Parsing to Expression Tree**
 *      Uses `nnf_to_tree()` along with a `CNF_Stack` to build a binary tree (`s_cnf_node`)
 *      that structurally represents the NNF expression.
 *   3. **CNF Transformation**
 *      Applies distributive laws via `to_cnf()` on the NNF tree to restructure it into a
 *      tree that satisfies the properties of CNF (i.e., a conjunction of disjunctions).
 *   4. **Normalization**
 *      Flattens nested conjunctions and disjunctions, removes duplicates, and canonicalizes
 *      ordering by calling `normalize_cnf()`.
 *   5. **Serialization to RPN**
 *      Traverses the normalized CNF tree with `tree_to_rpn()` to produce the final RPN token
 *      sequence, then concatenates tokens into a single `std::string`.
 *
 * @param[in] formula
 *   A null-terminated C-string containing the input logical formula in infix notation.
 *   Supported operators include:
 * 		- '!' (NOT, unary)
 * 		- '&' (AND, binary)
 * 		- '|' (OR, binary)
 * 		- '>' (IMPLICATION)
 * 		- '=' (EQUIVALENCE)
 * 		- '^' (XOR)
 *
 * @return
 *   A `std::string` representing the input formula transformed into its CNF equivalent,
 *   encoded in Reverse Polish Notation (postfix) without spaces between tokens.
 *
 * @throws BoolFtException
 *   If any stage of the conversion fails (e.g., malformed input, stack underflow,
 *   memory allocation error), a `BoolFtException` is thrown containing diagnostic details.
 */
std::string conjunctive_normal_form(char* formula) {
	std::string nnf_rpn = negation_normal_form(formula);
	try {
		CNF_Stack	cnfStack;
		s_cnf_node* nnf_root = nnf_to_tree(nnf_rpn, cnfStack);
		s_cnf_node* cnf_root = to_cnf(nnf_root, cnfStack);
		cnf_root = normalize_cnf(cnf_root, cnfStack);
		std::vector<std::string> cnf_tokens;
		tree_to_rpn(cnf_root, cnf_tokens);
		std::ostringstream oss;
		for (size_t i = 0; i < cnf_tokens.size(); ++i) {
			if (i) oss << "";
			oss << cnf_tokens[i];
		}
		return oss.str();
	} catch (std::exception& e) {
		std::ostringstream details;
		details << "Error Building CNF function: " << e.what();
		throw BoolFtException(details.str());
	}
}
