#include "RateMatrix.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "TypedDagNode.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix::RateMatrix(size_t n) : RateGenerator(n)
{

}





/** Destructor */
RateMatrix::~RateMatrix(void)
{
    
}


RateMatrix& RateMatrix::assign(const Assignable &m)
{
    const RateMatrix *rm = dynamic_cast<const RateMatrix*>(&m);
    if ( rm != NULL )
    {
        return operator=(*rm);
    }
    else
    {
        throw RbException("Could not assign rate matrix.");
    }
}

void RateMatrix::executeMethod(const std::string &n, const std::vector<const DagNode*> &args, RbVector<double> &rv) const
{
    size_t n_states = this->getNumberOfStates();
//    rv.resize(n_states);
    rv.clear();
    
    size_t from_idx = static_cast<const TypedDagNode<int> *>( args[0] )->getValue()-1;
    
    for (size_t to_idx = 0; to_idx < n_states; to_idx++)
    {
        rv.push_back(this->getRate(from_idx, to_idx, 0.0, 1.0));
    }
}