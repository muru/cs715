%skeleton "lalr1.cc"
%require "2.7"
%defines
%define parser_class_name "srtl_parser"

%code requires {
#include <string>
#include <sstream>
#include "pattern.hh"
#include "abstract.hh"
#include "concrete_pattern.hh"
#include "list.hh"
using namespace std;
class srtl_driver;

}
// The parsing context
%parse-param { srtl_driver& driver }
%lex-param { srtl_driver& driver }
%locations
%initial-action
{
    // Initialize the initial location.
    @$.begin.filename = @$.end.filename = &driver.file;
};
%debug
%error-verbose

%expect 2
%left '_'
%left '*'

%code {
#include "srtl-driver.hh"
}

%union{
    Pattern* pattern;
    std::string* string;
    Operand* operand;
    std::vector<Operand>* operandList;
    std::vector<int>* intVector;
    std::vector<std::string>* stringVector;
    ModeStmt* modeStmt;
    std::vector<ModeStmt>* modeStmtVector;
    ConcretePattern* concretePattern;
    ListPattern * L;
    ElemList * EL;
    Element  * E;
    Pattern::Type LP;
}

%token      END     0 "end of file"
%token      CONCRETE
%token      ABSTRACT
%token      EXTENDS
%token      OVERRIDES
%token      ROOT

%token      INSN
%token      EXP
%token      PEEP2
%token      SPLIT
%token      INSN_SPLIT
%token      INSN_RESERV
%token      PREDICATE
%token      SPECIAL_PREDICATE
%token      ATTR
%token      ASM_ATTR
%token      CONS
%token      REG_CONS
%token      MEM_CONS
%token      ADD_CONS
%token      BYPASS
%token      IN
%token      OUT
%token      LATENCY
%token      LOV
%token      DOCSTRING
%token      REGCLASS
%token      ALLCONSTRAINTS

%token      INSTANTIATES
%token      CONST_INT
%token      DUPLICATE
%token<string> FIXED_REG
%token      CMD_SPEC
%token<string> CMD_SPEC_BODY


%token      NONAME
%token      MODE_ATTR
%token      SET_ATTR
%token      AUTOMATON
%token      CODE_ATTR
%token      CONSTANTS
%token      CPU_UNIT
%token      LIST
%token      C_ITER
%token      C_ATTR
%token      M_ITER
%token      M_ATTR
%token      RESERVATION

%token      C_ENUM
%token      CBRACE
%token      OBRACE
%token      PIPE
%token      PLUS
%token      ASSIGN

%token      DDDD

%token<string> ID
%token<string> QUOTED_ID
%token<string> INT
%token<string> rid
%token<string> aid
%token<string> HEX
%type <string> Id_Reg	
%type <string>  Quoted_Id
%type <L> List_Pattern
//%type <string> AutoId
%type <string> PipedId
%type <E> SEntry
%type <EL> ListEntries
%type <EL> ListPID
%type <EL> ListQEntries
%type <EL>  ListAsmEntries 
%type <LP>  Code_Mode_Attr
%type <LP>  Code_Mode_Iter
%type <E>   IEntryPair
%type <E>   AsmEntry 
%type <E>   SQEntryPair

%type <string>  Abstract_Pattern_Name
%type <string>  Concrete_Pattern_Name
%type <stringVector>  Concrete_Pattern_Name_List
%type <string> Concrete_Pattern_Name_With_Brackets
%type <string>  Constraint
%type <string> QIDList
%type <stringVector> Predicate_Or_Mode_List
%type <string>  Predicate_Or_Mode
%type <string> Predicate_Or_Mode_With_Brackets
%type <operand> Operand
%type <operand> Fixed_Reg_Or_Const
%type <operandList> Operands
%type <intVector> Operand_Access
%type <modeStmt> Attr_Access;
%type <modeStmt> Mode_Stmt
%type <modeStmt> Stmt;
%type <modeStmtVector> Concrete_Inst_Stmt_List
%type <modeStmtVector> Abstract_Extends_Body
%type <string> Pattern_Name
%type <string> Concrete_Pattern_Name_Qualifier
%type <string> Base_Name
%type <stringVector> Quoted_Id_List
%%
%start Pattern_List;
Pattern_List    :   Pattern_List Pattern
                |   Pattern
                /* Only one error per pattern */
                |   error '}' { yyclearin; yyerrok; }
                ;

Pattern 
    :   Abstract_Pattern {
        if (!driver.currentPattern->inError ()){
            driver.debug (srtl_driver::deb , "Abstract Pattern.");
            driver.addSymTabEntry (driver.currentPattern);
            driver.addToConstructTally (driver.currentPattern->getPatType ());
        } else {
            driver.debug (srtl_driver::err, "Abstract Pattern in Error: " + driver.currentPattern->getPatName ());
        }
    }
    |   Concrete_Pattern {
        if (!driver.currentPattern->inError ()) {
        	driver.debug (srtl_driver::deb , "Concrete Pattern: " + driver.currentPattern->getPatName ());
            driver.currentPattern->createPattern ();
            driver.addToConstructTally (driver.currentPattern->getPatType ());
            if (driver.currentPattern->getPatType () >= Pattern::insnAndSplit && 
                driver.currentPattern->getPatType () <= Pattern::peep2) {
                driver.debug (srtl_driver::deb, driver.currentPattern->getPatName ());
                driver.currentPattern->createPatternOut ();         
            }
            driver.write (srtl_driver::outPattern);
        	driver.addSymTabEntry (driver.currentPattern);
		}else{
			/* Report Error about the pattern and 
			 * proceed to the next pattern. */
			 driver.debug(srtl_driver::err, "Pattern in Error: " + driver.currentPattern->getPatName ());
		}
    }
    |   List_Pattern {
        driver.debug (srtl_driver::deb , "List Pattern.");
        $1->createPattern();
     //   driver.addToConstructTally (driver.currentPattern->getPatType ());
        if ($1->getPatType () == Pattern::define_code_iterator) {
            ListPattern* lp = static_cast <ListPattern*> ($1);
            string name = *(lp->getElemList ()->getList ().front ()->getName ());
            int k = driver.findRtlOperandSymTabEntry (name);
            driver.addRtlOperandSymTabEntry ($1->getPatName (),  k);

        }
        driver.write (srtl_driver::outPattern);
        delete $1;
    }
	| 	CMD_SPEC_BODY {
        driver.write (*$1);
    }
    |   DDDD {
        printf (" ");
    }
    ;

/* Abstract Pattern Grammar */
Abstract_Pattern    
    :   ABSTRACT Abstract_Pattern_Name Abstract_Rtl_Spec {
    }
    ;

Abstract_Pattern_Name : ID {
        driver.currentPattern = 0;
        driver.currentPattern = new AbstPattern(*$1);
        delete $1;
    }
    ;

Abstract_Rtl_Spec   :   EXTENDS Base_Name {
        /* Search Symbol Table Or Known Names
         * for abstract patterns. */
        Pattern* p = driver.findSymTabEntry (*$2);

        if(p == NULL) {
            int i = driver.findRtlOperandSymTabEntry (*$2);
            if (i < 0) {
                driver.currentPattern->setError ();
            } else {
                ModeStmt* ms = new ModeStmt ();
                vector<int>* access = new vector<int>();
                //access->push_back(0);
                ms->addAccessTree (access);
                ms->setPatternName(*$2);
                AbstPattern* ap = static_cast<AbstPattern*> (driver.currentPattern);
                ap->addStmt(*ms);
                delete access;
                delete ms;
                if (driver.generateDotFile) {
                    driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=orange];\n";
                    driver.dotFile << "\"" + *$2 + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\";\n";
                }
                driver.incrementAbstractPatternCount (*$2, Pattern::abstract);
                driver.addToStatTree (*$2, driver.currentPattern->getPatName (), 1);
            }
            delete $2;
        } else{
            if (p->getPatType () != Pattern::abstract) {
                int i = driver.findRtlOperandSymTabEntry (*$2);
                if (i < 0) {
                    driver.currentPattern->setError ();
                } else {
                    ModeStmt* ms = new ModeStmt ();
                    vector<int>* access = new vector<int>();
                    //access->push_back(0);
                    ms->addAccessTree (access);
                    ms->setPatternName(*$2);
                    AbstPattern* ap = static_cast<AbstPattern*> (driver.currentPattern);
                    ap->addStmt(*ms);
                    delete access;
                    delete ms;
                    if (driver.generateDotFile) {
                        driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=orange];\n";
                        driver.dotFile << "\"" + driver.formatNodeName (*$2) + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [color=red];\n";
                    }
                    driver.incrementAbstractPatternCount (*$2, Pattern::abstract);
                    driver.addToStatTree (*$2, driver.currentPattern->getPatName (), 1);
                }
                delete $2;    
            } else {
                driver.currentPattern->setParentPattern (p);
                if (driver.generateDotFile) {
                    driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=orange];\n";
                    driver.dotFile << "\"" + driver.formatNodeName (*$2) + "\" [shape=box,style=filled,color=orange];\n";
                    driver.dotFile << "\"" + driver.formatNodeName (*$2) + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [color=red];\n";
               }
                driver.incrementAbstractPatternCount (*$2, Pattern::abstract);
                driver.addToStatTree (*$2, driver.currentPattern->getPatName (), 1);
            }
        }
    }
    '{' Abstract_Extends_Body '}' {
        AbstPattern* ap = static_cast<AbstPattern*> (driver.currentPattern);
        ap->addStmts(*$5); 
        delete $5;
    }
    |   OVERRIDES Base_Name {

    }
    '{' Abstract_Overrides_Body '}' {

    }
    ;

Abstract_Extends_Body : Abstract_Extends_Body Stmt {
        ModeStmt* ms = $2;

        Pattern* p = driver.findSymTabEntry (ms->getPatternName ());
        if (p != NULL) {
          if (p->getPatType () != Pattern::abstract) {
            $$->push_back (*$2);
          } else {
          AbstPattern* ap = dynamic_cast <AbstPattern*> (p);
          vector<ModeStmt>* tmsv = new vector<ModeStmt> (*ap->getStmts ());

          vector<int>* at = ms->getAccessTree ();

          for (unsigned int i = 0; i < tmsv->size (); i++) {
            vector<int>* tat = tmsv->at(i).getAccessTree ();
            if ( tat->size () == 1 && i == 0) {
                if (tat->at (i) == 0) {
                    tat->clear ();
                }
            }
            tat->insert (tat->begin (), at->begin (), at->end ());
            $$->push_back (tmsv->at(i));
          }
          delete tmsv;
          }
        } else {
            $$->push_back (*$2);
        }
        delete ms;
    }
    |   Stmt {
        vector<ModeStmt>* msv = new vector<ModeStmt> ();

        ModeStmt* ms = $1;

        Pattern* p = driver.findSymTabEntry (ms->getPatternName ());
        if (p != NULL) {
          if (p->getPatType () != Pattern::abstract) {
            msv->push_back (*$1);
          } else {

              AbstPattern* ap = dynamic_cast <AbstPattern*> (p);
              vector<ModeStmt>* tmsv = new vector<ModeStmt> (*ap->getStmts ());

              vector<int>* at = ms->getAccessTree ();

              for (unsigned int i = 0; i < tmsv->size (); i++) {
                vector<int>* tat = tmsv->at(i).getAccessTree ();
                if ( tat->size () == 1 && i == 0) {
                    if (tat->at (i) == 0) {
                        tat->clear ();
                    }
                }
                tat->insert (tat->begin (), at->begin (), at->end ());
                msv->push_back (tmsv->at(i));
              }
              delete tmsv;
          }
        } else {
            msv->push_back (*$1);
        }
        delete ms;
        $$ = msv;
    }

Abstract_Overrides_Body : ID {    
    }
    ;

/* Concrete Pattern Grammar */
Concrete_Pattern    :   CONCRETE Concrete_Pattern_Name_Qualifier Concrete_Rtl_Spec {
    }
    |   CONCRETE Concrete_Pattern_Name_Qualifier_C Concrete_Rtl_Spec_C {
        }
    |   CONCRETE Concrete_Pattern_Name_Qualifier_S Concrete_Rtl_Spec_S {
        }
    | CONCRETE Concrete_Pattern_Name_List '.' REG_CONS '{' Concrete_Inst_Stmt_List '}' {
        string t = "";
        for (unsigned int i = 0; i < $2->size (); i++) {
            t.append ($2->at(i));
        }
        driver.currentPattern = new ConcretePattern (t, Pattern::regCons);
        // Make this pattern compatible with the rest of the patterns.
        driver.currentPattern->setSubType (Pattern::instantiates);
        ModeStmt* ms = new ModeStmt ();
        vector<int>* access = new vector<int>();
        //access->push_back(0);
        ms->addAccessTree (access);
        ms->setPatternName (t);
        AbstPattern* ap = new AbstPattern ("set");
        ap->addStmt(*ms);
        driver.currentPattern->setParentPattern (ap);
        ConcretePattern* cp = static_cast <ConcretePattern*> (driver.currentPattern);
        cp->addModeStmts (*$6);
        delete $2;
        delete $6;
    }
    ;

Concrete_Pattern_Name_Qualifier_S :  Concrete_Pattern_Name_List '.' CONS {
        string t = driver.appendStrings ($1);
        driver.currentPattern = new ConcretePattern (t, Pattern::cons);
        delete $1;
    }
    | Concrete_Pattern_Name_List '.' ATTR {
        string t = driver.appendStrings ($1);
        driver.currentPattern = new ConcretePattern (t, Pattern::attr);
        delete $1;
    } 
    | Concrete_Pattern_Name_List '.' MEM_CONS {
        string t = driver.appendStrings ($1);
        driver.currentPattern = new ConcretePattern (t, Pattern::memCons);
        delete $1;
    }
    | Concrete_Pattern_Name_List '.' ADD_CONS {
        string t = driver.appendStrings ($1);
        driver.currentPattern = new ConcretePattern (t, Pattern::addCons);
        delete $1;
    }
    ;

Concrete_Pattern_Name_Qualifier : Concrete_Pattern_Name_List '.' INSN {
        string t = driver.appendStrings ($1);
        driver.currentPattern = new ConcretePattern (t, Pattern::insn);
        delete $1;
    }
    | Concrete_Pattern_Name_List '.' EXP {
        string t = driver.appendStrings ($1);
        driver.currentPattern = new ConcretePattern (t, Pattern::exp);
        delete $1;
    }   
    | Concrete_Pattern_Name_List '.' INSN_RESERV {
        string t = driver.appendStrings ($1);
        driver.currentPattern = new ConcretePattern (t, Pattern::insnReservation);
        delete $1;
    }   
    | Concrete_Pattern_Name_List '.' PREDICATE {
        string t = driver.appendStrings ($1);
        driver.currentPattern = new ConcretePattern (t, Pattern::predicate);
        delete $1;
    } 
    | Concrete_Pattern_Name_List '.' SPECIAL_PREDICATE {
        string t = driver.appendStrings ($1);
        driver.currentPattern = new ConcretePattern (t, Pattern::specialPredicate);
        delete $1;
    } 
    ;

Concrete_Pattern_Name_Qualifier_C :  '.' PEEP2 {
        driver.currentPattern = new ConcretePattern(Pattern::peep2);
        driver.currentPattern->setPatName ("peep2_" + driver.itos (driver.getConstructIndex (driver.currentPattern->getPatType ())));
    }   
    | '.' SPLIT {
        driver.currentPattern = new ConcretePattern(Pattern::split);
        driver.currentPattern->setPatName ("split_" + driver.itos (driver.getConstructIndex (driver.currentPattern->getPatType ())));
    }
    | Concrete_Pattern_Name_List '.' INSN_SPLIT {
        string t = driver.appendStrings ($1);
        driver.currentPattern = new ConcretePattern (t, Pattern::insnAndSplit);
        delete $1;
    }
    ;

Concrete_Rtl_Spec_S : INSTANTIATES Concrete_Pattern_Name_List Concrete_Instantiates_Body {
        /* Search Symbol Table for abstract patterns. */
        string t = driver.appendStrings ($2);
        Pattern* p = driver.findSymTabEntry (t);
        if (p == NULL) {
            int i = driver.findRtlOperandSymTabEntry (t);
            if (i < 0) {
                driver.currentPattern->setError ();
            } else {
                ModeStmt* ms = new ModeStmt ();
                vector<int>* access = new vector<int>();
                //access->push_back(0);
                ms->addAccessTree (access);
                ms->setPatternName(t);
                AbstPattern* ap = new AbstPattern (t);
                ap->addStmt(*ms);
                driver.currentPattern->setParentPattern (ap);
                ConcretePattern* cp = static_cast <ConcretePattern*> (driver.currentPattern);
                cp->setSubType (Pattern::instantiates);
            }
            delete $2;
        } else {
            if (p->getPatType () != Pattern::abstract) {
                int i = driver.findRtlOperandSymTabEntry (t);
                if (i < 0) {
                    driver.currentPattern->setError ();
                } else {
                    ModeStmt* ms = new ModeStmt ();
                    vector<int>* access = new vector<int>();
                    //access->push_back(0);
                    ms->addAccessTree (access);
                    ms->setPatternName(t);
                    AbstPattern* ap = new AbstPattern (t);
                    ap->addStmt(*ms);
                    driver.currentPattern->setParentPattern (ap);
                    ConcretePattern* cp = static_cast <ConcretePattern*> (driver.currentPattern);
                    cp->setSubType (Pattern::instantiates);
                }
                delete $2;
            } else {
                driver.currentPattern->setParentPattern (p);
                ConcretePattern* cp = static_cast <ConcretePattern*> (driver.currentPattern);
                cp->setSubType (Pattern::instantiates);
                delete $2;
            }
        }
    }
    ;

Concrete_Rtl_Spec   : INSTANTIATES Concrete_Pattern_Name_List Concrete_Instantiates_Body CMD_SPEC_BODY {
        /* Search Symbol Table for abstract patterns. */
        string t = driver.appendStrings ($2);
        Pattern* p = driver.findSymTabEntry (t);
        if (p == NULL) {
            int i = driver.findRtlOperandSymTabEntry (t);
            if (i < 0) {
                driver.currentPattern->setError ();
            } else {
                ModeStmt* ms = new ModeStmt ();
                vector<int>* access = new vector<int>();
                //access->push_back(0);
                ms->addAccessTree (access);
                ms->setPatternName(t);
                AbstPattern* ap = new AbstPattern (t);
                ap->addStmt(*ms);
                driver.currentPattern->setParentPattern (ap); 
                ConcretePattern* cp = static_cast <ConcretePattern*> (driver.currentPattern);
                cp->setCmdBody (*$4);
                cp->setSubType (Pattern::instantiates);
                if (driver.generateDotFile) {
                    driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
                    driver.dotFile << "\"" + driver.formatNodeName (t) + "\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
                    driver.dotFile << "\"" + driver.formatNodeName (t) + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [color=blue];\n";
                }
                driver.incrementAbstractPatternCount (t, driver.currentPattern->getPatType ());
                driver.addToStatTree (t, driver.currentPattern->getPatName (), 2);
            }
            delete $2;
		} else {
            if (p->getPatType () != Pattern::abstract) {
                int i = driver.findRtlOperandSymTabEntry (t);
                if (i < 0) {
                    driver.currentPattern->setError ();
                } else {
                    ModeStmt* ms = new ModeStmt ();
                    vector<int>* access = new vector<int>();
                    //access->push_back(0);
                    ms->addAccessTree (access);
                    ms->setPatternName(t);
                    AbstPattern* ap = new AbstPattern (t);
                    ap->addStmt(*ms);
                    driver.currentPattern->setParentPattern (ap);
                    ConcretePattern* cp = static_cast <ConcretePattern*> (driver.currentPattern);
                    cp->setSubType (Pattern::instantiates);
                    cp->setCmdBody (*$4);
                    if (driver.generateDotFile) {
                        driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
                        driver.dotFile << "\"" + driver.formatNodeName (t) + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [color=blue];\n";
                    }
                driver.incrementAbstractPatternCount (t, driver.currentPattern->getPatType ());
                driver.addToStatTree (t, driver.currentPattern->getPatName (), 2);
                }
                delete $2;
            } else {
                driver.currentPattern->setParentPattern (p);
                ConcretePattern* cp = static_cast <ConcretePattern*> (driver.currentPattern);
                cp->setCmdBody (*$4);
                cp->setSubType (Pattern::instantiates);
                delete $2;
                if (driver.generateDotFile) {
                    driver.dotFile << "\"" + driver.formatNodeName (t) + "\" [shape=box,style=filled,color=orange];\n";
                    driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
                    driver.dotFile << "\"" + driver.formatNodeName (t) + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [color=blue];\n";
                }
                driver.incrementAbstractPatternCount (t, driver.currentPattern->getPatType ());
                driver.addToStatTree (t, driver.currentPattern->getPatName (),2 );
            }
        }
    }   
    | OVERRIDES Concrete_Pattern_Name_List '.' INSN Concrete_Overrides_Body CMD_SPEC_BODY {
        /* Search Symbol Table for Concrete Patterns. */
        string t = driver.appendStrings ($2);
        Pattern* p = driver.findSymTabEntry (t);
        if( p == NULL) {
        	driver.currentPattern->setError();
		} else {
            driver.currentPattern->setParentPattern (p);
            ConcretePattern* cp = static_cast <ConcretePattern*> (driver.currentPattern);
            cp->setCmdBody(*$6);
            cp->setSubType (Pattern::overrides);
            if (driver.generateDotFile) {
                driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
                driver.dotFile << "\"" + driver.formatNodeName (t) + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\";\n";
            } 
            driver.addToStatTree (t, driver.currentPattern->getPatName (), 2);
            driver.incrementConcretePatternCountForOverrides (t);
        }
        delete $2; 
    }
    | OVERRIDES Concrete_Pattern_Name_List '.' EXP Concrete_Overrides_Body CMD_SPEC_BODY {
        /* Search Symbol Table for Concrete Patterns. */
        string t = driver.appendStrings ($2);
        Pattern* p = driver.findSymTabEntry (t);
        if( p == NULL) {
            driver.currentPattern->setError();
        } else {
            driver.currentPattern->setParentPattern (p);
            ConcretePattern* cp = static_cast <ConcretePattern*> (driver.currentPattern);
            cp->setCmdBody(*$6);
            cp->setSubType (Pattern::overrides);
            if (driver.generateDotFile) {
                driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
                driver.dotFile << "\"" + driver.formatNodeName (t) + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\";\n";
            }
            driver.addToStatTree (t, driver.currentPattern->getPatName (), 2);
            driver.incrementConcretePatternCountForOverrides (t);
        }
        delete $2; 
    }
    ;

Concrete_Rtl_Spec_C : INSTANTIATES '.' IN Concrete_Pattern_Name_List Concrete_Instantiates_Body_C {
        string t = driver.appendStrings ($4);
        Pattern* p = driver.findSymTabEntry (t);
        if (p == NULL) {
            int i = driver.findRtlOperandSymTabEntry (t);
            if (i < 0) {
                driver.currentPattern->setError ();
            } else {
                ModeStmt* ms = new ModeStmt ();
                vector<int>* access = new vector<int>();
                //access->push_back(0);
                ms->addAccessTree (access);
                ms->setPatternName (t);
                AbstPattern* ap = new AbstPattern (t);
                ap->addStmt(*ms);
                driver.currentPattern->setParentPattern (ap);
                ConcretePattern* cp = static_cast <ConcretePattern*> (driver.currentPattern);
                cp->setSubType (Pattern::instantiates);
                if (driver.generateDotFile) {
                    driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=orange];\n";
                    driver.dotFile << "\"" + t + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\";\n";
                }   
                driver.incrementAbstractPatternCount (t, driver.currentPattern->getPatType ());
                driver.addToStatTree (t, driver.currentPattern->getPatName (), 2);
            }
            delete $4;
        } else {
            driver.currentPattern->setParentPattern (p);
            ConcretePattern* cp = static_cast <ConcretePattern*> (driver.currentPattern);
            cp->setSubType (Pattern::instantiates);
            delete $4;
            if (driver.generateDotFile) {
                driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=orange];\n";
                driver.dotFile << "\"" + t + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\";\n";
            }   
            driver.incrementAbstractPatternCount (t, driver.currentPattern->getPatType ());
            driver.addToStatTree (t, driver.currentPattern->getPatName (), 2);
        }
    }
    | OVERRIDES '.' IN Concrete_Pattern_Name_List Concrete_Instantiates_Body_C {
        string t = driver.appendStrings ($4);
        Pattern* p = driver.findSymTabEntry (t);
        if (p == NULL) {
            driver.currentPattern->setError ();
        } else {
            driver.currentPattern->setParentPattern (p);
            ConcretePattern* cp = static_cast<ConcretePattern*> (driver.currentPattern);
            cp->setSubType (Pattern::overrides);
        }
        delete $4;
    }
    ;

Concrete_Instantiates_Body_C : Concrete_Instantiates_Body CMD_SPEC '.' IN CMD_SPEC_BODY {
        ConcretePattern* cp = static_cast<ConcretePattern*> (driver.currentPattern);
        cp->setCmdBody (*$5);
    }
    Concrete_Instantiates_Body_C2 {

    }
    ;

Concrete_Instantiates_Body_C2 : INSTANTIATES '.' OUT Concrete_Pattern_Name_List Concrete_Instantiates_Body_Out CMD_SPEC '.' OUT CMD_SPEC_BODY {
        string t = driver.appendStrings ($4);
        Pattern* p = driver.findSymTabEntry (t);
        if (p == NULL) {
            int i = driver.findRtlOperandSymTabEntry (t);
            if (i < 0) {
                driver.currentPattern->setError ();
            } else {
                ModeStmt* ms = new ModeStmt ();
                vector<int>* access = new vector<int>();
                //access->push_back(0);
                ms->addAccessTree (access);
                ms->setPatternName (t);
                AbstPattern* ap = new AbstPattern (t);
                ap->addStmt(*ms);
                ConcretePattern* cp = static_cast <ConcretePattern*> (driver.currentPattern);
                cp->setSubTypeOut (Pattern::instantiates);
                cp->setCmdBodyOut (*$9);
                cp->setParentPatternOut (ap);
                if (driver.generateDotFile) {
                    driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=orange];\n";
                    driver.dotFile << "\"" + t + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\";\n";
                }   
//                driver.incrementAbstractPatternCount (t, driver.currentPattern->getPatType ());
            }
            delete $4;
        } else {
            ConcretePattern* cp = static_cast <ConcretePattern*> (driver.currentPattern);
            cp->setSubTypeOut (Pattern::instantiates);
            cp->setCmdBodyOut (*$9);
            cp->setParentPatternOut (p);
            delete $4;
            if (driver.generateDotFile) {
                driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=orange];\n";
                driver.dotFile << "\"" + t + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\";\n";
            }   
            driver.incrementAbstractPatternCount (t, driver.currentPattern->getPatType ());
//            driver.addToStatTree (t, driver.currentPattern->getPatName (), 2);
        }
    }
    | INSTANTIATES '.' OUT Concrete_Pattern_Name_List Concrete_Instantiates_Body_Out {
        string t = driver.appendStrings ($4);
        Pattern* p = driver.findSymTabEntry (t);
        if (p == NULL) {
            int i = driver.findRtlOperandSymTabEntry (t);
            if (i < 0) {
                driver.currentPattern->setError ();
            } else {
                ModeStmt* ms = new ModeStmt ();
                vector<int>* access = new vector<int>();
                //access->push_back(0);
                ms->addAccessTree (access);
                ms->setPatternName (t);
                AbstPattern* ap = new AbstPattern (t);
                ap->addStmt(*ms);
                ConcretePattern* cp = static_cast <ConcretePattern*> (driver.currentPattern);
                cp->setSubTypeOut (Pattern::instantiates);
                cp->setCmdBodyOut ("");
                cp->setParentPatternOut (ap);
                if (driver.generateDotFile) {
                   driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=orange];\n";
                   driver.dotFile << "\"" + t + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\";\n";
                }   
                driver.incrementAbstractPatternCount (t, driver.currentPattern->getPatType ());
//                driver.addToStatTree (t, driver.currentPattern->getPatName (), 2);
            }
            delete $4;
        } else {
            ConcretePattern* cp = static_cast <ConcretePattern*> (driver.currentPattern);
            cp->setSubTypeOut (Pattern::instantiates);
            cp->setCmdBodyOut ("");
            cp->setParentPatternOut (p);
            delete $4;
            if (driver.generateDotFile) {
                driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=orange];\n";
                driver.dotFile << "\"" + t + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\";\n";
            }   
            driver.incrementAbstractPatternCount (t, driver.currentPattern->getPatType ());
//           driver.addToStatTree (t, driver.currentPattern->getPatName (), 2);
        }
    }
    | OVERRIDES '.' OUT Concrete_Pattern_Name_List Concrete_Overrides_Body_Out CMD_SPEC '.' OUT CMD_SPEC_BODY {
        string t = driver.appendStrings ($4);
        ConcretePattern* cp = static_cast<ConcretePattern*> (driver.currentPattern);
        cp->setCmdBodyOut (*$9);
        cp->setSubTypeOut (Pattern::overrides);

        Pattern* p = driver.findSymTabEntry (t);
        if (p == NULL) {
            driver.currentPattern->setError ();
            } else {
            cp->setParentPatternOut (p);
        }
        delete $9;
        delete $4;
    }
    ;

Concrete_Instantiates_Body : '{' Concrete_Instantiates_Body_Inner '}' {
    }
    ;

Concrete_Overrides_Body : '{' Concrete_Overrides_Body_Inner '}' {
    }
    ;

Concrete_Instantiates_Body_Inner : ROOT '(' Operands ')' ';' Concrete_Inst_Stmt_List {
        ConcretePattern* cp = static_cast<ConcretePattern*> (driver.currentPattern);
        cp->addOperands (*$3);
        cp->addModeStmts (*$6);
        delete $3;
        delete $6;
    }
    | ROOT '(' Operands ')' ';' {
        ConcretePattern* cp = static_cast<ConcretePattern*> (driver.currentPattern);
        cp->addOperands (*$3);
        delete $3;
    } 
    | Concrete_Inst_Stmt_List {
        ConcretePattern* cp = static_cast<ConcretePattern*> (driver.currentPattern);
        cp->addModeStmts (*$1);
        delete $1;
    }
    ;

Quoted_Id_List : Quoted_Id_List ',' Quoted_Id {
        $$->push_back (*$3);
    }
    | Quoted_Id {
        $$ =  new std::vector<std::string> ();
        $$->push_back (*$1);
    }
    ;

Concrete_Overrides_Body_Inner : Concrete_Inst_Stmt_List {
        ConcretePattern* cp = static_cast<ConcretePattern*> (driver.currentPattern);
        cp->addModeStmts (*$1);
        delete $1;
    }
    ;

Concrete_Instantiates_Body_Out : '{' Concrete_Instantiates_Body_Inner_Out '}' {
    }
    ;

Concrete_Overrides_Body_Out : '{' Concrete_Overrides_Body_Inner_Out '}' {
    }
    ;

Concrete_Instantiates_Body_Inner_Out    : ROOT '(' Operands ')' ';' Concrete_Inst_Stmt_List {
        ConcretePattern* cp = static_cast<ConcretePattern*> (driver.currentPattern);
        cp->addOperandsOut (*$3);
        cp->addModeStmtsOut (*$6);
        delete $3;
        delete $6;
    }
    | ROOT '(' Operands ')' ';' {
        ConcretePattern* cp = static_cast<ConcretePattern*> (driver.currentPattern);
        cp->addOperandsOut (*$3);
        delete $3;
    }
    ;

Concrete_Overrides_Body_Inner_Out : Concrete_Inst_Stmt_List {
        ConcretePattern* cp = static_cast<ConcretePattern*> (driver.currentPattern);
        cp->addModeStmtsOut (*$1);
        delete $1;
    }
    ;

Concrete_Inst_Stmt_List: Concrete_Inst_Stmt_List Mode_Stmt {
        $$->push_back (*$2);
        delete $2;
    }
    |   Mode_Stmt {
            vector<ModeStmt>* msv = new vector<ModeStmt> ();
            msv->push_back (*$1);
            $$ = msv;
            delete $1;
        }
    ;

Stmt : ROOT Operand_Access ASSIGN Pattern_Name ';' {
        ModeStmt* ms = new ModeStmt ();
        ms->addAccessTree ($2);
        ms->setPatternName (*$4);
        ms->setType (ModeStmt::root);
        $$ = ms;
        driver.findRtlOperandSymTabEntry (*$4);
        delete $4;
    }
    ;

Mode_Stmt   : ROOT Attr_Access ASSIGN Predicate_Or_Mode ';' {
        $2->setMode (*$4); 
        if ($2->getType () == ModeStmt::predicate) {
            $2->setOperand (*$4, "", "");
        } else if ($2->getType () == ModeStmt::constraint) {
            $2->setOperand ("", "", "\"" + *$4 + "\"");
        }
        delete $4;
        $$ = $2;
    }
    | ROOT Attr_Access ASSIGN Quoted_Id ';' {
        $2->setMode (*$4);
        if ($2->getType () == ModeStmt::constraint) {
            $2->setOperand ("", "", *$4);
            delete $4;
        }
        $$ = $2;
    }
    | ROOT Attr_Access ASSIGN '<' Predicate_Or_Mode '>' ';' {
        $2->setMode ("<" + *$5 + ">");
        if ($2->getType () == ModeStmt::predicate) {
        $2->setOperand ("<" + *$5 + ">", "", "");
        } else if ($2->getType () == ModeStmt::constraint) {
        $2->setOperand ("", "", "\"<" + *$5 + ">\"");
        }
        delete $5;
        $$ = $2;
    }
    | ROOT Attr_Access ASSIGN CONST_INT ':' INT ';' {
        if ($2->getType () == ModeStmt::operand) {
            Operand* o = new Operand (Operand::constSpec, srtl_driver::stoi(*$6));
            $2->setOperand (*o);
            delete $6;
        }
        $$ = $2;
    }
    | ROOT Attr_Access ASSIGN ID '(' ID ':' FIXED_REG ')' ';' {
        if ($2->getType () == ModeStmt::operand) {
            Operand* o = new Operand (Operand::reg, *$4, *$6, *$8);
            $2->setOperand (*o);
            delete $4;delete $6; delete $8;
            $$ = $2;
        }
    }
    | ROOT Operand_Access ASSIGN Predicate_Or_Mode_List ':' Predicate_Or_Mode_List ':' Quoted_Id ';' {
        string t = driver.appendStrings ($4);
        string m = driver.appendStrings ($6);

        ModeStmt* ms = new ModeStmt ();
        ms->setType (ModeStmt::pmc);
        ms->addAccessTree ($2); 
        ms->setOperand (t, m, *$8);
        delete $4; delete $6; delete $8;
        $$ = ms;
    }
    | ROOT Operand_Access ASSIGN INT '=' Predicate_Or_Mode_List ':' Predicate_Or_Mode_List ':' Quoted_Id ';' {
        string t = driver.appendStrings ($6);
        string m = driver.appendStrings ($8);

        ModeStmt* ms = new ModeStmt ();
        ms->setType (ModeStmt::pmc);
        ms->addAccessTree ($2); 
        ms->setOperand (srtl_driver::stoi(*$4), t, m, *$10);
        delete $4; delete $6; delete $8; delete $10;
        $$ = ms;
    }
    | LATENCY ASSIGN INT ';' {
        ModeStmt* ms = new ModeStmt ();
        ms->setType (ModeStmt::latency);
        ms->setMode (*$3);
        delete $3;
        $$ = ms;
    }
    | LOV ASSIGN Quoted_Id ';' {
        ModeStmt* ms = new ModeStmt ();
        ms->setType (ModeStmt::listOfValues);
        ms->setMode (*$3);
        delete $3;
        $$ = ms;
    }
    | DOCSTRING ASSIGN Quoted_Id ';' {
        ModeStmt* ms = new ModeStmt ();
        ms->setType (ModeStmt::docstring);
        ms->setMode (*$3);
        delete $3;
        $$ = ms;
    }
    | REGCLASS ASSIGN Quoted_Id ';' {
        ModeStmt* ms = new ModeStmt ();
        ms->setType (ModeStmt::regclass);
        ms->setMode (*$3);
        delete $3;
        $$ = ms;
    }
    | ALLCONSTRAINTS ASSIGN '(' Quoted_Id_List ')' ';' {
        ModeStmt* ms = new ModeStmt ();
        ms->setType (ModeStmt::allConstraints);
        ms->setConstraints ($4);
        $$ = ms;
    }
    | ID '-' '>' ID ';' {
        ModeStmt* ms = new ModeStmt ();
        ms->setType (ModeStmt::replaceMode);
        ms->setMode (*$1);
        ms->setModeReplacement (*$4);
        $$ = ms;
        delete $1;
        delete $4;
    }
;

Attr_Access : '.' ID {   /* Change the attribute of a node */
        /*This should be mode. */
        /* Check again whether this is required ?*/
        delete $2;
    }
    | Operand_Access '.' ID {
        ModeStmt* ms = new ModeStmt ();
        ms->addAccessTree ($1);
        if (*$3 == "mode") {
            ms->setType (ModeStmt::mode);
            $$ = ms;
        } else if (*$3 == "constraint") {
            ms->setType (ModeStmt::constraint);
            $$ = ms;
        } else if (*$3 == "operand") {
            ms->setType (ModeStmt::operand);
            $$ = ms;
        } else {
            // TODO report error.
        }
        delete $3;
    } 
    | Operand_Access '.' PREDICATE {
        ModeStmt* ms = new ModeStmt ();
        ms->addAccessTree ($1);
        ms->setType (ModeStmt::predicate);
        $$ = ms;
    }
    ;

Operand_Access  : Operand_Access '.' INT {
        $$->push_back(driver.stoi (*$3));
        delete $3;
    }
    | '.' INT 
    {
        vector<int>* intVector = new vector<int>();
        $$ = intVector;
        $$->push_back(driver.stoi (*$2));
        delete $2;
    }
    ;

Operands    :  Operands ',' '(' Operands ')' {
        vector <Operand>* oplist = $4;
        Operand* start = new Operand (Operand::bracketOpen, "");
        Operand* end = new Operand (Operand::bracketClose, "");
        $4->insert ($4->begin (), *start);
        $4->push_back (*end);

        for (unsigned int i = 0; i < oplist->size (); i++) {
            $$->push_back ($4->at(i));
        }

           delete oplist;
        }
    | '(' Operands ')' {
        Operand* start = new Operand (Operand::bracketOpen, "");
        Operand* end = new Operand (Operand::bracketClose, "");
        $2->insert ($2->begin (), *start);
        $2->push_back (*end);
        $$ = $2;
    }
    | Operands ',' Operand {
        $$->push_back (*$3);
        delete $3;
    }
    | Operand {
        vector<Operand>* oplist = new vector<Operand>();
        $$ = oplist;
        $$->push_back (*$1);
        delete $1;
    }
    ;

Operand : Predicate_Or_Mode_List ':' Predicate_Or_Mode_List ':' Constraint {
        string t = driver.appendStrings ($1);
        string m = driver.appendStrings ($3);
        
        Operand* o = new Operand (Operand::pmc, t, m, *$5);
        $$ = o;
        driver.debug (srtl_driver::grammar, "Operand: PML:PML:C ::" + t + ":" + m + ":" + *$5);
        delete $3; delete $5; delete $1;
    }
    | INT '=' Predicate_Or_Mode_List ':' Predicate_Or_Mode_List ':' Constraint {
        string t = driver.appendStrings ($3);
        string m = driver.appendStrings ($5);
        Operand* o = new Operand (Operand::pmc, driver.stoi (*$1), t, m, *$7);
        $$ = o;
        driver.debug (srtl_driver::grammar, "Operand: INT:PML:PML:C ::" + *$1 +"=" + t + ":"+m+":"+*$7);
        delete $1;
        delete $3;
        delete $5;
        delete $7;
    }
    | DUPLICATE INT {
        Operand* o = new Operand (Operand::dup, driver.stoi (*$2));
        driver.debug (srtl_driver::grammar, "Operand: Duplicate INT ::" + *$2);
        $$ = o;
        delete $2;
    }
    | Predicate_Or_Mode_List ':' Constraint {
        string t = driver.appendStrings ($1);
        if (driver.currentPattern->getPatType () == Pattern::cons || 
                driver.currentPattern->getPatType () == Pattern::regCons ||
                driver.currentPattern->getPatType () == Pattern::memCons ||
                driver.currentPattern->getPatType () == Pattern::addCons || 
                driver.currentPattern->getPatType () == Pattern::attr ||
                driver.currentPattern->getPatType () == Pattern::asmAttr ||
                driver.currentPattern->getPatType () == Pattern::insnReservation) {
            Operand* o = new Operand (Operand::eqAttr, t, *$3);
            driver.debug (srtl_driver::grammar, "Operand: PM:C :: " + t + ":" + *$3);
            delete $1; delete $3;
            $$ = o;
        } else {
            Operand* o = new Operand (Operand::scratch, t, *$3);
            driver.debug (srtl_driver::grammar, "Operand: PM:C ::" + t + ":" + *$3);
            delete $1; delete $3;
            $$ = o;
        }
    } 
    | ATTR ':' Constraint {
        Operand* o = new Operand (Operand::eqAttr, "attr", *$3);
        driver.debug (srtl_driver::grammar, "Operand: A:C :: attr:" + *$3);
        delete $3;
        $$ = o;
    }
    | INT '=' Predicate_Or_Mode_List ':' Constraint {
        string t = driver.appendStrings ($3);
        Operand* o = new Operand (Operand::scratch, driver.stoi (*$1), t, *$5);
        driver.debug (srtl_driver::grammar, "Operand: INT:PML:C ::" + *$1 + "=" + t + ":" + *$5);
        delete $1; delete $3; delete $5;
        $$ = o;
    }
    | INT '=' Predicate_Or_Mode_List {
        string t = driver.appendStrings ($3);
        Operand* o = new Operand (Operand::matchOperator, driver.stoi (*$1), t);
        $$ = o;
            driver.debug (srtl_driver::grammar, "Operand: INT:PML ::" + *$1 + "=" + t);
        delete $1; delete $3;
    }
    | INT {
        Operand* o = new Operand (Operand::num, driver.stoi (*$1));
        $$ = o;
        driver.debug (srtl_driver::grammar, "Operand: INT" + *$1);
        delete $1;
    } 
    | Fixed_Reg_Or_Const {
        /* For register, memory, constant etc. operands */
        $$ = $1;
        driver.debug (srtl_driver::grammar, "Operand: Fixed_Reg_Or_Const");
    }
    ;

Fixed_Reg_Or_Const : ID '(' Predicate_Or_Mode_List ':' FIXED_REG ')' {
        string t = driver.appendStrings ($3);
        Operand* o = new Operand (Operand::reg, *$1, t, *$5);
        $$ = o;
        driver.debug (srtl_driver::grammar, "Fixed_Reg_Or_Const: ID(PM:C) ::" + *$1 + "(" + t + ":" + *$5 + ")");
        delete $1;
        delete $3;
        delete $5;
    }
    | ID '(' Predicate_Or_Mode_List ':' INT ')' {
        string t = driver.appendStrings ($3);
        Operand * o = new Operand (Operand::regNum, *$1, t, *$5);
        $$ = o;
        driver.debug (srtl_driver::grammar, "Fixed_Reg_Or_Const: ID(PM:INT) ::"  + *$1 + "(" + t + ":" + *$5 + ")");
        delete $1; delete $3; delete $5;
    }
    | FIXED_REG {
        Operand* o = new Operand (Operand::fixedReg, *$1);
        $$ = o;
        driver.debug (srtl_driver::grammar, "Fixed_Reg_Or_Const: FIXED_REG ::" + *$1);
        delete $1;
    } 
    | ID {
        Operand* o = new Operand (Operand::extraReg, *$1);
        $$ = o;
        driver.debug (srtl_driver::grammar, "Fixed_Reg_Or_Const: ID ::" + *$1);
        delete $1;
    } 
    | CONST_INT ':' INT {
        Operand* o = new Operand (Operand::constSpec, driver.stoi (*$3));
        $$ = o;
        driver.debug (srtl_driver::grammar, "Fixed_Reg_Or_Const: CONST_INT:INT ::" + *$3);
        delete $3;
    }
    | CONST_INT ':' INT ':' ID {
        Operand* o = new Operand (Operand::constSpec, driver.stoi (*$3), *$5);
        $$ = o;
        driver.debug (srtl_driver::grammar, "Fixed_Reg_Or_Const: CONST_INT:INT:ID ::" + *$3 + ":" + *$5);
        delete $3; delete  $5;
    }
    | '<' INT '>' {
        Operand* o = new Operand (Operand::integer, driver.stoi (*$2));
        $$ = o;
        driver.debug (srtl_driver::grammar, "Fixed_Reg_Or_Const: <INT> ::" + *$2);
        delete $2;
    }
    | '<' ID '>' {
        Operand* o = new Operand (Operand::string, *$2);
        $$ = o;
        driver.debug (srtl_driver::grammar, "Fixed_Reg_Or_Const: <ID> ::" + *$2);
        delete $2;
    }
    ;

Constraint : Quoted_Id {
        $$ = $1;
        driver.debug (srtl_driver::grammar, "Constraint: " + *$1);
    }
    ;

Predicate_Or_Mode_List : Predicate_Or_Mode_List Predicate_Or_Mode_With_Brackets {
        $$->push_back (*$2);
        driver.debug (srtl_driver::grammar, "Predicate_Or_Mode_List: PML PM ::" + *$2);
        delete $2;
    }
    | Predicate_Or_Mode_List Predicate_Or_Mode {
        $$->push_back (*$2);
        driver.debug (srtl_driver::grammar, "Predicate_Or_Mode_List: PML PM ::" + *$2);
        delete $2;
    }
    | Predicate_Or_Mode_With_Brackets {
        $$ = new vector<string>();
        $$->push_back (*$1);
        driver.debug (srtl_driver::grammar, "Predicate_Or_Mode_List: PMWB ::" + *$1);
        delete $1;
    }
    | Predicate_Or_Mode {
        $$ = new vector<string> ();
        $$->push_back (*$1);
        driver.debug (srtl_driver::grammar, "Predicate_Or_Mode_List: PM ::" + *$1);
        delete $1;
    }
    ;

Predicate_Or_Mode_With_Brackets: '<' Predicate_Or_Mode '>' {
        $$ = new string ("<" + *$2 + ">");
        driver.debug (srtl_driver::grammar, "Predicate_Or_Mode_With_Brackets: <PM> ::<" + *$2 + ">");
        delete $2;
    }
    ;

Predicate_Or_Mode : INT {
        $$ = $1;
        driver.debug (srtl_driver::grammar, "Predicate_Or_Mode: INT ::" + *$1);
    }
    | ID {
        $$ = $1;
        driver.debug (srtl_driver::grammar, "Predicate_Or_Mode: ID::" + *$1);
    }
    ;


Quoted_Id   : QUOTED_ID {
        $$ = $1;
    }
    ;

Concrete_Pattern_Name_List : Concrete_Pattern_Name_List Concrete_Pattern_Name_With_Brackets {
        $$->push_back (*$2);
        delete $2;
        driver.debug (srtl_driver::grammar, "Concrete_Pattern_Name_List: Concrete_Pattern_Name_List Concrete_Pattern_Name_With_Brackets");
    }
    | Concrete_Pattern_Name_List Concrete_Pattern_Name {
        $$->push_back (*$2);
        delete $2;
        driver.debug (srtl_driver::grammar, "Concrete_Pattern_Name_List: Concrete_Pattern_Name_List Concrete_Pattern_Name");
    }
    | Concrete_Pattern_Name_With_Brackets {
        $$ = new vector<string> ();
        $$->push_back (*$1);
        delete $1;
        driver.debug (srtl_driver::grammar, "Concrete_Pattern_Name_List: Concrete_Pattern_Name_With_Brackets");

    }
    | Concrete_Pattern_Name {
        $$ = new vector<string> ();
        $$->push_back (*$1);
        delete $1;
        driver.debug (srtl_driver::grammar, "Concrete_Pattern_Name_List: Concrete_Pattern_Name_List");
    }
    ;

Concrete_Pattern_Name_With_Brackets : '<' Concrete_Pattern_Name '>' {
        $$ = new string ("<" + *$2 + ">");
        driver.debug (srtl_driver::grammar, "Concrete_Pattern_Name_With_Brackets: " + *$$);
        delete $2;
    }
    | '<' Concrete_Pattern_Name ':' Concrete_Pattern_Name '>' {
        $$ = new string ("<" + *$2 + ":" + *$4 + ">");
        driver.debug (srtl_driver::grammar, "Concrete_Pattern_Name_With_Brackets: " + *$$);
        delete $2; delete $4;
    }
    ;

Concrete_Pattern_Name : ID {
        $$ = $1;
        driver.debug (srtl_driver::grammar, "Concrete_Pattern_Name: ID: " + *$1);
    }
    | INT {
        $$ = new string (*$1);
        delete $1;
        driver.debug (srtl_driver::grammar, "Concrete_Patttern_Name: INT");
    }
    | '*' {
        $$ = new string ("*");
        driver.debug (srtl_driver::grammar, "Concrete_Pattern_Name: *");
    }
    | ':' {
        $$ = new string (":");
        driver.debug (srtl_driver::grammar, "Concrete_Pattern_Name: :");
    }
    | '_' {
        $$ = new string ("_");
        driver.debug (srtl_driver::grammar, "ConcretePatternName: _");
    }
    ;

Base_Name : ID {
        $$ = $1;
    }
    ;

Pattern_Name : ID {
        $$ = $1;
    }
    ;
            
/* List Pattern Grammar */
List_Pattern: LIST QUOTED_ID '.' AUTOMATON {
        ListPattern * l =new ListPattern();
        string sq ="\"", eq="\"", p;
        p= *$2;
        l->setPatType(Pattern::define_automaton);
        l->setPatName(p);
        $$=l;	
    }
    | LIST QUOTED_ID '.' C_ENUM OBRACE ListEntries CBRACE {
        ListPattern * l=new ListPattern();
        l->setPatName(*$2);
        delete $2;;
        l->setPatType(Pattern::define_c_enum);
        l->append($6);
        $$=l;
    }
    | LIST INT '.' BYPASS QIDList {
    	ListPattern * l =new ListPattern();
    	l->setPatType(Pattern::define_bypass);
    	l->setPatName(*$2);
    	delete $2;
    	l->append($5);
		$$=l;
        } 
    | LIST QUOTED_ID '.' CPU_UNIT QUOTED_ID {
    	ListPattern * l =new ListPattern();
        l->setPatType(Pattern::define_cpu_unit);
        l->setPatName(*$2);
        delete $2;
        l->append($5);
    	$$=l;
    }
    | LIST QUOTED_ID '.' RESERVATION QUOTED_ID {
        ListPattern * l =new ListPattern();
        string sq ="\"", eq="\"",* p;
        l->setPatType(Pattern::define_reservation);
        l->setPatName(*$2);
        delete $2;
        p= new string (*$5);
        l->append(p);
        $$=l;
    }
    | LIST QUOTED_ID '.' RESERVATION '"' PipedId '"' {
        ListPattern * l =new ListPattern();
        string sq ="\"", eq="\"",* p;
        l->setPatType(Pattern::define_reservation);
        l->setPatName(*$2);
        delete $2;
        p= new string (sq + *$6 +eq);
        l->append(p);
        $$=l;
    }
    | LIST  NONAME '.' CONSTANTS OBRACE ListPID CBRACE {
        ListPattern * l =new ListPattern();
        l->setPatType(Pattern::define_constants);
        l->append($6);
        $$=l;
    } 
    | LIST NONAME '.' ASM_ATTR OBRACE ListAsmEntries CBRACE {
        ListPattern * l =new ListPattern();
	    l->setPatType(Pattern::define_asm_attributes);
	    l->append($6);
	    $$=l;
    }
    | LIST ID '.' Code_Mode_Iter OBRACE ListEntries CBRACE {
        ListPattern * l=new ListPattern();
        l->setPatName(*$2);
        l->setPatType($4);
        l->append($6);
        $$=l;
        delete $2;
    }
    | LIST ID '.' Code_Mode_Attr OBRACE ListQEntries CBRACE {
    	 ListPattern * l=new ListPattern();
    	 l->setPatName(*$2);
    	 l->setPatType($4);
    	 l->append($6);
    	 $$=l;
    	 delete $2;
    }
    ;

Code_Mode_Iter: C_ITER {
        $$=Pattern::define_code_iterator;
    }
    | M_ITER {
        $$=Pattern::define_code_iterator;
    }
    ;

Code_Mode_Attr: C_ATTR {
        $$=Pattern::define_code_attr;
    }
    | M_ATTR {
        $$=Pattern::define_mode_attr;
    }
    ;

ListAsmEntries  : ListAsmEntries ',' AsmEntry {
	   ElemList * el=new ElemList($1,$3);
       $$=el;	
    }
    | AsmEntry {
        ElemList * el=new ElemList($1);
        $$=el;
    }
	;

AsmEntry : '(' SET_ATTR ',' Quoted_Id ',' QUOTED_ID ')' {
        Element *e;
		string q = "\"";
		string *s = new string (*$6);
		e=new Element($4,s);
		delete $6;
        $$=e;
	}
    ;

ListPID   : ListPID ',' '(' IEntryPair ')'
                {
		ElemList * el = new ElemList($1,$4);
		$$ =el;
                }
          | '(' IEntryPair ')'
                {
               	ElemList * el = new ElemList($2);
		$$ = el;
		}
          ;
ListEntries : ListEntries  ',' SEntry {
		ElemList * el = new ElemList($1,$3);
		$$ =el;
    }
    | SEntry {
		ElemList * el = new ElemList($1);
		$$ = el;
	}
	|ListEntries  ',''(' SQEntryPair ')' {
    	ElemList * el = new ElemList($1,$4);
    	$$ =el;
    }
    | '(' SQEntryPair ')' {
    	ElemList * el = new ElemList($2);
    	$$ = el;
	}
    ;

ListQEntries : ListQEntries  ',''(' SQEntryPair ')' {
    	ElemList * el = new ElemList($1,$4);
    	$$ =el;
    }
    | ListQEntries ',' SEntry {
		ElemList * el = new ElemList($1,$3);
		$$ = el;
	}
    | '(' SQEntryPair ')' {
		ElemList * el = new ElemList($2);
		$$ = el;
    }
	| SEntry {
		ElemList * el = new ElemList($1);
		$$ = el;
    }
    ;

SQEntryPair : ID ',' Quoted_Id {
    Element * e;
    e= new Element($1,$3);
    $$ =e;
    }
	;

IEntryPair : Id_Reg ','  ID {
        Element * e;
        string *s = new string( *$3),* t= new string(*$1);
        delete $3;delete $1;
        e= new Element(t,s);
        $$=e;
    }
    | Id_Reg ',' INT { 
        Element * e;
        ostringstream temp;
        temp<<*$3;
        string *t=new string  (temp.str()); 
        e = new Element($1,t);
        $$=e; 
        delete $3;
    } 
    ;
    | Id_Reg',' INT ID {
        Element * e;
        string *s = new string( *$3+*$4),* t= new string(*$1);
        delete $3;delete $1;
        e= new Element(t,s);
        $$=e;
    }
	;

Id_Reg:  ID {
	   $$ = $1;
	}
	| FIXED_REG { 
	   $$ = $1;
	}
	;

QIDList : QUOTED_ID {
        string *s;
        s=new string(*$1);
        $$=s;
        delete $1;
    }
	|  QIDList QUOTED_ID {
		string *s;
		s= new string (*$1);
		*s=*s+ *$2;
		$$=s;	
	}
    ;

SEntry :   ID {
        Element * e;
        string *s=new string(*$1);
        e= new Element(s) ;
        $$=e;
        delete $1;
    }
    ;

PipedId : ID PIPE PipedId {
		string *s;
		s= new string (*$1);
		*s=*s+ "|"+*$3;
		$$=s;	
		delete $3; 
		delete $1;
	}
	| '(' PipedId ')' {
		string *s;
		s= new string (*$2);
		*s= "("+ *s+ ")";
		$$=s;	
		delete $2;
	}
	|  ID PLUS PipedId {
		string *s;
		s= new string (*$1);
		*s=*s+ "+"+*$3;
		$$=s;	
		delete $1;
		delete $3;
    }
	| ID {
        string *s;
        s=new string(*$1);
        $$=s;
        delete $1;
    }
	| QUOTED_ID	{
        string *s;
        s=new string(*$1);
        $$=s;
        delete $1;
    }
	| ID ','  PipedId {
		string *s;
		s= new string (*$1);
		*s=*s+ ","+*$3;
		$$=s;	
		delete $3;
		delete $1;
	}
	;		
/*
AutoId  : ID ',' AutoId
		 {
		string *s;
		s= new string (*$1);
		*s=*s+ ","+*$3;
		$$=s;
		delete $3;
		delete $1;
		}	
		| ID
		{
		  string *s;
		  s=new string(*$1);
		  $$=s;
		  delete $1;
		}
		;	
*/

%%

void yy::srtl_parser::error (const yy::srtl_parser::location_type& l,
                             const std::string& m) {
    driver.error (l, m);
}

