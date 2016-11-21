#include "HiddenStateRateMatrixFunction.h"
#include "RateMatrix_FreeK.h"
#include "RbException.h"

#include <cmath>

using namespace RevBayesCore;

HiddenStateRateMatrixFunction::HiddenStateRateMatrixFunction(bool r) : TypedFunction<RateGenerator>( NULL ),
    observed_transition_rates( NULL ),
    observed_transition_rates_flat( NULL ),
    hidden_transition_rates( NULL ),
    hidden_transition_rates_flat( NULL ),
    num_observed_states( 0 ),
    num_hidden_states( 0 ),
    rescale( r )
{

}


HiddenStateRateMatrixFunction::~HiddenStateRateMatrixFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}


HiddenStateRateMatrixFunction* HiddenStateRateMatrixFunction::clone( void ) const
{
    return new HiddenStateRateMatrixFunction( *this );
}


void HiddenStateRateMatrixFunction::setObservedTransitionRates(const TypedDagNode< RbVector<double> > *tr)
{
    observed_transition_rates_flat = tr;
    num_observed_states = ceil( sqrt( tr->getValue().size() ) );
    addParameter( observed_transition_rates_flat );
    
    if ( ( observed_transition_rates_flat != NULL || observed_transition_rates != NULL ) &&
         ( hidden_transition_rates_flat != NULL || hidden_transition_rates != NULL ) )
    {
        size_t num_states = num_observed_states * num_hidden_states;
//        value = new RateMatrix_FreeK( num_states, rescale );
        value = new RateMatrix_FreeK( num_states );
        update();
    }
}


void HiddenStateRateMatrixFunction::setObservedTransitionRates(const TypedDagNode< RbVector<RbVector<double> > > *tr)
{
    observed_transition_rates = tr;
    num_observed_states = tr->getValue().size();
    addParameter( observed_transition_rates );
    
    if ( ( observed_transition_rates_flat != NULL || observed_transition_rates != NULL ) &&
        ( hidden_transition_rates_flat != NULL || hidden_transition_rates != NULL ) )
    {
        size_t num_states = num_observed_states * num_hidden_states;
//        value = new RateMatrix_FreeK( num_states, rescale );
        value = new RateMatrix_FreeK( num_states );
        update();
    }
}


void HiddenStateRateMatrixFunction::setHiddenTransitionRates(const TypedDagNode< RbVector<double> > *tr)
{
    hidden_transition_rates_flat = tr;
    num_hidden_states = ceil( sqrt( tr->getValue().size() ) );
    addParameter( hidden_transition_rates_flat );
    
    if ( ( observed_transition_rates_flat != NULL || observed_transition_rates != NULL ) &&
        ( hidden_transition_rates_flat != NULL || hidden_transition_rates != NULL ) )
    {
        size_t num_states = num_observed_states * num_hidden_states;
//        value = new RateMatrix_FreeK( num_states, rescale );
        value = new RateMatrix_FreeK( num_states );
        update();
    }
}


void HiddenStateRateMatrixFunction::setHiddenTransitionRates(const TypedDagNode< RbVector<RbVector<double> > > *tr)
{
    hidden_transition_rates = tr;
    num_hidden_states = tr->getValue().size();
    addParameter( hidden_transition_rates );
    
    if ( ( observed_transition_rates_flat != NULL || observed_transition_rates != NULL ) &&
        ( hidden_transition_rates_flat != NULL || hidden_transition_rates != NULL ) )
    {
        size_t num_states = num_observed_states * num_hidden_states;
//        value = new RateMatrix_FreeK( num_states, rescale );
        value = new RateMatrix_FreeK( num_states );
        update();
    }
}


void HiddenStateRateMatrixFunction::update( void )
{
    
    // get the 2-d matrix of observed state transition rates
    RbVector< RbVector<double> > observed_rate_matrix( num_observed_states, RbVector<double>( num_observed_states, 0.0 ) );
    if (observed_transition_rates == NULL && observed_transition_rates_flat != NULL)
    {
        std::vector<double> observed_rates_flat = observed_transition_rates_flat->getValue();
        size_t k = 0;
        
        for (size_t i = 0; i < num_observed_states; i++)
        {
            for (size_t j = 0; j < num_observed_states; j++)
            {
                if (i != j)
                {
                    observed_rate_matrix[i][j] = observed_rates_flat[ k++ ];
                }
            }
        }
    }
    else if (observed_transition_rates != NULL && observed_transition_rates_flat == NULL)
    {
        observed_rate_matrix = observed_transition_rates->getValue();
    }
    
    // get the 2-d matrix of hidden state transition rates
    RbVector< RbVector<double> > hidden_rate_matrix( num_hidden_states, RbVector<double>( num_hidden_states, 0.0 ) );
    if (hidden_transition_rates == NULL && hidden_transition_rates_flat != NULL)
    {
        std::vector<double> hidden_rates_flat = hidden_transition_rates_flat->getValue();
        size_t k = 0;
        
        for (size_t i = 0; i < num_hidden_states; i++)
        {
            for (size_t j = 0; j < num_hidden_states; j++)
            {
                if (i != j)
                {
                    hidden_rate_matrix[i][j] = hidden_rates_flat[ k++ ];
                }
            }
        }
    }
    else if (hidden_transition_rates != NULL && hidden_transition_rates_flat == NULL)
    {
        hidden_rate_matrix = hidden_transition_rates->getValue();
    }
    
    // set up a 2-d matrix to hold the rates for the combined observed and hidden states
    size_t num_states = num_observed_states * num_hidden_states;
    std::vector< std::vector<double> > rate_matrix = std::vector< std::vector<double> >( num_states, std::vector<double>( num_states, 0.0 ) );
    for (size_t initial_hidden_state = 0; initial_hidden_state < num_hidden_states; initial_hidden_state++)
    {
        for (size_t initial_observed_state = 0; initial_observed_state < num_observed_states; initial_observed_state++)
        {
            size_t i = (initial_hidden_state * num_observed_states) + initial_observed_state;
            
            for (size_t final_hidden_state = 0; final_hidden_state < num_hidden_states; final_hidden_state++)
            {
                for (size_t final_observed_state = 0; final_observed_state < num_observed_states; final_observed_state++)
                {
                    size_t j = (final_hidden_state * num_observed_states) + final_observed_state;
            
                    if (initial_hidden_state == final_hidden_state)
                    {
                        rate_matrix[i][j] = observed_rate_matrix[initial_observed_state][final_observed_state];
                    }
                    else if (initial_observed_state == final_observed_state)
                    {
                        rate_matrix[i][j] = hidden_rate_matrix[initial_hidden_state][final_hidden_state];
                    }
                }
            }
        }
    }
    
    // flatten the 2-d rate matrix into a vector
    std::vector<double> all_rates_flat = std::vector<double>( num_states * (num_states - 1), 0.0 );
    size_t k = 0;
    for (size_t i = 0; i < num_states; i++)
    {
        for (size_t j = 0; j < num_states; j++)
        {
            if (i != j)
            {
                all_rates_flat[k++] = rate_matrix[i][j];
            }
        }
    }
    
    // finally set the rates in the actual matrix
    static_cast< RateMatrix_FreeK* >(value)->setTransitionRates( all_rates_flat );
    value->update();
}


void HiddenStateRateMatrixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == observed_transition_rates)
    {
        observed_transition_rates = static_cast<const TypedDagNode< RbVector<RbVector<double> > >* >( newP );
    }
    else if (oldP == observed_transition_rates_flat)
    {
        observed_transition_rates_flat = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == hidden_transition_rates)
    {
        hidden_transition_rates = static_cast<const TypedDagNode< RbVector<RbVector<double> > >* >( newP );
    }
    else if (oldP == hidden_transition_rates_flat)
    {
        hidden_transition_rates_flat = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    
}

