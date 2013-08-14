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


double EvaluateMathInfix(char *input, char *failure)
{
    return 0;
}

//     for (int cycle=0; cycle < 1000; cycle++)
//     {
//     }

//     if (SeqLength(stack) < 1)
//     {
//         strcpy(failure, "Unexpectedly empty evaluation stack");
//         return -1;
//     }

//     char* item = SeqAt(stack, 0);
//     SeqRemove(stack, 0);

//     if (NULL == item)
//     {
//         strcpy(failure, "NULL item off stack");
//         return -1;
//     }

//     double scanned = 0;

//     //CfOut(OUTPUT_LEVEL_INFORM, "", "eval(): item = %s", item);
//     if (0 == strcmp(item, "=="))
//     {
//         double temp = EvaluateMathPrefix(stack, failure);
//         if (strlen(failure) > 0) return -1;
//         // epsilon = 1e-16
//         return 0.00000000000000001 > fabs(temp - EvaluateMathPrefix(stack, failure)) ? 1 : 0;
//     }
//     else if (0 == strcmp(item, "+"))
//     {
//         double temp = EvaluateMathPrefix(stack, failure);
//         if (strlen(failure) > 0) return -1;
//         return temp + EvaluateMathPrefix(stack, failure);
//     }
//     else if (0 == strcmp(item, "-"))
//     {
//         double temp = EvaluateMathPrefix(stack, failure);
//         if (strlen(failure) > 0) return -1;
//         return temp - EvaluateMathPrefix(stack, failure);
//     }
//     else if (0 == strcmp(item, "%"))
//     {
//         double temp = EvaluateMathPrefix(stack, failure);
//         if (strlen(failure) > 0) return -1;
//         return fmod(temp, EvaluateMathPrefix(stack, failure));
//     }
//     else if (0 == strcmp(item, "**") || 0 == strcmp(item, "pow")) // note order: before '*'
//     {
//         double temp = EvaluateMathPrefix(stack, failure);
//         if (strlen(failure) > 0) return -1;
//         return pow(temp, EvaluateMathPrefix(stack, failure));
//     }
//     else if (0 == strcmp(item, "*"))
//     {
//         double temp = EvaluateMathPrefix(stack, failure);
//         if (strlen(failure) > 0) return -1;
//         return temp * EvaluateMathPrefix(stack, failure);
//     }
//     else if (0 == strcmp(item, "/"))
//     {
//         double temp = EvaluateMathPrefix(stack, failure);
//         if (strlen(failure) > 0) return -1;
//         double over = EvaluateMathPrefix(stack, failure);
//         if (over == 0) // yes, this can be fuzzy with floating point numbers
//         {
//             strcpy(failure, "Division by zero");
//             return -1;
//         }

//         return temp / over;
//     }
//     else if (0 == strcmp(item, "sin"))
//     {
//         double temp = EvaluateMathPrefix(stack, failure);
//         if (strlen(failure) > 0) return -1;
//         return sin(temp);
//     }
//     else if (0 == strcmp(item, "exp"))
//     {
//         double temp = EvaluateMathPrefix(stack, failure);
//         if (strlen(failure) > 0) return -1;
//         return exp(temp);
//     }
//     else if (0 == strcmp(item, "asin"))
//     {
//         double temp = EvaluateMathPrefix(stack, failure);
//         if (strlen(failure) > 0) return -1;
//         return asin(temp);
//     }
//     else if (0 == strcmp(item, "cos"))
//     {
//         double temp = EvaluateMathPrefix(stack, failure);
//         if (strlen(failure) > 0) return -1;
//         return cos(temp);
//     }
//     else if (0 == strcmp(item, "acos"))
//     {
//         double temp = EvaluateMathPrefix(stack, failure);
//         if (strlen(failure) > 0) return -1;
//         return acos(temp);
//     }
//     else if (0 == strcmp(item, "tan"))
//     {
//         double temp = EvaluateMathPrefix(stack, failure);
//         if (strlen(failure) > 0) return -1;
//         return tan(temp);
//     }
//     else if (0 == strcmp(item, "atan"))
//     {
//         double temp = EvaluateMathPrefix(stack, failure);
//         if (strlen(failure) > 0) return -1;
//         return atan(temp);
//     }
//     else if (0 == strcmp(item, "log"))
//     {
//         double temp = EvaluateMathPrefix(stack, failure);
//         if (strlen(failure) > 0) return -1;
//         return log(temp);
//     }
//     else if (0 == strcmp(item, "log10"))
//     {
//         double temp = EvaluateMathPrefix(stack, failure);
//         if (strlen(failure) > 0) return -1;
//         return log10(temp);
//     }
//     else if (0 == strcmp(item, "log2"))
//     {
//         double temp = EvaluateMathPrefix(stack, failure);
//         if (strlen(failure) > 0) return -1;
//         return log2(temp);
//     }
//     else if (0 == strcmp(item, "sqrt"))
//     {
//         double temp = EvaluateMathPrefix(stack, failure);
//         if (strlen(failure) > 0) return -1;
//         return sqrt(temp);
//     }
//     else if (0 == strcmp(item, "ceil"))
//     {
//         double temp = EvaluateMathPrefix(stack, failure);
//         if (strlen(failure) > 0) return -1;
//         return ceil(temp);
//     }
//     else if (0 == strcmp(item, "floor"))
//     {
//         double temp = EvaluateMathPrefix(stack, failure);
//         if (strlen(failure) > 0) return -1;
//         return floor(temp);
//     }
//     else if (0 == strcmp(item, "abs"))
//     {
//         double temp = EvaluateMathPrefix(stack, failure);
//         if (strlen(failure) > 0) return -1;
//         return fabs(temp);
//     }
//     else if (0 == strcmp(item, "pi"))
//     {
//         return M_PI;
//     }
//     else if (0 == strcmp(item, "e"))
//     {
//         return M_E;
//     }
//     else if (1 == sscanf(item, "%lf", &scanned))
//     {
//         //CfOut(OUTPUT_LEVEL_INFORM, "", "eval(): number = %lf", scanned);
//         return scanned;
//     }

//     snprintf(failure, CF_BUFSIZE, "unhandled term '%s'", item);
//     return -1;
// }
