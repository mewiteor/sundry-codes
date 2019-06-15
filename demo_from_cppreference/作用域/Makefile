all: if_scope_c99 if_scope_c89

if_scope_c89:if_scope.c
	@$(CC) -o $@ $< -std=c89

if_scope_c99:if_scope.c
	@$(CC) -o $@ $< -std=c99

run:if_scope_c99 if_scope_c89
	@echo 代码:
	@cat if_scope.c
	@echo c89的结果
	@./if_scope_c89
	@echo c99的结果
	@./if_scope_c99

clean:
	@rm if_scope_c89 if_scope_c99

.PHONY:all run clean
