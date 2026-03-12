

#--------------------------------------------------
#                  ENVIRONMENT
#--------------------------------------------------

#----- CPP language compiler -----
CPL=c++
CPL_FLAGS=-std=c++98 -Wall -Wextra -Werror

#----- Program informations -----
PNAME=easyfind

#----- RM Command -----
RM_FLAGS=-f

#----- Sanitize flags -----
SAN_FLAGS=-fsanitize=address -fsanitize=leak


#--------------------------------------------------
#              SOURCES AND OBJECTS
#--------------------------------------------------

#----- Sources -----
SRCS=	./main.cpp

#----- Objects -----
OBJS=$(SRCS:.cpp=.o)


#--------------------------------------------------
#              DEPENDENCIES (LIBS)
#--------------------------------------------------

#----- Common -----
DEPS_LIST=
DEPS_CALLS=


#--------------------------------------------------
#                COMPILING RULES
#--------------------------------------------------

#----- Main Rules -----
all: $(PNAME)

$(PNAME): $(DEPS_LIST) $(OBJS)
	$(CPL) $(CPL_FLAGS) $(if $(SAN_ACTIVE), $(SAN_FLAGS)) $(DEPS_LIST) $(OBJS) $(DEPS_CALLS) -o $@

sanitize: fclean
	$(MAKE)  SAN_ACTIVE=true

%.o: %.cpp
	$(CPL) $(CPL_FLAGS) -I./ -c $< -o $@

#----- Libs Rules -----


#--------------------------------------------------
#                CLEANING RULES
#--------------------------------------------------
clean:
	rm $(RM_FLAGS) $(OBJS)

fclean: clean
	rm $(RM_FLAGS) $(PNAME)


#--------------------------------------------------
#                EXTRA RULES
#--------------------------------------------------
re: fclean all

.phony: all clean fclean re
