/*--------------------------------------------------------------------------- *
 * srtl implementation in c++                                                 *
 *                                                                            *
 * This file contains class header definitions for the concrete pattern class *
 *                                                                            *
 *                                                                            *
 * concrete_pattern.hh                                                        *
 * Change History                                                             *
 * -------------------------------------------------------------------------- *
 * Name        Date          Change Description                               *
 * Aniket      14-Aug-13     Initial Version                                  *
 *                                                                            *
 *--------------------------------------------------------------------------- */

#ifndef __CPATTERN_H_INCLUDED__
#define __CPATTERN_H_INCLUDED__

#include <vector>

#include "node.hh"
#include "pattern.hh"

/**
 * Different types of concrete patterns are 
 * denoted by the member variable type.
 * The value of the variable denotes the following.
 * 1. Insn
 * 2. Expand
 * 3. peephole2
 * 4. split
 * 5. insn_and_split
 ***/

class ConcretePattern: public Pattern {
private:
    std::string name;
    
    /*
     * The following two variables describe whether the in / out parts of 
     * the construct are instantiates or overrides. 
     * Do not use these variable directly. Use the methods 
     * isInstantiates / isOverrides or setInstantiates / setOverrides.
     */

    SubType subTypeOut;

    std::vector<Operand> operandsIn;
    std::vector<ModeStmt> modeStmtsIn;
    std::vector<Operand> operandsOut; /* This is only required for peep2, splits & insn_split */
    std::vector<ModeStmt> modeStmtsOut;

    Pattern* parentPattern;
    Pattern* parentPatternOut;

    /*
     * This is the structure that stores 
     * the tree structure. It is constructed based on 
     * the abstract patterns.
     */
    Node* tree;
    Node* treeOut;
    
    bool error;

    std::string cmdBody;
    std::string cmdBodyOut;
public:
    ConcretePattern (Type pt) {
        error = false;
        type = pt;
        name = "";
    }

    ConcretePattern (std::string V, Type t) {
        name = V;
        type = t;
        error = false;
    }

    ~ConcretePattern ();

    std::string getPatName () {
        return name;
    }

    void setPatName (std::string V) {
        name = V;
    }

    Type getPatType () {
        return type;
    }

    void setPatType (Type t) {
        type = t;
    }

    void setError(){
        error = true;
    }

    bool inError(){
        return error;
    }
    
    void setParentPattern (Pattern* p){
        parentPattern = p;
    }

    void setParentPatternOut (Pattern* p) {
        parentPatternOut = p;
    }

    Pattern* getParentPattern () {
        return parentPattern;
    }

    Pattern* getParentPatternOut () {
        return parentPatternOut;
    }

    std::vector<Operand> getOperands () {
        return operandsIn;
    }

    std::vector<Operand> getOperandsOut () {
        return operandsOut;
    }

    void setCmdBody (std::string str) {
        cmdBody = str;
    }

    void setCmdBodyOut (std::string str) {
        cmdBodyOut = str;
    }

    std::vector<ModeStmt> getModeStmts () {
        return modeStmtsIn;
    }

    std::vector<ModeStmt> getModeStmtsOut () {
        return modeStmtsOut;
    }

    void setSubTypeIn (SubType st) {
        subType = st;
    }

    void setSubTypeOut (SubType st) {
        subTypeOut = st;
    }

    SubType getSubTypeIn () {
        return subType;
    }

    SubType getSubTypeOut () {
        return subTypeOut;
    }

    void addOperand (const Operand o);
    void addOperands (const std::vector<Operand> ol);
    void addModeStmt (const ModeStmt ms);
    void addModeStmts (const std::vector<ModeStmt> msv);
    void addOperandOut (const Operand o);
    void addModeStmtOut (const ModeStmt ms);
    void addOperandsOut (const std::vector<Operand> ol);
    void addModeStmtsOut (const std::vector<ModeStmt> msv);
    void createPattern ();
    void createPatternOut ();
};
#endif