#include "RbUtil.h"
#include "Tree.h"
#include "TraceTree.h"
#include "TreeSummary.h"


#include <sstream>
#include <string>
#include <vector>

using namespace RevBayesCore;


TraceTree::TraceTree( bool c ) :
    clock( c )
{
    outgroup = "";
    values.clear();
    invalidate();
}


TraceTree::~TraceTree()
{

    values.clear();
}


void TraceTree::addObject(Tree *t)
{
    // re-root the tree so that we can compare the the trees
    //        if ( outgroup == "" )
    //            outgroup = t->getTipNode(0).getName();
    //        t->reroot( outgroup );
    
    
    values.push_back(*t);
    
    delete t;
    
    // invalidate for recalculation of meta data
    invalidate();
}


void TraceTree::addValueFromString(const std::string &s)
{
    
    Tree *t = new Tree();
    Serializer<Tree, IsDerivedFrom<Tree, Serializable>::Is >::ressurectFromString( t, s );
    
    addObject( t );
    
}

/** Clone function */
TraceTree* TraceTree::clone() const
{
    
    return new TraceTree(*this);
}


void TraceTree::invalidate()
{

    // set values to defaults and mark for recalculation
    burnin                          = -1;
    ess                             = -1;
    stepSize                        = 1;
    
    converged                       = NOT_CHECKED;
    passedStationarityTest          = NOT_CHECKED;
    passedGewekeTest                = NOT_CHECKED;
    //    passedHeidelbergerWelchStatistic = NOT_CHECKED;
    //    passedRafteryLewisStatistic = NOT_CHECKED;
    passedEssThreshold              = NOT_CHECKED;
    passedSemThreshold              = NOT_CHECKED;
    passedIidBetweenChainsStatistic = NOT_CHECKED;
    passedGelmanRubinTest           = NOT_CHECKED;
    
    
}


bool TraceTree::isCoveredInInterval(const std::string &v, double i) const
{
    
    Tree t = Tree();
    t.initFromString( v );
    
    TreeSummary summary = TreeSummary( *this );
    bool covered = summary.isTreeContainedInCredibleInterval(t, 0.9);
    
    return covered;
}


/** Print value for user */
void TraceTree::printValue(std::ostream &o) const
{
    
    o << "TraceTree values to be printed ...";
}


void TraceTree::removeObjectAtIndex (int index)
{
    // create a iterator for the vector
    std::vector<Tree>::iterator it = values.begin();
    
    //jump to the position to remove
    it += index;
    
    // remove the element
    values.erase(it);
    
    // invalidate for recalculation of meta data
    invalidate();
}


void TraceTree::removeLastObject()
{
    // remove object from list
    values.pop_back();
    
    // invalidate for recalculation of meta data
    invalidate();
}
