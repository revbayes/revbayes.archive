#include "RbUtil.h"
#include "Tree.h"
#include "TreeTrace.h"


#include <sstream>
#include <string>
#include <vector>

using namespace RevBayesCore;


TreeTrace::TreeTrace( bool c ) :
    clock( c )
{
    outgroup = "";
    values.clear();
    invalidate();
}


TreeTrace::~TreeTrace()
{

    values.clear();
}


void TreeTrace::addObject(Tree *t)
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

/** Clone function */
TreeTrace* TreeTrace::clone() const
{
    
    return new TreeTrace(*this);
}


void TreeTrace::invalidate()
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

/** Print value for user */
void TreeTrace::printValue(std::ostream &o) const
{
    
    o << "TreeTrace values to be printed ...";
}


void TreeTrace::removeObjectAtIndex (int index)
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


void TreeTrace::removeLastObject()
{
    // remove object from list
    values.pop_back();
    
    // invalidate for recalculation of meta data
    invalidate();
}
