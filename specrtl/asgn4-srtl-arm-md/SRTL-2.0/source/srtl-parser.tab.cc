// A Bison parser, made by GNU Bison 3.0.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2013 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


// First part of user declarations.

#line 37 "srtl-parser.tab.cc" // lalr1.cc:399

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "srtl-parser.tab.hh"

// User implementation prologue.

#line 51 "srtl-parser.tab.cc" // lalr1.cc:407
// Unqualified %code blocks.
#line 33 "srtl-parser.yy" // lalr1.cc:408

#include "srtl-driver.hh"

#line 57 "srtl-parser.tab.cc" // lalr1.cc:408


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyempty = true)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace yy {
#line 143 "srtl-parser.tab.cc" // lalr1.cc:474

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  srtl_parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  srtl_parser::srtl_parser (srtl_driver& driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg)
  {}

  srtl_parser::~srtl_parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  inline
  srtl_parser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  srtl_parser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  srtl_parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
    value = other.value;
  }


  template <typename Base>
  inline
  srtl_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  inline
  srtl_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  inline
  srtl_parser::basic_symbol<Base>::~basic_symbol ()
  {
  }

  template <typename Base>
  inline
  void
  srtl_parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
    value = s.value;
    location = s.location;
  }

  // by_type.
  inline
  srtl_parser::by_type::by_type ()
     : type (empty)
  {}

  inline
  srtl_parser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  srtl_parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  srtl_parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.type = empty;
  }

  inline
  int
  srtl_parser::by_type::type_get () const
  {
    return type;
  }


  // by_state.
  inline
  srtl_parser::by_state::by_state ()
    : state (empty)
  {}

  inline
  srtl_parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  srtl_parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.state = empty;
  }

  inline
  srtl_parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  srtl_parser::symbol_number_type
  srtl_parser::by_state::type_get () const
  {
    return state == empty ? 0 : yystos_[state];
  }

  inline
  srtl_parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  srtl_parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
    value = that.value;
    // that is emptied.
    that.type = empty;
  }

  inline
  srtl_parser::stack_symbol_type&
  srtl_parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  srtl_parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YYUSE (yysym.type_get ());
  }

#if YYDEBUG
  template <typename Base>
  void
  srtl_parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  srtl_parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  srtl_parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  srtl_parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  srtl_parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  srtl_parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  srtl_parser::debug_level_type
  srtl_parser::debug_level () const
  {
    return yydebug_;
  }

  void
  srtl_parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline srtl_parser::state_type
  srtl_parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  srtl_parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  srtl_parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  srtl_parser::parse ()
  {
    /// Whether yyla contains a lookahead.
    bool yyempty = true;

    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    // User initialization code.
    #line 22 "srtl-parser.yy" // lalr1.cc:725
{
    // Initialize the initial location.
    yyla.location.begin.filename = yyla.location.end.filename = &driver.file;
}

#line 487 "srtl-parser.tab.cc" // lalr1.cc:725

    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyempty)
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location, driver));
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
        yyempty = false;
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Discard the token being shifted.
    yyempty = true;

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 4:
#line 163 "srtl-parser.yy" // lalr1.cc:847
    { yyclearin; yyerrok; }
#line 601 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 5:
#line 167 "srtl-parser.yy" // lalr1.cc:847
    {
        if (!driver.currentPattern->inError ()){
            driver.debug (srtl_driver::deb , "Abstract Pattern.");
            driver.addSymTabEntry (driver.currentPattern);
            driver.addToConstructTally (driver.currentPattern->getPatType ());
        } else {
            driver.debug (srtl_driver::err, "Abstract Pattern in Error: " + driver.currentPattern->getPatName ());
        }
    }
#line 615 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 6:
#line 176 "srtl-parser.yy" // lalr1.cc:847
    {
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
#line 638 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 7:
#line 194 "srtl-parser.yy" // lalr1.cc:847
    {
        driver.debug (srtl_driver::deb , "List Pattern.");
        (yystack_[0].value.L)->createPattern();
     //   driver.addToConstructTally (driver.currentPattern->getPatType ());
        if ((yystack_[0].value.L)->getPatType () == Pattern::define_code_iterator) {
            ListPattern* lp = static_cast <ListPattern*> ((yystack_[0].value.L));
            string name = *(lp->getElemList ()->getList ().front ()->getName ());
            int k = driver.findRtlOperandSymTabEntry (name);
            driver.addRtlOperandSymTabEntry ((yystack_[0].value.L)->getPatName (),  k);

        }
        driver.write (srtl_driver::outPattern);
        delete (yystack_[0].value.L);
    }
#line 657 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 8:
#line 208 "srtl-parser.yy" // lalr1.cc:847
    {
        driver.write (*(yystack_[0].value.string));
    }
#line 665 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 9:
#line 211 "srtl-parser.yy" // lalr1.cc:847
    {
        printf (" ");
    }
#line 673 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 10:
#line 218 "srtl-parser.yy" // lalr1.cc:847
    {
    }
#line 680 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 11:
#line 222 "srtl-parser.yy" // lalr1.cc:847
    {
        driver.currentPattern = 0;
        driver.currentPattern = new AbstPattern(*(yystack_[0].value.string));
        delete (yystack_[0].value.string);
    }
#line 690 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 12:
#line 229 "srtl-parser.yy" // lalr1.cc:847
    {
        /* Search Symbol Table Or Known Names
         * for abstract patterns. */
        Pattern* p = driver.findSymTabEntry (*(yystack_[0].value.string));

        if(p == NULL) {
            int i = driver.findRtlOperandSymTabEntry (*(yystack_[0].value.string));
            if (i < 0) {
                driver.currentPattern->setError ();
            } else {
                ModeStmt* ms = new ModeStmt ();
                vector<int>* access = new vector<int>();
                //access->push_back(0);
                ms->addAccessTree (access);
                ms->setPatternName(*(yystack_[0].value.string));
                AbstPattern* ap = static_cast<AbstPattern*> (driver.currentPattern);
                ap->addStmt(*ms);
                delete access;
                delete ms;
                if (driver.generateDotFile) {
                    driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=orange];\n";
                    driver.dotFile << "\"" + *(yystack_[0].value.string) + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\";\n";
                }
                driver.incrementAbstractPatternCount (*(yystack_[0].value.string), Pattern::abstract);
                driver.addToStatTree (*(yystack_[0].value.string), driver.currentPattern->getPatName (), 1);
            }
            delete (yystack_[0].value.string);
        } else{
            if (p->getPatType () != Pattern::abstract) {
                int i = driver.findRtlOperandSymTabEntry (*(yystack_[0].value.string));
                if (i < 0) {
                    driver.currentPattern->setError ();
                } else {
                    ModeStmt* ms = new ModeStmt ();
                    vector<int>* access = new vector<int>();
                    //access->push_back(0);
                    ms->addAccessTree (access);
                    ms->setPatternName(*(yystack_[0].value.string));
                    AbstPattern* ap = static_cast<AbstPattern*> (driver.currentPattern);
                    ap->addStmt(*ms);
                    delete access;
                    delete ms;
                    if (driver.generateDotFile) {
                        driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=orange];\n";
                        driver.dotFile << "\"" + driver.formatNodeName (*(yystack_[0].value.string)) + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [color=red];\n";
                    }
                    driver.incrementAbstractPatternCount (*(yystack_[0].value.string), Pattern::abstract);
                    driver.addToStatTree (*(yystack_[0].value.string), driver.currentPattern->getPatName (), 1);
                }
                delete (yystack_[0].value.string);    
            } else {
                driver.currentPattern->setParentPattern (p);
                if (driver.generateDotFile) {
                    driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=orange];\n";
                    driver.dotFile << "\"" + driver.formatNodeName (*(yystack_[0].value.string)) + "\" [shape=box,style=filled,color=orange];\n";
                    driver.dotFile << "\"" + driver.formatNodeName (*(yystack_[0].value.string)) + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [color=red];\n";
               }
                driver.incrementAbstractPatternCount (*(yystack_[0].value.string), Pattern::abstract);
                driver.addToStatTree (*(yystack_[0].value.string), driver.currentPattern->getPatName (), 1);
            }
        }
    }
#line 757 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 13:
#line 291 "srtl-parser.yy" // lalr1.cc:847
    {
        AbstPattern* ap = static_cast<AbstPattern*> (driver.currentPattern);
        ap->addStmts(*(yystack_[1].value.modeStmtVector)); 
        delete (yystack_[1].value.modeStmtVector);
    }
#line 767 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 14:
#line 296 "srtl-parser.yy" // lalr1.cc:847
    {

    }
#line 775 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 15:
#line 299 "srtl-parser.yy" // lalr1.cc:847
    {

    }
#line 783 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 16:
#line 304 "srtl-parser.yy" // lalr1.cc:847
    {
        ModeStmt* ms = (yystack_[0].value.modeStmt);

        Pattern* p = driver.findSymTabEntry (ms->getPatternName ());
        if (p != NULL) {
          if (p->getPatType () != Pattern::abstract) {
            (yylhs.value.modeStmtVector)->push_back (*(yystack_[0].value.modeStmt));
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
            (yylhs.value.modeStmtVector)->push_back (tmsv->at(i));
          }
          delete tmsv;
          }
        } else {
            (yylhs.value.modeStmtVector)->push_back (*(yystack_[0].value.modeStmt));
        }
        delete ms;
    }
#line 818 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 17:
#line 334 "srtl-parser.yy" // lalr1.cc:847
    {
        vector<ModeStmt>* msv = new vector<ModeStmt> ();

        ModeStmt* ms = (yystack_[0].value.modeStmt);

        Pattern* p = driver.findSymTabEntry (ms->getPatternName ());
        if (p != NULL) {
          if (p->getPatType () != Pattern::abstract) {
            msv->push_back (*(yystack_[0].value.modeStmt));
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
            msv->push_back (*(yystack_[0].value.modeStmt));
        }
        delete ms;
        (yylhs.value.modeStmtVector) = msv;
    }
#line 857 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 18:
#line 369 "srtl-parser.yy" // lalr1.cc:847
    {    
    }
#line 864 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 19:
#line 374 "srtl-parser.yy" // lalr1.cc:847
    {
    }
#line 871 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 20:
#line 376 "srtl-parser.yy" // lalr1.cc:847
    {
        }
#line 878 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 21:
#line 378 "srtl-parser.yy" // lalr1.cc:847
    {
        }
#line 885 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 22:
#line 380 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = "";
        for (unsigned int i = 0; i < (yystack_[5].value.stringVector)->size (); i++) {
            t.append ((yystack_[5].value.stringVector)->at(i));
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
        cp->addModeStmts (*(yystack_[1].value.modeStmtVector));
        delete (yystack_[5].value.stringVector);
        delete (yystack_[1].value.modeStmtVector);
    }
#line 911 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 23:
#line 403 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[2].value.stringVector));
        driver.currentPattern = new ConcretePattern (t, Pattern::cons);
        delete (yystack_[2].value.stringVector);
    }
#line 921 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 24:
#line 408 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[2].value.stringVector));
        driver.currentPattern = new ConcretePattern (t, Pattern::attr);
        delete (yystack_[2].value.stringVector);
    }
#line 931 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 25:
#line 413 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[2].value.stringVector));
        driver.currentPattern = new ConcretePattern (t, Pattern::memCons);
        delete (yystack_[2].value.stringVector);
    }
#line 941 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 26:
#line 418 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[2].value.stringVector));
        driver.currentPattern = new ConcretePattern (t, Pattern::addCons);
        delete (yystack_[2].value.stringVector);
    }
#line 951 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 27:
#line 425 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[2].value.stringVector));
        driver.currentPattern = new ConcretePattern (t, Pattern::insn);
        delete (yystack_[2].value.stringVector);
    }
#line 961 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 28:
#line 430 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[2].value.stringVector));
        driver.currentPattern = new ConcretePattern (t, Pattern::exp);
        delete (yystack_[2].value.stringVector);
    }
#line 971 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 29:
#line 435 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[2].value.stringVector));
        driver.currentPattern = new ConcretePattern (t, Pattern::insnReservation);
        delete (yystack_[2].value.stringVector);
    }
#line 981 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 30:
#line 440 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[2].value.stringVector));
        driver.currentPattern = new ConcretePattern (t, Pattern::predicate);
        delete (yystack_[2].value.stringVector);
    }
#line 991 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 31:
#line 445 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[2].value.stringVector));
        driver.currentPattern = new ConcretePattern (t, Pattern::specialPredicate);
        delete (yystack_[2].value.stringVector);
    }
#line 1001 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 32:
#line 452 "srtl-parser.yy" // lalr1.cc:847
    {
        driver.currentPattern = new ConcretePattern(Pattern::peep2);
        driver.currentPattern->setPatName ("peep2_" + driver.itos (driver.getConstructIndex (driver.currentPattern->getPatType ())));
    }
#line 1010 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 33:
#line 456 "srtl-parser.yy" // lalr1.cc:847
    {
        driver.currentPattern = new ConcretePattern(Pattern::split);
        driver.currentPattern->setPatName ("split_" + driver.itos (driver.getConstructIndex (driver.currentPattern->getPatType ())));
    }
#line 1019 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 34:
#line 460 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[2].value.stringVector));
        driver.currentPattern = new ConcretePattern (t, Pattern::insnAndSplit);
        delete (yystack_[2].value.stringVector);
    }
#line 1029 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 35:
#line 467 "srtl-parser.yy" // lalr1.cc:847
    {
        /* Search Symbol Table for abstract patterns. */
        string t = driver.appendStrings ((yystack_[1].value.stringVector));
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
            delete (yystack_[1].value.stringVector);
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
                delete (yystack_[1].value.stringVector);
            } else {
                driver.currentPattern->setParentPattern (p);
                ConcretePattern* cp = static_cast <ConcretePattern*> (driver.currentPattern);
                cp->setSubType (Pattern::instantiates);
                delete (yystack_[1].value.stringVector);
            }
        }
    }
#line 1081 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 36:
#line 516 "srtl-parser.yy" // lalr1.cc:847
    {
        /* Search Symbol Table for abstract patterns. */
        string t = driver.appendStrings ((yystack_[2].value.stringVector));
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
                cp->setCmdBody (*(yystack_[0].value.string));
                cp->setSubType (Pattern::instantiates);
                if (driver.generateDotFile) {
                    driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
                    driver.dotFile << "\"" + driver.formatNodeName (t) + "\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
                    driver.dotFile << "\"" + driver.formatNodeName (t) + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [color=blue];\n";
                }
                driver.incrementAbstractPatternCount (t, driver.currentPattern->getPatType ());
                driver.addToStatTree (t, driver.currentPattern->getPatName (), 2);
            }
            delete (yystack_[2].value.stringVector);
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
                    cp->setCmdBody (*(yystack_[0].value.string));
                    if (driver.generateDotFile) {
                        driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
                        driver.dotFile << "\"" + driver.formatNodeName (t) + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [color=blue];\n";
                    }
                driver.incrementAbstractPatternCount (t, driver.currentPattern->getPatType ());
                driver.addToStatTree (t, driver.currentPattern->getPatName (), 2);
                }
                delete (yystack_[2].value.stringVector);
            } else {
                driver.currentPattern->setParentPattern (p);
                ConcretePattern* cp = static_cast <ConcretePattern*> (driver.currentPattern);
                cp->setCmdBody (*(yystack_[0].value.string));
                cp->setSubType (Pattern::instantiates);
                delete (yystack_[2].value.stringVector);
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
#line 1156 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 37:
#line 586 "srtl-parser.yy" // lalr1.cc:847
    {
        /* Search Symbol Table for Concrete Patterns. */
        string t = driver.appendStrings ((yystack_[4].value.stringVector));
        Pattern* p = driver.findSymTabEntry (t);
        if( p == NULL) {
        	driver.currentPattern->setError();
		} else {
            driver.currentPattern->setParentPattern (p);
            ConcretePattern* cp = static_cast <ConcretePattern*> (driver.currentPattern);
            cp->setCmdBody(*(yystack_[0].value.string));
            cp->setSubType (Pattern::overrides);
            if (driver.generateDotFile) {
                driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
                driver.dotFile << "\"" + driver.formatNodeName (t) + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\";\n";
            } 
            driver.addToStatTree (t, driver.currentPattern->getPatName (), 2);
            driver.incrementConcretePatternCountForOverrides (t);
        }
        delete (yystack_[4].value.stringVector); 
    }
#line 1181 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 38:
#line 606 "srtl-parser.yy" // lalr1.cc:847
    {
        /* Search Symbol Table for Concrete Patterns. */
        string t = driver.appendStrings ((yystack_[4].value.stringVector));
        Pattern* p = driver.findSymTabEntry (t);
        if( p == NULL) {
            driver.currentPattern->setError();
        } else {
            driver.currentPattern->setParentPattern (p);
            ConcretePattern* cp = static_cast <ConcretePattern*> (driver.currentPattern);
            cp->setCmdBody(*(yystack_[0].value.string));
            cp->setSubType (Pattern::overrides);
            if (driver.generateDotFile) {
                driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
                driver.dotFile << "\"" + driver.formatNodeName (t) + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\";\n";
            }
            driver.addToStatTree (t, driver.currentPattern->getPatName (), 2);
            driver.incrementConcretePatternCountForOverrides (t);
        }
        delete (yystack_[4].value.stringVector); 
    }
#line 1206 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 39:
#line 628 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[1].value.stringVector));
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
            delete (yystack_[1].value.stringVector);
        } else {
            driver.currentPattern->setParentPattern (p);
            ConcretePattern* cp = static_cast <ConcretePattern*> (driver.currentPattern);
            cp->setSubType (Pattern::instantiates);
            delete (yystack_[1].value.stringVector);
            if (driver.generateDotFile) {
                driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=orange];\n";
                driver.dotFile << "\"" + t + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\";\n";
            }   
            driver.incrementAbstractPatternCount (t, driver.currentPattern->getPatType ());
            driver.addToStatTree (t, driver.currentPattern->getPatName (), 2);
        }
    }
#line 1250 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 40:
#line 667 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[1].value.stringVector));
        Pattern* p = driver.findSymTabEntry (t);
        if (p == NULL) {
            driver.currentPattern->setError ();
        } else {
            driver.currentPattern->setParentPattern (p);
            ConcretePattern* cp = static_cast<ConcretePattern*> (driver.currentPattern);
            cp->setSubType (Pattern::overrides);
        }
        delete (yystack_[1].value.stringVector);
    }
#line 1267 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 41:
#line 681 "srtl-parser.yy" // lalr1.cc:847
    {
        ConcretePattern* cp = static_cast<ConcretePattern*> (driver.currentPattern);
        cp->setCmdBody (*(yystack_[0].value.string));
    }
#line 1276 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 42:
#line 685 "srtl-parser.yy" // lalr1.cc:847
    {

    }
#line 1284 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 43:
#line 690 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[5].value.stringVector));
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
                cp->setCmdBodyOut (*(yystack_[0].value.string));
                cp->setParentPatternOut (ap);
                if (driver.generateDotFile) {
                    driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=orange];\n";
                    driver.dotFile << "\"" + t + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\";\n";
                }   
//                driver.incrementAbstractPatternCount (t, driver.currentPattern->getPatType ());
            }
            delete (yystack_[5].value.stringVector);
        } else {
            ConcretePattern* cp = static_cast <ConcretePattern*> (driver.currentPattern);
            cp->setSubTypeOut (Pattern::instantiates);
            cp->setCmdBodyOut (*(yystack_[0].value.string));
            cp->setParentPatternOut (p);
            delete (yystack_[5].value.stringVector);
            if (driver.generateDotFile) {
                driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=orange];\n";
                driver.dotFile << "\"" + t + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\";\n";
            }   
            driver.incrementAbstractPatternCount (t, driver.currentPattern->getPatType ());
//            driver.addToStatTree (t, driver.currentPattern->getPatName (), 2);
        }
    }
#line 1329 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 44:
#line 730 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[1].value.stringVector));
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
            delete (yystack_[1].value.stringVector);
        } else {
            ConcretePattern* cp = static_cast <ConcretePattern*> (driver.currentPattern);
            cp->setSubTypeOut (Pattern::instantiates);
            cp->setCmdBodyOut ("");
            cp->setParentPatternOut (p);
            delete (yystack_[1].value.stringVector);
            if (driver.generateDotFile) {
                driver.dotFile << "\"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\" [shape=box,style=filled,color=orange];\n";
                driver.dotFile << "\"" + t + "\" -> \"" + driver.formatNodeName (driver.currentPattern->getPatName ()) + "\";\n";
            }   
            driver.incrementAbstractPatternCount (t, driver.currentPattern->getPatType ());
//           driver.addToStatTree (t, driver.currentPattern->getPatName (), 2);
        }
    }
#line 1375 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 45:
#line 771 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[5].value.stringVector));
        ConcretePattern* cp = static_cast<ConcretePattern*> (driver.currentPattern);
        cp->setCmdBodyOut (*(yystack_[0].value.string));
        cp->setSubTypeOut (Pattern::overrides);

        Pattern* p = driver.findSymTabEntry (t);
        if (p == NULL) {
            driver.currentPattern->setError ();
            } else {
            cp->setParentPatternOut (p);
        }
        delete (yystack_[0].value.string);
        delete (yystack_[5].value.stringVector);
    }
#line 1395 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 46:
#line 788 "srtl-parser.yy" // lalr1.cc:847
    {
    }
#line 1402 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 47:
#line 792 "srtl-parser.yy" // lalr1.cc:847
    {
    }
#line 1409 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 48:
#line 796 "srtl-parser.yy" // lalr1.cc:847
    {
        ConcretePattern* cp = static_cast<ConcretePattern*> (driver.currentPattern);
        cp->addOperands (*(yystack_[3].value.operandList));
        cp->addModeStmts (*(yystack_[0].value.modeStmtVector));
        delete (yystack_[3].value.operandList);
        delete (yystack_[0].value.modeStmtVector);
    }
#line 1421 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 49:
#line 803 "srtl-parser.yy" // lalr1.cc:847
    {
        ConcretePattern* cp = static_cast<ConcretePattern*> (driver.currentPattern);
        cp->addOperands (*(yystack_[2].value.operandList));
        delete (yystack_[2].value.operandList);
    }
#line 1431 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 50:
#line 808 "srtl-parser.yy" // lalr1.cc:847
    {
        ConcretePattern* cp = static_cast<ConcretePattern*> (driver.currentPattern);
        cp->addModeStmts (*(yystack_[0].value.modeStmtVector));
        delete (yystack_[0].value.modeStmtVector);
    }
#line 1441 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 51:
#line 815 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.stringVector)->push_back (*(yystack_[0].value.string));
    }
#line 1449 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 52:
#line 818 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.stringVector) =  new std::vector<std::string> ();
        (yylhs.value.stringVector)->push_back (*(yystack_[0].value.string));
    }
#line 1458 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 53:
#line 824 "srtl-parser.yy" // lalr1.cc:847
    {
        ConcretePattern* cp = static_cast<ConcretePattern*> (driver.currentPattern);
        cp->addModeStmts (*(yystack_[0].value.modeStmtVector));
        delete (yystack_[0].value.modeStmtVector);
    }
#line 1468 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 54:
#line 831 "srtl-parser.yy" // lalr1.cc:847
    {
    }
#line 1475 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 55:
#line 835 "srtl-parser.yy" // lalr1.cc:847
    {
    }
#line 1482 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 56:
#line 839 "srtl-parser.yy" // lalr1.cc:847
    {
        ConcretePattern* cp = static_cast<ConcretePattern*> (driver.currentPattern);
        cp->addOperandsOut (*(yystack_[3].value.operandList));
        cp->addModeStmtsOut (*(yystack_[0].value.modeStmtVector));
        delete (yystack_[3].value.operandList);
        delete (yystack_[0].value.modeStmtVector);
    }
#line 1494 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 57:
#line 846 "srtl-parser.yy" // lalr1.cc:847
    {
        ConcretePattern* cp = static_cast<ConcretePattern*> (driver.currentPattern);
        cp->addOperandsOut (*(yystack_[2].value.operandList));
        delete (yystack_[2].value.operandList);
    }
#line 1504 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 58:
#line 853 "srtl-parser.yy" // lalr1.cc:847
    {
        ConcretePattern* cp = static_cast<ConcretePattern*> (driver.currentPattern);
        cp->addModeStmtsOut (*(yystack_[0].value.modeStmtVector));
        delete (yystack_[0].value.modeStmtVector);
    }
#line 1514 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 59:
#line 860 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.modeStmtVector)->push_back (*(yystack_[0].value.modeStmt));
        delete (yystack_[0].value.modeStmt);
    }
#line 1523 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 60:
#line 864 "srtl-parser.yy" // lalr1.cc:847
    {
            vector<ModeStmt>* msv = new vector<ModeStmt> ();
            msv->push_back (*(yystack_[0].value.modeStmt));
            (yylhs.value.modeStmtVector) = msv;
            delete (yystack_[0].value.modeStmt);
        }
#line 1534 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 61:
#line 872 "srtl-parser.yy" // lalr1.cc:847
    {
        ModeStmt* ms = new ModeStmt ();
        ms->addAccessTree ((yystack_[3].value.intVector));
        ms->setPatternName (*(yystack_[1].value.string));
        ms->setType (ModeStmt::root);
        (yylhs.value.modeStmt) = ms;
        driver.findRtlOperandSymTabEntry (*(yystack_[1].value.string));
        delete (yystack_[1].value.string);
    }
#line 1548 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 62:
#line 883 "srtl-parser.yy" // lalr1.cc:847
    {
        (yystack_[3].value.modeStmt)->setMode (*(yystack_[1].value.string)); 
        if ((yystack_[3].value.modeStmt)->getType () == ModeStmt::predicate) {
            (yystack_[3].value.modeStmt)->setOperand (*(yystack_[1].value.string), "", "");
        } else if ((yystack_[3].value.modeStmt)->getType () == ModeStmt::constraint) {
            (yystack_[3].value.modeStmt)->setOperand ("", "", "\"" + *(yystack_[1].value.string) + "\"");
        }
        delete (yystack_[1].value.string);
        (yylhs.value.modeStmt) = (yystack_[3].value.modeStmt);
    }
#line 1563 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 63:
#line 893 "srtl-parser.yy" // lalr1.cc:847
    {
        (yystack_[3].value.modeStmt)->setMode (*(yystack_[1].value.string));
        if ((yystack_[3].value.modeStmt)->getType () == ModeStmt::constraint) {
            (yystack_[3].value.modeStmt)->setOperand ("", "", *(yystack_[1].value.string));
            delete (yystack_[1].value.string);
        }
        (yylhs.value.modeStmt) = (yystack_[3].value.modeStmt);
    }
#line 1576 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 64:
#line 901 "srtl-parser.yy" // lalr1.cc:847
    {
        (yystack_[5].value.modeStmt)->setMode ("<" + *(yystack_[2].value.string) + ">");
        if ((yystack_[5].value.modeStmt)->getType () == ModeStmt::predicate) {
        (yystack_[5].value.modeStmt)->setOperand ("<" + *(yystack_[2].value.string) + ">", "", "");
        } else if ((yystack_[5].value.modeStmt)->getType () == ModeStmt::constraint) {
        (yystack_[5].value.modeStmt)->setOperand ("", "", "\"<" + *(yystack_[2].value.string) + ">\"");
        }
        delete (yystack_[2].value.string);
        (yylhs.value.modeStmt) = (yystack_[5].value.modeStmt);
    }
#line 1591 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 65:
#line 911 "srtl-parser.yy" // lalr1.cc:847
    {
        if ((yystack_[5].value.modeStmt)->getType () == ModeStmt::operand) {
            Operand* o = new Operand (Operand::constSpec, srtl_driver::stoi(*(yystack_[1].value.string)));
            (yystack_[5].value.modeStmt)->setOperand (*o);
            delete (yystack_[1].value.string);
        }
        (yylhs.value.modeStmt) = (yystack_[5].value.modeStmt);
    }
#line 1604 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 66:
#line 919 "srtl-parser.yy" // lalr1.cc:847
    {
        if ((yystack_[8].value.modeStmt)->getType () == ModeStmt::operand) {
            Operand* o = new Operand (Operand::reg, *(yystack_[6].value.string), *(yystack_[4].value.string), *(yystack_[2].value.string));
            (yystack_[8].value.modeStmt)->setOperand (*o);
            delete (yystack_[6].value.string);delete (yystack_[4].value.string); delete (yystack_[2].value.string);
            (yylhs.value.modeStmt) = (yystack_[8].value.modeStmt);
        }
    }
#line 1617 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 67:
#line 927 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[5].value.stringVector));
        string m = driver.appendStrings ((yystack_[3].value.stringVector));

        ModeStmt* ms = new ModeStmt ();
        ms->setType (ModeStmt::pmc);
        ms->addAccessTree ((yystack_[7].value.intVector)); 
        ms->setOperand (t, m, *(yystack_[1].value.string));
        delete (yystack_[5].value.stringVector); delete (yystack_[3].value.stringVector); delete (yystack_[1].value.string);
        (yylhs.value.modeStmt) = ms;
    }
#line 1633 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 68:
#line 938 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[5].value.stringVector));
        string m = driver.appendStrings ((yystack_[3].value.stringVector));

        ModeStmt* ms = new ModeStmt ();
        ms->setType (ModeStmt::pmc);
        ms->addAccessTree ((yystack_[9].value.intVector)); 
        ms->setOperand (srtl_driver::stoi(*(yystack_[7].value.string)), t, m, *(yystack_[1].value.string));
        delete (yystack_[7].value.string); delete (yystack_[5].value.stringVector); delete (yystack_[3].value.stringVector); delete (yystack_[1].value.string);
        (yylhs.value.modeStmt) = ms;
    }
#line 1649 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 69:
#line 949 "srtl-parser.yy" // lalr1.cc:847
    {
        ModeStmt* ms = new ModeStmt ();
        ms->setType (ModeStmt::latency);
        ms->setMode (*(yystack_[1].value.string));
        delete (yystack_[1].value.string);
        (yylhs.value.modeStmt) = ms;
    }
#line 1661 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 70:
#line 956 "srtl-parser.yy" // lalr1.cc:847
    {
        ModeStmt* ms = new ModeStmt ();
        ms->setType (ModeStmt::listOfValues);
        ms->setMode (*(yystack_[1].value.string));
        delete (yystack_[1].value.string);
        (yylhs.value.modeStmt) = ms;
    }
#line 1673 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 71:
#line 963 "srtl-parser.yy" // lalr1.cc:847
    {
        ModeStmt* ms = new ModeStmt ();
        ms->setType (ModeStmt::docstring);
        ms->setMode (*(yystack_[1].value.string));
        delete (yystack_[1].value.string);
        (yylhs.value.modeStmt) = ms;
    }
#line 1685 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 72:
#line 970 "srtl-parser.yy" // lalr1.cc:847
    {
        ModeStmt* ms = new ModeStmt ();
        ms->setType (ModeStmt::regclass);
        ms->setMode (*(yystack_[1].value.string));
        delete (yystack_[1].value.string);
        (yylhs.value.modeStmt) = ms;
    }
#line 1697 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 73:
#line 977 "srtl-parser.yy" // lalr1.cc:847
    {
        ModeStmt* ms = new ModeStmt ();
        ms->setType (ModeStmt::allConstraints);
        ms->setConstraints ((yystack_[2].value.stringVector));
        (yylhs.value.modeStmt) = ms;
    }
#line 1708 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 74:
#line 983 "srtl-parser.yy" // lalr1.cc:847
    {
        ModeStmt* ms = new ModeStmt ();
        ms->setType (ModeStmt::replaceMode);
        ms->setMode (*(yystack_[4].value.string));
        ms->setModeReplacement (*(yystack_[1].value.string));
        (yylhs.value.modeStmt) = ms;
        delete (yystack_[4].value.string);
        delete (yystack_[1].value.string);
    }
#line 1722 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 75:
#line 994 "srtl-parser.yy" // lalr1.cc:847
    {   /* Change the attribute of a node */
        /*This should be mode. */
        /* Check again whether this is required ?*/
        delete (yystack_[0].value.string);
    }
#line 1732 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 76:
#line 999 "srtl-parser.yy" // lalr1.cc:847
    {
        ModeStmt* ms = new ModeStmt ();
        ms->addAccessTree ((yystack_[2].value.intVector));
        if (*(yystack_[0].value.string) == "mode") {
            ms->setType (ModeStmt::mode);
            (yylhs.value.modeStmt) = ms;
        } else if (*(yystack_[0].value.string) == "constraint") {
            ms->setType (ModeStmt::constraint);
            (yylhs.value.modeStmt) = ms;
        } else if (*(yystack_[0].value.string) == "operand") {
            ms->setType (ModeStmt::operand);
            (yylhs.value.modeStmt) = ms;
        } else {
            // TODO report error.
        }
        delete (yystack_[0].value.string);
    }
#line 1754 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 77:
#line 1016 "srtl-parser.yy" // lalr1.cc:847
    {
        ModeStmt* ms = new ModeStmt ();
        ms->addAccessTree ((yystack_[2].value.intVector));
        ms->setType (ModeStmt::predicate);
        (yylhs.value.modeStmt) = ms;
    }
#line 1765 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 78:
#line 1024 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.intVector)->push_back(driver.stoi (*(yystack_[0].value.string)));
        delete (yystack_[0].value.string);
    }
#line 1774 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 79:
#line 1029 "srtl-parser.yy" // lalr1.cc:847
    {
        vector<int>* intVector = new vector<int>();
        (yylhs.value.intVector) = intVector;
        (yylhs.value.intVector)->push_back(driver.stoi (*(yystack_[0].value.string)));
        delete (yystack_[0].value.string);
    }
#line 1785 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 80:
#line 1037 "srtl-parser.yy" // lalr1.cc:847
    {
        vector <Operand>* oplist = (yystack_[1].value.operandList);
        Operand* start = new Operand (Operand::bracketOpen, "");
        Operand* end = new Operand (Operand::bracketClose, "");
        (yystack_[1].value.operandList)->insert ((yystack_[1].value.operandList)->begin (), *start);
        (yystack_[1].value.operandList)->push_back (*end);

        for (unsigned int i = 0; i < oplist->size (); i++) {
            (yylhs.value.operandList)->push_back ((yystack_[1].value.operandList)->at(i));
        }

           delete oplist;
        }
#line 1803 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 81:
#line 1050 "srtl-parser.yy" // lalr1.cc:847
    {
        Operand* start = new Operand (Operand::bracketOpen, "");
        Operand* end = new Operand (Operand::bracketClose, "");
        (yystack_[1].value.operandList)->insert ((yystack_[1].value.operandList)->begin (), *start);
        (yystack_[1].value.operandList)->push_back (*end);
        (yylhs.value.operandList) = (yystack_[1].value.operandList);
    }
#line 1815 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 82:
#line 1057 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.operandList)->push_back (*(yystack_[0].value.operand));
        delete (yystack_[0].value.operand);
    }
#line 1824 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 83:
#line 1061 "srtl-parser.yy" // lalr1.cc:847
    {
        vector<Operand>* oplist = new vector<Operand>();
        (yylhs.value.operandList) = oplist;
        (yylhs.value.operandList)->push_back (*(yystack_[0].value.operand));
        delete (yystack_[0].value.operand);
    }
#line 1835 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 84:
#line 1069 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[4].value.stringVector));
        string m = driver.appendStrings ((yystack_[2].value.stringVector));
        
        Operand* o = new Operand (Operand::pmc, t, m, *(yystack_[0].value.string));
        (yylhs.value.operand) = o;
        driver.debug (srtl_driver::grammar, "Operand: PML:PML:C ::" + t + ":" + m + ":" + *(yystack_[0].value.string));
        delete (yystack_[2].value.stringVector); delete (yystack_[0].value.string); delete (yystack_[4].value.stringVector);
    }
#line 1849 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 85:
#line 1078 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[4].value.stringVector));
        string m = driver.appendStrings ((yystack_[2].value.stringVector));
        Operand* o = new Operand (Operand::pmc, driver.stoi (*(yystack_[6].value.string)), t, m, *(yystack_[0].value.string));
        (yylhs.value.operand) = o;
        driver.debug (srtl_driver::grammar, "Operand: INT:PML:PML:C ::" + *(yystack_[6].value.string) +"=" + t + ":"+m+":"+*(yystack_[0].value.string));
        delete (yystack_[6].value.string);
        delete (yystack_[4].value.stringVector);
        delete (yystack_[2].value.stringVector);
        delete (yystack_[0].value.string);
    }
#line 1865 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 86:
#line 1089 "srtl-parser.yy" // lalr1.cc:847
    {
        Operand* o = new Operand (Operand::dup, driver.stoi (*(yystack_[0].value.string)));
        driver.debug (srtl_driver::grammar, "Operand: Duplicate INT ::" + *(yystack_[0].value.string));
        (yylhs.value.operand) = o;
        delete (yystack_[0].value.string);
    }
#line 1876 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 87:
#line 1095 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[2].value.stringVector));
        if (driver.currentPattern->getPatType () == Pattern::cons || 
                driver.currentPattern->getPatType () == Pattern::regCons ||
                driver.currentPattern->getPatType () == Pattern::memCons ||
                driver.currentPattern->getPatType () == Pattern::addCons || 
                driver.currentPattern->getPatType () == Pattern::attr ||
                driver.currentPattern->getPatType () == Pattern::asmAttr ||
                driver.currentPattern->getPatType () == Pattern::insnReservation) {
            Operand* o = new Operand (Operand::eqAttr, t, *(yystack_[0].value.string));
            driver.debug (srtl_driver::grammar, "Operand: PM:C :: " + t + ":" + *(yystack_[0].value.string));
            delete (yystack_[2].value.stringVector); delete (yystack_[0].value.string);
            (yylhs.value.operand) = o;
        } else {
            Operand* o = new Operand (Operand::scratch, t, *(yystack_[0].value.string));
            driver.debug (srtl_driver::grammar, "Operand: PM:C ::" + t + ":" + *(yystack_[0].value.string));
            delete (yystack_[2].value.stringVector); delete (yystack_[0].value.string);
            (yylhs.value.operand) = o;
        }
    }
#line 1901 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 88:
#line 1115 "srtl-parser.yy" // lalr1.cc:847
    {
        Operand* o = new Operand (Operand::eqAttr, "attr", *(yystack_[0].value.string));
        driver.debug (srtl_driver::grammar, "Operand: A:C :: attr:" + *(yystack_[0].value.string));
        delete (yystack_[0].value.string);
        (yylhs.value.operand) = o;
    }
#line 1912 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 89:
#line 1121 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[2].value.stringVector));
        Operand* o = new Operand (Operand::scratch, driver.stoi (*(yystack_[4].value.string)), t, *(yystack_[0].value.string));
        driver.debug (srtl_driver::grammar, "Operand: INT:PML:C ::" + *(yystack_[4].value.string) + "=" + t + ":" + *(yystack_[0].value.string));
        delete (yystack_[4].value.string); delete (yystack_[2].value.stringVector); delete (yystack_[0].value.string);
        (yylhs.value.operand) = o;
    }
#line 1924 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 90:
#line 1128 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[0].value.stringVector));
        Operand* o = new Operand (Operand::matchOperator, driver.stoi (*(yystack_[2].value.string)), t);
        (yylhs.value.operand) = o;
            driver.debug (srtl_driver::grammar, "Operand: INT:PML ::" + *(yystack_[2].value.string) + "=" + t);
        delete (yystack_[2].value.string); delete (yystack_[0].value.stringVector);
    }
#line 1936 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 91:
#line 1135 "srtl-parser.yy" // lalr1.cc:847
    {
        Operand* o = new Operand (Operand::num, driver.stoi (*(yystack_[0].value.string)));
        (yylhs.value.operand) = o;
        driver.debug (srtl_driver::grammar, "Operand: INT" + *(yystack_[0].value.string));
        delete (yystack_[0].value.string);
    }
#line 1947 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 92:
#line 1141 "srtl-parser.yy" // lalr1.cc:847
    {
        /* For register, memory, constant etc. operands */
        (yylhs.value.operand) = (yystack_[0].value.operand);
        driver.debug (srtl_driver::grammar, "Operand: Fixed_Reg_Or_Const");
    }
#line 1957 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 93:
#line 1148 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[3].value.stringVector));
        Operand* o = new Operand (Operand::reg, *(yystack_[5].value.string), t, *(yystack_[1].value.string));
        (yylhs.value.operand) = o;
        driver.debug (srtl_driver::grammar, "Fixed_Reg_Or_Const: ID(PM:C) ::" + *(yystack_[5].value.string) + "(" + t + ":" + *(yystack_[1].value.string) + ")");
        delete (yystack_[5].value.string);
        delete (yystack_[3].value.stringVector);
        delete (yystack_[1].value.string);
    }
#line 1971 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 94:
#line 1157 "srtl-parser.yy" // lalr1.cc:847
    {
        string t = driver.appendStrings ((yystack_[3].value.stringVector));
        Operand * o = new Operand (Operand::regNum, *(yystack_[5].value.string), t, *(yystack_[1].value.string));
        (yylhs.value.operand) = o;
        driver.debug (srtl_driver::grammar, "Fixed_Reg_Or_Const: ID(PM:INT) ::"  + *(yystack_[5].value.string) + "(" + t + ":" + *(yystack_[1].value.string) + ")");
        delete (yystack_[5].value.string); delete (yystack_[3].value.stringVector); delete (yystack_[1].value.string);
    }
#line 1983 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 95:
#line 1164 "srtl-parser.yy" // lalr1.cc:847
    {
        Operand* o = new Operand (Operand::fixedReg, *(yystack_[0].value.string));
        (yylhs.value.operand) = o;
        driver.debug (srtl_driver::grammar, "Fixed_Reg_Or_Const: FIXED_REG ::" + *(yystack_[0].value.string));
        delete (yystack_[0].value.string);
    }
#line 1994 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 96:
#line 1170 "srtl-parser.yy" // lalr1.cc:847
    {
        Operand* o = new Operand (Operand::extraReg, *(yystack_[0].value.string));
        (yylhs.value.operand) = o;
        driver.debug (srtl_driver::grammar, "Fixed_Reg_Or_Const: ID ::" + *(yystack_[0].value.string));
        delete (yystack_[0].value.string);
    }
#line 2005 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 97:
#line 1176 "srtl-parser.yy" // lalr1.cc:847
    {
        Operand* o = new Operand (Operand::constSpec, driver.stoi (*(yystack_[0].value.string)));
        (yylhs.value.operand) = o;
        driver.debug (srtl_driver::grammar, "Fixed_Reg_Or_Const: CONST_INT:INT ::" + *(yystack_[0].value.string));
        delete (yystack_[0].value.string);
    }
#line 2016 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 98:
#line 1182 "srtl-parser.yy" // lalr1.cc:847
    {
        Operand* o = new Operand (Operand::constSpec, driver.stoi (*(yystack_[2].value.string)), *(yystack_[0].value.string));
        (yylhs.value.operand) = o;
        driver.debug (srtl_driver::grammar, "Fixed_Reg_Or_Const: CONST_INT:INT:ID ::" + *(yystack_[2].value.string) + ":" + *(yystack_[0].value.string));
        delete (yystack_[2].value.string); delete  (yystack_[0].value.string);
    }
#line 2027 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 99:
#line 1188 "srtl-parser.yy" // lalr1.cc:847
    {
        Operand* o = new Operand (Operand::integer, driver.stoi (*(yystack_[1].value.string)));
        (yylhs.value.operand) = o;
        driver.debug (srtl_driver::grammar, "Fixed_Reg_Or_Const: <INT> ::" + *(yystack_[1].value.string));
        delete (yystack_[1].value.string);
    }
#line 2038 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 100:
#line 1194 "srtl-parser.yy" // lalr1.cc:847
    {
        Operand* o = new Operand (Operand::string, *(yystack_[1].value.string));
        (yylhs.value.operand) = o;
        driver.debug (srtl_driver::grammar, "Fixed_Reg_Or_Const: <ID> ::" + *(yystack_[1].value.string));
        delete (yystack_[1].value.string);
    }
#line 2049 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 101:
#line 1202 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.string) = (yystack_[0].value.string);
        driver.debug (srtl_driver::grammar, "Constraint: " + *(yystack_[0].value.string));
    }
#line 2058 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 102:
#line 1208 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.stringVector)->push_back (*(yystack_[0].value.string));
        driver.debug (srtl_driver::grammar, "Predicate_Or_Mode_List: PML PM ::" + *(yystack_[0].value.string));
        delete (yystack_[0].value.string);
    }
#line 2068 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 103:
#line 1213 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.stringVector)->push_back (*(yystack_[0].value.string));
        driver.debug (srtl_driver::grammar, "Predicate_Or_Mode_List: PML PM ::" + *(yystack_[0].value.string));
        delete (yystack_[0].value.string);
    }
#line 2078 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 104:
#line 1218 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.stringVector) = new vector<string>();
        (yylhs.value.stringVector)->push_back (*(yystack_[0].value.string));
        driver.debug (srtl_driver::grammar, "Predicate_Or_Mode_List: PMWB ::" + *(yystack_[0].value.string));
        delete (yystack_[0].value.string);
    }
#line 2089 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 105:
#line 1224 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.stringVector) = new vector<string> ();
        (yylhs.value.stringVector)->push_back (*(yystack_[0].value.string));
        driver.debug (srtl_driver::grammar, "Predicate_Or_Mode_List: PM ::" + *(yystack_[0].value.string));
        delete (yystack_[0].value.string);
    }
#line 2100 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 106:
#line 1232 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.string) = new string ("<" + *(yystack_[1].value.string) + ">");
        driver.debug (srtl_driver::grammar, "Predicate_Or_Mode_With_Brackets: <PM> ::<" + *(yystack_[1].value.string) + ">");
        delete (yystack_[1].value.string);
    }
#line 2110 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 107:
#line 1239 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.string) = (yystack_[0].value.string);
        driver.debug (srtl_driver::grammar, "Predicate_Or_Mode: INT ::" + *(yystack_[0].value.string));
    }
#line 2119 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 108:
#line 1243 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.string) = (yystack_[0].value.string);
        driver.debug (srtl_driver::grammar, "Predicate_Or_Mode: ID::" + *(yystack_[0].value.string));
    }
#line 2128 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 109:
#line 1250 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.string) = (yystack_[0].value.string);
    }
#line 2136 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 110:
#line 1255 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.stringVector)->push_back (*(yystack_[0].value.string));
        delete (yystack_[0].value.string);
        driver.debug (srtl_driver::grammar, "Concrete_Pattern_Name_List: Concrete_Pattern_Name_List Concrete_Pattern_Name_With_Brackets");
    }
#line 2146 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 111:
#line 1260 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.stringVector)->push_back (*(yystack_[0].value.string));
        delete (yystack_[0].value.string);
        driver.debug (srtl_driver::grammar, "Concrete_Pattern_Name_List: Concrete_Pattern_Name_List Concrete_Pattern_Name");
    }
#line 2156 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 112:
#line 1265 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.stringVector) = new vector<string> ();
        (yylhs.value.stringVector)->push_back (*(yystack_[0].value.string));
        delete (yystack_[0].value.string);
        driver.debug (srtl_driver::grammar, "Concrete_Pattern_Name_List: Concrete_Pattern_Name_With_Brackets");

    }
#line 2168 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 113:
#line 1272 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.stringVector) = new vector<string> ();
        (yylhs.value.stringVector)->push_back (*(yystack_[0].value.string));
        delete (yystack_[0].value.string);
        driver.debug (srtl_driver::grammar, "Concrete_Pattern_Name_List: Concrete_Pattern_Name_List");
    }
#line 2179 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 114:
#line 1280 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.string) = new string ("<" + *(yystack_[1].value.string) + ">");
        driver.debug (srtl_driver::grammar, "Concrete_Pattern_Name_With_Brackets: " + *(yylhs.value.string));
        delete (yystack_[1].value.string);
    }
#line 2189 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 115:
#line 1285 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.string) = new string ("<" + *(yystack_[3].value.string) + ":" + *(yystack_[1].value.string) + ">");
        driver.debug (srtl_driver::grammar, "Concrete_Pattern_Name_With_Brackets: " + *(yylhs.value.string));
        delete (yystack_[3].value.string); delete (yystack_[1].value.string);
    }
#line 2199 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 116:
#line 1292 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.string) = (yystack_[0].value.string);
        driver.debug (srtl_driver::grammar, "Concrete_Pattern_Name: ID: " + *(yystack_[0].value.string));
    }
#line 2208 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 117:
#line 1296 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.string) = new string (*(yystack_[0].value.string));
        delete (yystack_[0].value.string);
        driver.debug (srtl_driver::grammar, "Concrete_Patttern_Name: INT");
    }
#line 2218 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 118:
#line 1301 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.string) = new string ("*");
        driver.debug (srtl_driver::grammar, "Concrete_Pattern_Name: *");
    }
#line 2227 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 119:
#line 1305 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.string) = new string (":");
        driver.debug (srtl_driver::grammar, "Concrete_Pattern_Name: :");
    }
#line 2236 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 120:
#line 1309 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.string) = new string ("_");
        driver.debug (srtl_driver::grammar, "ConcretePatternName: _");
    }
#line 2245 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 121:
#line 1315 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.string) = (yystack_[0].value.string);
    }
#line 2253 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 122:
#line 1320 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.string) = (yystack_[0].value.string);
    }
#line 2261 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 123:
#line 1326 "srtl-parser.yy" // lalr1.cc:847
    {
        ListPattern * l =new ListPattern();
        string sq ="\"", eq="\"", p;
        p= *(yystack_[2].value.string);
        l->setPatType(Pattern::define_automaton);
        l->setPatName(p);
        (yylhs.value.L)=l;	
    }
#line 2274 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 124:
#line 1334 "srtl-parser.yy" // lalr1.cc:847
    {
        ListPattern * l=new ListPattern();
        l->setPatName(*(yystack_[5].value.string));
        delete (yystack_[5].value.string);;
        l->setPatType(Pattern::define_c_enum);
        l->append((yystack_[1].value.EL));
        (yylhs.value.L)=l;
    }
#line 2287 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 125:
#line 1342 "srtl-parser.yy" // lalr1.cc:847
    {
    	ListPattern * l =new ListPattern();
    	l->setPatType(Pattern::define_bypass);
    	l->setPatName(*(yystack_[3].value.string));
    	delete (yystack_[3].value.string);
    	l->append((yystack_[0].value.string));
		(yylhs.value.L)=l;
        }
#line 2300 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 126:
#line 1350 "srtl-parser.yy" // lalr1.cc:847
    {
    	ListPattern * l =new ListPattern();
        l->setPatType(Pattern::define_cpu_unit);
        l->setPatName(*(yystack_[3].value.string));
        delete (yystack_[3].value.string);
        l->append((yystack_[0].value.string));
    	(yylhs.value.L)=l;
    }
#line 2313 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 127:
#line 1358 "srtl-parser.yy" // lalr1.cc:847
    {
        ListPattern * l =new ListPattern();
        string sq ="\"", eq="\"",* p;
        l->setPatType(Pattern::define_reservation);
        l->setPatName(*(yystack_[3].value.string));
        delete (yystack_[3].value.string);
        p= new string (*(yystack_[0].value.string));
        l->append(p);
        (yylhs.value.L)=l;
    }
#line 2328 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 128:
#line 1368 "srtl-parser.yy" // lalr1.cc:847
    {
        ListPattern * l =new ListPattern();
        string sq ="\"", eq="\"",* p;
        l->setPatType(Pattern::define_reservation);
        l->setPatName(*(yystack_[5].value.string));
        delete (yystack_[5].value.string);
        p= new string (sq + *(yystack_[1].value.string) +eq);
        l->append(p);
        (yylhs.value.L)=l;
    }
#line 2343 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 129:
#line 1378 "srtl-parser.yy" // lalr1.cc:847
    {
        ListPattern * l =new ListPattern();
        l->setPatType(Pattern::define_constants);
        l->append((yystack_[1].value.EL));
        (yylhs.value.L)=l;
    }
#line 2354 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 130:
#line 1384 "srtl-parser.yy" // lalr1.cc:847
    {
        ListPattern * l =new ListPattern();
	    l->setPatType(Pattern::define_asm_attributes);
	    l->append((yystack_[1].value.EL));
	    (yylhs.value.L)=l;
    }
#line 2365 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 131:
#line 1390 "srtl-parser.yy" // lalr1.cc:847
    {
        ListPattern * l=new ListPattern();
        l->setPatName(*(yystack_[5].value.string));
        l->setPatType((yystack_[3].value.LP));
        l->append((yystack_[1].value.EL));
        (yylhs.value.L)=l;
        delete (yystack_[5].value.string);
    }
#line 2378 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 132:
#line 1398 "srtl-parser.yy" // lalr1.cc:847
    {
    	 ListPattern * l=new ListPattern();
    	 l->setPatName(*(yystack_[5].value.string));
    	 l->setPatType((yystack_[3].value.LP));
    	 l->append((yystack_[1].value.EL));
    	 (yylhs.value.L)=l;
    	 delete (yystack_[5].value.string);
    }
#line 2391 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 133:
#line 1408 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.LP)=Pattern::define_code_iterator;
    }
#line 2399 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 134:
#line 1411 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.LP)=Pattern::define_code_iterator;
    }
#line 2407 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 135:
#line 1416 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.LP)=Pattern::define_code_attr;
    }
#line 2415 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 136:
#line 1419 "srtl-parser.yy" // lalr1.cc:847
    {
        (yylhs.value.LP)=Pattern::define_mode_attr;
    }
#line 2423 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 137:
#line 1424 "srtl-parser.yy" // lalr1.cc:847
    {
	   ElemList * el=new ElemList((yystack_[2].value.EL),(yystack_[0].value.E));
       (yylhs.value.EL)=el;	
    }
#line 2432 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 138:
#line 1428 "srtl-parser.yy" // lalr1.cc:847
    {
        ElemList * el=new ElemList((yystack_[0].value.E));
        (yylhs.value.EL)=el;
    }
#line 2441 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 139:
#line 1434 "srtl-parser.yy" // lalr1.cc:847
    {
        Element *e;
		string q = "\"";
		string *s = new string (*(yystack_[1].value.string));
		e=new Element((yystack_[3].value.string),s);
		delete (yystack_[1].value.string);
        (yylhs.value.E)=e;
	}
#line 2454 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 140:
#line 1445 "srtl-parser.yy" // lalr1.cc:847
    {
		ElemList * el = new ElemList((yystack_[4].value.EL),(yystack_[1].value.E));
		(yylhs.value.EL) =el;
                }
#line 2463 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 141:
#line 1450 "srtl-parser.yy" // lalr1.cc:847
    {
               	ElemList * el = new ElemList((yystack_[1].value.E));
		(yylhs.value.EL) = el;
		}
#line 2472 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 142:
#line 1455 "srtl-parser.yy" // lalr1.cc:847
    {
		ElemList * el = new ElemList((yystack_[2].value.EL),(yystack_[0].value.E));
		(yylhs.value.EL) =el;
    }
#line 2481 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 143:
#line 1459 "srtl-parser.yy" // lalr1.cc:847
    {
		ElemList * el = new ElemList((yystack_[0].value.E));
		(yylhs.value.EL) = el;
	}
#line 2490 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 144:
#line 1463 "srtl-parser.yy" // lalr1.cc:847
    {
    	ElemList * el = new ElemList((yystack_[4].value.EL),(yystack_[1].value.E));
    	(yylhs.value.EL) =el;
    }
#line 2499 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 145:
#line 1467 "srtl-parser.yy" // lalr1.cc:847
    {
    	ElemList * el = new ElemList((yystack_[1].value.E));
    	(yylhs.value.EL) = el;
	}
#line 2508 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 146:
#line 1473 "srtl-parser.yy" // lalr1.cc:847
    {
    	ElemList * el = new ElemList((yystack_[4].value.EL),(yystack_[1].value.E));
    	(yylhs.value.EL) =el;
    }
#line 2517 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 147:
#line 1477 "srtl-parser.yy" // lalr1.cc:847
    {
		ElemList * el = new ElemList((yystack_[2].value.EL),(yystack_[0].value.E));
		(yylhs.value.EL) = el;
	}
#line 2526 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 148:
#line 1481 "srtl-parser.yy" // lalr1.cc:847
    {
		ElemList * el = new ElemList((yystack_[1].value.E));
		(yylhs.value.EL) = el;
    }
#line 2535 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 149:
#line 1485 "srtl-parser.yy" // lalr1.cc:847
    {
		ElemList * el = new ElemList((yystack_[0].value.E));
		(yylhs.value.EL) = el;
    }
#line 2544 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 150:
#line 1491 "srtl-parser.yy" // lalr1.cc:847
    {
    Element * e;
    e= new Element((yystack_[2].value.string),(yystack_[0].value.string));
    (yylhs.value.E) =e;
    }
#line 2554 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 151:
#line 1498 "srtl-parser.yy" // lalr1.cc:847
    {
        Element * e;
        string *s = new string( *(yystack_[0].value.string)),* t= new string(*(yystack_[2].value.string));
        delete (yystack_[0].value.string);delete (yystack_[2].value.string);
        e= new Element(t,s);
        (yylhs.value.E)=e;
    }
#line 2566 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 152:
#line 1505 "srtl-parser.yy" // lalr1.cc:847
    { 
        Element * e;
        ostringstream temp;
        temp<<*(yystack_[0].value.string);
        string *t=new string  (temp.str()); 
        e = new Element((yystack_[2].value.string),t);
        (yylhs.value.E)=e; 
        delete (yystack_[0].value.string);
    }
#line 2580 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 153:
#line 1515 "srtl-parser.yy" // lalr1.cc:847
    {
        Element * e;
        string *s = new string( *(yystack_[1].value.string)+*(yystack_[0].value.string)),* t= new string(*(yystack_[3].value.string));
        delete (yystack_[1].value.string);delete (yystack_[3].value.string);
        e= new Element(t,s);
        (yylhs.value.E)=e;
    }
#line 2592 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 154:
#line 1524 "srtl-parser.yy" // lalr1.cc:847
    {
	   (yylhs.value.string) = (yystack_[0].value.string);
	}
#line 2600 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 155:
#line 1527 "srtl-parser.yy" // lalr1.cc:847
    { 
	   (yylhs.value.string) = (yystack_[0].value.string);
	}
#line 2608 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 156:
#line 1532 "srtl-parser.yy" // lalr1.cc:847
    {
        string *s;
        s=new string(*(yystack_[0].value.string));
        (yylhs.value.string)=s;
        delete (yystack_[0].value.string);
    }
#line 2619 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 157:
#line 1538 "srtl-parser.yy" // lalr1.cc:847
    {
		string *s;
		s= new string (*(yystack_[1].value.string));
		*s=*s+ *(yystack_[0].value.string);
		(yylhs.value.string)=s;	
	}
#line 2630 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 158:
#line 1546 "srtl-parser.yy" // lalr1.cc:847
    {
        Element * e;
        string *s=new string(*(yystack_[0].value.string));
        e= new Element(s) ;
        (yylhs.value.E)=e;
        delete (yystack_[0].value.string);
    }
#line 2642 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 159:
#line 1555 "srtl-parser.yy" // lalr1.cc:847
    {
		string *s;
		s= new string (*(yystack_[2].value.string));
		*s=*s+ "|"+*(yystack_[0].value.string);
		(yylhs.value.string)=s;	
		delete (yystack_[0].value.string); 
		delete (yystack_[2].value.string);
	}
#line 2655 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 160:
#line 1563 "srtl-parser.yy" // lalr1.cc:847
    {
		string *s;
		s= new string (*(yystack_[1].value.string));
		*s= "("+ *s+ ")";
		(yylhs.value.string)=s;	
		delete (yystack_[1].value.string);
	}
#line 2667 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 161:
#line 1570 "srtl-parser.yy" // lalr1.cc:847
    {
		string *s;
		s= new string (*(yystack_[2].value.string));
		*s=*s+ "+"+*(yystack_[0].value.string);
		(yylhs.value.string)=s;	
		delete (yystack_[2].value.string);
		delete (yystack_[0].value.string);
    }
#line 2680 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 162:
#line 1578 "srtl-parser.yy" // lalr1.cc:847
    {
        string *s;
        s=new string(*(yystack_[0].value.string));
        (yylhs.value.string)=s;
        delete (yystack_[0].value.string);
    }
#line 2691 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 163:
#line 1584 "srtl-parser.yy" // lalr1.cc:847
    {
        string *s;
        s=new string(*(yystack_[0].value.string));
        (yylhs.value.string)=s;
        delete (yystack_[0].value.string);
    }
#line 2702 "srtl-parser.tab.cc" // lalr1.cc:847
    break;

  case 164:
#line 1590 "srtl-parser.yy" // lalr1.cc:847
    {
		string *s;
		s= new string (*(yystack_[2].value.string));
		*s=*s+ ","+*(yystack_[0].value.string);
		(yylhs.value.string)=s;	
		delete (yystack_[0].value.string);
		delete (yystack_[2].value.string);
	}
#line 2715 "srtl-parser.tab.cc" // lalr1.cc:847
    break;


#line 2719 "srtl-parser.tab.cc" // lalr1.cc:847
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state,
                                           yyempty ? yyempty_ : yyla.type_get ()));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyempty)
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyempty = true;
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyempty)
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyempty)
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  srtl_parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  srtl_parser::yysyntax_error_ (state_type yystate, symbol_number_type yytoken) const
  {
    std::string yyres;
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yytoken) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (yytoken != yyempty_)
      {
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short int srtl_parser::yypact_ninf_ = -257;

  const signed char srtl_parser::yytable_ninf_ = -97;

  const short int
  srtl_parser::yypact_[] =
  {
     162,   -14,    66,   -16,  -257,   106,  -257,   135,  -257,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,  -257,   108,   173,  -257,
      60,    11,    23,   127,  -257,  -257,  -257,   120,    41,    96,
     104,   117,  -257,  -257,  -257,  -257,    76,    85,  -257,    85,
      85,  -257,   147,   171,  -257,   324,  -257,  -257,   128,   128,
    -257,    14,   241,   -21,   172,  -257,   173,   130,   151,   130,
     230,   231,  -257,  -257,  -257,  -257,  -257,  -257,  -257,  -257,
     183,  -257,  -257,  -257,  -257,  -257,   218,   232,  -257,  -257,
    -257,  -257,   250,   261,  -257,   209,     4,   280,   293,   277,
     223,  -257,   215,   316,    85,    85,   269,   295,   311,   312,
     313,    26,   203,  -257,  -257,    37,    26,  -257,   319,  -257,
     178,   325,   326,   327,   328,   329,   314,   322,   269,  -257,
     323,   323,  -257,   130,   130,   321,    17,   381,   333,   352,
      -3,  -257,    -7,    20,  -257,   335,    30,  -257,   335,    33,
    -257,   124,  -257,    37,   320,    47,  -257,   268,   205,   338,
      86,   337,   336,   336,   336,   331,   330,  -257,  -257,   269,
     362,   363,  -257,   365,  -257,  -257,   339,    13,  -257,  -257,
     340,   341,  -257,   312,  -257,  -257,   342,   343,  -257,   345,
     344,   347,  -257,   216,   348,  -257,   290,    37,    37,    37,
     349,  -257,  -257,  -257,  -257,   334,   346,   358,  -257,   114,
     288,   205,   301,   142,  -257,  -257,    65,  -257,  -257,   234,
     253,    18,   351,  -257,   353,   354,   355,   336,   350,   357,
     269,  -257,  -257,   359,   366,   152,  -257,  -257,  -257,   336,
    -257,  -257,   305,    -7,   336,  -257,   335,  -257,  -257,   335,
    -257,  -257,  -257,  -257,  -257,   336,   367,  -257,   258,   258,
     296,   356,   360,   361,   368,   224,  -257,  -257,   309,   272,
    -257,  -257,   369,   364,   309,   370,   371,   332,   131,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,   300,  -257,   372,  -257,
     378,   376,   375,   373,  -257,   380,   377,  -257,   379,   382,
    -257,  -257,   383,   176,   202,  -257,  -257,  -257,  -257,   269,
     205,  -257,  -257,   206,   384,   388,   385,  -257,  -257,   258,
     258,   386,   336,  -257,   392,  -257,   389,   390,  -257,  -257,
    -257,  -257,   393,    -8,   272,   269,   303,   336,   391,   394,
     395,   244,   248,  -257,  -257,  -257,  -257,   397,  -257,   398,
     400,  -257,   249,  -257,  -257,  -257,   374,  -257,   258,   336,
      24,  -257,  -257,  -257,   336,   401,   252,   402,   387,   396,
    -257,  -257,   403,   336,  -257,   404,   410,  -257,   405,    85,
      85,  -257,   156,   170,   269,   412,   443,   418,   399,   269,
     407,   408,   406,   411,  -257,   433,   205,  -257,   435,   439,
     304,   441,  -257,   413,  -257,   269,   269
  };

  const unsigned char
  srtl_parser::yydefact_[] =
  {
       0,     0,     0,     0,     8,     0,     9,     0,     3,     5,
       6,     7,     4,   120,   118,   116,   117,     0,     0,   119,
       0,     0,     0,     0,   112,   113,    11,     0,     0,     0,
       0,     0,     1,     2,    32,    33,     0,     0,    21,     0,
       0,    19,     0,     0,    20,     0,   110,   111,     0,     0,
      10,     0,     0,     0,     0,   114,     0,     0,     0,     0,
       0,     0,    27,    28,    34,    29,    30,    31,    24,    23,
       0,    25,    26,   121,    12,    14,     0,     0,   133,   135,
     134,   136,     0,     0,   123,     0,     0,     0,     0,     0,
       0,    35,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   126,   127,     0,     0,   156,   125,   115,
       0,     0,     0,     0,     0,     0,     0,     0,    50,    60,
       0,     0,    36,     0,     0,     0,     0,     0,     0,     0,
       0,   138,     0,     0,   158,     0,     0,   143,     0,     0,
     149,   162,   163,     0,     0,     0,   157,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    46,    59,     0,
       0,     0,    40,     0,    39,    22,     0,     0,    17,    18,
       0,     0,   130,     0,   155,   154,     0,     0,   129,     0,
       0,     0,   131,     0,     0,   132,     0,     0,     0,     0,
       0,   128,   124,    75,    79,     0,     0,     0,    95,   108,
     107,     0,     0,     0,    83,    92,     0,   104,   105,     0,
       0,     0,     0,   109,     0,     0,     0,     0,     0,     0,
      53,    37,    38,     0,     0,     0,    13,    16,    15,     0,
     137,   141,     0,     0,     0,   145,     0,   142,   148,     0,
     147,   159,   161,   164,   160,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,   108,   107,     0,     0,
     102,   103,     0,   108,     0,     0,     0,   107,     0,    77,
      76,    78,    69,    70,    71,    72,     0,    52,     0,    47,
       0,     0,     0,     0,   151,   152,     0,   150,     0,     0,
      88,   101,    97,     0,    90,    81,   100,    99,   106,    49,
       0,    82,    87,     0,     0,     0,     0,    62,    63,     0,
       0,     0,     0,    74,     0,   122,     0,     0,   153,   140,
     144,   146,     0,     0,     0,    48,     0,     0,     0,     0,
       0,     0,     0,    73,    51,    41,    61,     0,    98,     0,
       0,    89,     0,    80,    84,    65,     0,    64,     0,     0,
       0,   139,    93,    94,     0,     0,     0,     0,     0,     0,
      42,    85,     0,     0,    67,     0,     0,    66,     0,     0,
       0,    68,     0,     0,     0,     0,     0,    44,     0,    58,
       0,     0,     0,     0,    55,     0,     0,    54,     0,     0,
       0,     0,    45,     0,    43,    57,    56
  };

  const short int
  srtl_parser::yypgoto_[] =
  {
    -257,  -257,   472,  -257,  -257,  -257,  -257,  -257,  -257,  -257,
    -257,  -257,  -257,  -257,  -257,  -257,  -257,   409,  -257,  -257,
     318,   414,  -257,  -257,  -257,  -257,  -257,  -257,  -257,   -94,
     317,  -110,  -257,   315,  -189,   225,  -257,  -256,  -195,  -181,
    -185,  -143,   -33,   -22,   -18,   434,  -257,  -257,  -257,  -257,
    -257,   415,  -257,   416,  -257,  -120,   254,  -257,  -257,   -89,
    -129
  };

  const short int
  srtl_parser::yydefgoto_[] =
  {
      -1,     7,     8,     9,    27,    50,    97,    98,   167,   170,
      10,    20,    21,    22,    38,    41,    44,   162,   350,   360,
     163,   160,   117,   276,   219,   377,   375,   382,   378,   118,
     168,   119,   149,   150,   203,   204,   205,   290,   206,   207,
     208,   291,    23,    24,    25,    74,   316,    11,    82,    83,
     130,   131,   133,   136,   139,   181,   176,   177,   108,   137,
     144
  };

  const short int
  srtl_parser::yytable_[] =
  {
      36,    46,   126,   302,    57,    47,    58,    59,   158,   214,
     215,   216,   250,   140,   190,   268,   158,   253,   184,    39,
      84,   261,   166,    85,   265,   260,   125,   339,   174,    86,
      87,    42,   358,    76,   269,    46,    46,    46,    89,    47,
      47,    47,    26,    40,   111,   112,   113,   114,   115,   172,
      12,   175,   340,   293,   294,    43,   359,    77,   241,   242,
     243,   123,   124,   104,   303,   220,   266,   173,   341,    13,
      14,   344,   178,   253,   277,   116,   270,   226,   271,   306,
     105,   165,   182,   261,   134,   185,   283,   260,    13,    14,
     179,   287,    37,   135,   237,   141,   142,   240,   361,   192,
     183,    46,    46,   186,   143,    47,    47,    51,   261,   261,
     158,   326,   260,   260,   331,   332,   288,   183,   261,   289,
      34,    35,   260,   256,    15,   257,    16,    48,    49,   342,
      13,    14,    17,    13,    14,    32,   258,    18,   259,    19,
       2,     3,   210,    15,    28,    16,   261,   261,    55,    56,
     260,   260,   211,   356,    13,    14,    18,   261,    19,    13,
      14,   260,    52,     1,    29,    30,    31,     2,     3,   334,
      53,   261,     4,    13,    14,   260,    13,    14,   187,   188,
       5,   248,   -96,    54,   -96,    15,    73,    16,    15,   256,
      16,   257,     6,    45,   189,    90,    88,   390,    18,     4,
      19,    18,   258,    19,   310,   325,   357,     5,   281,    15,
     254,    16,   255,    60,    15,   158,    16,    92,   282,     6,
     368,   374,    18,   195,    19,   120,   121,    18,    15,    19,
      16,    15,   110,    16,   256,   376,   257,    61,   196,   197,
     198,    18,   195,    19,   147,   148,    19,   258,    96,   323,
     111,   112,   113,   114,   115,    94,    95,   196,   197,   198,
     256,   134,   257,   199,   256,   200,   257,   262,   103,   158,
     138,    99,   201,   258,   134,   324,   202,   258,   125,   327,
     379,   116,   199,   236,   200,   100,   158,    78,    79,    80,
      81,   300,   263,   213,   257,   202,   111,   112,   113,   114,
     115,   396,   256,   101,   257,   264,   256,   256,   257,   257,
     256,   256,   257,   267,   102,   258,   256,   348,   257,   258,
     258,   349,   354,   258,   258,   363,   193,   116,   194,   258,
     256,   213,   257,   106,    62,    63,   372,   373,    64,    65,
      66,    67,    68,   258,    69,    70,    71,    72,   134,   109,
      46,    46,   107,   122,    47,    47,   -91,   239,   -91,   251,
     127,   252,   249,   284,   295,   285,   255,   256,   311,   257,
     312,   343,   393,   255,   255,    91,   128,    93,   146,   129,
     132,   151,   152,   153,   154,   155,   157,   147,   159,   156,
     166,   169,   171,   180,   209,   213,   191,   212,   217,   221,
     222,   223,   218,   314,   228,   224,   309,   245,   278,   355,
     231,   229,   233,   232,   234,   235,   238,   244,   247,   246,
     272,   279,   273,   274,   275,   280,   194,   292,   296,   335,
     369,   305,   297,   298,   315,   271,   370,   299,   318,   307,
     308,   313,   304,   317,   328,   319,   329,   320,   380,   337,
     321,   338,   381,   365,   383,   333,   322,   330,   336,   389,
     345,   391,   366,   384,   347,   351,   352,   346,   353,   362,
     387,   364,   367,   385,   371,   386,   392,   388,   394,    33,
     301,   225,   395,    75,   227,     0,     0,   286,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   145,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   164,     0,   161,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   230
  };

  const short int
  srtl_parser::yycheck_[] =
  {
      18,    23,    96,   259,    37,    23,    39,    40,   118,   152,
     153,   154,   201,   102,   143,   210,   126,   202,   138,     8,
      41,   206,     9,    44,   209,   206,     9,    35,    35,    50,
      51,     8,     8,    19,    16,    57,    58,    59,    56,    57,
      58,    59,    58,    32,    27,    28,    29,    30,    31,    52,
      64,    58,    60,   248,   249,    32,    32,    43,   187,   188,
     189,    94,    95,    59,   259,   159,   209,    70,   324,     3,
       4,   327,    52,   258,   217,    58,    58,    64,    60,   264,
      76,    64,    52,   268,    58,    52,   229,   268,     3,     4,
      70,   234,    32,    67,   183,    58,    59,   186,   354,    52,
      70,   123,   124,    70,    67,   123,   124,    66,   293,   294,
     220,   300,   293,   294,   309,   310,   236,    70,   303,   239,
      12,    13,   303,    58,    58,    60,    60,     7,     8,   324,
       3,     4,    66,     3,     4,     0,    71,    71,    73,    73,
       5,     6,    56,    58,    38,    60,   331,   332,    72,    73,
     331,   332,    66,   348,     3,     4,    71,   342,    73,     3,
       4,   342,    66,     1,    58,    59,    60,     5,     6,   312,
      66,   356,    37,     3,     4,   356,     3,     4,    54,    55,
      45,    67,    68,    66,    70,    58,    58,    60,    58,    58,
      60,    60,    57,    66,    70,    65,    24,   386,    71,    37,
      73,    71,    71,    73,    73,   299,   349,    45,    56,    58,
      68,    60,    70,    66,    58,   325,    60,    66,    66,    57,
     363,    65,    71,    18,    73,    10,    11,    71,    58,    73,
      60,    58,     9,    60,    58,    65,    60,    66,    33,    34,
      35,    71,    18,    73,    66,    67,    73,    71,    65,    73,
      27,    28,    29,    30,    31,    25,    25,    33,    34,    35,
      58,    58,    60,    58,    58,    60,    60,    33,    59,   379,
      67,    53,    67,    71,    58,    73,    71,    71,     9,    73,
     374,    58,    58,    67,    60,    53,   396,    46,    47,    48,
      49,    67,    58,    59,    60,    71,    27,    28,    29,    30,
      31,   395,    58,    53,    60,    71,    58,    58,    60,    60,
      58,    58,    60,    60,    53,    71,    58,    73,    60,    71,
      71,    73,    73,    71,    71,    73,    58,    58,    60,    71,
      58,    59,    60,    53,    10,    11,   369,   370,    14,    15,
      16,    17,    18,    71,    20,    21,    22,    23,    58,    72,
     372,   373,    59,    37,   372,   373,    68,    67,    70,    58,
      65,    60,    74,    58,    68,    60,    70,    58,    68,    60,
      70,    68,    68,    70,    70,    57,    65,    59,    59,    67,
      67,    56,    56,    56,    56,    56,    64,    66,    65,    75,
       9,    58,    40,    58,    56,    59,    76,    60,    67,    37,
      37,    36,    72,    25,    64,    66,    74,    73,    58,    35,
      68,    70,    67,    70,    70,    68,    68,    68,    60,    73,
      69,    64,    69,    69,    69,    66,    60,    60,    72,    37,
      26,    67,    72,    72,    58,    60,    26,    69,    58,    69,
      69,    69,    73,    70,    60,    68,    58,    68,    36,    59,
      68,    58,     9,    66,    36,    69,    73,    72,    69,    26,
      69,    26,    66,    64,    69,    68,    68,    73,    68,    68,
      64,    69,    69,    66,    69,    67,    37,    66,    37,     7,
     255,   166,    69,    49,   167,    -1,    -1,   233,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173
  };

  const unsigned char
  srtl_parser::yystos_[] =
  {
       0,     1,     5,     6,    37,    45,    57,    78,    79,    80,
      87,   124,    64,     3,     4,    58,    60,    66,    71,    73,
      88,    89,    90,   119,   120,   121,    58,    81,    38,    58,
      59,    60,     0,    79,    12,    13,   121,    32,    91,     8,
      32,    92,     8,    32,    93,    66,   120,   121,     7,     8,
      82,    66,    66,    66,    66,    72,    73,   119,   119,   119,
      66,    66,    10,    11,    14,    15,    16,    17,    18,    20,
      21,    22,    23,    58,   122,   122,    19,    43,    46,    47,
      48,    49,   125,   126,    41,    44,    50,    51,    24,   121,
      65,    97,    66,    97,    25,    25,    65,    83,    84,    53,
      53,    53,    53,    59,    59,    76,    53,    59,   135,    72,
       9,    27,    28,    29,    30,    31,    58,    99,   106,   108,
      10,    11,    37,   119,   119,     9,   106,    65,    65,    67,
     127,   128,    67,   129,    58,    67,   130,   136,    67,   131,
     136,    58,    59,    67,   137,   130,    59,    66,    67,   109,
     110,    56,    56,    56,    56,    56,    75,    64,   108,    65,
      98,    98,    94,    97,    94,    64,     9,    85,   107,    58,
      86,    40,    52,    70,    35,    58,   133,   134,    52,    70,
      58,   132,    52,    70,   132,    52,    70,    54,    55,    70,
     137,    76,    52,    58,    60,    18,    33,    34,    35,    58,
      60,    67,    71,   111,   112,   113,   115,   116,   117,    56,
      56,    66,    60,    59,   118,   118,   118,    67,    72,   101,
     106,    37,    37,    36,    66,   110,    64,   107,    64,    70,
     128,    68,    70,    67,    70,    68,    67,   136,    68,    67,
     136,   137,   137,   137,    68,    73,    73,    60,    67,    74,
     111,    58,    60,   117,    68,    70,    58,    60,    71,    73,
     116,   117,    33,    58,    71,   117,   118,    60,   115,    16,
      58,    60,    69,    69,    69,    69,   100,   118,    58,    64,
      66,    56,    66,   118,    58,    60,   133,   118,   132,   132,
     114,   118,    60,   115,   115,    68,    72,    72,    72,    69,
      67,   112,   114,   115,    73,    67,   117,    69,    69,    74,
      73,    68,    70,    69,    25,    58,   123,    70,    58,    68,
      68,    68,    73,    73,    73,   106,   111,    73,    60,    58,
      72,   115,   115,    69,   118,    37,    69,    59,    58,    35,
      60,   114,   115,    68,   114,    69,    73,    69,    73,    73,
      95,    68,    68,    68,    73,    35,   115,   118,     8,    32,
      96,   114,    68,    73,    69,    66,    66,    69,   118,    26,
      26,    69,   119,   119,    65,   103,    65,   102,   105,   106,
      36,     9,   104,    36,    64,    66,    67,    64,    66,    26,
     111,    26,    37,    68,    37,    69,   106
  };

  const unsigned char
  srtl_parser::yyr1_[] =
  {
       0,    77,    78,    78,    78,    79,    79,    79,    79,    79,
      80,    81,    83,    82,    84,    82,    85,    85,    86,    87,
      87,    87,    87,    88,    88,    88,    88,    89,    89,    89,
      89,    89,    90,    90,    90,    91,    92,    92,    92,    93,
      93,    95,    94,    96,    96,    96,    97,    98,    99,    99,
      99,   100,   100,   101,   102,   103,   104,   104,   105,   106,
     106,   107,   108,   108,   108,   108,   108,   108,   108,   108,
     108,   108,   108,   108,   108,   109,   109,   109,   110,   110,
     111,   111,   111,   111,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   113,   113,   113,   113,   113,   113,   113,
     113,   114,   115,   115,   115,   115,   116,   117,   117,   118,
     119,   119,   119,   119,   120,   120,   121,   121,   121,   121,
     121,   122,   123,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   125,   125,   126,   126,   127,   127,   128,
     129,   129,   130,   130,   130,   130,   131,   131,   131,   131,
     132,   133,   133,   133,   134,   134,   135,   135,   136,   137,
     137,   137,   137,   137,   137
  };

  const unsigned char
  srtl_parser::yyr2_[] =
  {
       0,     2,     2,     1,     2,     1,     1,     1,     1,     1,
       3,     1,     0,     6,     0,     6,     2,     1,     1,     3,
       3,     3,     7,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     3,     3,     4,     6,     6,     5,
       5,     0,     7,     9,     5,     9,     3,     3,     6,     5,
       1,     3,     1,     1,     3,     3,     6,     5,     1,     2,
       1,     5,     5,     5,     7,     7,    10,     9,    11,     4,
       4,     4,     4,     6,     5,     2,     3,     3,     3,     2,
       5,     3,     3,     1,     5,     7,     2,     3,     3,     5,
       3,     1,     1,     6,     6,     1,     1,     3,     5,     3,
       3,     1,     2,     2,     1,     1,     3,     1,     1,     1,
       2,     2,     1,     1,     3,     5,     1,     1,     1,     1,
       1,     1,     1,     4,     7,     5,     5,     5,     7,     7,
       7,     7,     7,     1,     1,     1,     1,     3,     1,     7,
       5,     3,     3,     1,     5,     3,     5,     3,     3,     1,
       3,     3,     3,     4,     1,     1,     1,     2,     1,     3,
       3,     3,     1,     1,     3
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const srtl_parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "'_'", "'*'", "CONCRETE",
  "ABSTRACT", "EXTENDS", "OVERRIDES", "ROOT", "INSN", "EXP", "PEEP2",
  "SPLIT", "INSN_SPLIT", "INSN_RESERV", "PREDICATE", "SPECIAL_PREDICATE",
  "ATTR", "ASM_ATTR", "CONS", "REG_CONS", "MEM_CONS", "ADD_CONS", "BYPASS",
  "IN", "OUT", "LATENCY", "LOV", "DOCSTRING", "REGCLASS", "ALLCONSTRAINTS",
  "INSTANTIATES", "CONST_INT", "DUPLICATE", "FIXED_REG", "CMD_SPEC",
  "CMD_SPEC_BODY", "NONAME", "MODE_ATTR", "SET_ATTR", "AUTOMATON",
  "CODE_ATTR", "CONSTANTS", "CPU_UNIT", "LIST", "C_ITER", "C_ATTR",
  "M_ITER", "M_ATTR", "RESERVATION", "C_ENUM", "CBRACE", "OBRACE", "PIPE",
  "PLUS", "ASSIGN", "DDDD", "ID", "QUOTED_ID", "INT", "rid", "aid", "HEX",
  "'}'", "'{'", "'.'", "'('", "')'", "';'", "','", "'<'", "'>'", "':'",
  "'='", "'-'", "'\"'", "$accept", "Pattern_List", "Pattern",
  "Abstract_Pattern", "Abstract_Pattern_Name", "Abstract_Rtl_Spec", "$@1",
  "$@2", "Abstract_Extends_Body", "Abstract_Overrides_Body",
  "Concrete_Pattern", "Concrete_Pattern_Name_Qualifier_S",
  "Concrete_Pattern_Name_Qualifier", "Concrete_Pattern_Name_Qualifier_C",
  "Concrete_Rtl_Spec_S", "Concrete_Rtl_Spec", "Concrete_Rtl_Spec_C",
  "Concrete_Instantiates_Body_C", "$@3", "Concrete_Instantiates_Body_C2",
  "Concrete_Instantiates_Body", "Concrete_Overrides_Body",
  "Concrete_Instantiates_Body_Inner", "Quoted_Id_List",
  "Concrete_Overrides_Body_Inner", "Concrete_Instantiates_Body_Out",
  "Concrete_Overrides_Body_Out", "Concrete_Instantiates_Body_Inner_Out",
  "Concrete_Overrides_Body_Inner_Out", "Concrete_Inst_Stmt_List", "Stmt",
  "Mode_Stmt", "Attr_Access", "Operand_Access", "Operands", "Operand",
  "Fixed_Reg_Or_Const", "Constraint", "Predicate_Or_Mode_List",
  "Predicate_Or_Mode_With_Brackets", "Predicate_Or_Mode", "Quoted_Id",
  "Concrete_Pattern_Name_List", "Concrete_Pattern_Name_With_Brackets",
  "Concrete_Pattern_Name", "Base_Name", "Pattern_Name", "List_Pattern",
  "Code_Mode_Iter", "Code_Mode_Attr", "ListAsmEntries", "AsmEntry",
  "ListPID", "ListEntries", "ListQEntries", "SQEntryPair", "IEntryPair",
  "Id_Reg", "QIDList", "SEntry", "PipedId", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  srtl_parser::yyrline_[] =
  {
       0,   160,   160,   161,   163,   167,   176,   194,   208,   211,
     218,   222,   229,   229,   296,   296,   304,   334,   369,   374,
     376,   378,   380,   403,   408,   413,   418,   425,   430,   435,
     440,   445,   452,   456,   460,   467,   516,   586,   606,   628,
     667,   681,   681,   690,   730,   771,   788,   792,   796,   803,
     808,   815,   818,   824,   831,   835,   839,   846,   853,   860,
     864,   872,   883,   893,   901,   911,   919,   927,   938,   949,
     956,   963,   970,   977,   983,   994,   999,  1016,  1024,  1028,
    1037,  1050,  1057,  1061,  1069,  1078,  1089,  1095,  1115,  1121,
    1128,  1135,  1141,  1148,  1157,  1164,  1170,  1176,  1182,  1188,
    1194,  1202,  1208,  1213,  1218,  1224,  1232,  1239,  1243,  1250,
    1255,  1260,  1265,  1272,  1280,  1285,  1292,  1296,  1301,  1305,
    1309,  1315,  1320,  1326,  1334,  1342,  1350,  1358,  1368,  1378,
    1384,  1390,  1398,  1408,  1411,  1416,  1419,  1424,  1428,  1434,
    1444,  1449,  1455,  1459,  1463,  1467,  1473,  1477,  1481,  1485,
    1491,  1498,  1505,  1515,  1524,  1527,  1532,  1538,  1546,  1555,
    1563,  1570,  1578,  1584,  1590
  };

  // Print the state stack on the debug stream.
  void
  srtl_parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  srtl_parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  // Symbol number corresponding to token number t.
  inline
  srtl_parser::token_number_type
  srtl_parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    76,     2,     2,     2,     2,     2,
      67,    68,     4,     2,    70,    75,    66,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    73,    69,
      71,    74,    72,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     3,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    65,     2,    64,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63
    };
    const unsigned int user_token_number_max_ = 316;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }


} // yy
#line 3456 "srtl-parser.tab.cc" // lalr1.cc:1155
#line 1619 "srtl-parser.yy" // lalr1.cc:1156


void yy::srtl_parser::error (const yy::srtl_parser::location_type& l,
                             const std::string& m) {
    driver.error (l, m);
}

