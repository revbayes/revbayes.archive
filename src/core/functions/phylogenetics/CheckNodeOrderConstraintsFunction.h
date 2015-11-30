//
//  CheckNodeOrderConstraintsFunction.h
//  revbayes-proj
//
//  Created by Bastien Boussau on 16/11/15.
//  Copyright (c) 2015 Bastien Boussau. All rights reserved.
//

#ifndef CheckNodeOrderConstraintsFunction_H
#define CheckNodeOrderConstraintsFunction_H

#include "RelativeNodeAgeConstraints.h"
#include "RbVector.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <unordered_set>
#include <vector>

#include <boost/functional/hash.hpp>


namespace RevBayesCore {
    
    class CheckNodeOrderConstraintsFunction : public TypedFunction<Boolean> {
        
    public:
        CheckNodeOrderConstraintsFunction(const TypedDagNode<Tree> *t, const TypedDagNode<RelativeNodeAgeConstraints> *co);
        virtual                                            ~CheckNodeOrderConstraintsFunction(void);                                                         //!< Virtual destructor
        
        // public member functions
        CheckNodeOrderConstraintsFunction*                                  clone(void) const;                                                                  //!< Create an independent clone
        void                                                keep(DagNode* affecter);
        void                                                restore(DagNode *restorer);
        void                                                reInitialized(void);                                                                //!< The arguments have been re-initialized
        void                                                touch(DagNode *toucher );
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                    //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode<Tree>*                           tau;
        const TypedDagNode<RelativeNodeAgeConstraints>*         constraints;
        std::map<std::pair<std::string, std::string>, double >  nodeAges;
        std::unordered_set< std::pair < std::string, std::string >, boost::hash<std::pair<std::string, std::string> > > constrainedNodes;
        
        // functions
        void updateSetOfConstrainedNodes();
        void updateMapOfNodeAges();
        bool checkNodeAgeConstraints();
        double getAgeOfMRCA(std::string first, std::string second) ;

    };
    
}

#endif