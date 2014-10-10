/* 
 * File:   T92GCBranchTree.cpp
 * Author: nl
 * 
 * Created on 25 juillet 2014, 20:20
 */

#include "T92GCBranchTree.h"
#include "RateMatrix_HKY.h"

using namespace RevBayesCore;


// constructor(s)
T92GCBranchTree::T92GCBranchTree(const TypedDagNode< TimeTree > *t, const TypedDagNode< RbVector<double> > *g, const TypedDagNode<double>* k): TypedFunction< RbVector<RateMatrix> >( new RbVector< RateMatrix >(t->getValue().getNumberOfNodes()-1, RateMatrix_HKY() ) ),
    tau(t),
    gctree(g),
    kappa(k)
{
    
    this->addParameter( tau );
    this->addParameter( gctree );
    this->addParameter( kappa );   

    update();    
}


T92GCBranchTree* T92GCBranchTree::clone(void) const {
    return new T92GCBranchTree( *this );
}


void T92GCBranchTree::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == tau ) {
        tau = static_cast< const TypedDagNode<TimeTree> * >( newP );
    }

    if ( oldP == gctree ) {
        gctree = static_cast< const TypedDagNode< RbVector<double> > * >( newP );
    }

    if (oldP == kappa) {
        kappa = static_cast<const TypedDagNode< double >* >(newP);
    }
}

void T92GCBranchTree::update(void)    {
    
    recursiveUpdate(tau->getValue().getRoot());
}

void T92GCBranchTree::recursiveUpdate(const RevBayesCore::TopologyNode &from)    {

    if (! from.isRoot())    {
        size_t index = from.getIndex();

        double gc = gctree->getValue()[index];
        if ((gc < 0) || (gc > 1))   {
            std::cerr << "error: gc value : " << gc << '\n';
            exit(1);
        }

        RateMatrix_HKY* matrix = dynamic_cast<RateMatrix_HKY*> (&(*value)[index]);
        std::vector<double> v(4);
        v[0] = v[3] = 0.5 * (1 - gc);
        v[1] = v[2] = 0.5 * gc;
        matrix->setStationaryFrequenciesByCopy(v);
        matrix->setKappa(kappa->getValue());
    }    
    // simulate the val for each child (if any)
    size_t numChildren = from.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i) {
        const TopologyNode& child = from.getChild(i);
        recursiveUpdate(child);
    }
}
