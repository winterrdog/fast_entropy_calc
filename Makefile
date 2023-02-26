CXX := g++
SRC := get_entropy.cc
BIN := get_entropy
DBG_BIN := get_entropy.dbg

all:
	@${CXX} -Ofast -flto -s -o ${BIN} ${SRC}

debug:
	@${CXX} -O0 -ggdb3 -Wextra -Wall -o ${DBG_BIN} ${SRC}

clean:
	@rm -rf ${BIN} ${DBG_BIN}
