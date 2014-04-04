#include "concrete_pattern.hh"
#include "abstract.hh"
#include "node.hh"
#include "srtl-driver.hh"

void ConcretePattern::addOperand(const Operand o){
    operandsIn.push_back(o);
}

void ConcretePattern::addOperands(const std::vector<Operand> ol){
    operandsIn.insert(operandsIn.end(), ol.begin(), ol.end());
}

void ConcretePattern::addModeStmt(const ModeStmt ms){
    modeStmtsIn.push_back(ms);
}

void ConcretePattern::addModeStmts(const std::vector<ModeStmt> msv){
    modeStmtsIn.insert(modeStmtsIn.end (), msv.begin (), msv.end ());
}

void ConcretePattern::addOperandOut(const Operand o){
    operandsOut.push_back(o);
}

void ConcretePattern::addModeStmtOut(const ModeStmt ms){
    modeStmtsOut.push_back(ms);
}

void ConcretePattern::addOperandsOut (const std::vector<Operand> ol) {
    operandsOut.insert (operandsOut.end (), ol.begin (), ol.end ());
}

void ConcretePattern::addModeStmtsOut (const std::vector<ModeStmt> msv) {
    modeStmtsOut.insert (modeStmtsOut.end (), msv.begin (), msv.end ());
}

/**
 * createPattern creates a tree structure
 * based on the parent abstract pattern in case of instantiates
 * or concrete pattern in case of overrides.
 *
 * Different types of cases that are required to be considered
 * in createPattern:
 *
 * 1. It is a pattern which instantiates another pattern.
 * 2. It is a pattern which overrides another pattern.
 * 3. It is a composite pattern which instantiates OR overrides another
 *      pattern.
 *
 * So now we have the following cases:
 * 1. insn instantiates.
 * 2. insn overrides.
 * 3. expand instantiates.
 * 4. expand overrides.
 * 5. peep2 instantiates instantiates.
 * 6. peep2 instantiates overrides
 * Cases 5 and 6 also hold true for splits and insn_and_split.
 * 
 * The above cases can be done using 3 different variables.
 * The first member variable should be pattern_type.
 *  The value of should be one of the following:
 *      1. insn.
 *      2. expand.
 *      3. peep2.
 *      4. split.
 *      5. insn_and_split.
 * The second and third variables would have similar behavior / uses.
 * Second variable will specify the instantiates / overrides nature of the "IN"
 * part of the pattern. This variable will also describe the behaviour of 
 * non composite patterns like insn and expand.
 *  The value of the this variable should be one of the following:
 *      1. true.
 *      2. false.
 *
 * The third variable will specify the instantiates / overrides nature of the 
 * "OUT" part of the patterns. This will only be used for peep2, splits and 
 * insn_and_splits. * This will remain unused for insn and expand. The values 
 * and the behavior of the variable is similar to the second variable.
 *
 */

void ConcretePattern::createPatternOut () {
    ConcretePattern* cp = 0;
    AbstPattern* ap = 0;

    if (subTypeOut == overrides) {
        cp = static_cast <ConcretePattern*> (parentPatternOut);
        Pattern *tp = cp->getParentPatternOut ();
        ap = static_cast <AbstPattern*> (tp);
        setParentPatternOut (dynamic_cast <Pattern*> (ap));
    } else if (subTypeOut == instantiates) {
        ap = static_cast <AbstPattern*> (parentPatternOut);
    }
    
    std::vector<ModeStmt>* ms = ap->getStmts ();

    if (subTypeOut == overrides) {
        vector <Operand> ops = cp->getOperandsOut ();
        if (ops.size () != 0) {
            operandsOut.insert (operandsOut.begin (), ops.begin (), 
                ops.end ());
        }

        if (cp->getModeStmts ().size () != 0 ) {
            vector<ModeStmt> mss = cp->getModeStmtsOut ();
            ms->insert (ms->end (), mss.begin (), mss.end ());
        }
    }
    
    for (unsigned int i = 0; i < ms->size (); i++) {
        if (ms->at (i).getType () == ModeStmt::root) {
            if (i == 0) {
                treeOut = new Node (ms->at(i).getPatternName (), false);
            } else {
                Node* iNode = new Node (ms->at(i).getPatternName (), false);
                treeOut->replaceNode (iNode, ms->at(i).getAccessTree ());
            }
        }
    }

    for (unsigned int i = 0; i < ms->size (); i++) {
        if (ms->at(i).getType () == ModeStmt::mode) {
            treeOut->replaceMode (ms->at(i).getMode (), 
                ms->at (i).getAccessTree ());
        }
    }

    int pmcCount = 0;
    for (unsigned int i = 0; i < operandsOut.size (); i++) {
        if (operandsOut[i].getType () == Operand::pmc || 
                operandsOut[i].getType () == Operand::matchOperator) {
            if (operandsOut[i].getOperandNumber () < 0) {
                operandsOut[i].setOperandNumber (pmcCount);
                pmcCount++;
            } else {
                pmcCount++;
            }
        }
    }
    for (unsigned int i = 0; i < modeStmtsOut.size (); i++) {
        if (modeStmtsOut[i].getType() == ModeStmt::allConstraints 
            && subType == overrides){
            int k = 0;
            for (unsigned int j = 0; j < operandsOut.size (); j++) {
                if (operandsOut[j].getType () == Operand::pmc ||
                    operandsIn[j].getType () == Operand::scratch) {
                    operandsOut[j].setConstraint(
                        modeStmtsOut[i].getConstraint (k));
                    k++;
                }
            }
        } else if (modeStmtsOut[i].getType () == ModeStmt::replaceMode 
            && subType == overrides) {
            for (unsigned int j = 0; j < operandsOut.size (); j++) {
                if (operandsOut[j].getType () == Operand::pmc) {
                    if (modeStmtsOut[i].getMode () == 
                        operandsOut[j].getMode ()) {
                        operandsOut[j].setMode (
                            modeStmtsIn[i].getModeReplacement ());
                    }
                }
            }
        } else if (modeStmtsOut[i].getType () == ModeStmt::mode) {
            treeOut->replaceMode (modeStmtsOut[i].getMode (), 
                modeStmtsOut[i].getAccessTree ());
        } else if (modeStmtsOut[i].getType () == ModeStmt::pmc) {
            treeOut->replacePmc (modeStmtsOut[i].getOperand (), 
                modeStmtsOut[i].getAccessTree ());
        } else if (modeStmtsOut[i].getType () == ModeStmt::operand) {
            treeOut->replacePmc (modeStmtsOut[i].getOperand (), 
                modeStmtsOut[i].getAccessTree ());
        } else if (modeStmtsOut[i].getType () == ModeStmt::predicate) {
            treeOut->replacePredicate (modeStmtsOut[i].getOperand (), 
                modeStmtsOut[i].getAccessTree ());
        } else if (modeStmtsOut[i].getType () == ModeStmt::constraint) {
            treeOut->replaceConstraint (modeStmtsOut[i].getOperand(), 
                modeStmtsIn[i].getAccessTree ());
        }
    }

    if(type > 2) {
        srtl_driver::outPattern += "[ ";
    }

    vector<Operand>* newVector = new vector<Operand> (operandsOut);
    treeOut->traverseTree (newVector);
    delete newVector;

    srtl_driver::outPattern += "]";

    if (!cmdBodyOut.empty ()) {
        srtl_driver::outPattern += cmdBodyOut;
    }
    
    srtl_driver::outPattern += ")\n\n";
    return;
} 

void ConcretePattern::createPattern (){
    ConcretePattern* cp = 0;
    AbstPattern* ap = 0;
    
    if (subType == overrides) {
        cp = static_cast <ConcretePattern*> (parentPattern);
        Pattern *tp = cp->getParentPattern ();
        ap = static_cast <AbstPattern*> (tp);
        setParentPattern (dynamic_cast <Pattern*> (ap));
    } else if (subType == instantiates) {
        ap = static_cast <AbstPattern*> (parentPattern);
    }

    std::vector<ModeStmt>* ms = ap->getStmts ();
    
    if (subType == overrides) {
        vector <Operand> ops = cp->getOperands ();
        if (ops.size () != 0) {
            operandsIn.insert (operandsIn.begin (), ops.begin (), ops.end ());
        }
        
        if (cp->getModeStmts ().size () != 0) {
            vector<ModeStmt> mss = cp->getModeStmts ();
            // ms->insert (ms->end (), mss.begin (), mss.end ());
            modeStmtsIn.insert (modeStmtsIn.begin (), mss.begin (), mss.end ());
        }
    }

    if (type != regCons) {
        for (unsigned int i = 0; i < ms->size (); i++) {
            if(ms->at (i).getType () == ModeStmt::root) {
                if (i == 0) {
                    int children = srtl_driver::findRtlOperandSymTabEntry (
                        ms->at (i).getPatternName ());
                    if (children < 0) {
                        srtl_driver::debugS (srtl_driver::err, 
                            "For Abst Pattern: " + ap->getPatName ());
                        srtl_driver::debugS (srtl_driver::err, 
                            "Invalid RTL Operand: " + 
                            ms->at (i).getPatternName ());
                        return;
                    }
                    tree = new Node (ms->at (i).getPatternName (), false);
                } else {
                    Node* iNode = new Node (ms->at (i).
                        getPatternName (), false);
                    int children = srtl_driver::findRtlOperandSymTabEntry (
                        ms->at (i).getPatternName ());
                    if (children < 0) {
                        srtl_driver::debugS (srtl_driver::err, 
                            "For Abst Pattern: " + ap->getPatName ());
                        srtl_driver::debugS (srtl_driver::err, 
                            "Invalid RTL Operand: " + 
                            ms->at (i).getPatternName ());
                        delete iNode;
                        delete tree;
                        return;
                    }
                    tree->replaceNode (iNode, ms->at (i).getAccessTree ());
                }
            }
        }
        
        std::string replaceTo;
        std::string replaceFrom;

        for (unsigned int i = 0; i < modeStmtsIn.size (); i++) {
            if (modeStmtsIn[i].getType () == ModeStmt::replaceMode 
                && subType == overrides) {
                replaceTo = modeStmtsIn[i].getModeReplacement ();
                replaceFrom = modeStmtsIn[i].getMode ();
            }
        }

        for (unsigned int i = 0; i < ms->size (); i++) {
            if (ms->at(i).getType() == ModeStmt::mode){
                if (ms->at(i).getMode () == replaceFrom) {
                    tree->replaceMode (replaceTo, ms->at(i).getAccessTree ());
                } else {
                    tree->replaceMode (ms->at(i).getMode (), 
                        ms->at(i).getAccessTree ());
                }
            }
        }

        int pmcCount = 0;
        for (unsigned int i = 0; i < operandsIn.size (); i++) {
            if (operandsIn[i].getType () == Operand::pmc || 
                    operandsIn[i].getType () == Operand::matchOperator) {
                if (operandsIn[i].getOperandNumber () < 0) {
                    operandsIn[i].setOperandNumber (pmcCount);
                    pmcCount++;
                } else {
                    pmcCount++;
                }
            }
        }

        for (unsigned int i = 0; i < modeStmtsIn.size (); i++) {
            if (modeStmtsIn[i].getType() == ModeStmt::allConstraints 
                && subType == overrides){
                int k = 0;
                for (unsigned int j = 0; j < operandsIn.size (); j++) {
                    if (operandsIn[j].getType () == Operand::pmc ||
                        operandsIn[j].getType () == Operand::scratch) {
                    /**
                     * This replaces *all* of the constraints in the particular
                     * constraints. 
                     **/
                        operandsIn[j].setConstraint(
                            modeStmtsIn[i].getConstraint (k));
                        k++;
                    } 
                }
            } else if (modeStmtsIn[i].getType () == ModeStmt::replaceMode 
                && subType == overrides) {
                for (unsigned int j = 0; j < operandsIn.size (); j++) {
                    if (operandsIn[j].getType () == Operand::pmc || 
                            operandsIn[j].getType () == Operand::reg) {
                        if (modeStmtsIn[i].getMode () == 
                                operandsIn[j].getMode ()) {
                /**
                 * This mode replacement is for the replacement of *all* of the 
                 * modes in the particular construct.
                 **/
                            operandsIn[j].setMode (
                                modeStmtsIn[i].getModeReplacement ());
                        }
                    }
                }
            } else if (modeStmtsIn[i].getType() == ModeStmt::mode){
                if (modeStmtsIn[i].getMode () == replaceFrom) 
                    tree->replaceMode (replaceTo, 
                        modeStmtsIn[i].getAccessTree ());
                else
                    tree->replaceMode (modeStmtsIn[i].getMode (), 
                        modeStmtsIn[i].getAccessTree ());
            } else if (modeStmtsIn[i].getType () == ModeStmt::pmc) {
                tree->replacePmc (modeStmtsIn[i].getOperand (), 
                    modeStmtsIn[i].getAccessTree ());
            } else if (modeStmtsIn[i].getType () == ModeStmt::operand) {
                tree->replacePmc (modeStmtsIn[i].getOperand (), 
                    modeStmtsIn[i].getAccessTree ());
            } else if (modeStmtsIn[i].getType () == ModeStmt::predicate) {
                tree->replacePredicate (modeStmtsIn[i].getOperand (), 
                    modeStmtsIn[i].getAccessTree ());
            } else if (modeStmtsIn[i].getType () == ModeStmt::constraint) {
                tree->replaceConstraint (modeStmtsIn[i].getOperand (), 
                    modeStmtsIn[i].getAccessTree ());
            }
        }
    }
    srtl_driver::outPattern.clear ();
    srtl_driver::outPattern += "(define_";

    if(type == insn) {
        srtl_driver::outPattern += "insn ";
    } else if (type == exp) {
        srtl_driver::outPattern += "expand ";
    } else if (type == peep2) {
        srtl_driver::outPattern += "peephole2 ";
    } else if (type == split) {
        srtl_driver::outPattern += "split ";
    } else if (type == insnAndSplit) {
        srtl_driver::outPattern += "insn_and_split ";
    } else if (type == insnReservation) {
        srtl_driver::outPattern += "insn_reservation ";
    } else if (type == predicate) {
        srtl_driver::outPattern += "predicate ";
    } else if (type == specialPredicate) {
        srtl_driver::outPattern += "special_predicate ";
    } else if (type == attr) {
        srtl_driver::outPattern += "attr ";
    } else if (type == asmAttr) {
        srtl_driver::outPattern += "asm_attributes ";
    } else if (type == cons) {
       srtl_driver::outPattern += "constraint ";
    } else if (type == regCons) {
        srtl_driver::outPattern += "register_constraint ";
    } else if (type == memCons) {
        srtl_driver::outPattern += "memory_constraint ";
    } else if (type == addCons) {
        srtl_driver::outPattern += "address_constraint ";
    }
    
    if (type == insnReservation) {
        srtl_driver::outPattern += "\"" + name + "\" ";
        for (unsigned int i = 0; i < modeStmtsIn.size (); i++) {
            if (modeStmtsIn[i].getType () == ModeStmt::latency){
                srtl_driver::outPattern += modeStmtsIn[i].getMode () + " ";
            }
        }
        srtl_driver::outPattern += "\n\t\t";
    } else if (type == predicate || type == specialPredicate || 
          type == attr || type == asmAttr || type == cons ||
          type == regCons || type == memCons || type == addCons) {
        srtl_driver::outPattern += "\"" + name + "\"\n ";
        if (type == cons || type == regCons || type == memCons ||
                type == addCons) {
            string docstring = "";
            string regclass = "";
            for (unsigned int i = 0; i < modeStmtsIn.size (); i++) {
                if (modeStmtsIn[i].getType () == ModeStmt::docstring){
                    docstring += modeStmtsIn[i].getMode ();
                }
                if (modeStmtsIn[i].getType () == ModeStmt::regclass) {
                    regclass += modeStmtsIn[i].getMode ();
                }
            }
            if (type == regCons)
                srtl_driver::outPattern += " " + regclass + " ";
            srtl_driver::outPattern += "\n " + docstring;
        } else if (type == attr) {
            string listOfValues = "";
            for (unsigned int i = 0; i < modeStmtsIn.size (); i++) {
                if (modeStmtsIn[i].getType () == ModeStmt::listOfValues)
                    listOfValues = modeStmtsIn[i].getMode ();
            }
            if (!listOfValues.empty ())
                srtl_driver::outPattern += " " + listOfValues + "\n\t";
        }
    } else if (type == split || type == peep2) {
        srtl_driver::outPattern += "\n[";
    } else {
        srtl_driver::outPattern += "\"" + name + "\"\n[";
    }

    if (type != regCons) {
    vector<Operand>* newVector = new vector<Operand> (operandsIn);
    tree->traverseTree (newVector);
    delete newVector;
    }
    
    if (type != insnReservation && type != predicate && 
            type != specialPredicate &&
            type != attr && type != asmAttr && type != cons &&
            type != regCons && type != memCons && type != addCons)
        srtl_driver::outPattern += "]";

    if (!cmdBody.empty ()) {
        if (type == predicate || type == specialPredicate) {
            srtl_driver::outPattern += "\n{";
        }
        srtl_driver::outPattern += cmdBody;
        if (type == predicate || type == specialPredicate) {
            srtl_driver::outPattern += "}";
        }
    }

    if (type < 3 || type == insnReservation ||
            type == predicate || type == specialPredicate ||
            type == attr || type == asmAttr || type == cons ||
            type == regCons || type == memCons || type == addCons) {
        srtl_driver::outPattern += ")\n\n";
    }
    return;
}

ConcretePattern::~ConcretePattern () {
}
