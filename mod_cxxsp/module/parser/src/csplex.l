/*
 * FastServerPages - an inline C++ dynamic web pages technology
 * Copyright (C) 2003 Krzysztof Rzymkowski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

%option prefix="Cxxsp"
%option yyclass="cxxsp::lexer"
/* %option noyywrap */
%option yylineno
%option c++

%x HTML_COMMENT
%x CXXSP_COMMENT
%x CODE
%x DBL_QUOTE
%x COMMENT
%x SPECIAL_TAG
%x LIBTAG

QUOTE_VALUE ('[^']*')|(\"[^\"]*\")
ID [a-zA-Z0-9_]+

%{
    #include "lexer.h"

    extern "C" int yywrap();
    
    /*
    "<!--"                  { BEGIN(HTML_COMMENT); save_text(); return CSP_DATA; }
    <HTML_COMMENT>"-->"     { BEGIN(INITIAL); save_text(); return CSP_DATA; }
    */

%}

%%


"<%--"                  { BEGIN(CXXSP_COMMENT); }
<CXXSP_COMMENT>{
    "--%>"              { BEGIN(INITIAL); }
    .                   /* eat */
}

"<%"                    { BEGIN(CODE); return CSP_CODE_START; }
"<%="                   { BEGIN(CODE); return CSP_EXPR_START; }
"<%!"                   { BEGIN(CODE); return CSP_DECL_START; }
"<%#"                   { BEGIN(CODE); return CSP_HEADER_START; }

"<%"[[:space:]]*"@"     { BEGIN(SPECIAL_TAG); return CSP_DIRECTIVE_START; }

"<"{ID}":"              { BEGIN(LIBTAG); save_text(1,1); return CSP_LIBTAG_START;}
"</"{ID}":"             { BEGIN(LIBTAG); save_text(2,1); return CSP_LIBTAG_STOP; }

<CODE>{
    \"                  { BEGIN(DBL_QUOTE); save_text(); return CSP_DATA; }
    "//"[^\n]*\n        /* eat */
    "/*"                { BEGIN(COMMENT); save_text(); return CSP_DATA; }
}
<DBL_QUOTE>\"           { BEGIN(CODE); save_text(); return CSP_DATA; }
<COMMENT>"*/"           { BEGIN(CODE); save_text(); return CSP_DATA; }

<CODE>"%>"              { BEGIN(INITIAL); return CSP_CODE_END; }

<SPECIAL_TAG,LIBTAG>{
    {QUOTE_VALUE}       { save_text(1,1); return CSP_VALUE; }
    [[:space:]]+        /* eat */
    {ID}                { save_text(); return CSP_ID; }
    "="                 { save_text(); return CSP_DATA; }
}
<SPECIAL_TAG>"%>"       { BEGIN(INITIAL); return CSP_SPECIAL_TAG_CLOSED; }
<LIBTAG>">"             { BEGIN(INITIAL); return CSP_SPECIAL_TAG_CLOSED; }


<*>.|"\n"               { save_text(); return CSP_DATA;}

<*><<EOF>>              { if(eof()) return CSP_EOF;  }


%%

    int cxxsp::lexer::buffer_size = YY_BUF_SIZE;

    yy_buffer_state* cxxsp::lexer::get_current_buffer() {
        return YY_CURRENT_BUFFER;
    }
