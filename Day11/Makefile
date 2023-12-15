SOURCE_DIR := .
CC := gcc
CFLAGS := -Wall -O0

SRC_FILES := $(wildcard $(SOURCE_DIR)/*.c)

OUT_DIR := bin
OUT_FILE := ./$(OUT_DIR)/main.exe


test:
	$(CC) $(SRC_FILES) $(CFLAGS) -o $(OUT_FILE)

make_and_run:
	mkdir $(OUT_DIR)
	make test
	$(OUT_FILE)
