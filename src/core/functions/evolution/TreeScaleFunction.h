//
//  TreeScaleFunction.h
//  revbayes-proj
//
//  Created by Michael Landis on 2/6/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef TreeScaleFunction_H
#define TreeScaleFunction_H

#include "RbVector.h"
#include "TimeTree.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    class TreeScaleFunction : public TypedFunction<TimeTree> {
        
    public:
        TreeScaleFunction(const TypedDagNode<TimeTree> *t, const TypedDagNode<double> *b, std::vector<double> m);
        virtual                                            ~TreeScaleFunction(void);                                                         //!< Virtual destructor
        
        // public member functions
        TreeScaleFunction*                                  clone(void) const;                                                                  //!< Create an independent clone
        void                                                keep(DagNode* affecter);
        void                                                restore(DagNode *restorer);
        void                                                reInitialized(void);                                                                //!< The arguments have been re-initialized
        void                                                touch(DagNode *toucher );
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                    //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode<TimeTree>*                       tau;
        const TypedDagNode<double>*                         scale;
        std::vector<double>                                 tipAges;
        double                                              scaleLimit;
        std::set<size_t>                                    touchedNodeIndices;
    };
    
}

#endif