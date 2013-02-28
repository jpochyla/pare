include config.mk

SRC = pare.c render.c
OBJ = ${SRC:.c=.o}

all: pare

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

pare: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

${OBJ}: config.mk

clean:
	@echo cleaning
	@rm -f pare ${OBJ}

.PHONY: all clean
