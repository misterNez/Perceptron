CC = g++
PROG = PLA
OBJS = perceptron.o
DEPS = sample.h

all: $(PROG)

$(PROG): $(OBJS) $(DEPS)
	$(CC) -o $@ $^
	
clean:
	rm -rf *.o $(PROG)
