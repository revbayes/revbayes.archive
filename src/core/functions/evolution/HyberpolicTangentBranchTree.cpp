/* 
 * File:   HyberpolicTangentBranchTree.cpp
 * Author: nl
 * 
 * Created on 25 juillet 2014, 19:52
 */

#include "HyberpolicTangentBranchTree.h"
#include <cstdlib>


using namespace RevBayesCore;


// constructor(s)
HyperbolicTangentBranchTree::HyperbolicTangentBranchTree(const TypedDagNode< TimeTree > *t, const TypedDagNode< MultivariateRealNodeContainer > *p, const TypedDagNode<double>* o, const TypedDagNode< int >* i) : TypedFunction< RbVector< double > >( new RbVector< double >(p->getValue().getTimeTree()->getNumberOfNodes() -1, 0.0 ) ),
    tau(t), process(p), offset(o), traitindex(i)
{
    
    this->addParameter( tau );
    this->addParameter( process );
    if (offset != NULL)
    {
         this->addParameter( offset );   
    }    
    if (traitindex != NULL)
    {
        this->addParameter(traitindex) ;
    }
    update();
}


HyperbolicTangentBranchTree* HyperbolicTangentBranchTree::clone(void) const {
    return new HyperbolicTangentBranchTree( *this );
}


void HyperbolicTangentBranchTree::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == tau ) {
        tau = static_cast< const TypedDagNode<TimeTree> * >( newP );
    }

    if ( oldP == process ) {
        process = static_cast< const TypedDagNode<MultivariateRealNodeContainer> * >( newP );
    }

    if (oldP == offset) {
        offset = static_cast<const TypedDagNode< double >* >(newP);
    }
    
    if (oldP == traitindex) {
        traitindex = static_cast<const TypedDagNode< int >* >(newP);
    }
}

void HyperbolicTangentBranchTree::update(void)    {
    
    // get the root
    const TopologyNode& root = process->getValue().getTimeTree()->getRoot();
    recursiveUpdate(root);
}

int HyperbolicTangentBranchTree::getTraitIndex()  {
    return traitindex->getValue() - 1;
}

void HyperbolicTangentBranchTree::recursiveUpdate(const RevBayesCore::TopologyNode &from)    {

    size_t index = from.getIndex();

    if (! from.isRoot())    {
        // get the index
        
        size_t upindex = from.getParent().getIndex();

        if (getTraitIndex() >= 0)    {
            
            double x1 = exp(process->getValue()[index][getTraitIndex()] + offset->getValue());
            double x2 = exp(process->getValue()[upindex][getTraitIndex()] + offset->getValue());
            double y1 = x1 / (1+x1);
            double y2 = x2 / (1+x2);
            double y = 0.5 * (y1 + y2);
        
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

/*
void HyperbolicTangentBranchTree::corruptAll() {
    recursiveCorruptAll(tau->getValue().getRoot());
}

void HyperbolicTangentBranchTree::recursiveCorruptAll(const TopologyNode& from)    {
    
    dagNode->addTouchedElementIndex(from.getIndex());
    for (size_t i = 0; i < from.getNumberOfChildren(); ++i) {
        recursiveCorruptAll(from.getChild(i));
    }    
}

void HyperbolicTangentBranchTree::flagNodes() {

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

void HyperbolicTangentBranchTree::touch(DagNode *toucher)    {

    if (toucher == process) {
        flagNodes();
    }
    else if (toucher == tau)    {
        // nothing to do here! these are rates...
    } 
    else    {
        corruptAll();
    }
    dagNode->clearTouchedElementIndices();    
}


void HyperbolicTangentBranchTree::restore(DagNode *restorer)    {

    if (restorer == process) {
        flagNodes();
    }
    else if (restorer == tau)    {
        // nothing to do here! these are rates...
    } 
    else    {
        corruptAll();
    }
    dagNode->clearTouchedElementIndices();    
}

*/

