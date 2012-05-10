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

#include "ConstArgumentRule.h"
#include "RbUtil.h"

#include <string>


template <typename valType>
class RangeRule : public ConstArgumentRule {

    public:
                                    RangeRule(const std::string& argName, valType min, valType max);                    //!< Constructor of rule without default value
                                    RangeRule(const std::string& argName, valType* defVal, valType min, valType max);   //!< Constructor of rule with default value

        // Basic utility functions
        RangeRule*                  clone(void) const { return new RangeRule(*this); }                                  //!< Clone object
        static const std::string&   getClassName(void);                                                                 //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                                             //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                                            //!< Get language type of the object
        void                        printValue(std::ostream& o) const;                                                  //!< Print value for user
        std::string                 debugInfo(void) const;                                                              //!< General info on object

        // MinmaxRule functions
        bool                        isArgumentValid(const RbPtr<const Variable>& var, bool convert = false) const;              //!< Is var valid argument?

    protected:
        valType                     minVal;                                                                             //!< Min value
        valType                     maxVal;                                                                             //!< Max value
    
};

#endif


#include "ConstantNode.h"


/** Construct rule without default value; use "" for no label. */
template <typename valType>
RangeRule<valType>::RangeRule( const std::string& argName, valType min, valType max)
    : ConstArgumentRule( argName, min.getTypeSpec() ), minVal( min ), maxVal( max ) {

    if ( min > max )
        throw RbException( "Min larger than max in range rule" );
}


/** Construct rule with default value; use "" for no label. */
template <typename valType>
RangeRule<valType>::RangeRule( const std::string& argName, valType* defVal, valType min, valType max )
    : ConstArgumentRule( argName, defVal ), minVal( min ), maxVal( max ) {

    if ( min > max )
        throw RbException( "Min larger than max in range rule" );
    }


/** Get class name of object */
template<typename valType>
const std::string& RangeRule<valType>::getClassName(void) { 
    
    static std::string rbClassName = "Range rule<" + valType::getClassName() + ">";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
template<typename valType>
const TypeSpec& RangeRule<valType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
template<typename valType>
const TypeSpec& RangeRule<valType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Check whether argument is constant and within the permissible range */
template <typename valType>
bool RangeRule<valType>::isArgumentValid( const RbPtr<const Variable>& var, bool convert ) const {

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


/** Provide complete information about object */
template <typename valType>
std::string RangeRule<valType>::debugInfo(void) const {

    std::ostringstream o;

    o << "RangeRule:" << std::endl;
    o << "label         = " << label << std::endl;
    o << "hasDefaultVal = " << hasDefaultVal << std::endl;
    o << "defaultVaribale   = ";
    if ( defaultVar.getDagNode() != NULL ) {
        defaultVar.getValue().printValue(o);
    } 
    else {
        o << "NULL";
    }
    o << std::endl;
    o << "min           = " << minVal << std::endl;
    o << "max           = " << maxVal << std::endl;

    return o.str();
}

