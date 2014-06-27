/**
 * @file
 * This file contains the declaration and implementation of RangeRule,
 * which is a templated class used for argument rules describing arguments
 * with min and max values.
 *
 * @brief Declaration of RangeRule
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RangeRule_H
#define RangeRule_H

#include "ArgumentRule.h"
#include "RbUtil.h"

#include <string>


template <typename valType>
class RangeRule : public ArgumentRule {

    public:
                                    RangeRule(const std::string& argName, valType min, valType max);                    //!< Constructor of rule without default value
                                    RangeRule(const std::string& argName, valType* defVal, valType min, valType max);   //!< Constructor of rule with default value

        // Basic utility functions
        RangeRule*                  clone(void) const { return new RangeRule(*this); }                                  //!< Clone object
        void                        printValue(std::ostream& o) const;                                                  //!< Print value for user

        // MinmaxRule functions
        bool                        isArgumentValid(const RevPtr<const Variable>& var, bool convert = false) const;     //!< Is var valid argument?

    protected:
        valType                     minVal;                                                                             //!< Min value
        valType                     maxVal;                                                                             //!< Max value
    
};

#endif


#include "ConstantNode.h"


/** Construct rule without default value; use "" for no label. */
template <typename valType>
RangeRule<valType>::RangeRule( const std::string& argName, valType min, valType max)
    : ArgumentRule( argName, true, min.getTypeSpec() ), minVal( min ), maxVal( max ) {

    if ( min > max )
        throw RbException( "Min larger than max in range rule" );
}


/** Construct rule with default value; use "" for no label. */
template <typename valType>
RangeRule<valType>::RangeRule( const std::string& argName, valType* defVal, valType min, valType max )
    : ArgumentRule( argName, true, defVal ), minVal( min ), maxVal( max ) {

    if ( min > max )
        throw RbException( "Min larger than max in range rule" );
    }
}



/** Check whether argument is constant and within the permissible range */
template <typename valType>
bool RangeRule<valType>::isArgumentValid( const RevPtr<const Variable>& var, bool convert ) const {

    if ( !var->isTypeSpec( ConstantNode::getClassTypeSpec() ) )
        return false;

    const valType* val = static_cast<const valType*> ( var->getValue() );

    if ( *val >= minVal && *val <= maxVal )
        return true;
    
    return false;
}


/** Print value for user */
template <typename valType>
void RangeRule<valType>::printValue(std::ostream& o) const {

    ArgumentRule::printValue(o);

    o << "range = [ " << minVal << ", " << maxVal << " ]" << std::endl;
}

