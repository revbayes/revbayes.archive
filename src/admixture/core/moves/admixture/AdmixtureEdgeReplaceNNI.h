//
//  AdmixtureEdgeReplaceNNI.h
//  rb_mlandis
//
//  Created by Michael Landis on 1/29/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AdmixtureEdgeReplaceNNI__
#define __rb_mlandis__AdmixtureEdgeReplaceNNI__



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
    
    class AdmixtureEdgeReplaceNNI : public MoveOld {
        
    public:
        AdmixtureEdgeReplaceNNI( StochasticNode<AdmixtureTree> *n, DeterministicNode< RbVector<double> >* res, double delta, int ag, int me, bool asa, double weight);                                            //!<  constructor
        
        // Basic utility functions
        AdmixtureEdgeReplaceNNI*       clone(void) const;                                                                  //!< Clone object
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
        void                            storeAdmixtureEventsForLineage(AdmixtureNode* p);
        
        // member variables
        StochasticNode<AdmixtureTree>*  variable;
        DeterministicNode< RbVector<double> >* residuals;
        bool changed;
        bool failed;
        unsigned long activeGen;
        double delta;
        size_t numEvents;
        size_t maxEvents;
        bool allowSisterAdmixture;

        
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
        double storedPruneAge;
        double storedRegraftAge;
        
        std::list<AdmixtureEdgePosition> storedAdmixtureEdges;
        std::list<AdmixtureEdgePosition> newAdmixtureEdges;
        
    };
    
}


#endif /* defined(__rb_mlandis__AdmixtureEdgeReplaceNNI__) */
