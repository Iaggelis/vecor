CXXFLAGS      := -std=c++2a  -Wall -Wextra -Wshadow -Wpedantic \
                 -Wcast-align -ggdb -O0 -I../
LIBS          :=

simple: simple.cpp
	@printf "# compiling $<\n"
	$(CXX) $(CXXFLAGS)  $< -o $@
