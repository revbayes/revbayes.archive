//
//  AdmixtureEdgeRegraftReplace.h
//  rb_mlandis
//
//  Created by Michael Landis on 1/29/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AdmixtureEdgeRegraftReplace__
#define __rb_mlandis__AdmixtureEdgeRegraftReplace__



#include <ostream>
#include <set>
#include <string>
#include <list>

#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "MoveOld.h"
#include "StochasticNode.h"
#include "AdmixtureNode.h"
#include "AdmixtureTree.h"
#include "Tree.h"

namespace RevBayesCore {
    /*
    struct AdmixtureEdgePosition
    {
        AdmixtureNode* admixtureParent;
        AdmixtureNode* admixtureChild;
        AdmixtureNode* admixtureParentChild;
        AdmixtureNode* admixtureChildChild;
        double age;
        double weight;
        AdmixtureEdgePosition(AdmixtureNode* p, AdmixtureNode* c, AdmixtureNode* pc, AdmixtureNode* cc, double a, double w) : admixtureParent(p), admixtureChild(c), admixtureParentChild(pc), admixtureChildChild(cc), age(a), weight(w)
        {
            ;//std::cout << "hello\n";
        }
    };
    */
    
    class AdmixtureEdgeRegraftReplace : public MoveOld {
        
    public:
        AdmixtureEdgeRegraftReplace( StochasticNode<AdmixtureTree> *n, DeterministicNode< RbVector<double> >* res, double d, int ag, int me, bool asa, double weight);                                            //!<  constructor
        
        // Basic utility functions
        AdmixtureEdgeRegraftReplace*       clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        bool                            isActive(unsigned long g) const;
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            rejectSimpleMove(void);
        void                            acceptSimpleMove(void);
        
        void                            acceptMove(void);                                                                   //!< Accept the InferenceMoveSimple
        double                          performMove(double& probRatio);                                                     //!< Perform the InferenceMoveSimple
        void                            rejectMove(void);
        
    private:
        
        void                            findDescendantTips(std::set<AdmixtureNode*>& s, AdmixtureNode* p);
        
        // member variables
        StochasticNode<AdmixtureTree>*  variable;
        DeterministicNode< RbVector<double> >* residuals;
        bool changed;
        bool failed;
        unsigned long activeGen;
        size_t numEvents;
        size_t maxEvents;
        bool allowSisterAdmixture;
        double delta;
        
        // stored objects to undo proposal
        AdmixtureNode*                  pruneChild;
        AdmixtureNode*                  pruneParent;
        AdmixtureNode*                  pruneNephew;
        AdmixtureNode*                  pruneNode;
        AdmixtureNode*                  regraftParent;
        AdmixtureNode*                  regraftChild;
        std::vector<double>             storedResiduals;
        AdmixtureTree                   storedAdmixtureTree;
        double storedAge;
        
        std::list<AdmixtureEdgePosition> storedAdmixtureEdges;
        std::list<AdmixtureEdgePosition> newAdmixtureEdges;
        
    };
    
}


#endif /* defined(__rb_mlandis__AdmixtureEdgeRegraftReplace__) */
