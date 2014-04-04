#ifndef __NODE_H_INCLUDED__
#define __NODE_H_INCLUDED__

#include <iostream>
#include <vector>

#include "pattern.hh"

/**
 * PMC Mapping = Predicate Mode Constraint Mapping *
 */
class PmcMapping{
private:
    int childLocation;
    Operand op;
public:
    enum Type { pmc, mode, predicate, constraint };
    int getChildLocation () { return childLocation; }
    Operand getOperand () { return op; }
    
    void setChildLocation (int cl) { childLocation = cl; }
    void setPmc (Operand o) { op = o; }

    Type getType () { return type; }

    PmcMapping (int cL, Type t, Operand o) {
        childLocation = cL;
        op = o;
        type = t;
    }
private:
    Type type;
};

class Node {
private:
    bool leafNode;
    std::vector<Node*> children;
    std::string name;
    std::string mode;

    std::vector<PmcMapping> pmcReplacements;

    static const std::string DUMMYNODE;

public:
    Node (std::string name, bool leafNode);
    Node (std::string name, std::string mode, bool leafNode);
    ~Node ();
    std::string getName () const { return name; }
    void setName (std::string n) { name = n; }
    bool isLeafNode () { return leafNode; }
    void addChild (Node* node);
    void replaceNode (Node *node, std::vector<int>* position);
    void replaceMode (std::string m, std::vector<int>* position);
    void replacePmc (Operand op, std::vector<int>* position);
    void replacePredicate (Operand op, std::vector<int>* position);
    void replaceConstraint (Operand op, std::vector<int>* position);
    void replaceSingleNode (Node *node, int position);
    /* Temporary stub names should be recieved from
     * srtl-driver */
    int searchKnownNames (std::string nameToSearch);
    void printChildren ();
    void traverseTree (Node* tn = 0);
    int traverseTree (std::vector<Operand>* operands, Node* tn = 0);
    
    void setMode (std::string m){
        mode = m;
    }
    
    void addPmcReplacement(PmcMapping pm) {
        pmcReplacements.push_back (pm);
    }

    PmcMapping getPmcMapping (unsigned int i) {
        if (i < pmcReplacements.size ()) {
            return pmcReplacements[i];
        }else {
            // TODO RETURN ERROR:
            return pmcReplacements[0];
        }
    }    
};

#endif
