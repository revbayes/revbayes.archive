#include "ConstructorFunction.h"
#include "FunctionTable.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbHelpSystem.h"
#include "RevAbstractType.h"
#include "RevObject.h"
#include "RbUtil.h"
#include "RlDistribution.h"
#include "StringUtilities.h"
#include "Function.h"
#include "Workspace.h"

#include <cassert>
#include <sstream>
#include <vector>

using namespace RevLanguage;

/* Constructor of global workspace */
Workspace::Workspace(const std::string &n) : Environment( n ),
    typesInitialized(false)
{

}


/* Constructor of user workspace */
Workspace::Workspace(Environment* parentSpace, const std::string &n) : Environment(parentSpace, n),
    typesInitialized(false)
{
    
}

/* Constructor of user workspace */
Workspace::Workspace(const Workspace& x) : Environment(x),
    typesInitialized(x.typesInitialized)
{
    // copy all the types
    for (TypeTable::const_iterator it=x.typeTable.begin(); it!=x.typeTable.end(); ++it)
    {
        typeTable.insert(std::pair<std::string, RevObject*>(it->first, it->second->clone()));
    }

}



/* Assignment operator */
Workspace& Workspace::operator=(const Workspace& x)
{

    if (this != &x) 
    {
        // first we need to delegate to the base class assignment operator
        Environment::operator=(x);
        
        // free all the types
        for (TypeTable::iterator it=typeTable.begin(); it!=typeTable.end(); ++it)
        {
            RevObject *the_object = it->second;
            delete the_object;
        }
        
        // copy all the types
        for (TypeTable::const_iterator it=x.typeTable.begin(); it!=x.typeTable.end(); ++it)
        {
            typeTable.insert(std::pair<std::string, RevObject*>(it->first, it->second->clone()));
        }
    }

    return (*this);
}

/* Constructor of user workspace */
Workspace::~Workspace(void)
{
    
    // free all the types
    for (TypeTable::iterator it=typeTable.begin(); it!=typeTable.end(); ++it)
    {
        RevObject *the_object = it->second;
        delete the_object;
    }
    
}


/* Add distribution to the workspace */
bool Workspace::addDistribution(const std::string& name, Distribution *dist)
{

    if ( typeTable.find(name) != typeTable.end() )
    {
        throw RbException("There is already a type named '" + dist->getType() + "' in the workspace");
    }
    
    functionTable.addFunction(name, new ConstructorFunction( dist ) );
    
    // add the help entry for this distribution to the global help system instance
    RevBayesCore::RbHelpDistribution* entry = dist->getHelpEntry();
    entry->setName( name );
    RevBayesCore::RbHelpSystem::getHelpSystem().addHelpDistribution( entry );

    return true;
}


/** Add type to the workspace */
bool Workspace::addType(RevObject *exampleObj)
{

    std::string name = exampleObj->getType();

    if (typeTable.find(name) != typeTable.end())
    {
        // free memory
        delete exampleObj;
        
        throw RbException("There is already a type named '" + name + "' in the workspace");
    }
    
    typeTable.insert(std::pair<std::string, RevObject*>(name, exampleObj));

    return true;
}


/** Add type with constructor to the workspace */
bool Workspace::addTypeWithConstructor(const std::string& name, RevObject *templ)
{
    

    if (typeTable.find(name) != typeTable.end())
    {
        // free memory
        delete templ;
        
        throw RbException("There is already a type named '" + name + "' in the workspace");
    }
    
    typeTable.insert(std::pair<std::string, RevObject*>(templ->getType(), templ->clone()));
    
    functionTable.addFunction(name, new ConstructorFunction(templ));
    
    // add the help entry for this type to the global help system instance
    RevBayesCore::RbHelpSystem::getHelpSystem().addHelpType( static_cast<RevBayesCore::RbHelpType*>(templ->getHelpEntry()) );
    
    return true;
}


/** clone */
Workspace* Workspace::clone() const
{
    return new Workspace(*this);
}


const TypeSpec& Workspace::getClassTypeSpecOfType(std::string const &type) const
{
    
    std::map<std::string, RevObject*>::const_iterator it = typeTable.find( type );
    if ( it == typeTable.end() ) 
    {
        if ( parentEnvironment != NULL )
        {
            return static_cast<Workspace*>( parentEnvironment )->getClassTypeSpecOfType( type );
        }
        else
        {
            throw RbException( "Type '" + type + "' does not exist in environment" );;
        }
        
    }
    else
    {
        return it->second->getTypeSpec();
    }
    
}


/* Is the type added to the workspace? */
bool Workspace::existsType( const std::string& name ) const
{

    std::map<std::string, RevObject *>::const_iterator it = typeTable.find( name );
    if ( it == typeTable.end() ) 
    {
        if ( parentEnvironment != NULL )
        {
            return static_cast<Workspace*>( parentEnvironment )->existsType( name );
        }
        else
        {
            return false;
        }
        
    }
    else
    {
        return true;
    }
    
}



const TypeTable& Workspace::getTypeTable( void ) const
{
    
    return typeTable;
}


void Workspace::initializeGlobalWorkspace( void )
{
    
    initializeTypeGlobalWorkspace();
    initializeMonitorGlobalWorkspace();
    initializeMoveGlobalWorkspace();
    initializeDistGlobalWorkspace();
    initializeFuncGlobalWorkspace();
    initializeBasicGlobalWorkspace();

}



/**
 * Use the template object in the type table to make an example instance of
 * a specified Rev type. If the type is abstract, we provide an example
 * object of a non-abstract derived type by using the RevAbstractType
 * functionality.
 */
RevObject* Workspace::makeNewDefaultObject(const std::string& type) const {
    
    std::map<std::string, RevObject*>::const_iterator it = typeTable.find( type );
    
    if ( it == typeTable.end() )
    {
        if ( parentEnvironment != NULL )
            return static_cast<Workspace*>( parentEnvironment )->makeNewDefaultObject( type );
        else
            throw RbException( "Type '" + type + "' does not exist in environment" );;
    }
    else
    {
        if ( it->second->isAbstract() )
        {
            RevAbstractType* theAbstractType = static_cast< RevAbstractType* >( it->second );
            return theAbstractType->makeExampleObject();
        }
        
        return it->second->clone();
    }
}



/** Print the frame content, not the entire environment. */
void Workspace::printValue(std::ostream& o) const
{

    if ( variableTable.size() > 0 )
    {
        o << "Variable table:" << std::endl;
        o << "===============" << std::endl << std::endl;

        VariableTable::const_iterator it;
        for ( it = variableTable.begin(); it != variableTable.end(); it++)
        {
            std::ostringstream s;
            s << (*it).first << " = ";
            std::ostringstream t;
            (*it).second->printValue( t, true );
            o << StringUtilities::oneLiner( t.str(), 75 - s.str().length() ) << std::endl;
        }
        o << std::endl;
    }

    std::stringstream s;
    functionTable.printValue(s, false);

    if (s.str().size() > 0 )
    {
        o << "Function table:" << std::endl;
        o << "===============" << std::endl << std::endl;
        o << s.str() << std::endl;
    }

    if ( typeTable.size() > 0 )
    {
        o << "Type table:" << std::endl;
        o << "===========" << std::endl << std::endl;
        std::map<std::string, RevObject *>::const_iterator i;
        for (i=typeTable.begin(); i!=typeTable.end(); i++)
        {
            if ( (*i).second != NULL )
                o << (*i).first << " = " << (*i).second->getTypeSpec() << std::endl;
            else
                o << (*i).first << " = " << "unknown class vector" << std::endl;
        }
    }
}


