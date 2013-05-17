//
//  TreeLengthStatistic.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/10/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__TreeLengthStatistic__
#define __revbayes_mlandis__TreeLengthStatistic__

#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>
#include <string>

namespace RevBayesCore {
    
    template<class treeType>
    class TreeLengthStatistic : public TypedFunction<double> {
        
    public:
        TreeLengthStatistic(const TypedDagNode<treeType> *t);                                                                                   //!< Default constructor
        TreeLengthStatistic(const TreeLengthStatistic& t);                                                                                      //!< Copy constructor
        virtual                                    ~TreeLengthStatistic(void);                                                                  //!< Destructor
        
        TreeLengthStatistic&                        operator=(const TreeLengthStatistic& t);
        
        // Basic utility functions
        TreeLengthStatistic*                        clone(void) const;                                                                          //!< Clone object
        void                                        update(void);                                                                               //!< Clone the function
        
    protected:
        void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                            //!< Implementation of swaping parameters
        
    private:
        // members
        const TypedDagNode<treeType>*               tree;
        
    };
    
}

//#include "TreeLengthStatistic.h"

using namespace RevBayesCore;

template<class treeType>
TreeLengthStatistic<treeType>::TreeLengthStatistic(const TypedDagNode<treeType> *t) : TypedFunction<double>( new double(0.0) ), tree( t ) {
    // add the tree parameter as a parent
    addParameter( tree );
    
    update();
}

template<class treeType>
TreeLengthStatistic<treeType>::TreeLengthStatistic(const TreeLengthStatistic &n) : TypedFunction<double>( n ), tree( n.tree ) {
    // no need to add parameters, happens automatically
}

template<class treeType>
TreeLengthStatistic<treeType>::~TreeLengthStatistic( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}


template<class treeType>
TreeLengthStatistic<treeType>* TreeLengthStatistic<treeType>::clone( void ) const {
    return new TreeLengthStatistic<treeType>( *this );
}

template<class treeType>
void TreeLengthStatistic<treeType>::update( void ) {
    
    double treeHeight = tree->getValue().getRoot().getAge();
    
    std::vector<TopologyNode*> nodes = tree->getValue().getNodes();
    std::vector<TopologyNode*>::iterator it = nodes.begin();
    std::vector<TopologyNode*>::iterator it_end = nodes.end();
    double treeLength = 0.0;
    for ( ; it != it_end; it++)
        treeLength += (*it)->getBranchLength();
    
    *value = treeLength;
}


template<class treeType>
void TreeLengthStatistic<treeType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    if (oldP == tree) {
        tree = static_cast<const TypedDagNode<treeType>* >( newP );
    }
}

#endif /* defined(__revbayes_mlandis__TreeLengthStatistic__) */
