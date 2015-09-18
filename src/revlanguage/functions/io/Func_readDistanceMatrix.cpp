#include "AbstractHomologousDiscreteCharacterData.h"
#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "DistanceMatrix.h"
#include "DistanceMatrixReader.h"
#include "Ellipsis.h"
#include "Func_readDistanceMatrix.h"
#include "ModelVector.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RevNullObject.h"
#include "RlBoolean.h"
#include "RlDistanceMatrix.h"
#include "RlHomologousDiscreteCharacterData.h"
#include "RlString.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "RlUserInterface.h"

#include <map>
#include <set>
#include <sstream>


using namespace RevLanguage;

/** Clone object */
Func_readDistanceMatrix* Func_readDistanceMatrix::clone( void ) const
{
	
	return new Func_readDistanceMatrix( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_readDistanceMatrix::execute( void )
{
	
	// get the information from the arguments for reading the file
	const RlString& fn = static_cast<const RlString&>( args[0].getVariable()->getRevObject() );
	
	RevBayesCore::DistanceMatrixReader* dmr = new RevBayesCore::DistanceMatrixReader( fn.getValue(), ' ' );
	RevBayesCore::DistanceMatrix* dm = new RevBayesCore::DistanceMatrix(dmr);
		
	return new RevVariable( new RlDistanceMatrix(dm) );
}


/** Get argument rules */
const ArgumentRules& Func_readDistanceMatrix::getArgumentRules( void ) const
{
	
	static ArgumentRules argumentRules = ArgumentRules();
	static bool rulesSet = false;
	
	if (!rulesSet)
	{
		
		argumentRules.push_back( new ArgumentRule( "file", RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
		rulesSet = true;
		
	}
	
	return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_readDistanceMatrix::getClassType(void)
{
	
	static std::string revType = "Func_readDistanceMatrix";
	
	return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_readDistanceMatrix::getClassTypeSpec(void)
{
	
	static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
	
	return revTypeSpec;
}

/** Get type spec */
const TypeSpec& Func_readDistanceMatrix::getTypeSpec( void ) const
{
	
	static TypeSpec typeSpec = getClassTypeSpec();
	
	return typeSpec;
}


/** Get return type */
const TypeSpec& Func_readDistanceMatrix::getReturnType( void ) const
{
	
	static TypeSpec returnTypeSpec = RlDistanceMatrix::getClassTypeSpec();
	return returnTypeSpec;
}

