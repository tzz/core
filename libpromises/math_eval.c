/* 
   Copyright (C) CFEngine AS

   This file is part of CFEngine 3 - written and maintained by CFEngine AS.
 
   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; version 3.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License  
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA

  To the extent this program is licensed as part of the Enterprise
  versions of CFEngine, the applicable Commerical Open Source License
  (COSL) may apply to this file if you as a licensee so wish it. See
  included file COSL.txt.
*/

#include "math_eval.h"

double stack[1024];
int stackp= -1;

double push(double n) { return stack[++stackp]= n; }
double pop(void)   { return stack[stackp--]; }

#define YYSTYPE double
#define YYPARSE yymath_parse
#define YYPARSEFROM yymath_parsefrom
#define YY_CTX_LOCAL
#define YY_PARSE(T) T
#define YY_INPUT(ctx, buf, result, max_size) {                     \
    result = 0;                                                    \
    if (NULL != ctx->input)                                        \
    {                                                              \
        /*Log(LOG_LEVEL_ERR, "YYINPUT: %s", ctx->input);*/         \
        strncpy(buf, ctx->input, max_size);                        \
        int n = strlen(ctx->input)+1;                              \
        if (n > max_size) n = max_size;                            \
        if (n > 0) buf[n - 1]= '\0';                               \
        result = strlen(buf);                                      \
        ctx->input = NULL;                                         \
    }                                                              \
    }

#undef malloc
#undef realloc
#define malloc xmalloc
#define realloc xrealloc

#define YY_CTX_MEMBERS char *failure; char *input; char *original_input; EvalContext *eval_context; double result; char fname[50];

#include "math.pc"

double EvaluateMathInfix(EvalContext *ctx, char *input, char *failure)
{
    yycontext yyctx;
    memset(&yyctx, 0, sizeof(yycontext));
    yyctx.failure = failure;
    yyctx.original_input = input;
    yyctx.input = input;
    yyctx.eval_context = ctx;
    yyctx.result = 0;
    yymath_parse(&yyctx);
    return yyctx.result;
}

double EvaluateMathFunction(char *f, double p)
{
    int count = sizeof(math_eval_functions)/sizeof(math_eval_functions[0]);

    for (int i=0; i < count; i++)
    {
        if (0 == strcmp(math_eval_function_names[i], f))
        {
            return (*math_eval_functions[i])(p);
        }
    }

    return p;
}

double _math_eval_step(double p)
{
    return ((p < 0) ? 0 : 1);
}
