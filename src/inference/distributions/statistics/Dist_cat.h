/**
 * @file
 * This file contains the declaration of Dist_cat, which is used to hold
 * parameters and functions related to a distribution on a categorical
 * variable ( a factor).
 *
 * @brief Declaration of Dist_cat
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef Dist_cat_H
#define Dist_cat_H

#include "DistributionDiscrete.h"

#include <set>
#include <string>


template <typename setType>
class Dist_cat: public DistributionDiscrete {

    public:
    Dist_cat(void);                                                                                     //!< constructor
    
    // Basic utility functions
    Dist_cat*                       clone(void) const;                                                  //!< Clone object
    
    size_t                          getNumberOfStates(void) const;
protected:
    double                          lnPdfSingleValue(std::vector<size_t> &offset) const;                //!< Ln probability density
    double                          pdfSingleValue(std::vector<size_t> &offset) const;                  //!< Probability density
    void                            rvSingleValue(std::vector<size_t> &offset);                         //!< Generate random variable
    void                            setInternalParameters(const std::vector<RbValue<void*> > &p);       //!< Set the pointers to the variables of the distribution. The last one is always the random value.
    void                            setInternalObservedValue(const RbValue<void*> &v);                  //!< Set the pointers to the observation (random variable) of the distribution.
    
private:
    
    // parameters
    RbValue<std::vector<double>*>   probabilities;
    RbValue<std::set<setType>*>     states;
    RbValue<setType*>               randomVariable;

};

#endif


/**
 * @file
 * This file contains the implementation of Dist_cat, which is used to hold
 * parameters and functions related to a distribution on a categorical
 * variable (a factor).
 *
 * @brief Implementation of Dist_cat
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#include "RandomNumberGenerator.h"
#include "RbException.h"

#include <cmath>
#include <sstream>
#include <vector>


/** Default constructor */
template <typename setType>
Dist_cat<setType>::Dist_cat( void ) : DistributionDiscrete( ) {
}


/** Clone this object */
template <typename setType>
Dist_cat<setType>* Dist_cat<setType>::clone( void ) const {
    
    return new Dist_cat<setType>( *this );
}


/** Get the number of states in the distribution */
template <typename setType>
size_t Dist_cat<setType>::getNumberOfStates( void ) const {
    
    return probabilities.value[0].size();
}


/**
 * This function calculates the natural log of the
 * probability of a categorical random variable.
 *
 * @brief Natural log of categorical probability
 *
 * @param value Observed value
 * @return      Natural log of the probability density
 */
template <typename setType>
double Dist_cat<setType>::lnPdfSingleValue(std::vector<size_t> &offset) const {
    
	// Get the value and the parameters of the categorical distribution
    std::vector<double>& m = probabilities.value[offset[1]];
    const std::set<setType>& x = states.value[offset[0]];
    
    typename std::set<setType>::iterator it;
    setType& y = randomVariable.value[offset[0]];
    it = x.find( y );
    typename std::set<setType>::const_iterator curr_it;
    int dist;
    for ( dist = 0, curr_it = x.begin(); curr_it != x.end() && curr_it != it; ++dist, ++curr_it);
    
    if ( dist >= x.size() )
        return std::log( 0.0 );
    else
        return std::log( m[dist] );
}


/**
 * This function calculates the probability
 * of a categorical random variable.
 *
 * @brief Categorical probability density
 *
 * @param value Observed value
 * @return      Probability density
 */
template <typename setType>
double Dist_cat<setType>::pdfSingleValue(std::vector<size_t> &offset) const {
    
	// Get the value and the parameters of the categorical distribution
    std::vector<double>& m = probabilities.value[offset[1]];
    const std::set<setType>& x = states.value[offset[0]];
    
    typename std::set<setType>::iterator it;
    setType& y = randomVariable.value[offset[0]];
    it = x.find( y );
    typename std::set<setType>::const_iterator curr_it;
    int dist;
    for ( dist = 0, curr_it = x.begin(); curr_it != x.end() && curr_it != it; ++dist, ++curr_it);
    
    if ( dist >= x.size() )
        return 0.0;
    else
        return m[dist];
}


/**
 * This function generates a categorical
 * random variable.
 *
 * @brief Random draw from categorical distribution
 *
 * @return      Random draw from categorical distribution
 */
template <typename setType>
void Dist_cat<setType>::rvSingleValue(std::vector<size_t> &offset) {
    
	// Get the value and the parameters of the categorical distribution
    std::vector<double>& m = probabilities.value[offset[1]];
    const std::set<setType>& x = states.value[offset[0]];
    
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // Draw a random value
    double r   = rng->uniform01();
    double sum = 0.0;
    typename std::set<setType>::const_iterator i;
    i = x.begin();
    
    for ( std::vector<double>::const_iterator j = m.begin(); i != x.end(); ++i, ++j ) {
        sum += *j;
        if ( sum > r )
            break;
    }
    
    // get the random variable by accessing our state vector, selecting the i-th element and cloning it
    randomVariable.value[offset[2]] = *i;
    
}


/** We catch here the setting of the member variables to store our parameters. */
template <typename setType>
void Dist_cat<setType>::setInternalObservedValue(const RbValue<void *> &v) {
    
    randomVariable.value    = static_cast<setType *>( v.value );
    randomVariable.lengths  = v.lengths;
    
}


/** We catch here the setting of the member variables to store our parameters. */
template <typename setType>
void Dist_cat<setType>::setInternalParameters(const std::vector<RbValue<void *> > &p) {
    
    states.value            = static_cast<std::set<setType>*>( p[0].value );
    states.lengths          = p[0].lengths;
    
    probabilities.value     = static_cast<std::vector<double> *>( p[1].value );
    probabilities.lengths   = p[1].lengths;
    
    randomVariable.value    = static_cast<setType*>( p[2].value );
    randomVariable.lengths  = p[2].lengths;
    
}


