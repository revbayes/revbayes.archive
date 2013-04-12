//
//  BranchHistory.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 12/23/12.
//  Copyright (c) 2012 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__BranchHistory__
#define __revbayes_mlandis__BranchHistory__

#include <iostream>
#include <set>

#include "BranchEvent.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class TopologyNode;
    
    template<class eventType, class eventCompareType>
    class BranchHistory : public TypedDistribution<std::set<eventType,eventCompareType> >{
        
    public:
        BranchHistory(const TopologyNode* n);
        BranchHistory(const BranchHistory& v);
        ~BranchHistory(void);

        size_t                                      getNumEvents(void);
        const TopologyNode*                               getNodePtr(void);
        void                                        addEvent(eventType evt);
        void                                        removeEvent(eventType evt);
        void                                        removeAllEvents(void);
        
        // BranchHistory, pure virtual
        virtual void                                print(void) = 0;
        
        // TypedDistribution, pure virtual
        virtual BranchHistory<eventType,eventCompareType>*           clone(void) const = 0;                                              //!< Clone the distribution
        virtual double                              computeLnProbability(void) = 0;                                     //!< Clone the ln probability density
        virtual void                                redrawValue(void) = 0;                                              //!< Draw a new random value from the distribution
        virtual void                                swapParameter(const DagNode *oldP, const DagNode *newP) = 0;        //!< Exchange the parameter
        

// MJL: non-pure virtual functions -- add later??
        
        // TypedDistribution, virtual
//        virtual void                    setValue(valueType *v);                                             //!< Set the current value, e.g.
        
        // Distribution, virtual
//        virtual void                        getAffected(std::set<DagNode *>& affected, DagNode* affecter);  //!< get affected nodes
//        virtual void                        keepSpecialization(DagNode* affecter);
//        virtual void                        restoreSpecialization(DagNode *restorer);
//        virtual void                        touchSpecialization(DagNode *toucher);

        
        
    protected:
        const TopologyNode*                               nodePtr;
        
    };
    
    template<class eventType, class eventCompareType>
    BranchHistory<eventType, eventCompareType>::BranchHistory(const TopologyNode* n) :
        TypedDistribution< std::set<eventType,eventCompareType> >(new std::set<eventType,eventCompareType>),
        nodePtr(n)
    {
        ;
    }
    
    template<class eventType, class eventCompareType>
    BranchHistory<eventType, eventCompareType>::BranchHistory(const BranchHistory<eventType,eventCompareType>& v) :
        TypedDistribution< std::set<eventType,eventCompareType> >(v),
        nodePtr(v.nodePtr)
    {
        ;
    }

    template<class eventType, class eventCompareType>
    BranchHistory<eventType, eventCompareType>::~BranchHistory(void)
    {
        removeAllEvents();
    }
    
    template<class eventType, class eventCompareType>
    size_t BranchHistory<eventType, eventCompareType>::getNumEvents(void)
    {
        return this->getValue().size();
    }
    
    template<class eventType, class eventCompareType>
    const TopologyNode* BranchHistory<eventType, eventCompareType>::getNodePtr(void)
    {
        return nodePtr;
    }
    
    template<class eventType, class eventCompareType>
    void BranchHistory<eventType, eventCompareType>::addEvent(eventType evt)
    {
        this->getValue().insert(evt);
    }
    
    template<class eventType, class eventCompareType>
    void BranchHistory<eventType, eventCompareType>::removeEvent(eventType evt)
    {
        this->getValue().erase(evt);
    }
    
    template<class eventType, class eventCompareType>
    void BranchHistory<eventType, eventCompareType>::removeAllEvents(void)
    {
        this->getValue().clear();
    }
    
}

#endif /* defined(__revbayes_mlandis__BranchHistory__) */
