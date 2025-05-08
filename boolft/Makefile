SRCS		=	adder.cpp \
				multiplier.cpp \
				gray_code.cpp \
				eval_formula.cpp \
				print_truth_table.cpp \
				nnf.cpp \
				ccf.cpp \
				sat.cpp \
				powerset.cpp \
				eval_set.cpp
SRCS_DIR	=	srcs
OBJS 		=	$(addprefix $(OBJS_DIR)/,$(SRCS:.cpp=.o))
OBJS_SUBS	= 	obj/
CC			=	@c++
RM			=	@rm -rf
CFLAGS 		=	-std=c++20 -Wall -Wextra -Werror -Iinc
NAME		=	libboolft.a
HEADER		=	inc/boolft.hpp
OBJS_DIR	=	obj

all: $(NAME) $(HEADER)

$(NAME): $(OBJS_SUBS) $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "$(NAME) compiled"

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp $(HEADER)
	$(CC) -Iinc $(CFLAGS) -g -c $< -o $@

$(OBJS_SUBS):
	-@mkdir $(OBJS_SUBS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(OBJS_SUBS)

bonus: CFLAGS += -DBONUS
bonus: all

re:	fclean $(NAME)

.PHONY: all clean fclean re bonus