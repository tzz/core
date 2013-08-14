%{
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

/*******************************************************************/
/*                                                                 */
/*  LEXER for CFEngine 3 math expressions                          */
/*                                                                 */
/*******************************************************************/

#include "platform.h"

#include "cf3.defs.h"
#include "cf3mathparse.h"

/* yyinput/input are generated and unused */

#if defined(__GNUC__)
# define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
# if GCC_VERSION >= 40200
#  pragma GCC diagnostic ignored "-Wunused-function"
# endif
#endif

#undef malloc
#undef realloc
#define malloc xmalloc
#define realloc xrealloc

#define MathParserDebug if (LogGetGlobalLevel() >= LOG_LEVEL_DEBUG) printf

// Do not use lex - flex only

%}

%option prefix="yymath" outfile="lex.yy.c"

space [ \t]+
digit [0-9]
number ({digit}+|{digit}+\.{digit}*|{digit}*\.{digit}+)([Ee][+\-]?{digit}+)?
constant "e"|"log2e"|"log10e"|"ln2"|"ln10"|"pi"|"pi_2"|"pi_4"|"1_pi"|"2_pi"|"2_sqrtpi"|"sqrt2"|"sqrt1_2"
function "exp"|"log"|"sqrt"|"sin"|"cos"|"tan"|"cot"|"sec"|"csc"|"asin"|"acos"|"atan"|"acot"|"asec"|"acsc"|"sinh"|"cosh"|"tanh"|"coth"|"sech"|"csch"|"asinh"|"acosh"|"atanh"|"acoth"|"asech"|"acsch"|"abs"|"step"

%%

{space}+              {
                      }

{number}              {
                          return MATH_NUMBER;
                      }

{constant}            {
                          return MATH_CONSTANT;
                      }

{function}            {
                          return MATH_FUNCTION;
                      }

%%

/* EOF */
