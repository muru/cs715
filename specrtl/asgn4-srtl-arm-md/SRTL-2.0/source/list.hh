/*--------------------------------------------------------------------------- *
 * srtl implementation in c++                                                 *
 *                                                                            *
 * This file contains class header definitions for list pattern class and     *
 * supporting class.                                                          *
 *                                                                            *
 *                                                                            *
 * Change History                                                             *
 * -------------------------------------------------------------------------- *
 * Name        Date          Change Description                               *
 * Sheweta     14-Aug-13     Initial Version                                  *
 *                                                                            *
 *--------------------------------------------------------------------------- */

#ifndef __LIST_H_INCLUDED__
#define __LIST_H_INCLUDED__

#include <list>
#include "pattern.hh"

/*--------------------------------------------------------------------------- *
 * This class stores the lists of elements. It supports methods to append and *
 * display the final std::string of elements.                                      *
 *--------------------------------------------------------------------------- */ 

class  Element 
{
   private:
      std::string * Name;
      std::string * Value;
       public:
      Element();
      ~Element();
      Element(Element *E);
      Element (const Element & e);
      Element(std::string  *V);
      Element (std::string *N,std::string *V);
      void setName(std::string *V);
      void setValue(std::string * V);
      std::string * getName() const;
      std::string * getValue() const;
};

class ElemList : public Element
{
private :
std::list<Element *> eList;
public:
 ElemList();
 ~ElemList();
 ElemList(Element *e);
 ElemList(const ElemList & el);
 ElemList(ElemList *l,Element *e);
 ElemList(ElemList *l,ElemList *e);
 void append(Element *e);
 void append(ElemList * l);
std::list<Element *>  getList() const;
 
};
class ListPattern : public Pattern
{
   private:
      ElemList * elist;
      std::string pname;
       char separator;
       char start;
       char end;
       char estart ;
       char eend;
       bool error;
   public:
      ListPattern();
      ~ListPattern();
      ListPattern(Type t);
      ListPattern(std::string name);
      ListPattern(Type t,std::string name);
      ListPattern(Type t,std::string name,Element *e);
      ListPattern(Type t,std::string name,ElemList *e);
      ListPattern(Type t,std::string name,ElemList *e, char start, char end);
      std::string  getPatName();
      void setPatName(std::string  V);
      void setEStEnd(char s, char e);
      void setPatType(Type t);
      void setStEnd(char s, char e);
      Type getPatType();
      void createPattern();
      void setSeparator(char s);
      void append(std::string *  Name);
      void append(Element *e);
      void append(std::string *Name,std::string *Value);
      void append (ElemList *e);
      void setError();
      bool inError();
      ElemList* getElemList () { return elist; }
};

#endif

