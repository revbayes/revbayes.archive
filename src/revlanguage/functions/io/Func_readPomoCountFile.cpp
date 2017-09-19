#include "HomologousDiscreteCharacterData.h"
#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "PomoCountFileReader.h"
#include "Ellipsis.h"
#include "Func_readPomoCountFile.h"
#include "ModelVector.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RevNullObject.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "RlUserInterface.h"

#include <map>
#include <set>
#include <sstream>


using namespace RevLanguage;

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_readPomoCountFile* Func_readPomoCountFile::clone( void ) const
{

	return new Func_readPomoCountFile( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_readPomoCountFile::execute( void )
{

	// get the information from the arguments for reading the file
	const RlString& fn = static_cast<const RlString&>( args[0].getVariable()->getRevObject() );
	RevBayesCore::TypedDagNode<long>* virtualPopulationSize = static_cast<const Integer &>( this->args[1].getVariable()->getRevObject() ).getDagNode();

	RevBayesCore::PomoCountFileReader* pcfr = new RevBayesCore::PomoCountFileReader( fn.getValue(), virtualPopulationSize->getValue(), ' ' );

	RevBayesCore::HomologousDiscreteCharacterData<RevBayesCore::PomoState> *pomoAln = new RevBayesCore::HomologousDiscreteCharacterData<RevBayesCore::PomoState>( *(pcfr->getMatrix() ) );

	AbstractHomologousDiscreteCharacterData *rlPomoAln = new AbstractHomologousDiscreteCharacterData( pomoAln );

	return new RevVariable( rlPomoAln );
}


/** Get argument rules */
const ArgumentRules& Func_readPomoCountFile::getArgumentRules( void ) const
{

	static ArgumentRules argumentRules = ArgumentRules();
	static bool rules_set = false;

	if (!rules_set)
	{
		argumentRules.push_back( new ArgumentRule( "file", RlString::getClassTypeSpec(), "Relative or absolute name of the file.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
		argumentRules.push_back( new ArgumentRule( "virtualPopulationSize", Natural::getClassTypeSpec()                                , "", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );

		rules_set = true;

	}

	return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_readPomoCountFile::getClassType(void)
{

	static std::string rev_type = "Func_readPomoCountFile";

	return rev_type;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_readPomoCountFile::getClassTypeSpec(void)
{

	static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );

	return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_readPomoCountFile::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "readPomoCountFile";

    return f_name;
}


/** Get type spec */
const TypeSpec& Func_readPomoCountFile::getTypeSpec( void ) const
{

	static TypeSpec type_spec = getClassTypeSpec();

	return type_spec;
}


/** Get return type */
const TypeSpec& Func_readPomoCountFile::getReturnType( void ) const
{

	static TypeSpec returnTypeSpec = AbstractHomologousDiscreteCharacterData::getClassTypeSpec();
	return returnTypeSpec;
}
