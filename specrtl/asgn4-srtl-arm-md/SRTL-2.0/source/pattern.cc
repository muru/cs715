#include "srtl-driver.hh"
#include "pattern.hh"

Operand::Operand (Type t, int n, std::string p, std::string m, std::string c) {
    operandNumber = n;
    number = 0;
    predicate = (p == "NULL" ? "" : p);
    mode = (m == "NULL" ? "" : m);
    constraint = c;
    type = t;
//    bClose = false;
}

Operand::Operand (Type t, std::string p, std::string m, std::string c) {
    type = t;
    if (type == pmc)
        operandNumber = -1;
    else
        operandNumber = 0;
    number = 0;
    predicate = (p == "NULL" ? "" : p);
    mode = (m == "NULL" ? "" : m);
    constraint = c;
//    bClose = false;
}

Operand::Operand (Type t, unsigned long int n) {
    type = t;
    if (type == pmc)
        operandNumber = -1;
    else
        operandNumber = 0;
    number = n;
//    bClose = false;
}

Operand::Operand (Type t, int n, std::string p_m) {
    type = t;
    if (type == pmc)
        operandNumber = -1;
    else
        operandNumber = 0;
    number = n;
    predicate = p_m;
//    bClose = false;
    mode = "";
}

Operand::Operand (Type t,std::string p_m, std::string c) {
    constraint = c;
    type = t;
    if (type == pmc)
        operandNumber = -1;
    else
        operandNumber = 0;
    number = 0;
    predicate = p_m;
//    bClose = false;
    mode = "";
}

Operand::Operand (Type t, int n, std::string p_m, std::string c) {
    operandNumber = n;
    number = 0;
    predicate = p_m;
    mode = p_m;
    constraint = c;
    type = t;
//    bClose = false;
}

Operand::Operand (Type t, std::string p_m) {
    type = t;
    if (type == pmc)
        operandNumber = -1;
    else
        operandNumber = 0;
    predicate = p_m;
//    bClose = false;
}

Operand::~Operand () {}

/**
 * This method corresponds to the printOperand Method in 
 * srtl-build-patterns.c
 **/
std::string Operand::getOperand () {
    std::string op;

    if (type == pmc) {
        if (mode.empty () || (mode.compare("NULL") == 0)) {
            op.append ("match_operand " + srtl_driver::itos(operandNumber) + 
                " \"" + (predicate == "NULL" ? "" : predicate) + "\" " + 
                constraint);
        } else {
            op.append ("match_operand:" + mode + " " + 
                srtl_driver::itos(operandNumber) + " \"" + 
                (predicate == "NULL" ? "" : predicate) + "\" " + constraint);
        }
    } else if (type == dup) {
        op.append ("match_dup " + srtl_driver::itos (number));
    } else if (type == num) {
        op.append (srtl_driver::itos (number));
    } else if (type == reg) {
        /** 
         * Corresponds to register operands.
         * e.g. reg(CC:FLAGS_REG)
         **/
        if (mode.empty ()) {
            op.append (predicate + " " + constraint);
        } else {
            op.append (predicate + ":"  + mode + " " + constraint);
        }
    } else if (type == regNum) {
        /**
         * Corresponds to register operands. But, with a number.
         * e.g. reg(CC:5)
         **/
        op.append ("reg:" + mode + " " + constraint);
    } else if (type == extraReg) {
        /**
         * pc, cc0, scratch, return, eh_return.
         **/
        op.append (predicate);
    } else if (type == constSpec) {
        /**
         * Const_int.
         **/
        if (predicate.empty () || predicate.compare ("NULL")) {
            op.append ("const_int " + srtl_driver::itos(number));
        } else {
            op.append ("const_int " + srtl_driver::itos(number) + 
                ":" + predicate);
        }
    } else if (type == scratch) {
        op.append ("match_scratch" + (mode == "NULL" ? "" : ":"+
            mode)  +" " + srtl_driver::itos(operandNumber) +" " + constraint);
    } else if (type == eqAttr) {
        if (predicate == "match_code" || predicate == "match_test") {
            op.append (predicate + " " + constraint);
        } else if (predicate == "symbol_ref" || predicate == "const_string"
                || predicate == "attr") {
            op.append (predicate + " " + constraint);
        } else {
            op.append ("eq_attr \"" + predicate + "\" " + constraint + 
                "\n\t\t");
        }
    } else if (type == matchOperator) { 
        op.append ("match_operator" + (mode == "NULL" || mode.empty () ? 
            "" : ":" + mode) + " " + srtl_driver::itos(number) + " \"" + 
        predicate + "\" [");
    } else if (type == integer) {
        op.append (" " + srtl_driver::itos(number));
    } else if (type == string) {
        op.append ("] " + predicate);
    } else if (type == bracketOpen || type == bracketClose) {
        return "";
    } else {
        std::cout << "Bad Operand Type: " << predicate << " :" 
            << type << std::endl;
        return "";
    }
    return op;
}

void ModeStmt::addAccessTree (std::vector<int>* at) {
    accessTree = *at;
}

void ModeStmt::setMode (std::string mode) {
    predicateOrMode = mode;
}

