/*--------------------------------------------------------------------------- *
 * srtl implementation in c++                                                 *
 *                                                                            *
 * This file contains class header definitions for abstract pattern class     *
 *                                                                            *
 *                                                                            *
 *                                                                            *
 * Change History                                                             *
 * -------------------------------------------------------------------------- *
 * Name        Date          Change Description                               *
 * Sheweta     14-Aug-13     Initial Version                                  *
 *                                                                            *
 *--------------------------------------------------------------------------- */
#ifndef __PATTERN_H_INCLUDED__
#define __PATTERN_H_INCLUDED__
#include <string>
#include <vector>
#include <map>
#include <string>
#include <iostream>



/**
 * Operands can be of # types. The number below represents the
 * value of the type member variable of Operand of the respective 
 * operand object.
 *  1. PMC Spec         :- Has a predicate, mode and a constraint.
 *  2. Dup Num Spec     :- Is a match_dup.
 *  3. Num Spec         :- 
 *  4. Reg Spec         :-
 *  5. Reg Num Spec     :-
 *  6. Extra Reg Spec   :- pc, cc0, scratch, return, eh_return
 *  7. Const Spec       :- const_int 
 *  8. Scratch Spec     :- match_scratch
 *  9. eq_attr          :- For use in secondary tree patterns.
 *  10.match_operator   :- (match_operator 1 asdasd [Operands])
 *  11.fixed_reg        :- [A-Z]*[0-9]*_REG
 *  12. integer         :- <0> - 0
 *  13. string          :- <UNSPEC_XXX>
 **/
/*
 * Consider chaning member variable names to name1, name2, name3
 * instead of predicate, mode or constraint, to make this class more generic to
 * other operand types.
 */

class Operand {
    public:
        enum Type {pmc, dup, num, reg, regNum, extraReg, constSpec,
            scratch, eqAttr, matchOperator, fixedReg, integer, string,
            bracketOpen, bracketClose};
    private:
        Type type;
        int operandNumber;
        unsigned long int number;
        std::string predicate;
        std::string mode;
        std::string constraint;
        std::string misc; /* PlaceHolder for fixed_reg / Const */

//        bool bClose;

    public:
        Operand (Type t, int n, std::string p, std::string m, std::string c);
        Operand (Type t, std::string p, std::string m, std::string c);
        Operand (Type t, unsigned long int n);
        Operand (Type t, int n, std::string p_m, std::string c);
        Operand (Type t, int n, std::string p_m);
        Operand (Type t, std::string p_m, std::string c);
        Operand (Type t, std::string p_m);
        Operand () {}
        ~Operand ();
        std::string getOperand (); 
        Type getType () { return type; }

        std::string getMode () { return mode; }
        std::string getPredicate () { return predicate; }
        std::string getConstraint () { return constraint; }
        void setMode (std::string m) { mode = m; }
        void setPredicate (std::string p) { predicate = p; }
        void setConstraint (std::string c) { constraint = c; }
        void setType (Type t) { type = t; }
 /*       bool getBClose () { return bClose; }
        void setBClose (bool _bClose) {
            bClose = _bClose;
        }
*/      int getOperandNumber () { return operandNumber; }
        void setOperandNumber (int o) { operandNumber = o; }
        unsigned long int getNumber () { return number; }
        void setNumber (unsigned long int n) { number = n; }
};

class ModeStmt {
    private:
        std::vector <int> accessTree;
        std::string predicateOrMode;
        std::string patternName;
        std::string modeReplacement;

        std::vector <std::string> constraintsVector;

        Operand op;
    public:
        enum Type {root, latency, listOfValues, docstring, regclass, mode
            , predicate, constraint, operand, allConstraints, replaceMode, pmc};
       
        ~ModeStmt () {
        }

        void addAccessTree (std::vector<int>* at);
        void setMode (std::string mode);
        void setPatternName (std::string name) {
            patternName = name;
        }
        std::string getPatternName (){
            return patternName;
        }
        std::vector<int>* getAccessTree () {
            return &accessTree;        }
        std::string getMode () {
            return predicateOrMode;
        }
        void setType (Type t) {
            type = t;
        }
        Type getType () {
            return type;
        }

        Operand getOperand () {
            return op;
        }

        void setOperand (std::string p, std::string m, std::string c) {
            op.setPredicate (p);
            op.setMode (m);
            op.setConstraint (c);
            op.setType (Operand::pmc);
        }
        
        void setOperand (int opNumber, std::string p, std::string m, std::string c) {
            op.setPredicate (p);
            op.setMode (m);
            op.setConstraint (c);
            op.setType (Operand::pmc);
            op.setOperandNumber (opNumber);
            std::cout << opNumber << std::endl;
        }
        
        void setOperand (Operand o) {
            op = o;
        }

        void setConstraints (std::vector<std::string>* c) {
            constraintsVector = *c;
        }

        std::string getConstraint (unsigned int i) {
            if (i >= constraintsVector.size ()) {
                return "";
            }
            return constraintsVector[i];
        }

        void setModeReplacement (std::string mr) {
            modeReplacement = mr;
        }

        std::string getModeReplacement () {
            return modeReplacement;
        }
            
    private:
        Type type;
};

class Pattern {
public:
    enum Type {abstract, insn, exp, insnAndSplit, split, peep2, insnReservation, 
        predicate, specialPredicate, attr, asmAttr, cons, regCons, memCons, addCons,
        define_code_iterator, define_mode_iterator,define_code_attr,define_mode_attr, 
        define_constants,define_c_enum,define_asm_attributes,define_reservation,
        define_cpu_unit,define_automaton,define_bypass     
    };

    enum SubType {extends, instantiates, overrides};

    virtual std::string getPatName()=0;
    virtual void setPatName(std::string V)=0;
    
    virtual void setPatType (Type pt) = 0;
    virtual Type getPatType () = 0;
    
    virtual void createPattern () {}
    virtual void createPatternOut () {}

    virtual bool inError () = 0;
    virtual void setError () = 0;
    virtual void setParentPattern (Pattern* ) {};
    virtual Pattern* getParentPattern () { return 0; }

    virtual void setSubType (SubType st) { subType = st; }
    virtual void setSubTypeOut (SubType st) {}
    virtual SubType getSubType () { return subType; }
 
    // TODO
    virtual SubType getSubTypeOut () { return subType; }
 
    virtual ~Pattern () {}

protected:
    Type type;
    SubType subType;
};
#endif
