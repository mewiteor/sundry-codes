all: comment_c99 comment_c89

comment_c89:comment.c
	@$(CC) -o $@ $< -std=c89

comment_c99:comment.c
	@$(CC) -o $@ $< -std=c99

run:comment_c99 comment_c89
	@echo 代码:
	@cat comment.c
	@echo c89的结果
	@./comment_c89
	@echo c99的结果
	@./comment_c99

clean:
	@rm comment_c89 comment_c99

.PHONY:all run clean
