%{
#include <stdio.h>
#include <string.h>
extern int yylex();

void yyerror(const char *str) {
	fprintf(stderr, "error: %s\n", str);
}

int yywrap() {
	return 1;
}

int main() {
	yyparse();
}

%}

%token OPTION ARGS TOKQUIT TOKDGST TOKHELP TOKVERSION

%%

commands: /* empty */
		| commands command
		;

command:
		cmd_dgst
		|
		cmd_help
		|
		cmd_version
		|
		cmd_quit
		;

cmd_dgst:
		TOKDGST ARGS
		{
			printf("1(%d) 2(%d)\n", $1, $2);
		}
		;

cmd_help:
		TOKHELP ARGS
		{
			printf("1(%d) 2(%d)\n", $1, $2);
		}
		;

cmd_version:
		TOKVERSION ARGS
		{
			printf("1(%d) 2(%d)\n", $1, $2);
		}
		;

cmd_quit:
		TOKQUIT ARGS
		{
			printf("1(%d) 2(%d)\n", $1, $2);
		}
		;
