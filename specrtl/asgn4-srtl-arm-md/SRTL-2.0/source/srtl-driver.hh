#ifndef SRTL_DRIVER_HH
#define SRTL_DRIVER_HH

#include <string>
#include <vector>
#include <map>
#include <fstream>

#include "srtl-parser.tab.hh"
#include "statTreeNode.hh"

// Tell Flex the lexer's prototype ..
# define YY_DECL 									\
	yy::srtl_parser::token_type						\
	yylex (yy::srtl_parser::semantic_type* yylval,	\
		yy::srtl_parser::location_type* yylloc,		\
		srtl_driver& driver)
// ... and declare it for the parser's sake.
YY_DECL;

// The main driver of the compiler.
class srtl_driver {
private:
	/* 
	 * This is the symbol table which will be used to store 
	 * abstract & concrete patters. Abstract patterns 
	 * will be used to during extending / instantiating concrete patterns
	 * while concrete patterns will be used during 
	 * overriding concrete patterns.
	 */
    std::map <std::string, Pattern*> symTab;

    /*
     * This map contains rtl operands and the number of 
     * (maximum) children it can have.
     * e.g. plus:2. not:1.
     */ 
    static std::map <std::string, int> rtlOperandSymTab;

    /*
     * This is the outputFile that has conventional md patterns
     * converted from specmd.
     */
    std::ofstream outFile;
    
    /*
     * Name of the outputFile
     */
    std::string outFileName;

    /* Construct Counts
     * This data structure helps us in counting different 
     * types of specmd constructs used in the input file.
     */
    std::map <Pattern::Type, int> constructTally;
public:

    enum LogLevel {info, deb, err, fatal, grammar};
    
    /* 
     * Dot file which represents the abstract-concrete hierarchy of patterns 
     * in the input file.
     */
    std::ofstream dotFile;
    std::ofstream dotFileUses;
    /*
     * Dot file generation is dependant on this flag.
     */
    bool generateDotFile;
    
    srtl_driver();
    virtual ~srtl_driver ();
	
	/*
	 * This is the pointer to the current pattern that is being 
	 * worked on by the parser.
	 */
    Pattern* currentPattern;
    /*
     * String object which contains the conventional md pattern 
     * representation of the currentPatter above. 
     * After printout out the string in the outFile, 
     * this string is emptied.
     */
    static std::string outPattern;
    
    // Scanner
    int result;
    void scan_begin();
    void scan_end();

    // Parser
    int parse (const std::string& f);
    std::string file;

    // Symbol Table
    void addSymTabEntry (Pattern *p);
    void addRtlOperandSymTabEntry (std::string name, int c);
    Pattern* findSymTabEntry (std::string name);
    static int findRtlOperandSymTabEntry (std::string nameToSearch);
    void initialize();
    
    // Error Handling.
    void static error (const yy::location& l, const std::string& m);
    void static error (const std::string& m);

    // Debug
    void debug (LogLevel level, const std::string& m);
    void static debugS (LogLevel level, const std::string& m);
    void setLogLevel (LogLevel level);

    // Output
    void write (std::string s);
    void writeLineBreak ();
    std::string formatNodeName (std::string n);
    static unsigned long int stoi(const std::string t);
    static std::string itos(const long int i);
    void setOutFileName (std::string* name) { outFileName = *name; }
    void initializeOutFile ();
    void initializeDotFile ();

    // Statistical
    void addToConstructTally (Pattern::Type t);
    void displayConstructTally ();
    int getConstructIndex (Pattern::Type t);

    vector<StatTreeNode*> statTree;

    int highestDepth;
    int getDepth (StatTreeNode* n);
    void addToStatTree (std::string parentPatternName, 
        std::string patternName, int type = 1);
    StatTreeNode* searchStatTree (std::string patternName);
    StatTreeNode* searchSingleStatTree (StatTreeNode* n, 
        std::string patternName);

    // To know the degree of each pattern.
    std::map <std::string, int> abstractPatternCountForExtends;
    std::map <std::string, int> abstractPatternCountForInstantiates;
    void incrementAbstractPatternCount (std::string patternName, 
        Pattern::Type t = Pattern::abstract);
    std::map <std::string, int> concretePatternCountForOverrides;
    void incrementConcretePatternCountForOverrides (std::string patternName);


    // To know abstracts / concretes per level.
    std::map <int, int> abstractsPerLevel;
    std::map <int, int> concretesPerLevel;
    int currentDepth;
    void countSingleStatTree (StatTreeNode* n);
    void countStatTree ();

    void printDescendantsTable ();
    void printAbstractDescendantsTable ();
    void printConcreteDescendantsTable ();

    void calculateAndDisplayStats ();
        
    void addToRtlOperandSymTabWithDot (std::string, int);
    std::string appendStrings (vector<std::string>* v) {
        std::string t = "";
        for (unsigned int i = 0; i < v->size (); i++) {
            t.append (v->at(i));
        }
        return t;
    }

private:
    LogLevel logLevel;

};
#endif
