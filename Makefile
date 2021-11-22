.PHONY: all clean run deps
.IGNORE: clean

export CFLAGS=-c -O0 -Wall -g
export BUILD=$(realpath build)
export INCLUDE_DIR=-I$(realpath include)
export SOURCE_DIR=$(realpath src)

DOTS=$(realpath dots)

EXEC=cothello
OBJECTS=$(wildcard build/*.o build/*/*.o build/*/*/*.o)

all: $(EXEC)

$(EXEC): deps
	mkdir -p build
	echo $(OBJECTS)
	gcc $(OBJECTS) \
		-o $(EXEC) \
		-lncurses


deps:
	mkdir -p build
	$(MAKE) -C src


run:
	./$(EXEC)

clean:
	rm -r $(BUILD)/*
	rm $(EXEC)
