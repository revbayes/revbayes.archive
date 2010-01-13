/**
 * @file
 * This file contains the declaration of RbFunction_gtrQ, the
 * gtrQ() function that constructs a scaled instantaneous
 * rate matrix from two simplex parameters, one describing the
 * stationary state frequencies, the other the exchangeability
 * rates.
 *
 * Function: gtrQ
 *
 * Parameters:
 * pi      = stationary state frequencies
 * revrate = exchangeability rate parameters
 *
 * @brief Declaration of RbFunction_gtrQ
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#ifndef RbFunction_gtrQ_H
#define RbFunction_gtrQ_H

#include "ArgumentRule.h"
#include "RbDataType.h"
#include "RbRateMatrix.h"
#include "RbStandardFxn.h"
#include <iostream>
#include <string>

/** This is the class for the gtrQ() function, which constructs a scaled
 *  instantaneous rate matrix from two simplex parameters, one describing
 *  the stationary state frequencies of the process and the other the
 *  exchangeability rates.
 *
 *  @package    functions
 *  @implements RbFunction, RbStandardFxn
 */
class RbFunction_gtrQ :  public RbStandardFxn {

    public:
            RbFunction_gtrQ();                              //!< Default constructor, allocate workspace
            RbFunction_gtrQ(const RbFunction_gtrQ& s);      //!< Copy constructor
	        ~RbFunction_gtrQ();                             //!< Destructor, delete workspace

#pragma mark Parser help functions
        static const ArgumentRule   argRules[];             //!< The argument rules
        RbFunction_gtrQ*            copy() const;           //!< Return copy
        const ArgumentRule*         getArgRules() const;    //!< Get argument rules
        const std::string&          getDataType() const;    //!< Get data type of result

#pragma mark Regular functions
        const RbDataVector&         execute();              //!< Get result
        void                        print(std::ostream& c=std::cout) const;     //!< Print this object

    protected:
        RbDataVector                qmatVec;                //!< Workspace for the result vector

    private:
        int                         upper(int i, int j) const; //!< Return index in upper trianguar mat
};

#endif
