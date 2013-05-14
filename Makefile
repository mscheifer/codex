NAME	   = Server/drchao
ECHO	   = @echo
CC       = @g++
BINFLAGS = -O3 -DNDEBUG
WARNINGS = -Wall -Wextra -Wstrict-overflow=5 -Wshadow #-Wconversion
INCLUDE  = -IServer/
CCFLAGS  = $(BINFLAGS) $(WARNINGS) $(INCLUDE) -std=c++11
LDSFML = -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -lsfml-graphics
LDFLAGS  = $(CCFLAGS) $(LDSFML) -lGL -lGLEW -lassimp
OBJDIR   = obj/
SRCDIRS  = Server/ Server/graphics/
SOURCES  = $(foreach dir, $(SRCDIRS), $(wildcard $(dir)*.cpp))
OBJS     = $(patsubst %.cpp, $(OBJDIR)%.o, $(SOURCES))
DEPENDENCIES = $(patsubst %.o, %.d, $(OBJS))

.PHONY: all debug clean new

all: $(NAME)

debug: BINFLAGS = -g

debug: all

$(OBJS): $(OBJDIR)%.o: %.cpp
	$(ECHO) "Compiling $<"
#	$(ECHO) $(CC) $(CPPFLAGS) -c -o $@ $<
	$(CC) -MMD -MP $(CCFLAGS) -c -o $@ $<

-include $(DEPENDENCIES)

$(NAME): $(OBJS)
	$(ECHO) "Linking $@..."
	$(ECHO) $(CC) -o $@ $(OBJS) $(LDFLAGS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
	$(ECHO) "Built $@!"

clean:
	$(RM) core $(OBJS) $(DEPENDENCIES) $(NAME)
	$(ECHO) "All clean!"

new:
	make clean
	make
