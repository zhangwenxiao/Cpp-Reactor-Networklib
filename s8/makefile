exe := test

# wildcard扫描源文件
sources := ${wildcard *.cpp}

headfile := ${wildcard *.h}

objects := ${sources:.cpp=.o}

CC := g++ -std=c++11
RM := rm -rf

CFlag := -lmuduo_net -lmuduo_base -lpthread

${exe}: ${objects}
	${CC} -o $@ $^ ${CFlag}

${objects}: %.o: %.cpp ${headfile}
	${CC} -o $@ -c $<

# 伪目标，意味着clean不代表一个真正的文件名
.PHONY: clean
clean:
	${RM} ${exe} ${objects}

