//
//  ExponentialBranchTree.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-24.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include "ExponentialBranchTree.h"
#include "DeterministicNode.h"
#include <cstdlib>


using namespace RevBayesCore;


// constructor(s)
ExponentialBranchTree::ExponentialBranchTree(const TypedDagNode< TimeTree > *t, const TypedDagNode< MultivariatePhyloProcess > *p, const TypedDagNode<double>* o, const TypedDagNode< int >* i): 

    TypedFunction< std::vector< double > >( new std::vector< double >(p->getValue().getTimeTree()->getNumberOfNodes() -1, 0.0 ) ),
    tau(t), process(p), offset(o), traitindex(i) {
    
    this->addParameter( tau );
    this->addParameter( process );
    if (offset != NULL) {
         this->addParameter( offset );   
    }    
    if (traitindex != NULL) {
        this->addParameter(traitindex) ;
    }
    update();
}

ExponentialBranchTree::ExponentialBranchTree(const ExponentialBranchTree &n):

    TypedFunction< std::vector< double > >( n ), 
        tau(n.tau), process( n.process ), offset( n.offset ), traitindex( n.traitindex) {
    
}


ExponentialBranchTree* ExponentialBranchTree::clone(void) const {
    return new ExponentialBranchTree( *this );
}


void ExponentialBranchTree::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == tau ) {
        tau = static_cast< const TypedDagNode<TimeTree> * >( newP );
    }

    if ( oldP == process ) {
        process = static_cast< const TypedDagNode<MultivariatePhyloProcess> * >( newP );
    }

    if (oldP == offset) {
        offset = static_cast<const TypedDagNode< double >* >(newP);
    }
    
    if (oldP == traitindex) {
        traitindex = static_cast<const TypedDagNode< int >* >(newP);
    }
}

void ExponentialBranchTree::update(void)    {
    
    // get the root
    const TopologyNode& root = process->getValue().getTimeTree()->getRoot();
    recursiveUpdate(root);
}

int ExponentialBranchTree::getTraitIndex()  {
    return traitindex->getValue() - 1;
}

void ExponentialBranchTree::recursiveUpdate(const RevBayesCore::TopologyNode &from)    {

    size_t index = from.getIndex();

    if (! from.isRoot())    {
        // get the index
        
        size_t upindex = from.getParent().getIndex();

        if (getTraitIndex() >= 0)    {
            
            double x1 = process->getValue()[index][getTraitIndex()] + offset->getValue();
            double x2 = process->getValue()[upindex][getTraitIndex()] + offset->getValue();
            double y = 0.5 * (exp(x1) + exp(x2));
        
            // we store this val here
            (*value)[index] = y;
            
        }
        else    {

            std::cerr << "should not be in simple univariate case\n";
            exit(1);
            /*
            double x1 = nodeval->getValue()[index];
            double x2 = nodeval->getValue()[upindex];
            
            double y = 0.5 * (exp(x1) + exp(x2));
        
            // we store this val here
            (*value)[index] = y;
             */
            
        }        
    }
    
    // simulate the val for each child (if any)
    size_t numChildren = from.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i) {
        const TopologyNode& child = from.getChild(i);
        recursiveUpdate(child);
    }
}


void ExponentialBranchTree::corruptAll() {
    recursiveCorruptAll(tau->getValue().getRoot());
}

void ExponentialBranchTree::recursiveCorruptAll(const TopologyNode& from)    {
    
    dagNode->addTouchedElementIndex(from.getIndex());
    for (size_t i = 0; i < from.getNumberOfChildren(); ++i) {
        recursiveCorruptAll(from.getChild(i));
    }    
}

void ExponentialBranchTree::flagNodes() {

    // the value at some of the nodes has changed
    // flag them as well as their immediate children
    // only those nodes will recompute their probability

    const std::set<size_t> &indices = dagNode->getTouchedElementIndices();

    // flag recomputation only for the nodes
    for (std::set<size_t>::iterator it = indices.begin(); it != indices.end(); ++it) {
        dagNode->addTouchedElementIndex(*it);
        const TopologyNode& from = tau->getValue().getNode(*it);
        for (size_t i = 0; i < from.getNumberOfChildren(); ++i) {
            dagNode->addTouchedElementIndex(from.getChild(i).getIndex());
        }
    }
}
/*
void ExponentialBranchTree::touch(DagNode *toucher)    {

    
    if (toucher == process) {
        flagNodes();
    }
    else if (toucher == tau)    {
        // nothing to do here! these are rates...
    } 
    else    {
        corruptAll();
    }
//    dagNode->clearTouchedElementIndices();    
     
}


void ExponentialBranchTree::restore(DagNode *restorer)    {

    
    if (restorer == process) {
        flagNodes();
    }
    else if (restorer == tau)    {
        // nothing to do here! these are rates...
    } 
    else    {
        corruptAll();
    }
//    dagNode->clearTouchedElementIndices();    
     
}
*/


