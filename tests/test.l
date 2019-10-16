%{
#include <stdio.h>
#include "y.tab.h"
extern YYSTYPE yylval;
/* CMD			[a-z][a-z0-9\-_]+ */
/* OPT			-[a-z0-9\-]+ */
// -c				return OPTCHECK;
// -p				return OPTECHO;
// -q				return OPTQUIET;
// -r				return OPTREV;
// -s				return OPTSTR;
// -help			return OPTHELP;
// md5							return DGST_MD5
// sha1						return DGST_SHA1
// sha224						return DGST_SHA224
// sha256						return DGST_SHA256
// sha384						return DGST_SHA384
// sha512						return DGST_SHA512
// sha512-224					return DGST_SHA512224
// sha512-256					return DGST_SHA512256
// CMDS				(dgst|help|version|md5|sha1|sha224|sha256|sha384|sha512|sha512-224|sha512-256)
%}

OPT					-[a-z0-9\-]+
FIL					[.]+

%%

-[a-z0-9\-]+				{ printf("OPTION(%s)\n",yytext); return OPTION; }
{FIL}*						{ printf("ARGS(%s)\n",yytext); return ARGS; }
q|quit|exit					{ printf("TOKQUIT(%s)\n",yytext); return TOKQUIT; }
dgst						{ printf("TOKDGST(%s)\n",yytext); return TOKDGST; }
help						{ printf("TOKHELP(%s)\n",yytext); return TOKHELP; }
version						{ printf("TOKVERSION(%s)\n",yytext); return TOKVERSION; }
\n							/* ignore end of line */;
[ \t]+						/* ignore whitespace */;

%%
