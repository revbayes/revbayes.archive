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
#include "RbNames.h"

#include <string>

class DAGNode;
class RbObject;
class VectorString;

template <typename valType>
class RangeRule : public ArgumentRule {

    public:
                                    RangeRule(const std::string& argName, valType min, valType max);                    //!< Constructor of rule without default value
                                    RangeRule(const std::string& argName, valType* defVal, valType min, valType max);   //!< Constructor of rule with default value

        // Basic utility functions
        RangeRule*                  clone(void) const { return new RangeRule(*this); }                                  //!< Clone object
        const VectorString&         getClass(void) const;                                                               //!< Get class vector
        void                        printValue(std::ostream& o) const;                                                  //!< Print value for user
        std::string                 richInfo(void) const;                                                               //!< General info on object

        // MinmaxRule functions
        bool                        isArgumentValid(DAGNode* var, bool& needsConversion, bool once) const;              //!< Is var valid argument?

    protected:
        valType                     minVal;                                                                             //!< Min value
        valType                     maxVal;                                                                             //!< Max value
};

#endif


#include "VectorString.h"


/** Construct rule without default value; use "" for no label. */
template <typename valType>
RangeRule<valType>::RangeRule( const std::string& argName, valType min, valType max)
    : ArgumentRule( argName, min.getTypeSpec() ), minVal( min ), maxVal( max ) {

    if ( min > max )
        throw RbException( "Min larger than max in range rule" );
}


/** Construct rule with default value; use "" for no label. */
template <typename valType>
RangeRule<valType>::RangeRule( const std::string& argName, valType* defVal, valType min, valType max )
    : ArgumentRule( argName, defVal ), minVal( min ), maxVal( max ) {

    if ( min > max )
        throw RbException( "Min larger than max in range rule" );
}


/** Get class vector describing type of object */
template <typename valType>
const VectorString& RangeRule<valType>::getClass( void ) const {

    static VectorString rbClass = VectorString( RangeRule_name ) + RbString( "<" ) + minVal.getType() + RbString( ">" ) + ArgumentRule::getClass();
	return rbClass; 
}


/** Check whether argument is constant and within the permissible range */
template <typename valType>
bool RangeRule<valType>::isArgumentValid( DAGNode* var, bool& needsConversion, bool once ) const {

    needsConversion = false;
    if ( !var->isType( ConstantNode_name ) )
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


/** Provide complete information about object */
template <typename valType>
std::string RangeRule<valType>::richInfo(void) const {

    std::ostringstream o;

    o << "RangeRule:" << std::endl;
    o << "label         = " << label << std::endl;
    o << "argSlot       = " << argSlot << std::endl;
    o << "hasDefaultVal = " << hasDefaultVal << std::endl;
    o << "min           = " << minVal << std::endl;
    o << "max           = " << maxVal << std::endl;

    return o.str();
}

