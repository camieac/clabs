# Simple makefile to build a square.c in the labs.
# Path to the OpenCL libraries.  In this case AMD SDK
# is being used, as system uses AMD graphics card.

# C flags with strictest warnings.
CFLAGS        += -O3 -Wall -Wextra -std=c99 -D_GNU_SOURCE

# Linker flags.
LDFLAGS += -L$(OPENCL)/lib/x86_64  -lrt -pthread


all: pipe


# Build a binary from C source.
fork: pipe.c
	$(CC) $(CFLAGS) -std=c99 $(LDFLAGS) -o $@ $^



# Remove the binary.
clean:
	$(RM) fork 

