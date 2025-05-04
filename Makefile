NAME			= 	BoolTests

SRC_DIR			= 	srcs
OBJ_DIR			= 	objs
OBJ_SUBS		= 	objs
LIBS_DIR		=	boolft
LIBS_HEADERS	=	$(LIBS_DIR)/inc/boolft.h
LIBS_LIBS		=	$(LIBS_DIR)/libboolft.a
SRCS_FILES		= 	main.c \
					utils.c \
					entrypoints.c
HEAD_FILES		=	inc/readySetBool.h
SRCS 			=	$(addprefix $(SRC_DIR)/,$(SRCS_FILES))
OBJS			=	$(addprefix $(OBJ_DIR)/,$(SRCS_FILES:.c=.o))
CFLAGS			=	-Wall -Wextra -Werror
LIBRARIES		=	$(LIBS_LIBS)
INCLUDES		=	-I./boolft -Iinc
LIB_LINKS		=	-L./$(LIBS_DIR) -lboolft
RM				=	rm -rf
CC				=	gcc
MAKE 			=	make --no-print-directory

all: library $(OBJ_SUBS) $(NAME)

$(NAME): $(OBJS) $(HEAD_FILES) $(LIBRARIES)
	@$(CC) $(OBJS) $(CFLAGS) $(LIB_LINKS) -g -o $(NAME)
	@echo "Executable $@ compiled"

library:
	@echo "$(YELLOW)Building LIBRARIES$(DEF_COLOR)"
	@echo "$(YELLOW)------------------$(DEF_COLOR)"
	@echo "$(YELLOW)Building BOOLFT$(DEF_COLOR)" ; echo
	@$(MAKE) -C $(LIBS_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEAD_FILES) $(LIBS_HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -g -c $< -o $@
	@echo "$(patsubst $(SRCS_DIR)/%,%, $<)"

$(OBJ_SUBS):
	@-mkdir $(OBJ_SUBS)

clean:
	@$(MAKE) -C $(LIBS_DIR) clean
	@$(RM) $(OBJS)
	@echo "$(RED)All temporary objects removed successfully${DEF_COLOR}"

fclean: clean
	@$(MAKE) -C $(LIBS_DIR) fclean
	@$(RM) $(NAME)
	@$(RM) $(OBJ_SUBS)
	@echo "$(RED)Executable have been fully cleaned${DEF_COLOR}"

#memory: $(OBJ_SUBS) $(OBJS) $(HEAD_FILES)
#	@$(MAKE) CFLAGS+=-fsanitize=address re
#	@echo "$(RED)Executable have been fully cleaned${DEF_COLOR}"

re:	fclean all

.PHONY: all clean fclean re