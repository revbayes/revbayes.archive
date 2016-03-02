//
//  PruneTreeFunction.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 3/2/16.
//  Copyright © 2016 Michael Landis. All rights reserved.
//

#include "PruneTreeFunction.h"
#include "RbException.h"

using namespace RevBayesCore;

PruneTreeFunction::PruneTreeFunction(const TypedDagNode<Tree> *t, std::set<Taxon> pt, bool pf) : TypedFunction<Tree>( new Tree() ),
tau( t ),
retainedTaxa( std::set<Taxon>() ),
prunedTaxa( pt ),
pruneFossils( pf )
{
    // add the lambda parameter as a parent
    addParameter( tau );
    
    setRetainedTaxa();
    
    update();
}


PruneTreeFunction::~PruneTreeFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



PruneTreeFunction* PruneTreeFunction::clone( void ) const
{
    return new PruneTreeFunction( *this );
}


void PruneTreeFunction::keep(DagNode *affecter)
{
    //delegate to base class
    TypedFunction< Tree >::keep( affecter );
    
}


void PruneTreeFunction::reInitialized( void )
{
    *value = tau->getValue();
    
}


void PruneTreeFunction::restore(DagNode *restorer)
{
    //delegate to base class
    TypedFunction< Tree >::restore( restorer );
}


void PruneTreeFunction::touch(DagNode *toucher)
{
    
    //delegate to base class
    TypedFunction< Tree >::touch( toucher );
    
}

// NB: Could be vastly improved with TreeListener events and touchSpecialization
void PruneTreeFunction::update( void )
{
    delete value;
    
    // get a copy of the parent tree
    value = tau->getValue().clone();
    value->setRoot( &value->getRoot() );
    
    // prune the parent tree
    recursivelyRetainTaxa( &value->getRoot() );
    
}

int PruneTreeFunction::recursivelyRetainTaxa(RevBayesCore::TopologyNode *node)
{
    // tip node
    if (node->isTip())
    {
        const std::vector<Taxon>& taxa = tau->getValue().getTaxa();
        std::set<Taxon>::iterator it = retainedTaxa.find( taxa[node->getIndex()] );
        
        // found it
        if (it != retainedTaxa.end()) {
            return 1;
        }
        else {
            return 0;
        }
    }
    int total_count = 0;
    int number_children_retained = 0;
    
    // determine if subclade has any retained taxa
    std::vector<TopologyNode*> children = node->getChildren();
    std::vector<int> count( children.size(), 0 );
    for (size_t i = 0; i < children.size(); i++)
    {
        count[i] = recursivelyRetainTaxa(children[i]);
        total_count += count[i];
        if (count[i] > 0)
            number_children_retained++;
    }
    
    if (node->isRoot())
    {
        value->setRoot( node );
    }
    else if (node->isInternal())
    {
        // if zero daughters, drop all daughters
        if (number_children_retained == 0)
        {
            node->removeAllChildren();
        }
        // if one daughter, patch over node
        else if (number_children_retained == 1)
        {
            std::vector<TopologyNode*> fresh_children = node->getChildren();
            TopologyNode* parent = &node->getParent();
            for (size_t i = 0; i < count.size(); i++)
            {
                // one daughter lineage is dropped
                if (count[i] == 0)
                {
                    node->removeChild(fresh_children[i]);
                }
                
                // the remaining daughters are now children of the node's parent, not of the node itself
                else
                {
                    fresh_children[i]->setParent(parent);
                    parent->addChild(fresh_children[i]);
                }
            }
            node->setParent(NULL);
            parent->removeChild(node);
            // perhaps a memory leak, but it shoud be handled by setRoot()
            // delete node;
        }
        // if two or more daughters, do not prune
        // ...
        
        return total_count;
    }
    else
    {
        throw RbException("fnPruneTree encountered unexpected type of node!");
    }

    return true;
}

void PruneTreeFunction::setRetainedTaxa(void)
{
    retainedTaxa = std::set<Taxon>();
    
    std::vector<TopologyNode*> nodes = tau->getValue().getNodes();
    
    // add all taxa
    std::vector<Taxon> allTaxa = tau->getValue().getTaxa();
    for (size_t i = 0; i < allTaxa.size(); i++)
        retainedTaxa.insert(allTaxa[i]);
    
    // remove fossil taxa
    for (size_t i = 0; i < tau->getValue().getNumberOfTips(); i++)
    {
        if (pruneFossils && nodes[i]->isFossil()) {
            const Taxon& t = nodes[i]->getTaxon();
            bool found = retainedTaxa.find(t) != retainedTaxa.end();
            if (found)
                retainedTaxa.erase(t);
        }
    }
    
    // remove specified taxon set in prunedTaxa
    for (std::set<Taxon>::iterator it = prunedTaxa.begin(); it != prunedTaxa.end(); it++)
    {
        bool found = retainedTaxa.find(*it) != retainedTaxa.end();
        if (found)
            retainedTaxa.erase(*it);
    }
}

void PruneTreeFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == tau)
    {
        tau = static_cast<const TypedDagNode<Tree>* >( newP );
    }
}

