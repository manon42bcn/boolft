SRCS		=	adder.c
SRCS_DIR	=	srcs
OBJS 		=	$(addprefix $(OBJS_DIR)/,$(SRCS:.c=.o))
OBJS_SUBS	= 	obj/
CC			=	gcc
RM			=	@rm -rf
CFLAGS 		=	-Wall -Wextra -Werror -Iinc
NAME		=	boolft.a
HEADER		=	inc/boolft.h
OBJS_DIR	=	obj

all: $(NAME) $(HEADER)

$(NAME): $(OBJS_SUBS) $(OBJS)
	ar rcs $(NAME) $(OBJS)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c $(HEADER)
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