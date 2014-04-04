/*--------------------------------------------------------------------------- *
 * srtl implementation in c++                                                 *
 *                                                                            *
 * This file contains class  definitions for lists and its support class.     *
 *                                                                            *
 * Change History                                                             *
 * ---------hy----------------------------------------------------------------- *
 * Name        Date          Change Description                               *
 * Sheweta     14-Aug-13     Initial Version                                  *
 *                                                                            *
 *--------------------------------------------------------------------------- */

#include "srtl-driver.hh"
#include "list.hh"
#include <iostream>
#include <map>

/*--------------------------------------------------------------------------- *
 * This class stores element for the list. It supports methods to get and set *
 * the name and value of the element                                          *
 *--------------------------------------------------------------------------- */

Element::Element() {
    Name = 0;
    Value = 0;
}
Element::~Element() {
    delete Name;
    delete Value;
}
Element::Element(Element * E) {

    Name = E->getName();
    Value = E->getValue();
}

Element::Element(const Element & E) {
    Name = new std::string;
    *Name = *E.getName();
    Value = new std::string;
    *Value = *E.getValue();
}

Element::Element(std::string *V) {
    setName(V);
    Value = 0;

}

Element::Element(std::string *N, std::string *V) {
    setName(N);
    setValue(V);
}

void Element::setName(std::string *V) {
    Name = V;
}
void Element::setValue(std::string * V) {
    Value = V;
}
std::string *Element::getName() const {
    return Name;
}
std::string *Element::getValue() const {
    return Value;
}

ElemList::~ElemList() {
}
ElemList::ElemList(const ElemList & el) {
    eList = el.getList();
}

ElemList::ElemList() {
}

ElemList::ElemList(Element *e) {
    eList.push_back(e);
}

std::list<Element *> ElemList::getList() const {
    return eList;

}

void ElemList::append(ElemList *l) {
    std::list<Element *> el = l->getList();
    eList.splice(eList.end(), el);

}

ElemList::ElemList(ElemList *l, Element *e) {
    std::list<Element *> el = l->getList();
    eList.splice(eList.end(), el);

    eList.push_back(e);
}

ElemList::ElemList(ElemList *l, ElemList *e) {
    std::list<Element *> el = l->getList();
    eList.splice(eList.end(), el);

    append(e);
}

void ElemList::append(Element *e) {
    eList.push_back(e);
}

void ListPattern::setEStEnd(char s, char e) {
    estart = s;
    eend = e;
    elist = new ElemList;
}

/*--------------------------------------------------------------------------- *
 * This class stores the lists of elements. It supports methods to append and *
 * display the final string of elements.                                      *
 *--------------------------------------------------------------------------- */
ListPattern::ListPattern() {
    elist = new ElemList;
    setEStEnd('(', ')');
    separator = 0;
    start = 0;
    end = 0;

}

ListPattern::~ListPattern() {
    delete elist;

}
ListPattern::ListPattern(Type t)

{

    elist = new ElemList;
    setEStEnd('(', ')');
    separator = 0;
    start = 0;
    end = 0;
    setPatType(t);
}
ListPattern::ListPattern(std::string name)

{

    elist = new ElemList;
    setEStEnd('(', ')');
    separator = 0;
    start = 0;
    end = 0;
    setPatName(name);
}

ListPattern::ListPattern(Type t, std::string name)

{

    elist = new ElemList;
    setEStEnd('(', ')');
    separator = 0;
    start = 0;
    end = 0;
    setPatType(t);
    setPatName(name);
}

ListPattern::ListPattern(Type t, std::string name, Element *e)

{

    elist = new ElemList;
    setEStEnd('(', ')');
    separator = 0;
    start = 0;
    end = 0;
    setPatType(t);
    setPatName(name);
    append(e);
}

ListPattern::ListPattern(Type t, std::string name, ElemList *e)

{

    elist = new ElemList;
    setEStEnd('(', ')');
    separator = 0;
    start = 0;
    end = 0;
    setPatType(t);
    setPatName(name);
    append(e);
}

ListPattern::ListPattern(Type t, std::string name, ElemList *e, char start,
        char end)

        {

    elist = new ElemList;
    setEStEnd('(', ')');
    separator = 0;
    start = 0;
    end = 0;
    setPatType(t);
    setPatName(name);
    append(e);
    setStEnd(start, end);
}

void ListPattern::setPatName(std::string V) {
    pname = V;

}
std::string ListPattern::getPatName() {
    return pname;
}

void ListPattern::setPatType(Pattern::Type t) {
    type = (Type) t;
}

Pattern::Type ListPattern::getPatType() {
    return type;
}
void ListPattern::setStEnd(char s, char e) {
    start = s;
    end = e;
}

void ListPattern::setSeparator(char s) {
    separator = s;
}
void ListPattern::append(std::string *N) {
    Element *e = new Element;
    e->setName(N);

    elist->append(e);
}
void ListPattern::append(Element *e) {
    elist->append(e);
}

void ListPattern::append(ElemList *e) {
    elist->append(e);
}
void ListPattern::append(std::string * N, std::string * V) {
    Element *e = new Element;
    e->setName(N);
    e->setValue(V);
    elist->append(e);
}

void ListPattern::setError() {
    error = true;
}

bool ListPattern::inError() {
    return error;
}
void ListPattern::createPattern() {
    std::list<Element*> elemlist = elist->getList();
    std::list<Element *>::iterator it, itb = elemlist.begin(), ite =
            elemlist.end();
    int buffer = 0;
    std::string patt = " ";

    switch (type) {
    case define_code_iterator: {
        patt = "(define_code_iterator " + pname + " [ \n";
        buffer = patt.size() - 1;
        for (it = itb; it != ite; it++) {
            patt = patt + " " + *((*it)->getName()) + "\n";
        }
        if (patt.at(patt.size() - 1) == '\n') {
            patt = patt + std::string(buffer, ' ');
        }
        patt = patt + " ] ";
        break;
    }

    case define_mode_iterator: {
        patt = "(define_mode_iterator " + pname + " [\n ";
        buffer = patt.size() - 1;
        for (it = itb; it != ite; it++) {
            patt = patt + " " + *((*itb)->getName()) + "\n";
        }
        if (patt.at(patt.size() - 1) == '\n') {
            patt = patt + std::string(buffer, ' ');
        }
        patt = patt + " ] ";
        break;
    }
    case define_code_attr: {
        patt = "(define_code_attr " + pname + "[\n";
        buffer = patt.size() - 1;
        for (it = itb; it != ite; it++) {
            patt = patt + " (" + *((*it)->getName()) + "\t"
                    + *((*it)->getValue()) + ")\n";
        }

        if (patt.at(patt.size() - 1) == '\n') {
            patt = patt + std::string(buffer, ' ');
        }
        patt = patt + " ] ";
        break;
    }
    case define_mode_attr: {
        patt = "(define_mode_attr " + pname + "[\n";
        buffer = patt.size() - 1;
        for (it = itb; it != ite; it++) {
            patt = patt + " (" + *((*it)->getName()) + "\t"
                    + *((*it)->getValue()) + ")\n";
        }
        if (patt.at(patt.size() - 1) == '\n') {
            patt = patt + std::string(buffer, ' ');
        }
        patt = patt + " ] ";
        break;
    }

    case define_constants: {
        patt = "(define_constants [ ";
        buffer = patt.size() - 1;
        for (it = itb; it != ite; it++) {
            patt = patt + " (" + *((*it)->getName()) + "\t"
                    + *((*it)->getValue()) + ")\n";
        }
        if (patt.at(patt.size() - 1) == '\n') {
            patt = patt + std::string(buffer, ' ');
        }
        patt = patt + " ] ";
        break;
    }
    case define_c_enum: {
        patt = "(define_c_enum " + pname + " [\n ";
        buffer = patt.size() - 1;
        for (it = itb; it != ite; it++) {
            patt = patt + " " + *((*itb)->getName()) + "\n";
        }
        if (patt.at(patt.size() - 1) == '\n') {
            patt = patt + std::string(buffer, ' ');
        }
        patt = patt + " ] ";
        break;

    }
    case define_asm_attributes: {
        patt = "(define_asm_attributes  [\n";
        buffer = patt.size() - 1;
        for (it = itb; it != ite; it++) {
            patt = patt + "( set_attr\t" + *((*it)->getName()) + "\t"
                    + *((*it)->getValue()) + ")\n";
        }
        if (patt.at(patt.size() - 1) == '\n') {
            patt = patt + std::string(buffer, ' ');
        }
        patt = patt + " ] ";
        break;
    }
    case define_reservation: {
        patt = "(define_reservation " + pname + " " + *((*itb)->getName())
                + " )";
        break;
    }
    case define_cpu_unit: {
        patt = "(define_cpu_unit " + pname + " " + *((*itb)->getName()) + " )";
        break;
    }
    case define_bypass: {
        patt = "(define_bypass " + pname + " " + *((*itb)->getName()) + " )";
        break;
    }
    case define_automaton: {
        patt = "(define_automaton " + pname + " )";
        break;
    }
    default: {
        break;
    }
    }
    patt.erase(patt.size() - 1);
    patt = patt + ")\n";

    srtl_driver::outPattern.clear();
    srtl_driver::outPattern = patt;
}
/*
 int main()
 {
 Element * e = new Element;
 std::string * v= new std::string("v");
 std::string * n = new std::string ("n");
 e->setValue(v);
 e->setName(n);
 ListPattern *P= new ListPattern(define_code_iterator,"test",e);
 P->append(e);
 }
 */
