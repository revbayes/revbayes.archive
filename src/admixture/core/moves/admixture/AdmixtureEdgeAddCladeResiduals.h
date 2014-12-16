//
//  AdmixtureEdgeAddCladeResiduals.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/28/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AdmixtureEdgeAddCladeResiduals__
#define __rb_mlandis__AdmixtureEdgeAddCladeResiduals__


#include <ostream>
#include <set>
#include <string>
#include <map>

#include "MoveOld.h"
#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "StochasticNode.h"
#include "AdmixtureTree.h"
#include "Tree.h"

namespace RevBayesCore {
    
    class AdmixtureEdgeAddCladeResiduals : public MoveOld {
        
    public:
        //        AdmixtureEdgeAddCladeResiduals( StochasticNode<AdmixtureTree> *n, StochasticNode<double>* r, DeterministicNode< RbVector<double> >* res, ConstantNode<int>* dt, int me, bool asa, double weight);                                                          //!<  constructor
        AdmixtureEdgeAddCladeResiduals( StochasticNode<AdmixtureTree> *n, StochasticNode<double>* r, StochasticNode<int>* ac, DeterministicNode< RbVector<double> >* res, int ag, int me, bool asa, double weight);                                                          //!<  constructor
        
        // Basic utility functions
        AdmixtureEdgeAddCladeResiduals*              clone(void) const;                                                                  //!< Clone object
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
        
        // member variables
        StochasticNode<AdmixtureTree>*  variable;
        StochasticNode<double>*         rate;
        StochasticNode<int>*            admixtureCount;
        DeterministicNode<RbVector<double> >* residuals;
        //ConstantNode<int>*              delayTimer;
        unsigned long                   activeGen;
        bool                            changed;
        bool                            failed;
        bool                            allowSisterAdmixture;
        int                             delay;
        size_t                          maxEvents;
        size_t                          numEvents;
        size_t                          numNodes;
        size_t                          numTips;
        
        // stored objects to undo proposal
        AdmixtureNode*                  storedAdmixtureParent;
        AdmixtureNode*                  storedAdmixtureChild;
        AdmixtureNode*                  storedAdmixtureParentChild;
        AdmixtureNode*                  storedAdmixtureChildChild;
        AdmixtureNode*                  storedAdmixtureParentParent;
        AdmixtureNode*                  storedAdmixtureChildParent;
        std::vector<double>             storedResiduals;
        


        std::pair<AdmixtureNode*,AdmixtureNode*>    sampleAdmixtureEdgeFromResiduals(double& lnP);
        void                                        findCladeOrder(std::set<AdmixtureNode*>& cn);
        std::set<AdmixtureNode*>                    findClades(AdmixtureNode* p);
        void                                        findCladeResiduals(void);
        
        std::map<AdmixtureNode*,std::set<AdmixtureNode*> > clades;
        std::vector<std::map<std::pair<AdmixtureNode*, AdmixtureNode*>,double> > cladeResiduals;
        std::vector<double> cladeResidualSums;
        std::vector<std::set<AdmixtureNode*> > cladeOrder;
        std::vector<double> cladeOrderAges;
    };
    
}



#endif /* defined(__rb_mlandis__AdmixtureEdgeAddCladeResiduals__) */
