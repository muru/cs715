#include <iostream>
#include <sstream>
#include "statTreeNode.hh"
#include "srtl-driver.hh"
#include "srtl-parser.tab.hh"

std::map <std::string, int> srtl_driver::rtlOperandSymTab;

// Descendants related stats.
std::map <int, int> abstractStats;
std::map <int, int> concreteStats;
std::map <int, int> mixStats;

std::map <int, int> aa;
std::map <int, int> ac;

std::map <int, int> ca;
std::map <int, int> cc;

srtl_driver::srtl_driver() {
    /* Setup the operands & basis set. */
    dotFileUses.open ("srtlUses.dot");
    dotFileUses << "digraph srtlUses {\n";
//    dotFileUses << "rankdir=\"LR\";\n";

	initialize();

    generateDotFile = true;


}

void srtl_driver::initializeOutFile () {
    std::string finalFileName = outFileName;
    outFile.open (finalFileName.c_str ());
}

void srtl_driver::initializeDotFile () {
    generateDotFile = true;
    dotFile.open ("srtl.dot");
    dotFile << "digraph srtl {\n";
    dotFile << "rankdir=\"LR\";\n";
}

/*
 * Display Stats
 */
void srtl_driver::displayConstructTally () {
    map <Pattern::Type, int>::iterator pat;
    int total = 0; int abs = 0;
    for (pat = constructTally.begin (); pat != constructTally.end (); pat++) {
        total += pat->second;
        switch (pat->first) {
        case Pattern::abstract:
            std::cout << "Abstracts: \t\t\t" << pat->second << "\n";
            abs = pat->second;
            break;
        case Pattern::insn:
            std::cout << "Insn: \t\t\t\t" << pat->second << "\n";
            break;
        case Pattern::exp:
            std::cout << "Expand: \t\t\t" << pat->second << "\n";
            break;
        case Pattern::insnAndSplit:
            std::cout << "InsnAndSplit: \t\t\t" << pat->second << "\n";
            break;
        case Pattern::split:
            std::cout << "Split: \t\t\t\t" << pat->second << "\n";
            break;
        case Pattern::peep2:
            std::cout << "Peephole2: \t\t\t" << pat->second << "\n";
            break;
        case Pattern::insnReservation:
            std::cout << "InsnReservation: \t\t\t" << pat->second << "\n";
            break;
        case Pattern::predicate:
            std::cout << "Predicate: \t\t\t" << pat->second << "\n";
            break;
        case Pattern::specialPredicate:
            std::cout << "SpecialPredicate: \t\t\t" << pat->second << "\n";
            break;
        case Pattern::attr:
            std::cout << "Attributes: \t\t\t" << pat->second << "\n";
            break;
        case Pattern::asmAttr:
            std::cout << "AsmAttributes: \t\t\t" << pat->second << "\n";
            break;
        case Pattern::cons:
            std::cout << "Constraints: \t\t\t" << pat->second << "\n";
            break;
        case Pattern::regCons:
            std::cout << "RegConstraints: \t\t\t" << pat->second << "\n";
            break;
        case Pattern::memCons:
            std::cout << "MemConstraints: \t\t\t" << pat->second << "\n";
            break;
        case Pattern::addCons:
            std::cout << "AddConstraints: \t\t\t" << pat->second << "\n";
            break;
        case Pattern::define_code_iterator:
            std::cout << "CodeIterators: \t\t\t" << pat->second << "\n";
            break;
        case Pattern::define_mode_iterator:
            std::cout << "ModeIterators: \t\t\t" << pat->second << "\n";
            break;
        case Pattern::define_code_attr:
            std::cout << "CodeAttributes: \t\t\t" << pat->second << "\n";
            break;
        case Pattern::define_mode_attr:
            std::cout << "ModeAttributes: \t\t\t" << pat->second << "\n";
            break;
        case Pattern::define_constants:
            std::cout << "Constants: \t\t\t" << pat->second << "\n";
            break;
        case Pattern::define_c_enum:
            std::cout << "C_Enums: \t\t\t" << pat->second << "\n";
            break;
        case Pattern::define_asm_attributes:
            std::cout << "AsmAttributes: \t\t\t" << pat->second << "\n";
            break;
        case Pattern::define_reservation:
            std::cout << "Reservation: \t\t\t" << pat->second << "\n";
            break;
        case Pattern::define_cpu_unit:
            std::cout << "Cpu_unit: \t\t\t" << pat->second << "\n";
            break;
        case Pattern::define_automaton:
            std::cout << "Automaton: \t\t\t" << pat->second << "\n";
            break;
        }
    }
    std::cout << "Total: \t\t\t\t" << total << "\n";
    std::cout << "Concrete: \t\t\t" << total - abs << "\n";
}
extern int gInt;

srtl_driver::~srtl_driver()
{
    // Free up the symbol table. 
    for (std::map <std::string, Pattern*>::iterator i = symTab.begin (); 
        i != symTab.end (); i++) {
        delete i->second;
    }

    symTab.clear ();
    outFile.close ();
}

/*
 * Calculate and display stats like construct tally,
 * form trees and calculate the tree statistics. 
 */ 
void srtl_driver::calculateAndDisplayStats () {
    if (generateDotFile) {
        dotFile << "\n }";
        dotFile.close ();
    }

    dotFileUses << "\n }";
    dotFile.close ();

    /* Display construct tally */
    displayConstructTally ();


    /* Collect stats about our file. */
    /* Average Depth */
    float depth = 0;
    int depthHigh = 0;
    int validStatTrees = 0;
    int depthOne = 0; int depthTwo = 0; int depthThree = 0; int depthFour = 0;
    std::map<int, int> childrenCount;
    std::cout << "--------------------------------------------------------\n";
    for (unsigned int i = 0; i < statTree.size (); i++) {
        highestDepth = 1;
        getDepth(statTree[i]);
        if (highestDepth > depthHigh) {
            depthHigh = highestDepth;
        }
        if (highestDepth == 1 && statTree[i]->type == 1)
            continue;
//      std::cout << "Highest Depth for " << i << ": "  << 
//      statTree[i]->name << ": " <<  highestDepth << std::endl;
        depth += highestDepth;
        if (highestDepth == 1)  depthOne++; 
        //std::cout << "Depth1: " << statTree[i]->name << "\n"; }
        if (highestDepth == 2)  depthTwo++; 
        //std::cout << "Depth2: " << statTree[i]->name << "\n"; }
        if (highestDepth == 3)  depthThree++;
        // std::cout << "Depth3: " << statTree[i]->name << "\n"; }
        if (highestDepth == 4)  depthFour++; 
        //std::cout << "Depth4: " << statTree[i]->name << "\n"; }
        validStatTrees++;
    
        std::map <int, int>::iterator pat;
        pat = childrenCount.find (statTree[i]->children.size ());
        if (pat == childrenCount.end ()){
            childrenCount[statTree[i]->children.size ()] = 1;
        } else {
            pat->second++;
        }
    }
    std::cout << "-----------------------------------------------------\n";
    std::cout << "\bTotal Stat Trees: " << validStatTrees << std::endl;
    std::cout << "Average Depth: " << depth / validStatTrees << "\t\t";
    std::cout << "Highest Depth: " << depthHigh << std::endl;
    std::cout << "Patterns With Depth 1: " << depthOne 
        << "\t\tPatterns With Depth 2: " << depthTwo << std::endl;
    std::cout << "Patterns With Depth 3: " << depthThree 
        << "\t\tPatterns With Depth 4: " << depthFour << std::endl;
    std::cout << "------------------------------------------------------\n";

    countStatTree ();
    for (map<int,int>::iterator cc = abstractsPerLevel.begin ();
        cc != abstractsPerLevel.end (); cc++) {
        std::cout << cc->second << " " << cc->first << "\n";
    }

    for (map<int,int>::iterator cc = concretesPerLevel.begin ();
        cc != concretesPerLevel.end (); cc++) {
        std::cout << cc->second << " " << cc->first << "\n";
    }
    
    std::cout << "------------------------------------------------------\n";


    std::map <int, int> abstractPatternDegreesForExtends;
    std::map <int, int> abstractPatternDegreesForInstantiates;

    std::map <int, int> concretePatternDegreesForOverrides;

    std::cout << "Abstract Pattern Degrees:\n";

    map <string, int>::iterator pat;
    for (pat = abstractPatternCountForExtends.begin (); 
        pat != abstractPatternCountForExtends.end (); pat++) {
        if (findRtlOperandSymTabEntry (pat->first) < 0) {
            if (strcmp ("", pat->first.c_str ())) {
                std::map<int, int>::iterator cc = 
                abstractPatternDegreesForExtends.find (pat->second);
                if (cc == abstractPatternDegreesForExtends.end ())
                    abstractPatternDegreesForExtends[pat->second] = 1;
                else
                    cc->second++;
            }
        }
    }

    for (pat = abstractPatternCountForInstantiates.begin (); 
        pat != abstractPatternCountForInstantiates.end (); pat++) {
        if (findRtlOperandSymTabEntry (pat->first) < 0) {
            if (strcmp ("", pat->first.c_str ())) {
                std::map<int, int>::iterator cc = 
                abstractPatternDegreesForInstantiates.find (pat->second);
                if (cc == abstractPatternDegreesForInstantiates.end ())
                    abstractPatternDegreesForInstantiates[pat->second] = 1;
                else
                    cc->second++;
            }
        }
    }

    for (map<int,int>::iterator cc = abstractPatternDegreesForExtends.begin ();
            cc != abstractPatternDegreesForExtends.end (); cc++) {
        std::cout << cc->second << " abstract patterns used " 
            << cc->first << " times for extends.\n";
    }

    for (map<int,int>::iterator cc = abstractPatternDegreesForInstantiates.begin ();
            cc != abstractPatternDegreesForInstantiates.end (); cc++) {
        std::cout << cc->second << " abstract patterns used " 
            << cc->first << " times for instantiates.\n";
    }

    std::cout << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;
    std::cout << "Concrete Overrides Degrees" << std::endl;

    for (pat = concretePatternCountForOverrides.begin (); 
            pat != concretePatternCountForOverrides.end (); pat++) {
        if (findRtlOperandSymTabEntry (pat->first) < 0) {
            if (strcmp ("", pat->first.c_str ())) {
                std::map<int, int>::iterator cc = 
                    concretePatternDegreesForOverrides.find (pat->second);
                if (cc == concretePatternDegreesForOverrides.end ())
                    concretePatternDegreesForOverrides[pat->second] = 1;
                else
                    cc->second++;
            }
        }
    }

    for (map<int,int>::iterator cc = concretePatternDegreesForOverrides.begin ();
        cc != concretePatternDegreesForOverrides.end (); cc++) {
        std::cout << cc->second << " concrete patterns used " 
            << cc->first << " times for overrides.\n";
    }

    std::cout << "-----------------------------------------------------" 
        << std::endl;
    for (unsigned int i = 0; i < statTree.size (); i++) {
        statTree[i]->CountDescendants ();
//        printDescendantsTable ();
    }
    std::cout << "DescendantsCount" << gInt << std::endl;

    // Find Descendants
    printDescendantsTable ();
    printAbstractDescendantsTable ();
    printConcreteDescendantsTable ();

    // Generating Dot File
    if (generateDotFile) {
        dotFile << "\n }";
        dotFile.close ();
    }
}

/*
 * Call the parser
 */
int srtl_driver::parse (const std::string &f) {
    file = f;

    scan_begin();

    yy::srtl_parser parser(*this);
    result = parser.parse ();
    scan_end();
   
    return result;
}

void srtl_driver::addSymTabEntry (Pattern *p){
    symTab[p->getPatName ()] = p;
}

void srtl_driver::addRtlOperandSymTabEntry (std::string s, int c) {
    rtlOperandSymTab[s] = c;
}

void srtl_driver::write (std::string s ){
    outFile << s;
}

void srtl_driver::writeLineBreak ( ){
    outFile << std::endl;
}

/*
 * Find a sym Tab Entry.
 * If the entry does not exist.
 * Returns NULL if pattern is absent.
 */
Pattern* srtl_driver::findSymTabEntry (string name){
    map <string, Pattern *>::iterator pat;
    debug (deb, "Incoming Pattern:" + name );
    pat = symTab.find(name);
    if(pat == symTab.end()){
        return NULL;
    }else{
        Pattern* P = pat->second;
        return P;
    }
}

/*
 * Find an operand.
 * If found, return the number of children it can have.
 * Else return -1
 */
int srtl_driver::findRtlOperandSymTabEntry (string nameToSearch) {
    map <string, int>::iterator pat;
    pat = rtlOperandSymTab.find (nameToSearch);
    if (pat == rtlOperandSymTab.end ()){
        return -1;
    } else {
        return pat->second;
    }
}

void srtl_driver::error (const yy::location& l, const std::string& m) {
    std::cerr << l << ": " << m << std::endl;
}

void srtl_driver::error (const std::string& m) {
    std::cerr << m << std::endl;
}

void srtl_driver::debug (LogLevel level, const std::string& m) {
    if(logLevel <= level){
        if (level == err || level == fatal) {
            error ("\e[31m" + m + "\e[m");
        } else if (level == grammar) {
//            std::cout << "\e[36m" + m + "\e[m" << std::endl;
        } else {
//            std::cout << "\e[32m" + m + "\e[m" << std::endl;
        }
    }
}

void srtl_driver::debugS (LogLevel level, const std::string& m) {
    if (level == err || level == fatal) {
        error ("\e[31m" + m + "\e[m");
    } else if (level == grammar) {
//        std::cout << "\e[36m" + m + "\e[m" << std::endl;
    } else{
//        std::cout << "\e[32m" + m + "\e[m" << std::endl;
    }
}

void srtl_driver::setLogLevel(LogLevel level) {
    logLevel = level;
}

unsigned long int srtl_driver::stoi(const string t){
    unsigned long int i;
    stringstream convert(t);
    convert >> i;
    return i;
}

std::string srtl_driver::itos (const long int i){
    std::stringstream convert;
    convert << i;
    return convert.str ();
}

/*
 * Method to beautify long names so that they look
 * good in the dot file.
 */
std::string srtl_driver::formatNodeName (std::string n) {
    int l = n.length ();
    if (l > 15)
    for (int i = 15; i < l; i=i+15) {
        if (n.find ("_", i) != std::string::npos) {
            n.insert (n.find ("_",i) + 1, "\\n");
        }
    }
    return n;
}

/*
 * Increment the count in the construct tally.
 */
void srtl_driver::addToConstructTally (Pattern::Type t) {
    map <Pattern::Type, int>::iterator pat;
    pat = constructTally.find (t);
    if (pat == constructTally.end ()){
        constructTally[t] = 1;
    } else {
        pat->second++;
    }
}

/*
 * Get current count in the construct tally for Type t
 */
int srtl_driver::getConstructIndex (Pattern::Type t) {
    map <Pattern::Type, int>::iterator pat;
    pat = constructTally.find (t);
    if (pat == constructTally.end ()) 
        return 1;
    else
        return (pat->second + 1);
}

/*
 * Get the depth of the Stat Tree 
 */
int srtl_driver::getDepth (StatTreeNode* n) {
    if (n->children.size () == 0) {
        return 1;
    } else {

        for (unsigned int i = 0; i < n->children.size (); i++) {
            int d = getDepth (n->children[i]) + 1;
            if (highestDepth < d) {
                highestDepth = d;
            }
            return d;
        }
    }
    return highestDepth;
}

StatTreeNode* srtl_driver::searchSingleStatTree (StatTreeNode* n, 
    std::string patternName) {
    if (n->name == patternName) {
        return n;
    } else {
        for (unsigned int i = 0; i < n->children.size (); i++) {
            StatTreeNode* stn = searchSingleStatTree (n->children[i], 
                patternName);
            if (stn != NULL){
                return stn;
            }
        }
    }
    return NULL;
}

StatTreeNode* srtl_driver::searchStatTree (std::string patternName) {
    for (unsigned int i = 0; i < statTree.size (); i++) {
       StatTreeNode* n = searchSingleStatTree (statTree[i], patternName);
       if (n != NULL) {
           statTree[i]->depth++;
           return n;
       }
    }
    return NULL;
}
void srtl_driver::addToStatTree (std::string parentPatternName, 
    std::string patternName, int type) {
    StatTreeNode* stn = searchStatTree (parentPatternName);
    StatTreeNode* s = new StatTreeNode (patternName, type);
    if (stn == NULL) {
        stn = searchStatTree (patternName);
        if (stn == NULL) {
            statTree.push_back (s);
        } else {
        //  Uncomment to identify duplicate patterns.    
//            if (type == 1)
//            std::cout << "Duplicate: " << patternName << std::endl;
        
        }
    } else {
        stn->children.push_back (s);
    }
    return;
}

void srtl_driver::incrementAbstractPatternCount (std::string patternName, 
    Pattern::Type t) {
    map <string, int>::iterator pat;
    if (t == Pattern::abstract) {
        pat = abstractPatternCountForExtends.find (patternName);
        if (pat == abstractPatternCountForExtends.end ()){
            abstractPatternCountForExtends[patternName] = 1;
        } else {
            pat->second++;
        }
   } else {
       pat = abstractPatternCountForInstantiates.find (patternName);
        if (pat == abstractPatternCountForInstantiates.end ()){
            abstractPatternCountForInstantiates[patternName] = 1;
        } else {
            pat->second++;
        }
   }
}

void srtl_driver::incrementConcretePatternCountForOverrides (
    std::string patternName) {
    map <string, int>::iterator pat;
    pat = concretePatternCountForOverrides.find (patternName);
    if (pat == concretePatternCountForOverrides.end ()) {
        concretePatternCountForOverrides[patternName] = 1;
    } else {
        pat->second++;
    }
}

void srtl_driver::countSingleStatTree (StatTreeNode* n) {
    currentDepth++;
    for (unsigned int i = 0; i < n->children.size (); i++) {
        if (n->children[i]->type == 1) {
            /* Add (depth,count++) to abstractsPerLevel */
            std::map<int,int>::iterator j = abstractsPerLevel.find (currentDepth);
            if (j == abstractsPerLevel.end ()) {
                abstractsPerLevel[currentDepth] = 1;
            } else {
                j->second++;
            }

        } else {
            /* Add (depth, count++) to concretesPerLevel */
            std::map<int,int>::iterator j = concretesPerLevel.find (currentDepth);
            if (j == concretesPerLevel.end ()) {
                concretesPerLevel[currentDepth] = 1;
            } else {
                j->second++;
            }

        }
        countSingleStatTree (n->children[i]);
        currentDepth--;
    }
}

void srtl_driver::countStatTree () {
    for (unsigned int i = 0; i < statTree.size (); i++) {
        currentDepth = 0;
        countSingleStatTree (statTree[i]);
        if (statTree[i]->type == 1) {
            std::map<int,int>::iterator j = abstractsPerLevel.find (currentDepth);
            if (j == abstractsPerLevel.end ()) {
                abstractsPerLevel[currentDepth] = 1;
            } else {
                j->second++;
            }
        } else {
                        /* Add (depth, count++) to concretesPerLevel */
            std::map<int,int>::iterator j = concretesPerLevel.find (currentDepth);
            if (j == concretesPerLevel.end ()) {
                concretesPerLevel[currentDepth] = 1;
            } else {
                j->second++;
            }

        }
    }
}


void srtl_driver::printDescendantsTable () {
    std::map <int, int>::iterator pat;
    std::cout << "---------------" << std::endl << "Descendants" 
        << std::endl;std::cout << "---------------" << std::endl;
    for (pat = ::abstractStats.begin (); pat != ::abstractStats.end (); pat++)
        std::cout << pat->first << ":" << pat->second << std::endl;
    std::cout << "---------------" << std::endl;
    for (pat = ::concreteStats.begin (); pat != ::concreteStats.end (); pat++)
        std::cout << pat->first << ":" << pat->second << std::endl;

    std::cout << "---------------" << std::endl;
    std::cout << "---------------" << std::endl;
    for (pat = ::mixStats.begin (); pat != ::mixStats.end (); pat++)
        std::cout << pat->first << ":" << pat->second << std::endl;
    
}

void srtl_driver::printAbstractDescendantsTable () {
    std::map <int,int>::iterator pat;
    std::cout << "--------------" << std::endl << "Abstract Descendants" 
        << std::endl; std::cout << "--------------" << std::endl;
    for (pat = aa.begin (); pat != aa.end (); pat++)
        std::cout <<pat->first << ":" << pat->second << std::endl;
    std::cout << "--------------" << std::endl;
    for (pat = ac.begin (); pat != ac.end (); pat++)
        std::cout <<pat->first << ":" << pat->second << std::endl;
}
void srtl_driver::printConcreteDescendantsTable () {
    std::map <int,int>::iterator pat;
    for (pat = ca.begin (); pat != ca.end (); pat++)
        std::cout <<pat->first << ":" << pat->second << std::endl;
    std::cout << "--------------" << std::endl;
    for (pat = cc.begin (); pat != cc.end (); pat++)
        std::cout <<pat->first << ":" << pat->second << std::endl;
}

void srtl_driver::addToRtlOperandSymTabWithDot (std::string  p, int d) {
    rtlOperandSymTab[p]=d;
    dotFileUses << "\"" + p + "\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
    dotFileUses << "\"root\" -> \"" + p + "\" [color=blue];\n";
}

void srtl_driver::  initialize(){
    dotFileUses << "\"root\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
                    

    /* Fill up the rtl op symbol table with operands. */
    addToRtlOperandSymTabWithDot ("plus", 2);
    addToRtlOperandSymTabWithDot ("ss_plus", 2);
    addToRtlOperandSymTabWithDot ("us_plus", 2);
    addToRtlOperandSymTabWithDot ("lo_sum", 2);
    addToRtlOperandSymTabWithDot ("minus", 2);
    addToRtlOperandSymTabWithDot ("ss_minus", 2);
    addToRtlOperandSymTabWithDot ("us_minus", 2);
    addToRtlOperandSymTabWithDot ("compare", 2);
    addToRtlOperandSymTabWithDot ("neg", 1);
    addToRtlOperandSymTabWithDot ("ss_neg", 1);
    addToRtlOperandSymTabWithDot ("us_neg", 1);
    addToRtlOperandSymTabWithDot ("mult", 2);
    addToRtlOperandSymTabWithDot ("ss_mult", 2);
    addToRtlOperandSymTabWithDot ("us_mult", 2);
    addToRtlOperandSymTabWithDot ("div", 2);
    addToRtlOperandSymTabWithDot ("ss_div", 2);
    addToRtlOperandSymTabWithDot ("udiv", 2);  
    addToRtlOperandSymTabWithDot ("us_div", 2);
    addToRtlOperandSymTabWithDot ("mod", 2);
    addToRtlOperandSymTabWithDot ("umod", 2);  
    addToRtlOperandSymTabWithDot ("smin", 2);    
    addToRtlOperandSymTabWithDot ("smax", 2);    
    addToRtlOperandSymTabWithDot ("umin", 2);    
    addToRtlOperandSymTabWithDot ("umax", 2);    
    addToRtlOperandSymTabWithDot ("not", 1);
    addToRtlOperandSymTabWithDot ("and", 2);
    addToRtlOperandSymTabWithDot ("ior", 2);
    addToRtlOperandSymTabWithDot ("xor", 2);
    addToRtlOperandSymTabWithDot ("ashift", 2);
    addToRtlOperandSymTabWithDot ("ss_ashift", 2);
    addToRtlOperandSymTabWithDot ("us_ashift", 2);
    addToRtlOperandSymTabWithDot ("lshiftrt", 2);
    addToRtlOperandSymTabWithDot ("ashiftrt", 2);
    addToRtlOperandSymTabWithDot ("rotate", 2);
    addToRtlOperandSymTabWithDot ("rotatert", 2);
    addToRtlOperandSymTabWithDot ("abs", 1);
    addToRtlOperandSymTabWithDot ("ss_abs", 1);
    addToRtlOperandSymTabWithDot ("sqrt", 1);
    addToRtlOperandSymTabWithDot ("ffs", 1);
    addToRtlOperandSymTabWithDot ("clz", 1);
    addToRtlOperandSymTabWithDot ("ctz", 1);
    addToRtlOperandSymTabWithDot ("popcount", 1);
    addToRtlOperandSymTabWithDot ("parity", 1);
    addToRtlOperandSymTabWithDot ("bswap", 1);
    addToRtlOperandSymTabWithDot ("eq", 2);
    addToRtlOperandSymTabWithDot ("ne", 2);
    addToRtlOperandSymTabWithDot ("lt", 2);
    addToRtlOperandSymTabWithDot ("ltu", 2);
    addToRtlOperandSymTabWithDot ("gt", 2);
    addToRtlOperandSymTabWithDot ("gtu", 2);
    addToRtlOperandSymTabWithDot ("le", 2);
    addToRtlOperandSymTabWithDot ("leu", 2);
    addToRtlOperandSymTabWithDot ("ge", 2);
    addToRtlOperandSymTabWithDot ("geu", 2);
    addToRtlOperandSymTabWithDot ("if_then_else", 3);
    addToRtlOperandSymTabWithDot ("sign_extract", 3);
    addToRtlOperandSymTabWithDot ("zero_extract", 3);
    addToRtlOperandSymTabWithDot ("sign_extend", 1);
    addToRtlOperandSymTabWithDot ("zero_extend", 1);
    addToRtlOperandSymTabWithDot ("float_extend", 2);
    addToRtlOperandSymTabWithDot ("truncate", 1);
    addToRtlOperandSymTabWithDot ("ss_truncate", 1);
    addToRtlOperandSymTabWithDot ("us_truncate", 1);
    addToRtlOperandSymTabWithDot ("float_truncate", 1);
    addToRtlOperandSymTabWithDot ("float", 1);
    addToRtlOperandSymTabWithDot ("unsigned_float", 1);
    addToRtlOperandSymTabWithDot ("fix", 1);
    addToRtlOperandSymTabWithDot ("unsigned_fix", 1);
    addToRtlOperandSymTabWithDot ("fract_convert", 1);
    addToRtlOperandSymTabWithDot ("sat_fract", 1);
    addToRtlOperandSymTabWithDot ("unsigned_fract_convert", 1);
    addToRtlOperandSymTabWithDot ("unsigned_sat_fract", 1);
    addToRtlOperandSymTabWithDot ("strict_low_part", 1);
    addToRtlOperandSymTabWithDot ("set", 2);
    addToRtlOperandSymTabWithDot ("call", 2);
    addToRtlOperandSymTabWithDot ("clobber", 1);
    addToRtlOperandSymTabWithDot ("use", 1);
    addToRtlOperandSymTabWithDot ("parallel", 15);
    addToRtlOperandSymTabWithDot ("sequence", 40);
    addToRtlOperandSymTabWithDot ("asm_input", 0);
    addToRtlOperandSymTabWithDot ("unspec", 15);
    addToRtlOperandSymTabWithDot ("match_operator", 15);
    addToRtlOperandSymTabWithDot ("unspec_volatile", 15);
    addToRtlOperandSymTabWithDot ("addr_vec", 15);
    addToRtlOperandSymTabWithDot ("addr_diff_vec", 15);
    addToRtlOperandSymTabWithDot ("prefetch", 3);
    addToRtlOperandSymTabWithDot ("pre_inc", 1);
    addToRtlOperandSymTabWithDot ("pre_dec", 1);
    addToRtlOperandSymTabWithDot ("post_dec", 1);
    addToRtlOperandSymTabWithDot ("post_inc", 1);
    addToRtlOperandSymTabWithDot ("post_modify", 2);
    addToRtlOperandSymTabWithDot ("pre_modify", 2);
    addToRtlOperandSymTabWithDot ("subreg", 2);
    addToRtlOperandSymTabWithDot ("mem", 1);
    addToRtlOperandSymTabWithDot ("match_op_dup", 15);
    addToRtlOperandSymTabWithDot ("label_ref", 1);
    addToRtlOperandSymTabWithDot ("trap_if", 2);
    addToRtlOperandSymTabWithDot ("const", 1);
    addToRtlOperandSymTabWithDot ("vec_select", 2);
    addToRtlOperandSymTabWithDot ("vec_concat", 2);
    addToRtlOperandSymTabWithDot ("cond", 2);
    addToRtlOperandSymTabWithDot ("high", 1);
    addToRtlOperandSymTabWithDot ("unordered", 2);

    /* Fill up the basis set */
    ModeStmt s;
    AbstPattern *Abt;
	vector <int> *v;
	Pattern *p =0;

	ModeStmt ms ;
	vector<int>* access = new vector<int>();
	//access->push_back(0);
	ms.addAccessTree (access);
	ms.setPatternName("set");
    dotFileUses << "\"set\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
    dotFileUses << "\"root\" -> \"set\" [color=blue];\n";


	Abt = new AbstPattern("set_const"); Abt->addStmt(ms);Abt->initialized=true;
	Abt->setExtName("set");
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("const");
	Abt->addStmt(s); 
    addToStatTree ("", "set_const");
    incrementAbstractPatternCount ("const");
	addSymTabEntry(Abt);
    dotFileUses << "\"set_const\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
    dotFileUses << "\"set\" -> \"set_const\" [color=blue];\n";
    dotFileUses << "\"const\" -> \"set_const\" [color=blue];\n";

  

	Abt = new AbstPattern("set_const2"); Abt->addStmt(ms);Abt->initialized=true;
	Abt->setExtName("set");
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("const");
	Abt->addStmt(s);     
	addToStatTree ("", "set_const2");
    incrementAbstractPatternCount ("const");
    addSymTabEntry(Abt);
    dotFileUses << "\"set_const2\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
    dotFileUses << "\"set\" -> \"set_const2\" [color=blue];\n";
    dotFileUses << "\"const\" -> \"set_const2\" [color=blue];\n";

  

	Abt = new AbstPattern("set_const_int2"); Abt->addStmt(ms);Abt->initialized=true;
	Abt->setExtName("set");Abt->addStmt(ms);
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("const_int");
	Abt->addStmt(s);     
    addToStatTree ("", "set_const_int2");
    incrementAbstractPatternCount ("const_int");
	 
	addSymTabEntry(Abt);
    dotFileUses << "\"set_const_int2\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
    dotFileUses << "\"set\" -> \"set_const_int2\" [color=blue];\n";
    dotFileUses << "\"const_int2\" -> \"set_const_int2\" [color=blue];\n";

  

	Abt = new AbstPattern("set_const_double2"); Abt->addStmt(ms);Abt->initialized=true;
	Abt->setExtName("set");
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("const_double");
	Abt->addStmt(s);     
	addToStatTree ("", "set_const_double2");
    incrementAbstractPatternCount ("const_double");
	addSymTabEntry(Abt);

  

	Abt = new AbstPattern("set_const_fixed2"); Abt->addStmt(ms);Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("const_fixed");
	Abt->addStmt(s);     
	addToStatTree ("", "set_const_fixed2");
    incrementAbstractPatternCount ("const_fixed");
	
	addSymTabEntry(Abt);

  

	Abt = new AbstPattern("set_const_string2"); Abt->addStmt(ms);Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("const_string");
	Abt->addStmt(s);     
	addToStatTree ("", "set_const_string2");
    incrementAbstractPatternCount ("const_string");
	
	addSymTabEntry(Abt);

  

	Abt = new AbstPattern("set_const_vector2"); Abt->addStmt(ms);Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("const_vector");
	Abt->addStmt(s);     
	addToStatTree ("", "set_const_vector2");
    incrementAbstractPatternCount ("const_vector");
	
	addSymTabEntry(Abt);

  

	Abt = new AbstPattern("set_symbol_ref2"); Abt->addStmt(ms);Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("symbol_ref");
	Abt->addStmt(s);     
	addToStatTree ("", "set_symbol_ref2");
    incrementAbstractPatternCount ("symbol_ref");
	
	addSymTabEntry(Abt);

  

	Abt = new AbstPattern("set_label_ref2"); Abt->addStmt(ms);Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("label_ref");
	Abt->addStmt(s);     
	addToStatTree ("", "set_label_ref2");
    incrementAbstractPatternCount ("label_ref");
	
	addSymTabEntry(Abt);

  

	Abt = new AbstPattern("set_high2"); Abt->addStmt(ms);Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("high");
	Abt->addStmt(s);     
	addToStatTree ("", "set_high2");
    incrementAbstractPatternCount ("high");
	
	addSymTabEntry(Abt);

  

	Abt = new AbstPattern("set_plus2"); Abt->addStmt(ms);Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("plus");
	Abt->addStmt(s);     
    addToStatTree ("", "set_plus2");
    incrementAbstractPatternCount ("plus");
	addSymTabEntry(Abt);
    dotFileUses << "\"set_plus2\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
    dotFileUses << "\"set\" -> \"set_plus2\" [color=blue];\n";
    dotFileUses << "\"plus\" -> \"set_plus2\" [color=blue];\n";

  

	Abt = new AbstPattern("set_ss_plus2"); Abt->addStmt(ms);Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("ss_plus");
	Abt->addStmt(s);     
    addToStatTree ("", "set_ss_plus2"); 
    incrementAbstractPatternCount ("ss_plus");
	addSymTabEntry(Abt);
    dotFileUses << "\"set_ss_plus2\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
    dotFileUses << "\"set\" -> \"set_ss_plus2\" [color=blue];\n";
    dotFileUses << "\"ss_plus\" -> \"set_ss_plus2\" [color=blue];\n";

  

	Abt = new AbstPattern("set_us_plus2"); Abt->addStmt(ms);Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("us_plus");
	Abt->addStmt(s);     
	addToStatTree ("", "set_us_plus2");
    incrementAbstractPatternCount ("us_plus");
	addSymTabEntry(Abt);
    dotFileUses << "\"set_us_plus2\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
    dotFileUses << "\"set\" -> \"set_us_plus2\" [color=blue];\n";
    dotFileUses << "\"us_plus\" -> \"set_us_plus2\" [color=blue];\n";

  

	Abt = new AbstPattern("set_minus2"); Abt->addStmt(ms);Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("minus");
	Abt->addStmt(s);     
	addToStatTree ("", "set_minus2");
    incrementAbstractPatternCount ("minus");
	
	addSymTabEntry(Abt);
    dotFileUses << "\"set_minus2\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
    dotFileUses << "\"set\" -> \"set_minus2\" [color=blue];\n";
    dotFileUses << "\"minus\" -> \"set_minus2\" [color=blue];\n";

  

	Abt = new AbstPattern("set_ss_minus2"); Abt->addStmt(ms);Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("ss_minus");
	Abt->addStmt(s);     
	addToStatTree ("", "set_ss_minus2");
    incrementAbstractPatternCount ("ss_minus");
	
	addSymTabEntry(Abt);
    dotFileUses << "\"set_ss_minus2\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
    dotFileUses << "\"set\" -> \"set_ss_minus2\" [color=blue];\n";
    dotFileUses << "\"ss_minus\" -> \"set_ss_minus2\" [color=blue];\n";

  

	Abt = new AbstPattern("set_us_minus2"); Abt->addStmt(ms);Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("us_minus");
	Abt->addStmt(s);     
	addToStatTree ("", "set_us_minus2");
    incrementAbstractPatternCount ("us_minus");
	
	addSymTabEntry(Abt);
    dotFileUses << "\"set_us_minus2\" [shape=box,style=filled,color=\".3 .7 1.0\"];\n";
    dotFileUses << "\"set\" -> \"set_us_minus2\" [color=blue];\n";
    dotFileUses << "\"us_minus\" -> \"set_us_minus2\" [color=blue];\n";

  

	Abt = new AbstPattern("set_plusminus2"); Abt->addStmt(ms);Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("plusminus");
	Abt->addStmt(s);     
	addToStatTree ("", "set_plusminus2");
    incrementAbstractPatternCount ("plusminus");
	
	addSymTabEntry(Abt);

  

	Abt = new AbstPattern("set_mult2"); Abt->addStmt(ms);Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("mult");
	Abt->addStmt(s);     
	addToStatTree ("", "set_mult2");
    incrementAbstractPatternCount ("mult");
	
	addSymTabEntry(Abt);
	Abt = new AbstPattern("set_ssmult2"); Abt->addStmt(ms);Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("ssmult");
    incrementAbstractPatternCount ("ssmult");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_ssmult2");

	Abt = new AbstPattern("set_usmult2"); Abt->addStmt(ms);Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("usmult");
    incrementAbstractPatternCount ("usmult");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_usmult2");
    incrementAbstractPatternCount ("");

  

	Abt = new AbstPattern("set_ssdiv2"); Abt->addStmt(ms);Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("ssdiv");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_ssdiv2");
    incrementAbstractPatternCount ("ssdiv");

  

	Abt = new AbstPattern("set_usdiv2");Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("usdiv");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_usdiv2");
    incrementAbstractPatternCount ("usdiv");

  

	Abt = new AbstPattern("set_div2");Abt->addStmt(ms); Abt->initialized=true; 
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("div");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_div2");
    incrementAbstractPatternCount ("div");

  

	Abt = new AbstPattern("set_udiv2");Abt->addStmt(ms);Abt->initialized=true; 
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("udiv");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_udiv2");
    incrementAbstractPatternCount ("udiv");

  

	Abt = new AbstPattern("set_mod2");Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("mod");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_mod2");
    incrementAbstractPatternCount ("mod");

  

	Abt = new AbstPattern("set_umod2");Abt->addStmt(ms);Abt->initialized=true; 
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("umod");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_umod2");
    incrementAbstractPatternCount ("umod");

  

	Abt = new AbstPattern("set_smax2");Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("smax");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_smax2");
    incrementAbstractPatternCount ("smax");

  

	Abt = new AbstPattern("set_smin2");Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("smin");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_smin2");
    incrementAbstractPatternCount ("smin");

  

	Abt = new AbstPattern("set_smaxmin2");Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("smaxmin");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_smaxmin2");
    incrementAbstractPatternCount ("smaxmin");

  

	Abt = new AbstPattern("set_umax2");Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("umax");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_umax2");
    incrementAbstractPatternCount ("umax");

  

	Abt = new AbstPattern("set_umin2");Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("umin2");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_umin2");
    incrementAbstractPatternCount ("umin2");

  

	Abt = new AbstPattern("set_unsigned_float2");Abt->addStmt(ms);  Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("unsigned_float");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_unsigned_float2");
    incrementAbstractPatternCount ("unsigned_float");

  

	Abt = new AbstPattern("set_float2");Abt->addStmt(ms);  Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("float");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_float2");
    incrementAbstractPatternCount ("float");

  

	Abt = new AbstPattern("set_neg2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("neg");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_neg2");
    incrementAbstractPatternCount ("neg");

  

	Abt = new AbstPattern("set_ssneg2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("ssneg");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_ssneg2");
    incrementAbstractPatternCount ("ssneg");

  

	Abt = new AbstPattern("set_usneg2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("usneg");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_usneg2");
    incrementAbstractPatternCount ("usneg");

  

	Abt = new AbstPattern("set_absneg2");Abt->addStmt(ms);  Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("absneg");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_absneg2");
    incrementAbstractPatternCount ("absneg");

  

	Abt = new AbstPattern("set_abs2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("abs");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_abs2");
    incrementAbstractPatternCount ("abs");

  

	Abt = new AbstPattern("set_sign_extend1"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(1);
	s.addAccessTree(v);s.setPatternName("sign_extend");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_sign_extend1");
    incrementAbstractPatternCount ("sign_extend");
  

	Abt = new AbstPattern("set_sign_extend2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("sign_extend");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_sign_extend2");
    incrementAbstractPatternCount ("sign_extend");

  

	Abt = new AbstPattern("set_sign_extract2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("sign_extract");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_sign_extract2");
    incrementAbstractPatternCount ("sign_extract");

  

	Abt = new AbstPattern("set_zero_extract1"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(1);
	s.addAccessTree(v);s.setPatternName("zero_extract");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_zero_extract1");
    incrementAbstractPatternCount ("zero_extract");

  

	Abt = new AbstPattern("set_zero_extract2");Abt->addStmt(ms);  Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("zero_extract");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_zero_extract2");
    incrementAbstractPatternCount ("zero_extract");

  

	Abt = new AbstPattern("set_fe2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("float_extend");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_fe2");
    incrementAbstractPatternCount ("set_fe2");

  

	Abt = new AbstPattern("set_sqrt2");Abt->addStmt(ms);  Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("sqrt");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_sqrt2");
    incrementAbstractPatternCount ("sqrt");

  

	Abt = new AbstPattern("set_bswap2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("bswap");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_bswap2");
    incrementAbstractPatternCount ("bswap");

  

	Abt = new AbstPattern("set_ashift2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("ashift");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_ashift2");
    incrementAbstractPatternCount ("ashift");

  

	Abt = new AbstPattern("set_lshiftrt2");Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("lshiftrt");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_lshiftrt2");
    incrementAbstractPatternCount ("lshiftrt");

  

	Abt = new AbstPattern("set_ashiftrt2");Abt->addStmt(ms);  Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("ashiftrt");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_ashiftrt2");
    incrementAbstractPatternCount ("ashiftrt");

  

	Abt = new AbstPattern("set_any_shiftrt2");Abt->addStmt(ms);Abt->initialized=true;  
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("any_shiftrt");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_any_shiftrt2");
    incrementAbstractPatternCount ("any_shiftrt");

  

	Abt = new AbstPattern("set_not2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("not");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_not2");
    incrementAbstractPatternCount ("not");

  

	Abt = new AbstPattern("set_ior2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("ior");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_ior2");
    incrementAbstractPatternCount ("ior");

  

	Abt = new AbstPattern("set_and2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("and");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_and2");
    incrementAbstractPatternCount ("and");

  

	Abt = new AbstPattern("set_or2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("or");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_or2");
    incrementAbstractPatternCount ("or");

  

	Abt = new AbstPattern("set_ao2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("any_or");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_ao2");
    incrementAbstractPatternCount ("any_or");

  

	Abt = new AbstPattern("set_xor2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("xor");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_xor2");
    incrementAbstractPatternCount ("xor");

  

	Abt = new AbstPattern("set_compare2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("compare");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_compare2");
    incrementAbstractPatternCount ("compare");


    Abt = new AbstPattern("set_compare"); Abt->addStmt(ms);Abt->initialized=true;
    Abt->setExtName("set");
    v=new vector <int>;v->push_back(2);
    s.addAccessTree(v);s.setPatternName("compare");
    Abt->addStmt(s);

    addSymTabEntry(Abt);  addToStatTree ("", "set_compare");
    incrementAbstractPatternCount ("compare");


    Abt = new AbstPattern("set_if_then_else2"); Abt->addStmt(ms);Abt->initialized=true;
    Abt->setExtName("set");
    v=new vector <int>;v->push_back(2);
    s.addAccessTree(v);s.setPatternName("if_then_else");
    Abt->addStmt(s);

    addSymTabEntry(Abt);  addToStatTree ("", "set_if_then_else2");
    incrementAbstractPatternCount ("if_then_else");


    Abt = new AbstPattern("set_if_then_else"); Abt->addStmt(ms);Abt->initialized=true;
    Abt->setExtName("set");
    v=new vector <int>;v->push_back(2);
    s.addAccessTree(v);s.setPatternName("if_then_else");
    Abt->addStmt(s);

    addSymTabEntry(Abt);  addToStatTree ("", "set_if_then_else");
    incrementAbstractPatternCount ("if_then_else");

        Abt = new AbstPattern("set_ite2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("if_then_else");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_ite2");
    incrementAbstractPatternCount ("if_then_else");

    Abt = new AbstPattern("set_match_operator2"); Abt->addStmt(ms);Abt->initialized=true;
    Abt->setExtName("set");
    v=new vector <int>;v->push_back(2);
    s.addAccessTree(v);s.setPatternName("match_operator");
    Abt->addStmt(s);    

    addSymTabEntry(Abt);  addToStatTree ("", "set_match_operator2");
    incrementAbstractPatternCount ("match_operator");

    Abt = new AbstPattern("set_match_operator"); Abt->addStmt(ms);Abt->initialized=true;
    Abt->setExtName("set");
    v=new vector <int>;v->push_back(2);
    s.addAccessTree(v);s.setPatternName("match_operator");
    Abt->addStmt(s);    

    addSymTabEntry(Abt);  addToStatTree ("", "set_match_operator");
    incrementAbstractPatternCount ("match_operator");

/*
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("match_operator");
	Abt->addStmt(s);    
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_plus2");
*/
  

	Abt = new AbstPattern("set_match_op_dup2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("match_op_dup");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_match_op_dup2");
    incrementAbstractPatternCount ("match_op_dup");

  

	Abt = new AbstPattern("set_match_op_dup"); Abt->addStmt(ms); 
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("match_op_dup");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_match_op_dup");
    incrementAbstractPatternCount ("match_op_dup");

  

	Abt = new AbstPattern("set_eq2"); Abt->addStmt(ms); 
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("eq");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_eq2");
    incrementAbstractPatternCount ("eq");

  

	Abt = new AbstPattern("set_ne2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("ne");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_ne2");
    incrementAbstractPatternCount ("ne");

  

	Abt = new AbstPattern("set_lt2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("lt");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_lt2");
    incrementAbstractPatternCount ("lt");

  

	Abt = new AbstPattern("set_ltu2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("ltu");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_ltu2");
    incrementAbstractPatternCount ("ltu");

  

	Abt = new AbstPattern("set_gt2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("gt");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_gt2");
    incrementAbstractPatternCount ("gt");

  

	Abt = new AbstPattern("set_gtu2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("gtu");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_gtu2");
    incrementAbstractPatternCount ("gtu");

  

	Abt = new AbstPattern("set_ge2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("ge");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_ge2");
    incrementAbstractPatternCount ("ge");

  

	Abt = new AbstPattern("set_geu2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("geu");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_geu2");
    incrementAbstractPatternCount ("geu");

  

	Abt = new AbstPattern("set_le2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("le");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_le2");
    incrementAbstractPatternCount ("le");

  

	Abt = new AbstPattern("set_leu2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("leu");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_leu2");
    incrementAbstractPatternCount ("leu");

  

	Abt = new AbstPattern("set_mem1"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(1);
	s.addAccessTree(v);s.setPatternName("mem");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_mem1");
    incrementAbstractPatternCount ("mem");

	Abt = new AbstPattern("set_mem"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(1);
	s.addAccessTree(v);s.setPatternName("mem");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_mem");
    incrementAbstractPatternCount ("mem");

  

	Abt = new AbstPattern("set_mem2");Abt->addStmt(ms);  Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("mem");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_mem2");
    incrementAbstractPatternCount ("mem");

  

	Abt = new AbstPattern("set_srict_low_part1"); Abt->addStmt(ms);Abt->initialized=true; 
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(1);
	s.addAccessTree(v);s.setPatternName("strict_low_part");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_srict_low_part1");
    incrementAbstractPatternCount ("set_srict_low_part");

  

	Abt = new AbstPattern("set_unspec2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("unspec");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_unspec2");
    incrementAbstractPatternCount ("unspec");

  
	Abt = new AbstPattern("set_unspec"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("unspec");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_unspec");
    incrementAbstractPatternCount ("unspec");

	Abt = new AbstPattern("set_rotate2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("rotate");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_rotate2");
    incrementAbstractPatternCount ("rotate");

  

	Abt = new AbstPattern("set_call2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("call");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_call2");
    incrementAbstractPatternCount ("call");

  

	Abt = new AbstPattern("set_parity2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("parity");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_parity2");
    incrementAbstractPatternCount ("parity");

  

	Abt = new AbstPattern("set_truncate2");Abt->addStmt(ms);  Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("truncate");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_truncate2");
    incrementAbstractPatternCount ("truncate");

  

	Abt = new AbstPattern("set_sset_truncate2");Abt->addStmt(ms);  Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("sset_truncate");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_sset_truncate2");
    incrementAbstractPatternCount ("sset_truncate");

  

	Abt = new AbstPattern("set_unset_truncate2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("uset_truncate");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_unset_truncate2");
    incrementAbstractPatternCount ("uset_truncate");

  

	Abt = new AbstPattern("set_float_truncate2");Abt->addStmt(ms);  Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("float_truncate");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_float_truncate2");
    incrementAbstractPatternCount ("float_truncate");

  

	Abt = new AbstPattern("set_fix2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("fix");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_fix2");
    incrementAbstractPatternCount ("fix");

  

	Abt = new AbstPattern("set_unsigned_fix2");Abt->addStmt(ms);  Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("unsigned_fix");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_unsigned_fix2");
    incrementAbstractPatternCount ("unsigned_fix");

  

	Abt = new AbstPattern("set_vec_merge2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("vec_merge");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_vec_merge2");
    incrementAbstractPatternCount ("vec_merge");

  

	Abt = new AbstPattern("set_vec_select2");Abt->addStmt(ms);  Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("vec_select");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_vec_select2");
    incrementAbstractPatternCount ("vec_select");

  

	Abt = new AbstPattern("set_vec_concat2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("vec_concat");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_vec_concat2");
    incrementAbstractPatternCount ("vec_concat");

  

	Abt = new AbstPattern("set_vec_duplicate2");Abt->addStmt(ms);  Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("vec_duplicate");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_vec_duplicate2");
    incrementAbstractPatternCount ("vec_duplicate");

  

	Abt = new AbstPattern("set_fma2");Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("fma");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_fma2");
    incrementAbstractPatternCount ("fma");

  

	Abt = new AbstPattern("set_unspec_volatile2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("unspec_volatile");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_unspec_volatile2");
    incrementAbstractPatternCount ("unspec_volatile");

	Abt = new AbstPattern("set_zero_extract"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("zero_extract");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_zero_extract");
    incrementAbstractPatternCount ("zero_extract");

  

	Abt = new AbstPattern("set_subreg2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("subreg");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_subreg2");
    incrementAbstractPatternCount ("subreg");

  

	Abt = new AbstPattern("set_float_truncate2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("float_truncate");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_float_truncate2");
    incrementAbstractPatternCount ("float_truncate");

  

	Abt = new AbstPattern("set_zero_extract2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("zero_extract");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_zero_extract2");
    incrementAbstractPatternCount ("zero_extract");

  

	Abt = new AbstPattern("set_zero_extend2"); Abt->addStmt(ms);Abt->initialized=true; 
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("zero_extend");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_zero_extend2");
    incrementAbstractPatternCount ("zero_extend");

  

	Abt = new AbstPattern("set_vect_merge2"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("vect_merge");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_vect_merge2");
    incrementAbstractPatternCount ("vect_merge");

  
	Abt = new AbstPattern("set_strict_low_part1"); Abt->addStmt(ms); Abt->initialized=true;
	Abt->setExtName("set"); 
	v=new vector <int>;v->push_back(1);
	s.addAccessTree(v);s.setPatternName("strict_low_part");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_strict_low_part1");
    incrementAbstractPatternCount ("strict_low_part");
	

	Abt = new AbstPattern("set_mem1"); Abt->addStmt(ms);Abt->initialized=true; 
	Abt->setExtName("set");
	v=new vector <int>;v->push_back(1);
	s.addAccessTree(v);s.setPatternName("mem");
	Abt->addStmt(s);     	
	addSymTabEntry(Abt);  addToStatTree ("", "set_mem1");
    incrementAbstractPatternCount ("mem");

  	Abt = new AbstPattern("set_unspec2_compare"); Abt->initialized=true;
	Abt->setExtName("set_unspec2"); p = findSymTabEntry ("set_unspec2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("compare");
	Abt->addStmt(s);     	
	addSymTabEntry(Abt);  addToStatTree ("set_unspec2", "set_unspec2_compare");
    incrementAbstractPatternCount ("set_unspec2");
    incrementAbstractPatternCount ("compare");

  	Abt = new AbstPattern("set_unspec_compare"); Abt->initialized=true;
	Abt->setExtName("set_unspec2"); p = findSymTabEntry ("set_unspec2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("compare");
	Abt->addStmt(s);     	
	addSymTabEntry(Abt);  addToStatTree ("set_unspec2", "set_unspec_compare");
    incrementAbstractPatternCount ("set_unspec2");
    incrementAbstractPatternCount ("compare");


	Abt = new AbstPattern("set_compare2_minus1"); Abt->initialized=true;
	Abt->setExtName("set_compare2"); p = findSymTabEntry ("set_compare2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("minus");
	Abt->addStmt(s);     	
	addSymTabEntry(Abt);  addToStatTree ("set_compare2", "set_compare2_minus1");
    incrementAbstractPatternCount ("set_compare2");
    incrementAbstractPatternCount ("minus");


	Abt = new AbstPattern("set_compare_minus"); Abt->initialized=true;
	Abt->setExtName("set_compare2"); p = findSymTabEntry ("set_compare2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("minus");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_compare2", "set_compare_minus");
    incrementAbstractPatternCount ("set_compare2");
    incrementAbstractPatternCount ("minus");

  

	Abt = new AbstPattern("set_compare2_plus1"); Abt->initialized=true;
	Abt->setExtName("set_compare2");p = findSymTabEntry ("set_compare2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("plus");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_compare2", "set_compare2_plus1");
    incrementAbstractPatternCount ("set_compare2");
    incrementAbstractPatternCount ("plus");

	Abt = new AbstPattern("set_compare2_neg1"); Abt->initialized=true;
	Abt->setExtName("set_compare2");p = findSymTabEntry ("set_compare2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("neg");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_compare2", "set_compare2_neg1");
    incrementAbstractPatternCount ("set_compare2");
    incrementAbstractPatternCount ("neg");
  

	Abt = new AbstPattern("set_compare2_and1");Abt->initialized=true; 
	Abt->setExtName("set_compare2");p = findSymTabEntry ("set_compare2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("and");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_compare2", "set_compare2_and1");
    incrementAbstractPatternCount ("set_compare2");
    incrementAbstractPatternCount ("and");

  

	Abt = new AbstPattern("set_compare2_any_or1");Abt->initialized=true; 
	Abt->setExtName("set_compare2");p = findSymTabEntry ("set_compare2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("any_or");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_compare2", "set_compare2_any_or1");
    incrementAbstractPatternCount ("set_compare2");
    incrementAbstractPatternCount ("any_or");

  

	

  

	Abt = new AbstPattern("set_ite2_ne1"); Abt->initialized=true;
	Abt->setExtName("set_ite2");p = findSymTabEntry ("set_ite2");
	Abt->setParentPattern (p);
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("ite");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_ite2", "set_ite2_ne1");
    incrementAbstractPatternCount ("set_ite2");
    incrementAbstractPatternCount ("ne");

  

	
        Abt = new AbstPattern("set_zero_extend2_not1"); Abt->initialized=true;
	Abt->setExtName("set_zero_extend2");p = findSymTabEntry ("set_zero_extend2");
	Abt->setParentPattern (p);
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("not");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_zero_extend2", "set_zero_extend2_not1");
    incrementAbstractPatternCount ("set_zero_extend2");
    incrementAbstractPatternCount ("not");

  

	Abt = new AbstPattern("set_zero_extend2_ashift1"); Abt->initialized=true;
	Abt->setExtName("set_zero_extend2");p = findSymTabEntry ("set_zero_extend2");
	Abt->setParentPattern (p);	
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("ashift");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_zero_extend2", "set_zero_extend2_ashift1");
    incrementAbstractPatternCount ("set_zero_extend2");
    incrementAbstractPatternCount ("ashift");

  

	Abt = new AbstPattern("set_zero_extend2_any_shiftrt1"); Abt->initialized=true;
	Abt->setExtName("set_zero_extend2");p = findSymTabEntry ("set_zero_extend2");
	Abt->setParentPattern (p);
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("any_shiftrt");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_zero_extend2", "set_zero_extend2_any_shiftrt1");
    incrementAbstractPatternCount ("set_zero_extend2");
    incrementAbstractPatternCount ("any_shiftrt");

  

	Abt = new AbstPattern("set_zero_extend2_any_rotate1"); Abt->initialized=true;
	Abt->setExtName("set_zero_extend2");p = findSymTabEntry ("set_zero_extend2");
	Abt->setParentPattern (p);
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("any_rotate");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_zero_extend2", "set_zero_extend2_any_rotate1");
    incrementAbstractPatternCount ("set_zero_extend2");
    incrementAbstractPatternCount ("any_rotate");

	Abt = new AbstPattern("set_and2_zero_extract"); Abt->initialized=true;
	Abt->setExtName("set_and2");p = findSymTabEntry ("set_and2");
	Abt->setParentPattern (p);
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("zero_extract");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_and2", "set_and2_zero_extract");
    incrementAbstractPatternCount ("set_and2");
    incrementAbstractPatternCount ("zero_extract");

	Abt = new AbstPattern("set_and2_not1"); Abt->initialized=true;
	Abt->setExtName("set_and2");p = findSymTabEntry ("set_and2");
	Abt->setParentPattern (p);
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("not");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_and2", "set_and2_not1");
    incrementAbstractPatternCount ("set_and2");
    incrementAbstractPatternCount ("not");

  

	Abt = new AbstPattern("set_and2_neg1"); Abt->initialized=true;
	Abt->setExtName("set_and2");p = findSymTabEntry ("set_and2");
	Abt->setParentPattern (p);	
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("neg");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_and2", "set_and2_neg1");
    incrementAbstractPatternCount ("set_and2");
    incrementAbstractPatternCount ("neg");

  

	Abt = new AbstPattern("set_and2_plus1"); Abt->initialized=true;
	Abt->setExtName("set_and2");p = findSymTabEntry ("set_and2");
	Abt->setParentPattern (p);
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("plus");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_and2", "set_and2_plus1");
    incrementAbstractPatternCount ("set_and2");
    incrementAbstractPatternCount ("plus");

  
/*
	Abt = new AbstPattern("set_and2_plus1_not2");Abt->initialized=true; 
	Abt->setExtName("set_and2");p = findSymTabEntry ("set_and2");
	Abt->setParentPattern (p);
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("plus");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_and2_plus1_not2", "set_and2_plus1_not2");
*/
  

	Abt = new AbstPattern("set_ior2_plus1"); Abt->initialized=true;
	Abt->setExtName("set_ior2");p = findSymTabEntry ("set_ior2");
	Abt->setParentPattern (p);
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("plus");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_ior2", "set_ior2_plus1");
    incrementAbstractPatternCount ("set_ior2");
    incrementAbstractPatternCount ("plus");

/*
	Abt = new AbstPattern("set_ior2_plus1_not2"); Abt->initialized=true;
	Abt->setExtName("set_ior2");p = findSymTabEntry ("set_ior2");
	Abt->setParentPattern (p);
	v=new vector <int>;v->push_back(2);v->push_back(1);

	s.addAccessTree(v);s.setPatternName("plus");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_ior2_plus1_not2", "set_ior2_plus1_not2");
*/
  

	Abt = new AbstPattern("set_ior2_plus1"); Abt->initialized=true;
	Abt->setExtName("set_ior2");p = findSymTabEntry ("set_ior2");
	Abt->setParentPattern (p);
	v=new vector <int>;v->push_back(2);v->push_back(1);

	s.addAccessTree(v);s.setPatternName("plus");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_ior2", "set_ior2_plus1");
    incrementAbstractPatternCount ("set_ior2");
    incrementAbstractPatternCount ("plus");

  
/*
	Abt = new AbstPattern("set_vect_merge2_plus1_minus2"); Abt->initialized=true;
	Abt->setExtName("set_vect_merge2");p = findSymTabEntry ("set_vect_merge2");
	Abt->setParentPattern (p);
	v=new vector <int>;v->push_back(2);v->push_back(1);

	s.addAccessTree(v);s.setPatternName("plus");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("", "set_plus2");
*/
  

	Abt = new AbstPattern("set_plus2_plus1"); Abt->initialized=true;
	Abt->setExtName("set_plus2");p = findSymTabEntry ("set_plus2");
	Abt->setParentPattern (p);
	v=new vector <int>;v->push_back(2);v->push_back(1);

	s.addAccessTree(v);s.setPatternName("plus");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_plus2", "set_plus2_plus1");
    incrementAbstractPatternCount ("set_plus2");
    incrementAbstractPatternCount ("plus");

  

	Abt = new AbstPattern("set_plus2_mult1"); Abt->initialized=true;
	Abt->setExtName("set_plus2");p = findSymTabEntry ("set_plus2");
	Abt->setParentPattern (p);
	v=new vector <int>;v->push_back(2);v->push_back(1);

	s.addAccessTree(v);s.setPatternName("mult");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_plus2_mult1", "set_plus2");
    incrementAbstractPatternCount ("set_plus2");
    incrementAbstractPatternCount ("mult");

  

	Abt = new AbstPattern("set_match_operator2_eq1"); Abt->initialized=true;
	Abt->setExtName("set_match_operator2");p = findSymTabEntry ("set_match_operator2");
	Abt->setParentPattern (p);
	v=new vector <int>;v->push_back(2);v->push_back(1);

	s.addAccessTree(v);s.setPatternName("eq");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_match_operator2", "set_match_operator2_eq1");
    incrementAbstractPatternCount ("set_match_operator2");
    incrementAbstractPatternCount ("eq");

  

	Abt = new AbstPattern("set_absneg2_float_extend1");Abt->initialized=true; 
	Abt->setExtName("set_absneg2");p = findSymTabEntry ("set_absneg2");
	Abt->setParentPattern (p);
	v=new vector <int>;v->push_back(2);v->push_back(1);

	s.addAccessTree(v);s.setPatternName("float_extend");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_absneg2", "set_absneg2_float_extend1");
    incrementAbstractPatternCount ("set_absneg2");
    incrementAbstractPatternCount ("float_extend");

  

	Abt = new AbstPattern("set_ao2_zero_extend1"); Abt->initialized=true;
	Abt->setExtName("set_ao2");p = findSymTabEntry ("set_ao2");
	Abt->setParentPattern (p);
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("zero_extend");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_ao2", "set_ao2_zero_extend1");
    incrementAbstractPatternCount ("set_ao2");
    incrementAbstractPatternCount ("zero_extend");

      

	Abt = new AbstPattern("set_srict_low_part1_and1"); Abt->initialized=true;
	Abt->setExtName("set_strict_low_part1");p = findSymTabEntry ("set_strict_low_part1");
	Abt->setParentPattern (p);
	v=new vector <int>;v->push_back(1);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("and");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_srict_low_part1", "set_srict_low_part1_and1");
    incrementAbstractPatternCount ("set_srict_low_part1");
    incrementAbstractPatternCount ("and");

  

	Abt = new AbstPattern("set_zero_extract2_subreg1"); Abt->initialized=true;
	Abt->setExtName("set_zero_extract2");p = findSymTabEntry ("set_zero_extract2");
	Abt->setParentPattern (p);
	v=new vector <int>;v->push_back(2);v->push_back(1);

	s.addAccessTree(v);s.setPatternName("subreg");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_zero_extract2", "set_zero_extract2_subreg1");
    incrementAbstractPatternCount ("set_zero_extract2");
    incrementAbstractPatternCount ("subreg");

  

	Abt = new AbstPattern("set_zero_extract2_plus1"); Abt->initialized=true;
	Abt->setExtName("set_zero_extract2");p = findSymTabEntry ("set_zero_extract2");
	Abt->setParentPattern (p);
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("plus");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_zero_extract2", "set_zero_extract2_plus1");
    incrementAbstractPatternCount ("set_zero_extract2");
    incrementAbstractPatternCount ("plus");

  

	Abt = new AbstPattern("set_zero_extract2_minus1"); Abt->initialized=true;
	Abt->setExtName("set_zero_extract2");p = findSymTabEntry ("set_zero_extract2");
	Abt->setParentPattern (p);
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("minus");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_zero_extract2", "set_zero_extract2_minus1");
    incrementAbstractPatternCount ("set_zero_extract2");
    incrementAbstractPatternCount ("minus");

    /* TYPO */
    Abt = new AbstPattern("set_mult2_any_extend2"); Abt->initialized=true;
	Abt->setExtName("set_mult2");p = findSymTabEntry ("set_mult2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("mult");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_mult2", "set_mult2_any_extend2");
    incrementAbstractPatternCount ("set_mult2");
    incrementAbstractPatternCount ("any_extend");

	Abt = new AbstPattern("set_zero_extract2_plusminus1"); Abt->initialized=true;
	Abt->setExtName("set_zero_extract2");p = findSymTabEntry ("set_zero_extract2");
	Abt->setParentPattern (p);
	v->push_back(1);

	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("plusminus");
	Abt->addStmt(s);     
	
	addSymTabEntry(Abt);  addToStatTree ("set_zero_extract2", "set_zero_extract2_plusminus1");
    incrementAbstractPatternCount ("set_zero_extract2");
    incrementAbstractPatternCount ("plusminus");

	Abt = new AbstPattern("set_unspec_compare_match_operator"); Abt->initialized=true;
	Abt->setExtName("set_unspec_compare"); p = findSymTabEntry ("set_unspec_compare");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(1);v->push_back(2);
	s.addAccessTree(v);s.setPatternName("match_operator");
	Abt->addStmt(s);     	
	addSymTabEntry(Abt);  addToStatTree ("set_unspec_compare", "set_unspec_compare_match_operator");
    incrementAbstractPatternCount ("set_unspec_compare");
    incrementAbstractPatternCount ("match_operator");

	Abt = new AbstPattern("set_zero_extend2_sign_extend"); Abt->initialized=true;
	Abt->setExtName("set_zero_extend2"); p = findSymTabEntry ("set_zero_extend2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("sign_extend");
	Abt->addStmt(s);     	
	addSymTabEntry(Abt);  addToStatTree ("set_zero_extend2", "set_zero_extend2_sign_extend");
    incrementAbstractPatternCount ("set_zero_extend2");
    incrementAbstractPatternCount ("sign_extend");

	Abt = new AbstPattern("set_vec_concat2_float_truncate1");Abt->initialized=true; 
	Abt->setExtName("set_vec_concat2"); p = findSymTabEntry ("set_vec_concat2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("float_truncate");
	Abt->addStmt(s);     	
	addSymTabEntry(Abt);  addToStatTree ("set_vec_concat2", "set_vec_concat2_float_truncate1");
    incrementAbstractPatternCount ("set_vec_concat2");
    incrementAbstractPatternCount ("float_truncate");

	Abt = new AbstPattern("set_compare2_plus"); Abt->initialized=true;
	Abt->setExtName("set_compare2"); p = findSymTabEntry ("set_compare2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("plus");
	Abt->addStmt(s);     	
	addSymTabEntry(Abt);  addToStatTree ("set_compare2", "set_compare2_plus");
    incrementAbstractPatternCount ("set_compare2");
    incrementAbstractPatternCount ("plus");

	Abt = new AbstPattern("set_compare2_neg"); Abt->initialized=true;
	Abt->setExtName("set_compare2"); p = findSymTabEntry ("set_compare2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("neg");
	Abt->addStmt(s);     	
	addSymTabEntry(Abt);  addToStatTree ("set_compare2", "set_compare2_neg");
    incrementAbstractPatternCount ("set_compare2");
    incrementAbstractPatternCount ("neg");

	Abt = new AbstPattern("set_compare_not"); Abt->initialized=true;
	Abt->setExtName("set_compare2"); p = findSymTabEntry ("set_compare2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("not");
	Abt->addStmt(s);     	
	addSymTabEntry(Abt);  addToStatTree ("set_compare", "set_compare_not");
    incrementAbstractPatternCount ("");

	Abt = new AbstPattern("set_compare2_minus"); Abt->initialized=true;
	Abt->setExtName("set_compare2"); p = findSymTabEntry ("set_compare2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("minus");
	Abt->addStmt(s);     	
	addSymTabEntry(Abt);  addToStatTree ("set_compare2", "set_compare2_minus");
    incrementAbstractPatternCount ("");

	Abt = new AbstPattern("set_compare2_and"); Abt->initialized=true;
	Abt->setExtName("set_compare2"); p = findSymTabEntry ("set_compare2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("and");
	Abt->addStmt(s);     	
	addSymTabEntry(Abt);  addToStatTree ("set_compare2", "set_compare2_and");
    incrementAbstractPatternCount ("set_compare2");
    incrementAbstractPatternCount ("set_compare2_and");

	Abt = new AbstPattern("set_compare2_ashift1"); Abt->initialized=true;
	Abt->setExtName("set_compare2"); p = findSymTabEntry ("set_compare2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("ashift");
	Abt->addStmt(s);     	
	addSymTabEntry(Abt);  addToStatTree ("set_compare2", "set_compare2_ashift1");
    incrementAbstractPatternCount ("set_compare2");
    incrementAbstractPatternCount ("ashift");

	Abt = new AbstPattern("set_zero_extend2_minus1"); Abt->initialized=true;
	Abt->setExtName("set_zero_extend2"); p = findSymTabEntry ("set_zero_extend2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("minus");
	Abt->addStmt(s);     	
	addSymTabEntry(Abt);  addToStatTree ("set_zero_extend2", "set_zero_extend2_minus1");
    incrementAbstractPatternCount ("set_zero_extend2");
    incrementAbstractPatternCount ("minus");

	Abt = new AbstPattern("set_zero_extend2_minus"); Abt->initialized=true;
	Abt->setExtName("set_zero_extend2"); p = findSymTabEntry ("set_zero_extend2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("minus");
	Abt->addStmt(s);     	
	addSymTabEntry(Abt);  addToStatTree ("set_zero_extend2", "set_zero_extend2_minus");
    incrementAbstractPatternCount ("set_zero_extend2");
    incrementAbstractPatternCount ("minus");

	Abt = new AbstPattern("set_zero_extend_not"); Abt->initialized=true;
	Abt->setExtName("set_zero_extend2"); p = findSymTabEntry ("set_zero_extend2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("not");
	Abt->addStmt(s);     	
	addSymTabEntry(Abt);  addToStatTree ("set_zero_extend2", "set_zero_extend_not");
    incrementAbstractPatternCount ("set_zero_extend2");
    incrementAbstractPatternCount ("not");

	Abt = new AbstPattern("set_match_op_dup2_and1"); Abt->initialized=true;
	Abt->setExtName("set_match_op_dup2"); p = findSymTabEntry ("set_match_op_dup2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("and");
	Abt->addStmt(s);     	
	addSymTabEntry(Abt);  addToStatTree ("set_match_op_dup2", "set_match_op_dup2_and1");
    incrementAbstractPatternCount ("set_match_op_dup2");
    incrementAbstractPatternCount ("and");

	Abt = new AbstPattern("set_match_operator2_and1"); Abt->initialized=true;
	Abt->setExtName("set_match_operator2"); p = findSymTabEntry ("set_match_operator2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(2);
	s.addAccessTree(v);s.setPatternName("and");
	Abt->addStmt(s);     	
	addSymTabEntry(Abt);  addToStatTree ("set_match_operator2", "set_match_operator2_and1");
    incrementAbstractPatternCount ("set_match_operator2");
    incrementAbstractPatternCount ("and");

	Abt = new AbstPattern("set_strict_low_part_and2"); Abt->initialized=true;
	Abt->setExtName("set_strict_low_part1"); p = findSymTabEntry ("set_strict_low_part1");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("and");
	Abt->addStmt(s);     	
	addSymTabEntry(Abt);  addToStatTree ("set_strict_low_part", "set_strict_low_part_and2");
    incrementAbstractPatternCount ("set_strict_low_part");
    incrementAbstractPatternCount ("and");

	Abt = new AbstPattern("set_zero_extract1_and2"); Abt->initialized=true;
	Abt->setExtName("set_zero_extract1"); p = findSymTabEntry ("set_zero_extract1");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);
	s.addAccessTree(v);s.setPatternName("and");
	Abt->addStmt(s);     	
	addSymTabEntry(Abt);  addToStatTree ("set_zero_extract1", "set_zero_extract1_and2");
    incrementAbstractPatternCount ("set_zero_extract1");
    incrementAbstractPatternCount ("and");

	Abt = new AbstPattern("set_if_then_else2_ne1");Abt->initialized=true; 
	Abt->setExtName("set_if_then_else2"); p = findSymTabEntry ("set_if_then_else2");
	Abt->setParentPattern (p); 
	v=new vector <int>;v->push_back(2);v->push_back(1);
	s.addAccessTree(v);s.setPatternName("ne");
	Abt->addStmt(s);     	
	addSymTabEntry(Abt);  addToStatTree ("set_if_then_else2", "set_if_then_else2_ne1");
    incrementAbstractPatternCount ("set_if_then_else2");
    incrementAbstractPatternCount ("ne");


}
