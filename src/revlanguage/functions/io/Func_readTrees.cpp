#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "Ellipsis.h"
#include "Func_readTrees.h"
#include "ModelVector.h"
#include "NclReader.h"
#include "NewickConverter.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RevNullObject.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "Tree.h"
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
Func_readTrees* Func_readTrees::clone( void ) const
{

    return new Func_readTrees( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_readTrees::execute( void )
{

    // get the information from the arguments for reading the file
    const std::string&  fn = static_cast<const RlString&>( args[0].getVariable()->getRevObject() ).getValue();
    const std::string&  text = static_cast<const RlString&>( args[1].getVariable()->getRevObject() ).getValue();

    ModelVector<TimeTree> *trees = new ModelVector<TimeTree>();

    if (fn != "") {
      // get the global instance of the NCL reader and clear warnings from its warnings buffer
      RevBayesCore::NclReader reader = RevBayesCore::NclReader();

      std::vector<RevBayesCore::Tree*> tmp = reader.readTimeTrees( fn );
      for (std::vector<RevBayesCore::Tree*>::iterator t = tmp.begin(); t != tmp.end(); ++t)
      {
          trees->push_back( TimeTree(*t) );
      }
    }

    if (text != "") {

      string aux;
      std::istringstream iss(text);
      RevBayesCore::NewickConverter c;

      while (std::getline(iss, aux)) {
        RevBayesCore::Tree *blTree = c.convertFromNewick( aux );
        trees->push_back( TimeTree(*blTree) );

      }
    }


    return new RevVariable( trees );

}


/** Get argument rules */
const ArgumentRules& Func_readTrees::getArgumentRules( void ) const
{

    static ArgumentRules argumentRules = ArgumentRules();
    static bool rules_set = false;

    if (!rules_set)
    {

        argumentRules.push_back( new ArgumentRule( "file", RlString::getClassTypeSpec(), "The name of the file containing the trees.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("") ) );
        argumentRules.push_back( new ArgumentRule( "text", RlString::getClassTypeSpec(), "A string containing one or several newick trees, separated by end of lines.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("") ) );
        rules_set = true;
    }

    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_readTrees::getClassType(void)
{

    static std::string rev_type = "Func_readTrees";

	return rev_type;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_readTrees::getClassTypeSpec(void)
{

    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );

	return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_readTrees::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "readTrees";

    return f_name;
}


/** Get type spec */
const TypeSpec& Func_readTrees::getTypeSpec( void ) const
{

    static TypeSpec type_spec = getClassTypeSpec();

    return type_spec;
}


/** Get return type */
const TypeSpec& Func_readTrees::getReturnType( void ) const
{

    static TypeSpec returnTypeSpec = ModelVector<TimeTree>::getClassTypeSpec();
    return returnTypeSpec;
}
