# Makefile

CXX = g++
CXXFLAGS = -std=c++20 -pthread -O2
SRC = main.cpp handler/handler.cpp
OUT = main

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)
