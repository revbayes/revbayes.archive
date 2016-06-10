//
//  StitchTreeFunction.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 3/2/16.
//  Copyright © 2016 Michael Landis. All rights reserved.
//

#include "StitchTreeFunction.h"
#include "RbException.h"
#include <set>

using namespace RevBayesCore;

StitchTreeFunction::StitchTreeFunction(const TypedDagNode<Tree> *bt, const TypedDagNode<RbVector<Tree> >* pc, RbVector<RbVector<Taxon> > pt) : TypedFunction<Tree>( new Tree() ),
backboneTree( bt ),
patchClades( pc ),
patchTaxa( pt ),
numPatches( pt.size() ),
haveIndex(false)
{
    // add the lambda parameter as a parent
    addParameter( bt );
    addParameter( pc );
    
    initTaxonGroups();
    
    value = new Tree();
    
    update();
}


StitchTreeFunction::~StitchTreeFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



StitchTreeFunction* StitchTreeFunction::clone( void ) const
{
    return new StitchTreeFunction( *this );
}


void StitchTreeFunction::initTaxonGroups(void)
{
    
    // enter backbone taxa into set
    std::vector<Taxon> backboneTaxa_tmp = backboneTree->getValue().getTaxa();
    std::set<Taxon> backboneTaxa;
    for (size_t i = 0; i < backboneTaxa_tmp.size(); i++)
        backboneTaxa.insert(backboneTaxa_tmp[i]);
    
    // identify taxa to be stitched or to be dropped
    prunedTaxa.resize(numPatches);
    for (size_t i = 0; i < numPatches; i++)
    {
        std::set<Taxon> remainingTaxa;
        std::vector<Taxon> t = patchClades->getValue()[i].getTaxa();
        for (size_t j = 0; j < t.size(); j++)
        {
            remainingTaxa.insert(t[j]);
        }
        
        for (size_t j = 0; j < patchTaxa[i].size(); j++)
        {
            std::set<Taxon>::iterator it;
            Taxon x = patchTaxa[i][j];
            
            // determine which patch taxa should NOT be pruned
            it = remainingTaxa.find( x );
            if (it != remainingTaxa.end())
                remainingTaxa.erase( x );
            
            // determine which patch taxon is the stitch point
            it = backboneTaxa.find( x );
            if (it != backboneTaxa.end())
            {
                stitchTaxon.push_back( x );
                backboneTaxa.erase( x );
            }
        }
        
        // add remaining taxa to prune list
        for (std::set<Taxon>::iterator it = remainingTaxa.begin(); it != remainingTaxa.end(); it++)
        {
            prunedTaxa[i].insert(*it);
        }
    }
    
    // assign index to each taxon
    std::set<Taxon> allTaxa;
    std::set<Taxon>::iterator it;
    for (it = backboneTaxa.begin(); it != backboneTaxa.end(); it++)
    {
        allTaxa.insert(*it);
    }
    for (size_t i = 0; i < numPatches; i++)
    {
        patchClades->getValue()[i].getTaxa();
        std::vector<Taxon> t = patchClades->getValue()[i].getTaxa();
        for (size_t j = 0; j < t.size(); j++)
        {
            allTaxa.insert(t[j]);
        }
    }
    
    return;
}

void StitchTreeFunction::keep(DagNode *affecter)
{
    //delegate to base class
    TypedFunction< Tree >::keep( affecter );
    
}

void StitchTreeFunction::recursivelyCleanPatchClade(TopologyNode* node, TopologyNode*& newRoot, std::set<Taxon>& remainingTaxa)
{
    
    // remove self if found in remainingTaxa
    if (node->isTip())
    {
        std::set<Taxon>::iterator it = remainingTaxa.find( node->getTaxon() );
        if (it != remainingTaxa.end())
        {
            remainingTaxa.erase( node->getTaxon() );
            TopologyNode* parent = &node->getParent();
            std::vector<TopologyNode*> children = parent->getChildren();
            for (size_t i = 0; i < children.size(); i++)
            {
                if (children[i] != node)
                    newRoot = children[i];
                children[i]->setParent(NULL);
                parent->removeChild(children[i]);
            }
        }
        return;
    }
    
    // recurse towards tips
    std::vector<TopologyNode*> children = node->getChildren();
    for (size_t i = 0; i < children.size(); i++)
    {
        recursivelyCleanPatchClade(children[i], newRoot, remainingTaxa);
        if (remainingTaxa.size() == 0)
            break;
    }
    
    return;
}

void StitchTreeFunction::recursivelyStitchPatchClades(TopologyNode* node)
{
    
    if (!haveIndex)
    {
        stitchTreeIndex[ numPatches ][ node->getIndex() ] = 0;
    }
    
    // stich patch clade to matching tip taxon
    if (node->isTip())
    {
        for (size_t i = 0; i < patchTaxa.size(); i++)
        {
            if (node->getTaxon() == stitchTaxon[i])
            {
                // remove the node
                TopologyNode* parent = &node->getParent();
                parent->removeChild(node);
                node->setParent(NULL);
                
                // add the patch clade
                const Tree& t = patchClades->getValue()[i];
                
                if (!haveIndex)
                {
                    const std::vector<TopologyNode*>& nodes = t.getNodes();
                    for (size_t j = 0; j < nodes.size(); j++) {
                        stitchTreeIndex[i][ nodes[j]->getIndex() ] = 0;
                    }
                }
                
                TopologyNode* patchRoot = new TopologyNode( t.getRoot() );
                
                // prune out non-patch taxa
                std::set<Taxon> remainingTaxa = prunedTaxa[i];
                recursivelyCleanPatchClade(patchRoot, patchRoot, remainingTaxa);
                
                // add patch clade to base tree
                parent->addChild(patchRoot);
                patchRoot->setParent(parent);
                return;
            }
        }
    }
    
    // recurse towards tips
    std::vector<TopologyNode*> children = node->getChildren();
    for (size_t i = 0; i < children.size(); i++)
    {
        recursivelyStitchPatchClades(children[i]);
    }
    
    return;
}

void StitchTreeFunction::reInitialized( void )
{
    ; // *value = tau->getValue();
    
}


void StitchTreeFunction::restore(DagNode *restorer)
{
    //delegate to base class
    TypedFunction< Tree >::restore( restorer );
}


void StitchTreeFunction::touch(DagNode *toucher)
{
    
    //delegate to base class
    TypedFunction< Tree >::touch( toucher );
    
}


// NB: Could be vastly improved with TreeListener events and touchSpecialization
void StitchTreeFunction::update( void )
{
    updateStitchTree();
}

void StitchTreeFunction::updateStitchTree( void )
{
    
    delete value;
    
    // start with backbone tree
    value = backboneTree->getValue().clone();
    
    // build out the tree
    TopologyNode* root = &value->getRoot();
    recursivelyStitchPatchClades(root);
    
    // revise indexing
    // ...
    
    // set root
    value->setRoot( root );
    
//    root = &value->getRoot();
    
//    std::vector<TopologyNode*> nodes = value->getNodes();
//    
//    // update tip nodes with stored taxon-index
//    for (size_t i = 0; i < nodes.size(); i++)
//        nodes[i]->setIndex( taxonToIndex[ nodes[i]->getTaxon() ] );
//    
//    value->setRoot( root, true, true );
    
}

void StitchTreeFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == backboneTree)
    {
        backboneTree = static_cast<const TypedDagNode<Tree>* >( newP );
    }
    else if (oldP == patchClades)
    {
        patchClades = static_cast<const TypedDagNode<RbVector<Tree> >* >( newP );
    }
}

