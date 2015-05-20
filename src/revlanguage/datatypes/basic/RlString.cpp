/**
 * @file
 * This file contains the implementation of RlString, which is
 * a RevBayes wrapper around a regular RlString.
 *
 * @brief Implementation of RlString
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */
 
#include "ConstantNode.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RlString.h"
#include "TypeSpec.h"

using namespace RevLanguage;

/** Constructor from empty RlString */
RlString::RlString( void ) : ModelObject<std::string>()
{

    setGuiVariableName("String");
    setGuiLatexSymbol("");
}


/** Constructor from RlString */
RlString::RlString(const std::string& v) : ModelObject<std::string>( new std::string(v) )
{

    setGuiVariableName("String");
    setGuiLatexSymbol("");
    
    parseValue();
}


///** Constructor from int */
//RlString::RlString(int i) : ModelObject<std::string>()
//{
//
//    setGuiVariableName("String");
//    setGuiLatexSymbol("");
//    std::ostringstream o;
//    o << i;
//    dagNode = new RevBayesCore::ConstantNode<std::string>("", new std::string(o.str()) );
//}
//
//
//
///** Constructor from RlString */
//RlString::RlString(double x) : ModelObject<std::string>()
//{
//
//    setGuiVariableName("String");
//    setGuiLatexSymbol("");
//    std::ostringstream o;
//    o << x;
//    dagNode = new RevBayesCore::ConstantNode<std::string>("", new std::string(o.str()) );
//}


/* Construct from DAG node */
RlString::RlString( RevBayesCore::TypedDagNode<std::string> *v ) : ModelObject<std::string>( v )
{

    setGuiVariableName("String");
    setGuiLatexSymbol("");
    
    parseValue();
}


/**
 * Generic addition operator.
 * We test if the rhs is of a type that we use for a specialized addition operation.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the sum.
 */
RevObject* RlString::add( const RevObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec() == RlString::getClassTypeSpec() )
    {
        return add( static_cast<const RlString&>( rhs ) );
    }
    
    return ModelObject<std::string>::add( rhs );
}


/**
 * Specialized addition operation between two string values.
 * The return value is also of type string.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the concatenation.
 */
RlString* RlString::add(const RevLanguage::RlString &rhs) const
{
    
    RlString *n = new RlString( dagNode->getValue() + rhs.getValue() );
    
    return n;
}


/** Clone function */
RlString* RevLanguage::RlString::clone() const
{

	return new RlString(*this);
}


/** Get Rev type of object */
const std::string& RlString::getClassType(void)
{
    
    static std::string revType = "String";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& RlString::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get type spec */
const TypeSpec& RlString::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Print value */
void RlString::parseValue(void)
{
    
    const std::string &v = dagNode->getValue();
    
    std::string res;
    std::string::const_iterator it = v.begin();
    while (it != v.end())
    {
        char c = *it++;
        if (c == '\\' && it != v.end())
        {
            switch (*it++) {
                case '\\': c = '\\'; break;
                case 'n': c = '\n'; break;
                case 't': c = '\t'; break;
                    // all other escapes
                default:
                    // invalid escape sequence - skip it. alternatively you can copy it as is, throw an exception...
                    continue;
            }
        }
        res += c;
    }
    
    this->setValue( new std::string(res) );
}



/** Print value */
void RlString::printValue(std::ostream& o, bool toScreen) const
{

    if ( toScreen == true )
    {
        o << "\"";
        dagNode->printValue( o );
        o << "\"";
    }
    else
    {
        dagNode->printValue( o );
    }
}

