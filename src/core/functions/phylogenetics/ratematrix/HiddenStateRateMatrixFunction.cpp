#include "HiddenStateRateMatrixFunction.h"
#include "RateMatrix_FreeSymmetric.h"
#include "RbException.h"

#include <cmath>

using namespace RevBayesCore;

HiddenStateRateMatrixFunction::HiddenStateRateMatrixFunction(const TypedDagNode<double> *w, const TypedDagNode<double> *b, size_t n_org, size_t n_cat, bool r) : TypedFunction<RateGenerator>( NULL ),
    n_org_states( n_org ),
    n_categories( n_cat ),
    within_category_rate( w ),
    between_categories_rates( b )
{
    size_t num_states = n_org_states * n_categories;
    value = new RateMatrix_FreeSymmetric( num_states, r  );
    
    // add the rate and frequency parameters as parents
    addParameter( within_category_rate );
    addParameter( between_categories_rates );
    
    update();
}


HiddenStateRateMatrixFunction::~HiddenStateRateMatrixFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



HiddenStateRateMatrixFunction* HiddenStateRateMatrixFunction::clone( void ) const
{
    return new HiddenStateRateMatrixFunction( *this );
}


void HiddenStateRateMatrixFunction::update( void )
{
    size_t n_states = n_org_states*n_categories;
    // get the information from the arguments for reading the file
    std::vector<double> r = std::vector<double>( n_states*(n_states-1), 0.0);
    
    double within_rate  = within_category_rate->getValue();
    double between_rate = between_categories_rates->getValue();
    
    size_t index = 0;
    for (size_t i=0; i<n_states; ++i)
    {
        for (size_t j=i+1; j<n_states; ++j)
        {
            bool is_within_cat  = ( int(i/n_org_states) == int(j/n_org_states) );
            bool is_between_cat = ( (i % n_org_states)  == (j % n_org_states) );
            if ( is_within_cat == true  )
            {
                r[index] = within_rate;
            }
            else if ( is_between_cat == true )
            {
                r[index] = between_rate;
            }
            else
            {
                r[index] = 0.0;
            }
            
            ++index;
        }
        
    }
    
    // set the base frequencies
    static_cast< RateMatrix_FreeSymmetric* >(value)->setTransitionRates(r);
    
    value->update();
}



void HiddenStateRateMatrixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == within_category_rate)
    {
        within_category_rate = static_cast<const TypedDagNode< double >* >( newP );
    }
    
    if (oldP == between_categories_rates)
    {
        between_categories_rates = static_cast<const TypedDagNode< double >* >( newP );
    }
    
}

