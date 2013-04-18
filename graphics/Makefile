NAME	   = drchao
ECHO	   = @echo
CC       = @g++
WARNINGS = -Wall -Wextra -Wstrict-overflow=5 #-Wconversion
CPPFLAGS = -O3 $(WARNINGS) -ISFML/include -std=c++11
LDFLAGS  = -LSFML/lib -lsfml-window -lsfml-system -lGL -lGLEW
OBJDIR   = obj/
SOURCES  = $(wildcard *.cpp)
OBJS     = $(patsubst %.cpp, $(OBJDIR)%.o, $(SOURCES))

$(OBJDIR)%.o: %.cpp *.h
	$(ECHO) "Compiling $<"
	$(ECHO) $(CC) $(CPPFLAGS) -c -o $@ $<
	$(CC) $(CPPFLAGS) -c -o $@ $<

$(NAME): $(OBJS)
	$(ECHO) "Linking $@..."
	$(ECHO) $(CC) -o $@ $(OBJS) $(LDFLAGS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
	$(ECHO) "Built $@!"

clean:
	$(RM) core $(OBJS) $(NAME)
	$(ECHO) "All clean!"

new:
	make clean
	make
