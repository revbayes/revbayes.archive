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
T92GCBranchTree::T92GCBranchTree(const TypedDagNode< TimeTree > *t, const TypedDagNode< std::vector<double> > *g, const TypedDagNode< double>* r, const TypedDagNode<double>* k): 

    TypedFunction< RbVector<RateMatrix> >( new RbVector< RateMatrix >(t->getValue().getNumberOfNodes()-1, RateMatrix_HKY() ) ),
    tau(t), gctree(g), rootgc(r), kappa(k) {
    
    this->addParameter( tau );
    this->addParameter( gctree );
    this->addParameter( rootgc );   
    this->addParameter( kappa );   

    update();    
}

T92GCBranchTree::T92GCBranchTree(const T92GCBranchTree &n):

    TypedFunction< RbVector< RateMatrix > >( n ),
        tau(n.tau), gctree( n.gctree ), rootgc( n.rootgc ), kappa( n.kappa) {
    
}


T92GCBranchTree* T92GCBranchTree::clone(void) const {
    return new T92GCBranchTree( *this );
}


void T92GCBranchTree::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == tau ) {
        tau = static_cast< const TypedDagNode<TimeTree> * >( newP );
    }

    if ( oldP == gctree ) {
        gctree = static_cast< const TypedDagNode<std::vector<double> > * >( newP );
    }

    if (oldP == rootgc) {
        rootgc = static_cast<const TypedDagNode< double >* >(newP);
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

//        double gc = 0.5;
        /*
        if (from.isRoot()) {
            gc = rootgc->getValue();
        } else {
            gc = gctree->getValue()[index];
        }
        */
        
        double gc = gctree->getValue()[index];
        //  double gc = 0.5 + (gctree->getValue()[index] - 0.5) * 0.001;
        RateMatrix_HKY* matrix = dynamic_cast<RateMatrix_HKY*> (&(*value)[index]);
        std::vector<double> v(4);
        v[0] = v[3] = 0.5 * (1 - gc);
        v[1] = v[2] = 0.5 * gc;
        matrix->setStationaryFrequenciesByCopy(v);
        matrix->setKappa(kappa->getValue());
        matrix->updateMatrix();
    }    
    // simulate the val for each child (if any)
    size_t numChildren = from.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i) {
        const TopologyNode& child = from.getChild(i);
        recursiveUpdate(child);
    }
}
