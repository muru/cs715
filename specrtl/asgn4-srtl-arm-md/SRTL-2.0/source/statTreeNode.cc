#include <vector>
#include <map>
#include "statTreeNode.hh"

extern std::map <int, int> abstractStats;
extern std::map <int, int> concreteStats;
extern std::map <int, int> mixStats;

extern std::map <int, int> aa;
extern std::map <int, int> ac;

extern std::map <int, int> ca;
extern std::map <int, int> cc;

int gInt = 0;
int StatTreeNode::CountDescendants (StatTreeNode* t, StatTreeNode* p) {
    if (t == 0) {
        for (unsigned int i = 0; i < children.size (); i++) {
            descendantsCount += CountDescendants (children[i], this);
            if (children[i]->type == 1) {
                descendantsCountAbstract++;
            } else {
                descendantsCountConcrete++;
            }
        }

        std::map <int, int>::iterator pat;
        //if (type == 1) {   
            if (children.empty ()) {
                gInt++;
            }     
//            std::cout << name << ":" << children.size () << ":" << descendantsCountAbstract << std::endl;
            pat = ::abstractStats.find (descendantsCountAbstract);
            if (pat == abstractStats.end ()){
                abstractStats[descendantsCountAbstract] = 1;
            } else {
                pat->second++;
            }
        //} else if (type == 2) {
            pat = concreteStats.find (descendantsCountConcrete);
            if (pat == concreteStats.end ()){
                concreteStats[descendantsCountConcrete] = 1;
            } else {
                pat->second++;
            }
            pat = mixStats.find (descendantsCount);
            if (pat == mixStats.end ()) {
                mixStats [descendantsCount] = 1;
            } else {
                pat->second++;
            }
        //}
        if (type == 1) {
            pat = aa.find (descendantsCountAbstract);
            if (pat == aa.end ()){
                aa[descendantsCountAbstract] = 1;
            } else {
                pat->second++;
            }
            pat = ac.find (descendantsCountConcrete);
            if (pat == ac.end ()){
                ac[descendantsCountConcrete] = 1;
            } else {
                pat->second++;
            }
        } else if (type == 2) {
            pat = cc.find (descendantsCountConcrete);
            if (pat == cc.end ())
                cc[descendantsCountConcrete] = 1;
            else
                pat->second++;
            pat = ca.find (descendantsCountAbstract);
            if (pat == ca.end ())
                ca[descendantsCountAbstract] = 1;
            else 
                pat->second++;
        } else {
            std::cout << "Invalid pattern type" << std::endl;
        }
        return descendantsCount;
    
    } else {
        if (t->children.size () == 0) {
            return 1;
        } else {
            for (unsigned int i = 0; i < t->children.size (); i++) {
                t->descendantsCount += CountDescendants(t->children[i], t);
                if (t->children[i]->type == 1) {
                    t->descendantsCountAbstract++;
                } else {
                    t->descendantsCountConcrete++;
                }
            }
            p->descendantsCountAbstract += t->descendantsCountAbstract;
            p->descendantsCountConcrete += t->descendantsCountConcrete;

            std::map <int, int>::iterator pat;
        //    if (t->type == 1) {
                pat = abstractStats.find (t->descendantsCountAbstract);
                if (pat == abstractStats.end ()){
                    abstractStats[t->descendantsCountAbstract] = 1;
                } else {
                    pat->second++;
                }
        //    } else if (t->type == 2) {
                pat = concreteStats.find (t->descendantsCountConcrete);
                if (pat == concreteStats.end ()){
                    concreteStats[t->descendantsCountConcrete] = 1;
                } else {
                    pat->second++;
                }

                pat = mixStats.find (t->descendantsCount);
                if (pat == mixStats.end ()) {
                    mixStats [t->descendantsCount] = 1;
                } else {
                    pat->second++;
                }
        //    }
        if (t->type == 1) {
            pat = aa.find (t->descendantsCountAbstract);
            if (pat == aa.end ()){
                aa[t->descendantsCountAbstract] = 1;
            } else {
                pat->second++;
            }
            pat = ac.find (t->descendantsCountConcrete);
            if (pat == ac.end ()){
                ac[t->descendantsCountConcrete] = 1;
            } else {
                pat->second++;
            }
        }else if (t->type == 2) {
            if (t->descendantsCountAbstract > 1) {
//                std::cout << "Descendants Count > 1 NameP: " << ", " << t->descendantsCountAbstract << std::endl;
             }
            pat = cc.find (t->descendantsCountConcrete);
            if (pat == cc.end ())
                cc[t->descendantsCountConcrete] = 1;
            else
                pat->second++;
            pat = ca.find (t->descendantsCountAbstract);
            if (pat == ca.end ())
                ca[t->descendantsCountAbstract] = 1;
            else 
                pat->second++;

            if (t->descendantsCountConcrete == 0) {
 //               std::cout << "NameP: " << std::endl;
            }
        } else {
            std::cout << "Invalid Pattern Type" << std::endl;
        }
            return t->descendantsCount + 1;
        }
    }

}
