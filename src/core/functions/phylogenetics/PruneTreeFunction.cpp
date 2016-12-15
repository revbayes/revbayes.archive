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

PruneTreeFunction::PruneTreeFunction(const TypedDagNode<Tree> *t,  std::set<Taxon> rt, std::set<Taxon> pt, bool pf) : TypedFunction<Tree>( new Tree() ),
tau( t ),
retainedTaxa( std::set<Taxon>() ),
initRetainedTaxa( rt ),
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
    ; // *value = tau->getValue();
    
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

    // prune the parent tree
    pruneCount.clear();
    recursivelyRetainTaxa( &value->getRoot() );
}

int PruneTreeFunction::recursivelyRetainTaxa(RevBayesCore::TopologyNode *node)
{
    // tip node
    if (node->isTip())
    {
        const std::vector<Taxon>& taxa = tau->getValue().getTaxa();
        const Taxon& tipTaxon = taxa[node->getIndex()];

        std::set<Taxon>::iterator it = retainedTaxa.find( tipTaxon );
        
        // found it
        if (it != retainedTaxa.end()) {
//            std::cout << "retain\t" << node->getIndex() << "\t" << node << "\t" << tipTaxon.getName() << "\n";
            return 1;
        }
        else {
//            std::cout << "prune\t" << node->getIndex() << "\t" << node << "\t" << tipTaxon.getName() << "\n";
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
//        std::cout << "recurse\t" << node->getIndex() << "\t" << node << "\tfor child\t" << children[i]->getIndex() << "\t" << children[i] << "\n";
//        count[i] = recursivelyRetainTaxa(children[i]);
        pruneCount[ children[i] ] = recursivelyRetainTaxa(children[i]);
        total_count += pruneCount[ children[i] ];
        if (pruneCount[ children[i] ] > 0)
            number_children_retained++;
    }
    
    if (node->isRoot())
    {
        TopologyNode* root = node;
        if (number_children_retained == 0)
        {

        }
        // if one daughter, patch over node
        else if (number_children_retained == 1)
        {

            std::vector<TopologyNode*> fresh_children = node->getChildren();
            for (size_t i = 0; i < fresh_children.size(); i++)
            {
                // remove all of the node's children
                // NB: node->removeAllChildren() also calls delete
                node->removeChild(fresh_children[i]);
//                std::cout << "del-child\t" << fresh_children[i]->getIndex() << "\t" << fresh_children[i] << "\t" << fresh_children[i]->getTaxon().getName() << "\n";
            }
            for (size_t i = 0; i < fresh_children.size(); i++)
            {
                if (pruneCount[ fresh_children[i] ] == 0)
                    continue;
                
                root = fresh_children[i];
                fresh_children[i]->setParent(NULL);
//                std::cout << "add-child\t" << fresh_children[i]->getIndex() << "\t" << fresh_children[i] << "\t" << fresh_children[i]->getTaxon().getName() << "\n";
            }

            node->setParent(NULL);
            
            // free memory
            // delete node;
            // setRoot will free this memory
        }
        
        // check ntaxa > 2
        bool good = true;
        if (!good) {
            throw RbException("");
        }

        value->setRoot( root, true );
        std::vector<TopologyNode*> nodes = value->getNodes();
        
        // update tip nodes with stored taxon-index
        for (size_t i = 0; i < value->getNumberOfTips(); i++)
        {
            nodes[i]->setIndex( retainedIndices[ nodes[i]->getTaxon() ] );
        }
        
//        value->setRoot( root, false );
        value->setRoot(root, true);
    }
    else if (node->isInternal())
    {
//        std::cout << "patch\t" << node->getIndex() << "\t" << node << "\n";
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
            
            // want to retain order of fresh_children for parent...
            std::vector<TopologyNode*> fresh_parent_children = parent->getChildren();
            
            for (size_t i = 0; i < fresh_children.size(); i++)
            {
                // remove all of the node's children
                // NB: node->removeAllChildren() also calls delete
                node->removeChild(fresh_children[i]);
//                std::cout << "del-child\t" <<  pruneCount[ fresh_children[i] ] << "\t" << fresh_children[i]->getIndex() << "\t" << fresh_children[i] << "\t" << fresh_children[i]->getTaxon().getName() << "\n";
            }
            for (size_t i = 0; i < fresh_children.size(); i++)
            {
                if ( pruneCount[ fresh_children[i] ] == 0)
                    continue;
                
                fresh_children[i]->setParent(parent);
                parent->addChild(fresh_children[i]);
//                std::cout << "add-child\t" << pruneCount[ fresh_children[i] ] << "\t" << fresh_children[i]->getIndex() << "\t" << fresh_children[i] << "\t" << fresh_children[i]->getTaxon().getName() << "\n";
            }
            node->setParent(NULL);
            parent->removeChild(node);
            
            // free memory
            delete node;
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
    
    // initialize retainedTaxa
    if (initRetainedTaxa.size() > 2)
    {
        retainedTaxa = initRetainedTaxa;
    }
    else
    {
        std::vector<Taxon> allTaxa = tau->getValue().getTaxa();
        for (size_t i = 0; i < allTaxa.size(); i++)
            retainedTaxa.insert(allTaxa[i]);
    }
    
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
    
    // map retained taxa to tip indices
    int idx = 0;
    for (std::set<Taxon>::iterator it = retainedTaxa.begin(); it != retainedTaxa.end(); it++)
    {
        retainedIndices[*it] = idx++;
    }

}

void PruneTreeFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == tau)
    {
        tau = static_cast<const TypedDagNode<Tree>* >( newP );
    }
}

