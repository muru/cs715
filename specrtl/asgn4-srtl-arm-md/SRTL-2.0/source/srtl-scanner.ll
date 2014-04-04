%{
#include <cstdlib>
#include <cerrno>
#include <string>
#include "srtl-driver.hh"
#include "srtl-parser.tab.hh"

#define yyterminate() return token::END

string cmd_body;

%}

%option nounput
id    [a-zA-Z_][a-zA-Z0-9_]*
int   -?[0-9]+
blank [ \t]
first_char_in_names        [*a-z_A-Z]
other_chars_in_names       [a-z_A-Z0-9]
first_char_in_constraints  ({first_char_in_names}|[ %=<>?!+@,\(\)\- ]|[0-9])
other_chars_in_constraints ({first_char_in_constraints}|[!/\n\t0-9>_:*&\(\) @.\\-|])
%x in_conventional_md_constructs

%{
#define YY_USER_ACTION yylloc->columns (yyleng);    
%}

%%
%{
    yylloc->step();
%}
{blank}+   yylloc->step ();
[\n]+      yylloc->lines (yyleng); yylloc->step ();

%{
typedef yy::srtl_parser::token token;
%}

[().{}:",;=\n<>_*-] return  yy::srtl_parser::token_type (yytext[0]);

DDDD {
        return token::DDDD;
}
     

"{:"    {
            cmd_body.clear ();
            BEGIN(in_conventional_md_constructs);
        }

<in_conventional_md_constructs>{

":}"    {
            yylval->string = &cmd_body;
            BEGIN (INITIAL);
            return token::CMD_SPEC_BODY;
        }
.*      {
            cmd_body.append (yytext);
        }

[\n]+   {
            cmd_body.append ("\n");
            yylloc->step (); yylloc->lines (yyleng);
        }
}

concrete {
            return token::CONCRETE;
         }

abstract {
            return token::ABSTRACT;
         }

extends {
            return token::EXTENDS;
        }

overrides   {
                return token::OVERRIDES;
            }

insn    {
            return token::INSN;
        }

exp     {
            return token::EXP;
        }

list    {
            return token::LIST;
        }

constants {
            return token::CONSTANTS;
        }

c_enum  {
            return token::C_ENUM;
        }
cpu_unit  {

            return token::CPU_UNIT;
        }
automaton {
            return token::AUTOMATON;
        }

set_attr {
            return token::SET_ATTR;
        }
noname   {

        return token::NONAME;
    }

peep2   {
            return token::PEEP2;
        }

split   {
            return token::SPLIT;
        }

insn_and_split {
            return token::INSN_SPLIT;
        }

insn_reserv {
            return token::INSN_RESERV;
        }

predicate {
            return token::PREDICATE;
        }

specialPredicate {
            return token::SPECIAL_PREDICATE;
        }

attr    {
            return token::ATTR;
        }

asm_attr {
            return token::ASM_ATTR;
        }

cons    {
            return token::CONS;
        }

regCons {
            return token::REG_CONS;
        }

memCons {
            return token::MEM_CONS;   
        }

addCons {
            return token::ADD_CONS;
        }

in      {
            return token::IN;
        }

out     {
            return token::OUT;
        }

latency {
            return token::LATENCY;
        }

lov     {
            return token::LOV;
        }

docstring   {
            return token::DOCSTRING;
        }

regclass {
            return token::REGCLASS;
        }

root    {
            return token::ROOT;
        }
const_int   {
                return token::CONST_INT;
            }

duplicate   {
                return token::DUPLICATE;
            }

allconstraints {
                return token::ALLCONSTRAINTS;
            }

[A-Z]*[0-9]*_REG    {
                        yylval->string = new std::string (yytext);
                        return token::FIXED_REG;
                    }

[A-Z0-9_]*REGNUM    {
                        yylval->string = new std::string (yytext);
                        return token::FIXED_REG;
                    }


cmd_spec {
            return token::CMD_SPEC;
        }

instantiates {
                return token::INSTANTIATES;
            }

c_iter  {
            return token::C_ITER;
        }

c_attr  {
            return token::C_ATTR;
        }

m_iter  {
            return token::M_ITER;
        }

m_attr  {
            return token::M_ATTR;
        }

reserve {
            return token::RESERVATION;
        }

"["     {
            return token::OBRACE;
        }

"]"     {
            return token::CBRACE;
        }

\"({first_char_in_constraints}{other_chars_in_constraints}*)*\" {
            yylval->string = new std::string (yytext);
            return token::QUOTED_ID;
        }


{id}    {
            yylval->string = new std::string(yytext);
            return token::ID;
        }


{int}   {
            yylval->string = new std::string(yytext);
            return token::INT;
        }

":="    {
            return token::ASSIGN;
        }

"|"     { 
            return token::PIPE;
        }

"+"     { 
            return token::PLUS;
        }


"//".*      ;

include<[^ \t\n]+> {
    char* token;
    int i = 0;
    while ((token = strsep(&yytext, "<")) != NULL){
        if(i == 1){
            token = strsep (&token, ">");
            break;
        }
        i++;
    }
    yyin = fopen (token, "r");
    if(!yyin){
        driver.error (std::string ("Cannot open file: ") + token);
        exit (1);
    }
    yypush_buffer_state (yy_create_buffer(yyin, YY_BUF_SIZE));

    }

<<EOF>> {
    yypop_buffer_state();
    if ( !YY_CURRENT_BUFFER ){
        yyterminate();
    }
}
%%

void srtl_driver::scan_begin (){
    if (file == "-")
        yyin = stdin;
    else if (!(yyin = fopen (file.c_str (), "r"))){
        error (std::string ("Cannot open ") + file);
        exit (1);
    }
}

void srtl_driver::scan_end (){
    fclose (yyin);
}
