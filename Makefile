CC = g++
PROG = dectree
OBJS = main.o
DEPS = sample.h tree.h

all: $(PROG)

$(PROG): $(OBJS) $(DEPS)
	$(CC) -o $@ $^
	
clean:
	rm -rf *.o $(PROG)
