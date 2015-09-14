//
//  AdmixtureEdgeSlideLength.h
//  rb_mlandis
//
//  Created by Michael Landis on 4/10/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AdmixtureEdgeSlideLength__
#define __rb_mlandis__AdmixtureEdgeSlideLength__

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
    
    class AdmixtureEdgeSlideLength : public MoveOld {
        
    public:
        AdmixtureEdgeSlideLength( StochasticNode<AdmixtureTree> *n, double weight, unsigned long activeGen, double delta);                                            //!<  constructor
        
        // Basic utility functions
        AdmixtureEdgeSlideLength*              clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
         bool                           isActive(unsigned long g) const;
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            rejectSimpleMove(void);
        void                            acceptSimpleMove(void);
        
        void                            acceptMove(void);                                                                   //!< Accept the InferenceMoveSimple
        double                          performMove(double& probRatio);                                                     //!< Perform the InferenceMoveSimple
        void                            rejectMove(void);
        
    private:
        double                            topologySlide(AdmixtureNode* ap, AdmixtureNode* ac);
        double                            oneSlide(AdmixtureNode* ap, AdmixtureNode* ac, double& distance, int& direction);
        void                            findNewBrothers(std::vector<AdmixtureNode*> &b, AdmixtureNode &p, AdmixtureNode *n);
        
        // member variables
        StochasticNode<AdmixtureTree>*  variable;
        bool                            changed;
        bool                            failed;
        unsigned long                   activeGen;
        double                          delta;
        
        // stored objects to undo proposal
        AdmixtureNode*                  storedAdmixtureChild;
        AdmixtureNode*                  storedAdmixtureParent;
        AdmixtureNode*                  storedAdmixtureChildChild;
        AdmixtureNode*                  storedAdmixtureParentChild;
        AdmixtureNode*                  storedAdmixtureChildParent;
        AdmixtureNode*                  storedAdmixtureParentParent;
        double                          storedAdmixtureAge;
        
        std::list<AdmixtureEdgePosition> storedAdmixtureEdges;
    };
    
}

#endif /* defined(__rb_mlandis__AdmixtureEdgeSlideLength__) */
