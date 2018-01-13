src = $(wildcard *.c)
obj = $(src:.c=.o)

LDFLAGS = -lgmp

calc: $(obj)
	    $(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	    rm -f $(obj) calc
