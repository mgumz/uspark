
CC ?= cc
CFLAGS ?= -Os
LDFLAGS ?= -Os

SRC = \
	uspark.c \
	libowfat/str_diff.c \
	libowfat/str_len.c \
	libowfat/str_start.c \
	libowfat/scan_double.c
OBJS = $(SRC:.c=.o)

uspark: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ $*

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ -I. $^

uspark.diet:
	make CC="diet gcc" LDFLAGS="-static"

uspark.cmake:
	cmake -B build . && make -C build

clean:
	rm -f uspark $(OBJS)

compile_commands.json:
	compiledb make

build-dev-container:
	docker buildx build -t uspark-dev-c -f misc/Containerfile .

run-dev-container:
	docker run -it -v `pwd`:/src uspark-dev-c

.PHONY: clean uspark.diet uspark.cmake
.PHONY: build-env-container run-dev-container
