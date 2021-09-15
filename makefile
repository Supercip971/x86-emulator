
MKCWD=mkdir -p $(@D)

PROJECT_NAME=fprof

CXX ?= g++
LD ?= ld
CFLAGS := -O2 -Isrc/ -lfmt -Werror -Wextra -Wall 

BUILD_DIR = build

# source files

CFILES = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
HFILES = $(wildcard src/*.h)
DFILES = $(patsubst src/%.cpp, $(BUILD_DIR)/%.d, $(CFILES))
OFILES = $(patsubst src/%.cpp, $(BUILD_DIR)/%.o, $(CFILES))

OUTPUT = build/$(PROJECT_NAME).elf

include ./test/.build.mk

$(OUTPUT): $(OFILES)
	@$(MKCWD)
	@echo " LD [ $@ ] $^"
	@$(CXX) $(CFLAGS) -o $@ $^ 

$(BUILD_DIR)/%.o: src/%.cpp
	@$(MKCWD)
	@echo " CC [ $@ ] $<"
	@$(CXX) $(CFLAGS) -std=c++2b -MMD -MP $< -c -o $@ 

run: $(OUTPUT) $(TEST_ELF)
	@$(OUTPUT) $(TEST_ELF)

all: $(OUTPUT)

clean:
	@rm -rf build/

-include $(DFILES)
