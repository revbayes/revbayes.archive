#include "DistributionUniform.h"
#include "AddRemoveFossilProposal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "TreeUtilities.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>

using namespace RevBayesCore;

/**
 * Constructor
 *
 * Here we simply allocate and initialize the Proposal object.
 */
AddRemoveFossilProposal::AddRemoveFossilProposal( StochasticNode<Tree> *n, TypedDagNode<double> *o, double pr, double ps ) : Proposal(),
    tau( n ),
    origin( o ),
    storedFossil(NULL),
    removed(false),
    probRemove( pr ),
    probAncestor( ps)
{
    // tell the base class to add the node
    addNode( tau );
    addNode( origin );

}


/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
void AddRemoveFossilProposal::cleanProposal( void )
{
    ; // do nothing
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
AddRemoveFossilProposal* AddRemoveFossilProposal::clone( void ) const
{
    
    return new AddRemoveFossilProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& AddRemoveFossilProposal::getProposalName( void ) const
{
    static std::string name = "AddRemoveFossil";
    
    return name;
}


/**
 * Perform the proposal.
 *
 * A Uniform-simplex proposal randomly changes some values of a simplex, although the other values
 * change too because of the renormalization.
 * First, some random indices are drawn. Then, the proposal draws a new somplex
 *   u ~ Uniform(val[index] * alpha)
 * where alpha is the tuning parameter.The new value is set to u.
 * The simplex is then renormalized.
 *
 * \return The hastings ratio.
 */
double AddRemoveFossilProposal::doProposal( void )
{
    if(removed)
        delete &storedFossil->getParent();

    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    Tree &t = tau->getValue();

    // are we conditioning on the origin time?
    bool useOrigin = true;

    if(origin == NULL || origin->getValue() == t.getRoot().getAge())
    {
        useOrigin = false;
    }

    numFossils = 0;
    numParents = 0;

    for (size_t i = 0; i < t.getNumberOfNodes(); ++i)
    {
        TopologyNode* node = &t.getNode(i);
        numFossils += node->isFossil();
        numParents += !(node->isSampledAncestor() || (!useOrigin && node->isRoot()) );

    }

    TopologyNode* node;

    double u = rng->uniform01();

    double hr = 0;

    // pick a random fossil node to remove
    if(u < probRemove && numFossils > 0)
    {
        double p_rev = numFossils > 1 ? (1.0 - probRemove) : 1.0;
        hr -= log(probRemove/p_rev);

        do {
            u = rng->uniform01();
            size_t index = size_t( std::floor(t.getNumberOfNodes() * u) );
            node = &t.getNode(index);
        } while ( node->isFossil() == false );

        hr += removeFossil( node );
    }
    // or just pick a random node above which to add a fossil
    // but don't add one above the root if conditioning on the root age
    // and don't add fossils between sampled ancestors and their parents
    else
    {
        hr += log(probRemove/(1.0 - probRemove));

        do {
            u = rng->uniform01();
            size_t index = size_t( std::floor(t.getNumberOfNodes() * u) );
            node = &t.getNode(index);
        } while ( node->isSampledAncestor() || (!useOrigin && node->isRoot()) );

        hr += addFossil( node );
    }
    
    return hr;
    
}



/*
 * Add a fossil, either sampled ancestor or not
 */
double AddRemoveFossilProposal::addFossil(TopologyNode *n)
{
    removed = false;

    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;

    Tree &t = tau->getValue();

    std::stringstream name;

    name << "Fossil_" << std::rand();
    storedFossil = new TopologyNode(name.str());
    TopologyNode* new_parent_node = new TopologyNode();

    new_parent_node->addChild(storedFossil);
    storedFossil->setParent(new_parent_node);
    storedFossil->setFossil(true);

    storedSibling = n;

    double u = rng->uniform01();

    double min_age = n->getAge(),max_age;

    if(n->isRoot())
    {
        // determine upper bound
        max_age = origin->getValue();

        new_parent_node->addChild(n);
        n->setParent(new_parent_node);

        t.setRoot(new_parent_node);
    }
    else
    {
        TopologyNode* grandParent = &n->getParent();
    
        // determine upper bound
        max_age = grandParent->getAge();

        new_parent_node->addChild(n);
        n->setParent(new_parent_node);

        grandParent->removeChild(n);
        grandParent->addChild(new_parent_node);
        new_parent_node->setParent(grandParent);

        t.setRoot(&t.getRoot());
    }
    
    // draw the new age for the parent node
    storedAge = (max_age - min_age) * u + min_age;

    new_parent_node->setAge(storedAge);
    storedFossil->setAge(storedAge);

    double lnJacobian = log(max_age - min_age);

    double hr = log( numParents / (numFossils + 1) );

    if(rng->uniform01() > probAncestor )
    {
        double v = rng->uniform01();

        lnJacobian += log(storedAge);
        hr -= log(1-probAncestor);

        // draw the new age for the fossil node
        double new_fossil_age = storedAge * v;

        storedFossil->setAge(new_fossil_age);
    }
    else
    {
        hr -= log(probAncestor);
        storedFossil->setSampledAncestor(true);
    }

    return hr + lnJacobian;
}



/*
 * Remove a fossil
 */
double AddRemoveFossilProposal::removeFossil(TopologyNode *n)
{
    removed = true;

    Tree &t = tau->getValue();

    TopologyNode *parent = &n->getParent();
    storedSibling = &parent->getChild( 0 );
    if ( storedSibling == n )
    {
        storedSibling = &parent->getChild( 1 );
    }

    storedFossil = n;
    storedAge = parent->getAge();

    parent->removeChild(storedSibling);
    storedSibling->setParent(NULL);

    double max_age;
    if(parent->isRoot())
    {
        max_age = origin->getValue();

        t.setRoot(storedSibling);
    }
    else
    {
        TopologyNode* grandParent = &parent->getParent();

        max_age = grandParent->getAge();

        grandParent->removeChild(parent);
        parent->setParent(NULL);
        grandParent->addChild(storedSibling);
        storedSibling->setParent(grandParent);

        t.setRoot(&t.getRoot());
    }

    double lnJacobian = - log(max_age - storedSibling->getAge());

    double hr = log(numFossils / (numParents - !storedFossil->isSampledAncestor()));

    if(storedFossil->isSampledAncestor())
    {
        hr += log(probAncestor);
    }
    else
    {
        hr += log(1-probAncestor);
        lnJacobian -= log(storedAge);
    }

    return hr + lnJacobian;
}



/**
 *
 */
void AddRemoveFossilProposal::prepareProposal( void )
{
    
}


/**
 * Print the summary of the Proposal.
 *
 * The summary just contains the current value of the tuning parameter.
 * It is printed to the stream that it passed in.
 *
 * \param[in]     o     The stream to which we print the summary.
 */
void AddRemoveFossilProposal::printParameterSummary(std::ostream &o) const
{
    
}


/**
 * Reject the Proposal.
 *
 * Since the Proposal stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the variable/DAG-node to its original value.
 */
void AddRemoveFossilProposal::undoProposal( void )
{
    Tree &t = tau->getValue();
    
    TopologyNode *parent = &storedFossil->getParent();

    // undo the proposal
    if ( !removed )
    {

        parent->removeChild(storedSibling);
        storedSibling->setParent(NULL);

        if(parent->isRoot())
        {
            t.setRoot(storedSibling);
        }
        else
        {
            TopologyNode* grandParent = &parent->getParent();
            grandParent->removeChild(parent);
            parent->setParent(NULL);
            grandParent->addChild(storedSibling);
            storedSibling->setParent(grandParent);

            t.setRoot(&t.getRoot());
        }

        delete parent;
    }
    else
    {
        removed = false;

        if(storedSibling->isRoot())
        {
            parent->addChild(storedSibling);
            storedSibling->setParent(parent);

            t.setRoot(parent);
        }
        else
        {
            TopologyNode* grandParent = &storedSibling->getParent();

            grandParent->removeChild(storedSibling);
            grandParent->addChild(parent);
            parent->setParent(grandParent);
            parent->addChild(storedSibling);
            storedSibling->setParent(parent);

            t.setRoot(&t.getRoot());
        }
    }
    
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void AddRemoveFossilProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    if ( oldN == tau )
    {
        tau = static_cast<StochasticNode<Tree>* >(newN) ;
    }
    else if ( oldN == origin )
    {
        origin = static_cast<TypedDagNode<double>* >(newN);
    }
    
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 *
 * The acceptance ratio for this Proposal should be around 0.44.
 * If it is too large, then we increase the proposal size,
 * and if it is too small, then we decrease the proposal size.
 */
void AddRemoveFossilProposal::tune( double rate )
{
    
}

