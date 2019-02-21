OBJ_DIR = ./.obj/
SOURCE_DIR = ./src/
SOURCE_FILE = $(wildcard $(SOURCE_DIR)*.c)
OBJ = $(patsubst %.c,%.o,$(wildcard $(SOURCE_DIR)*.c))

TARGET = multisum

GCC = gcc
CFLAGS = -lpthread

$(TARGET):$(OBJ)
	$(GCC) -o $(TARGET) $(OBJ) $(CFLAGS)

$(OBJ):$(SOURCE_FILE)
	$(GCC) -c $(SOURCE_FILE) -o $(OBJ)

.PHONY:test
test :
	bash test.sh

.PHONY:clean
clean :
	rm -rf ./outfile/*
	find ./ -name "*.o" |xargs rm -rvf
	find ./ -name "*.obj" |xargs rm -rvf
	find ./ -name $(TARGET) |xargs rm -rvf