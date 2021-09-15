
TEST_C = test/test.c 
TEST_ELF = test/test.elf 

$(TEST_ELF): $(TEST_C)
	@echo "CC [ $@ ] $< "
	@clang -static-libgcc -static -m64 -o $(TEST_ELF) $(TEST_C) 
