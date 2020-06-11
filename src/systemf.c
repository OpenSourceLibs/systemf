#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "derived-parser.h"
#include "derived-lexer.h"
#include "task.h"

int _sf1_yyerror(_SF1_YYLTYPE *locp, yyscan_t scanner, systemf1_parse_args *result, const char *msg) {
  printf("ERROR: %d:%d:%s\n", locp->first_line, locp->first_column, msg);
  return 1;
}

int systemf1(const char *fmt, ...)
{
    // extern int _sf1_yydebug; _sf1_yydebug = 1; // for debugging issues
    va_list argp;
    yyscan_t scanner;
    systemf1_parse_args *result = calloc(1, sizeof(systemf1_parse_args));

    va_start(argp, fmt);
    result->argpp = &argp;

    if (_sf1_yylex_init(&scanner)) exit(1);
    YY_BUFFER_STATE buf = _sf1_yy_scan_string(fmt, scanner);
    if (_sf1_yyparse(scanner, result)) exit(1);
    va_end(argp);
    _sf1_yy_delete_buffer(buf, scanner);
    _sf1_yylex_destroy(scanner);

    int ret = systemf1_tasks_run(result->tasks);

    return (ret);
}
