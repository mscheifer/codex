NAME	    = Server/drchao
DEBUGNAME = Server/drchao-debug
ECHO	    = @echo
CC        = @g++
BINFLAGS  = -O3 -DNDEBUG
WARNINGS  = -Wall -Wextra -Wstrict-overflow=5 -Wshadow #-Wconversion
INCLUDE   = -IServer/
CCFLAGS   = $(WARNINGS) $(INCLUDE) -std=c++11
LDSFML = -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -lsfml-graphics
LDFLAGS   = $(CCFLAGS) $(LDSFML) -lGL -lGLEW -lassimp
OBJDIR    = obj/
DEBUGDIR  = $(OBJDIR)debug/
SRCDIRS   = Server/ Server/graphics/
SOURCES   = $(foreach dir, $(SRCDIRS), $(wildcard $(dir)*.cpp))
OBJS      = $(patsubst %.cpp, $(OBJDIR)%.o, $(SOURCES))
DEBUGOBJS = $(patsubst %.cpp, $(DEBUGDIR)%.o, $(SOURCES))
DEPENDENCIES = $(patsubst %.o, %.d, $(OBJS)) $(patsubst %.o, %.d, $(DEBUGOBJS))

.PHONY: all debug clean new

all: $(NAME)

debug: $(DEBUGNAME)

$(DEBUGOBJS): $(DEBUGDIR)%.o: %.cpp
	$(ECHO) "Compiling $< debug"
	$(CC) -MMD -MP -g $(CCFLAGS) -c -o $@ $<

$(DEBUGNAME): $(DEBUGOBJS)
	$(ECHO) "Linking $@..."
	$(ECHO) $(CC) -o $@ $(DEBUGOBJS) -g $(LDFLAGS)
	$(CC) -o $@ $(DEBUGOBJS) -g $(LDFLAGS)
	$(ECHO) "Built $@!"

$(OBJS): $(OBJDIR)%.o: %.cpp
	$(ECHO) "Compiling $<"
#	$(ECHO) $(CC) $(CPPFLAGS) -c -o $@ $<
	$(CC) -MMD -MP $(CCFLAGS) -c -o $@ $<

-include $(DEPENDENCIES)

$(NAME): $(OBJS)
	$(ECHO) "Linking $@..."
	$(ECHO) $(CC) -o $@ $(OBJS) $(BINFLAGS) $(LDFLAGS)
	$(CC) -o $@ $(OBJS) $(BINFLAGS) $(LDFLAGS)
	$(ECHO) "Built $@!"

clean:
	$(RM) core $(OBJS) $(DEPENDENCIES) $(NAME) $(DEBUGNAME)
	$(ECHO) "All clean!"

new:
	make clean
	make
