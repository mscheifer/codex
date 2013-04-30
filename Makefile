NAME	   = drchao
ECHO	   = @echo
CC       = @g++
WARNINGS = -Wall -Wextra -Wstrict-overflow=5 -Wshadow -Wconversion
CPPFLAGS = -O3 $(WARNINGS) -std=c++11
LDFLAGS  = -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -lsfml-graphics -lGL -lGLEW
OBJDIR   = obj/
SRCDIRS  = Server/ Server/graphics/
HEADERS  = $(foreach dir, $(SRCDIRS), $(wildcard $(dir)*.h))
SOURCES  = $(foreach dir, $(SRCDIRS), $(wildcard $(dir)*.cpp))
OBJS     = $(patsubst %.cpp, $(OBJDIR)%.o, $(SOURCES))

all: $(NAME)

$(OBJS): $(OBJDIR)%.o: %.cpp $(HEADERS)
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
