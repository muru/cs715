#include <iostream>
#include <iterator>

#include "srtl-driver.hh"
#include "node.hh"
#include "pattern.hh"

/* Sample Abstract Pattern
 * abstract bb extends set {
 *  root.2:=plus;
 *  root.2.1:=minus;
 *  root.2.2:=minus;
 * }
 */

/*
 * This pattern string should go in srtl-driver 
 */

std::string srtl_driver::outPattern;
const std::string Node::DUMMYNODE = "___";

Node::Node (std::string n, bool ln) {
    name = n;
    leafNode = ln;

    mode = "";

    int children = srtl_driver::findRtlOperandSymTabEntry (n);

    for (int i = 0; i < children; i++){
        Node* dummy = new Node (DUMMYNODE, true);
        addChild (dummy);
    }
}

Node::Node (std::string n, std::string m, bool ln ) {
    name = n;
    mode = m;
    leafNode = ln;

    int children = srtl_driver::findRtlOperandSymTabEntry (n);

    for (int i = 0; i < children; i++) {
        Node* dummy = new Node (DUMMYNODE, true);
        addChild (dummy);
    }
}

Node::~Node () {
    for (unsigned int i = 0; i < children.size (); i++){
       // delete children[i];
    }
    children.clear ();
}

void Node::addChild (Node* node) {
    children.push_back (node);
}

void Node::replaceNode (Node* node, std::vector<int>* position){
    if(position->size () == 1) {
        replaceSingleNode (node, position->at (0));
        return;
    }

    Node* n = children[position->at (0) - 1];

    for (unsigned int i = 1; i < position->size () - 1; i++ ) {
        n = n->children[position->at (i) - 1]; 
    }
    n->replaceSingleNode (node, position->at (position->size () -1));
}

void Node::replaceMode (std::string mode, std::vector<int>* position) {
    if(position->size () == 1) { 
        Node* n = children[position->at (0) - 1];
        n->setMode (mode);
        return;
    }

    Node* n = children[position->at (0) - 1];

    for (unsigned int i = 1; i < position->size (); i++) {
        n = n->children[position->at (i) - 1];
    }
    n->setMode (mode);
}

void Node::replacePmc (Operand op, std::vector<int>* position) {
    if (position->size () == 1) {
        PmcMapping pm(position->at(0), PmcMapping::pmc, op);
        addPmcReplacement (pm);
        return;
    } else {
        Node* n = children[position->at (0) - 1];
        position->erase (position->begin ());
        n->replacePmc (op, position);
    }
}

void Node::replacePredicate (Operand op, std::vector<int>* position) {
    if (position->size () == 1) {
        PmcMapping pm (position->at (0), PmcMapping::predicate, op);
        addPmcReplacement (pm);
        return;
    } else {
        Node* n = children[position->at(0) - 1];
        position->erase (position->begin ());
        n->replacePredicate (op, position);
    }
}

void Node::replaceConstraint (Operand op, std::vector<int>* position) {
    if (position->size () == 1) {
        PmcMapping pm (position->at (0), PmcMapping::constraint, op);
        addPmcReplacement (pm);
        return;
    } else {
        Node* n = children[position->at(0) - 1];
        position->erase (position->begin ());
        n->replaceConstraint (op, position);
    }
}

void Node::replaceSingleNode(Node* node, int position) {
    std::vector<Node*>::iterator it = children.begin ();
    std::advance (it,position - 1);
    delete children[position - 1];
    children.erase (it);
    children.insert (it, 1,node);
}

/* Stub */
void Node::printChildren () {
}

/* This method should be entirely identical to print_pattern_tree function
 * in the old srtl compiler file srtl-symtab.h  */
int Node::traverseTree (std::vector<Operand>* operands, Node* tn) {
    int returnValue = 0;

    if (tn == 0) 
        tn = this;

    if (!tn->isLeafNode ()) {
        if (name != "sequence" && name != "match_operator")
            srtl_driver::outPattern += "(" + name;

        if (!mode.empty ()){
            if (name == "match_operator" || name == "match_op_dup")
                operands->at(0).setMode (mode);
            else
                srtl_driver::outPattern += ":" + mode + " ";
        }
        else 
            srtl_driver::outPattern += " ";

        if (name == "unspec" || name == "parallel" || name == "match_op_dup"
                 || name == "unspec_volatile" || name == "cond") { 
            if (name == "match_op_dup") {
                srtl_driver::outPattern += " " + srtl_driver::itos (
                    operands->at(0).getNumber ()) + " [";
                operands->erase (operands-> begin ());
            } else {
                srtl_driver::outPattern += "[";
            }
        }

        for (unsigned int i = 0; i < tn->children.size (); i++){
            if (tn->name == "cond" && i == 1) {
                srtl_driver::outPattern += "]";
            }
            
            Node* tnu = tn->children[i];
            if (operands->size () != 0 && operands->at(0).getType()  == 
                Operand::bracketOpen) {
                    i--;
                    operands->erase (operands->begin ());
                    continue;
                }
            if (pmcReplacements.size () != 0) {
                for (unsigned int j = 0; j < pmcReplacements.size (); j++) {
                    if ((pmcReplacements[j].getChildLocation () - 1) == i) {
                        Operand tO = pmcReplacements[j].getOperand ();
                        if (tO.getType () == Operand::constSpec) {
                            operands->at(0).setNumber (tO.getNumber ());
                        } else {
                            operands->at(0).setType ( tO.getType ());
                            if (pmcReplacements[j].getType () == 
                                PmcMapping::pmc ||
                                    pmcReplacements[j].getType () == 
                                    PmcMapping::mode)
                                operands->at(0).setMode (tO.getMode ());
                            if (pmcReplacements[j].getType () == 
                                PmcMapping::pmc || pmcReplacements[j].
                                getType () == PmcMapping::predicate)
                                operands->at(0).setPredicate (
                                    tO.getPredicate ());
                            if (pmcReplacements[j].getType () == 
                                PmcMapping::pmc ||
                                    pmcReplacements[j].getType () == 
                                    PmcMapping::constraint)
                                operands->at(0).setConstraint (
                                    tO.getConstraint ());
                        }
                    }
                }
            }
            if (tnu->isLeafNode ()) {
                tnu->setName (name);
            } else {
                if (tnu->name == "match_operator") {
                    if (i == 0) {
                        if (!tnu->mode.empty ()) {
                            operands->at(0).setMode (tnu->mode);
                            tnu->mode = "";
                        }
                        srtl_driver::outPattern += " (" + 
                            operands->at(0).getOperand() + " ";
                        operands->erase (operands->begin ());
                    }
                }
            }
            if (operands->empty ())
                break;
            if (operands->at(0).getType()  == Operand::bracketClose) {
                if (name == "match_operator" || name == "unspec" || 
                        name == "parallel" || name == "unspec_volatile" ||
                        name == "match_op_dup"){
                    if (name == "match_operator" || name == "parallel" ||
                            name == "match_op_dup")
                        srtl_driver::outPattern += "]";
                    operands->erase (operands->begin ());
                    break;
                } else if (name == "sequence") {
                //    srtl_driver::outPattern += ")";
                    operands->erase (operands->begin ());
                } else {
                    operands->erase (operands->begin ());
                }
                i--;
                break;
            } else{
                returnValue = tnu->traverseTree (operands, tnu);
            }

            if (name == "parallel" && operands->empty ()) {
                   srtl_driver::outPattern += "]";
                   break;
               }
        }

        if (returnValue == 5 && name == "parallel")
            returnValue = 0;

        if (name != "sequence") 
            srtl_driver::outPattern += ") ";

        return returnValue;
    } else {
        if(!operands->empty ()) {
            if (operands->at (0).getType () == Operand::bracketClose) {
                srtl_driver::outPattern += ")";
                operands->erase (operands->begin ());
                return returnValue;
            } else if (operands->at (0).getType () == Operand::bracketOpen) {
                operands->erase (operands->begin ());
                return returnValue;
            }
            if (!mode.empty()) {
                operands->at(0).setMode (mode);
            }
            if (operands->at(0).getType () == Operand::integer ||
                    operands->at(0).getType () == Operand::string)
                srtl_driver::outPattern += operands->at(0).getOperand () ;
            else if (operands->at (0).getType () == Operand::matchOperator)
                srtl_driver::outPattern += "(" + operands->at(0).
                    getOperand () + " ";
            else if (operands->at (0).getType () == Operand::num)
                srtl_driver::outPattern += operands->at(0).getOperand ();
            else
                srtl_driver::outPattern += "(" + operands->at(0).
                    getOperand () + ") ";

            operands->erase (operands->begin ());
        } else {
            /* TODO Report pattern error. Jump drectly to the next pattern. */
        }
        return returnValue;
    }
}

void Node::traverseTree (Node* tn) {
    if( tn == 0 ){
        tn = this;
        srtl_driver::outPattern.clear ();
    }
    if(!tn->isLeafNode ()){
        srtl_driver::outPattern += " ( " + name;
        if (!mode.empty ())
            srtl_driver::outPattern += ":" + mode + " ";
        for (unsigned int i = 0 ; i < tn->children.size (); i++) {
            Node* tnu = tn->children[i];
            tnu->traverseTree (tnu);
        }
        srtl_driver::outPattern += " ) ";
    }else{
        srtl_driver::outPattern += " (  " +  name + " )";
        return;
    }
}

/*
   int main () {

   Node* rootNode = new Node ("set", false);
   Node* nodeOne = new Node ("minus", false);

   Node* nodeTwo = new Node ("plus","MODEF", false );

   std::vector<int> position;
   position.push_back (2);

   rootNode->traverseTree ();
   std::cout << pattern << std::endl;

   pattern.clear ();

   rootNode->replaceNode (nodeOne, position);
   rootNode->traverseTree ();
   std::cout << pattern << std::endl;

   pattern.clear ();

   position.push_back (2);
   rootNode->replaceNode (nodeTwo, position);

   rootNode->traverseTree ();

   std::cout << pattern;

   std::cout << std::endl;

   return 0;
   }
   */