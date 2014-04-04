#ifndef __STATTREENODE_HH__
#define __STATTREENODE_HH__

#include <iostream>
#include <vector>
#include <cstring>

class StatTreeNode {
    public:
        std::vector<StatTreeNode*> children;
        std::vector<int> edgeType;
        std::string name;
        int type;
        
        int depth;

        int descendantsCount;
        int descendantsCountAbstract;
        int descendantsCountConcrete;
        
        StatTreeNode (std::string n, int t) {
            name = n; type = t; depth = 1;

            descendantsCount = 0;
            descendantsCountAbstract = 0;
            descendantsCountConcrete = 0;
        }
        
        void addChild (StatTreeNode* stn){
            children.push_back (stn);
        }
        int CountDescendants (StatTreeNode* t = 0, StatTreeNode* p = 0);
};

#endif
