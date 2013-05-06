NAME	   = Server/drchao
ECHO	   = @echo
CC       = @g++
WARNINGS = -Wall -Wextra -Wstrict-overflow=5 #-Wshadow -Wconversion
INCLUDE  = -IServer/ #-ISFML-2.0/include -Iassimp/include
CPPFLAGS = -O3 $(WARNINGS) $(INCLUDE) -std=c++11
LDSFML = -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -lsfml-graphics
LDFLAGS  = $(LDSFML) -lGL -lGLEW -lassimp
OBJDIR   = obj/
SRCDIRS  = Server/ Server/graphics/
HEADERS  = $(foreach dir, $(SRCDIRS), $(wildcard $(dir)*.h))
SOURCES  = $(foreach dir, $(SRCDIRS), $(wildcard $(dir)*.cpp))
OBJS     = $(patsubst %.cpp, $(OBJDIR)%.o, $(SOURCES))

all: $(NAME)

$(OBJS): $(OBJDIR)%.o: %.cpp $(HEADERS)
	$(ECHO) "Compiling $<"
#	$(ECHO) $(CC) $(CPPFLAGS) -c -o $@ $<
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
