/*--------------------------------------------------------------------------- *
 * srtl implementation in c++                                                 *
 *                                                                            *
 * This file contains class header definitions abstract pattern class and     *
 * supporting class.                                                          *
 *                                                                            *
 *                                                                            *
 * Change History                                                             *
 * -------------------------------------------------------------------------- *
 * Name        Date          Change Description                               *
 * Sheweta     14-Aug-13     Initial Version                                  *
 *                                                                            *
 *--------------------------------------------------------------------------- */
#include "abstract.hh"

std::string AbstPattern::getPatName () 
{
    return pname;
}

void AbstPattern::setPatName (std::string V) 
{
    pname=V;
}

void AbstPattern::addStmt(ModeStmt s) 
{
    sList.push_back(s);
}

void AbstPattern::setSList (std::vector<ModeStmt>* s) 
{
    sList= * s;
}

void AbstPattern::setExtName (std::string V) 
{
    extname=V;
}

std::string AbstPattern::getExtName () 
{
    return extname; 
}

void AbstPattern:: setError()
{
   error = true;
}

bool AbstPattern::inError()
{
   return error;
}

void AbstPattern::setPatType(Type t){
    type = t;
}

void AbstPattern::setParentPattern(Pattern* p){
    AbstPattern* ab = static_cast <AbstPattern*> (p);
    std::vector<ModeStmt>* msv = ab->getStmts ();
    
    sList.insert (sList.begin (), msv->begin(), msv->end ());
}

Pattern* AbstPattern::getParentPattern () {
    return NULL;
}

AbstPattern::~AbstPattern () {
}

AbstPattern::AbstPattern () {

 initialized=false;
}

