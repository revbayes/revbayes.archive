#include "ConstantNode.h"
#include "Integer.h"
#include "Natural.h"
#include "RlBoolean.h"
#include "Real.h"
#include "RealPos.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
RlBoolean::RlBoolean(void) : ModelObject<RevBayesCore::Boolean>()
{

    setGuiVariableName("Boolean");
    setGuiLatexSymbol("B");
}

/** Construct from bool */
RlBoolean::RlBoolean(RevBayesCore::TypedDagNode<RevBayesCore::Boolean> *v) : ModelObject<RevBayesCore::Boolean>( v )
{
    
    setGuiVariableName("Boolean");
    setGuiLatexSymbol("B");
}

/** Construct from bool */
RlBoolean::RlBoolean(bool v) : ModelObject<RevBayesCore::Boolean>( new RevBayesCore::Boolean(v) )
{

    setGuiVariableName("Boolean");
    setGuiLatexSymbol("B");
}


/** Clone object */
RlBoolean* RlBoolean::clone(void) const {

	return new RlBoolean(*this);
}


/** Convert to type. The caller manages the returned object. */
RevObject* RlBoolean::convertTo(const TypeSpec& type) const
{

    if (type == Natural::getClassTypeSpec())
    {
        
        if ( dagNode->getValue() == true )
        {
            return new Natural(1);
        }
        else
        {
            return new Natural(0);
        }
        
    }
    else if (type == Integer::getClassTypeSpec())
    {
        
        if ( dagNode->getValue() == true )
        {
            return new Integer(1);
        }
        else
        {
            return new Integer(0);
        }
            
    }
    else if (type == Real::getClassTypeSpec())
    {
        
        if ( dagNode->getValue() == true )
        {
            return new Real(1.0);
        }
        else
        {
            return new Real(0.0);
        }
        
    }
    else if (type == RealPos::getClassTypeSpec())
    {
        
        if ( dagNode->getValue() == true )
        {
            return new RealPos(1.0);
        }
        else
        {
            return new RealPos(0.0);
        }
        
    }

    return RevObject::convertTo(type);
}


/** Get Rev type of object */
const std::string& RlBoolean::getClassType(void) { 
    
    static std::string revType = "Bool";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& RlBoolean::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get type spec */
const TypeSpec& RlBoolean::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Is convertible to type? */
double RlBoolean::isConvertibleTo(const TypeSpec& type, bool once) const
{

    if ( type == Natural::getClassTypeSpec() )
    {
        return 0.1;
    }
    else if ( type == Integer::getClassTypeSpec() )
    {
        return 0.3;
    }
    else if ( type == RealPos::getClassTypeSpec() )
    {
        return 0.2;
    }
    else if ( type == Real::getClassTypeSpec() )
    {
        return 0.4;
    }
    
    return RevObject::isConvertibleTo(type, once);
}


/** Print value for user */
void RlBoolean::printValue(std::ostream &o) const
{

    o << (dagNode->getValue() ? "true" : "false");
}

