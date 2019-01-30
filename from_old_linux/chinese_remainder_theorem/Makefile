all: chinese_remainder_theorem chinese_remainder_theorem_gmp

chinese_remainder_theorem:chinese_remainder_theorem.c
	@gcc -o $@ $^

chinese_remainder_theorem_gmp:chinese_remainder_theorem_gmp.c
	@gcc -lgmp -o $@ $^

.PHONY: all clean

clean:
	@rm -rf chinese_remainder_theorem chinese_remainder_theorem_gmp
