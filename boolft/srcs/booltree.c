#include "boolft.h"

t_node*	create_node(char symbol, int value, t_node_mode type) {
	t_node*	node = (t_node*)calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->symbol = symbol;
	node->eval = value;
	node->mode = type;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

void	clean_tree(t_node* head) {
	if (head->left) {
		clean_tree(head->left);
		head->left = NULL;
	}
	if (head->right) {
		clean_tree(head->right);
		head->right = NULL;
	}
	free(head);
}

t_bool	set_position(t_node* parent, t_node* current) {
	if (!parent || !current)
		return (FALSE);
	if (current->mode == LOGIC_SYMBOL) {
		if (parent->right == NULL) {
			parent->right = current;
			return (TRUE);
		} else {
			return (set_position(parent->right, current));
		}
	} else {
		if (parent->left == NULL) {
			parent->left = current;
			return (TRUE);
		}
		if (parent->right == NULL) {
			parent->right = current;
			return (TRUE);
		}
		if (parent->right->mode == LOGIC_SYMBOL) {
			return (set_position(parent->right, current));
		}
	}
	return (TRUE);
}

t_node_mode	get_mode (char symbol) {
	if (symbol == '1' || symbol == '0') {
		return (LOGIC_VALUE);
	}
	return (LOGIC_SYMBOL);
}

void print_tree(t_node *head, int space, char branch) {
	if (head == NULL)
		return;
	int level = 4;
	space += level;
	print_tree(head->right, space, '/');
	printf("\n");
	for (int i = level; i < space; i++)
		printf(" ");
	if (branch != ' ') {
		printf("%c", branch);
		printf("-> %c\n", head->symbol);
	} else {
		printf("%c\n", head->symbol);
	}
	print_tree(head->left, space, '\\');
}

char	logic_diccionary(char c) {
}

void	tree_try(void) {
	// char*	func = "101|&";
	char*	func = "1011||=";
	size_t	len = strlen(func);
	t_node*	head = NULL;
	t_node* new = NULL;

	while (len-- > 0) {
		printf("%c", func[len]);
		if (!head) {
			if (func[len] == '1' || func[len] == '0')
				return;
			else {
				head = create_node(func[len], 0, LOGIC_SYMBOL);
			}
		} else {
			new = create_node(func[len], 0, get_mode(func[len]));
			set_position(head, new);
		}
	}
	printf("\n");
	printf("final\n");
	print_tree(head, 0, ' ');
	clean_tree(head);
}

