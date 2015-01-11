#include "RbException.h"
#include "RbFileManager.h"
#include "RbHelpSystem.h"
#include "RbHelpParser.h"
#include "RbHelpRenderer.h"
#include "RbSettings.h"
#include "RbUtil.h"
#include "StringUtilities.h"

#include <iostream>
#include <sstream>
#include "pugixml.hpp"
#include <algorithm>
#include <string>
#include <vector>
#include <iterator>
#include <list>

using namespace RevBayesCore;


RbHelpSystem::RbHelpSystem()
{
    
    initializeHelp( RbSettings::userSettings().getHelpDir() );
}


RbHelpSystem::RbHelpSystem( const RbHelpSystem &hs) :
    helpForFunctions( hs.helpForFunctions ),
    helpForMethods( hs.helpForMethods ),
    helpForTypes(  ),
    helpFunctionNames( hs.helpFunctionNames ),
    helpTypeNames( hs.helpTypeNames )
{
    
    for ( std::map<std::string, RbHelpType*>::const_iterator it = hs.helpForTypes.begin(); it != hs.helpForTypes.end(); ++it)
    {
        helpForTypes.insert( std::pair<std::string, RbHelpType*>( it->first, it->second->clone() ) );
    }
    
}


RbHelpSystem::~RbHelpSystem( void )
{
    
    // free the pointers to the help types
    for ( std::map<std::string, RbHelpType*>::const_iterator it = helpForTypes.begin(); it != helpForTypes.end(); ++it)
    {
        delete it->second;
    }
    helpForTypes.clear();
    
}




RbHelpSystem& RbHelpSystem::operator=( const RbHelpSystem &hs )
{
    
    if ( this != &hs )
    {
        
        // free the pointers to the help types
        for ( std::map<std::string, RbHelpType*>::const_iterator it = helpForTypes.begin(); it != helpForTypes.end(); ++it)
        {
            delete it->second;
        }
        helpForTypes.clear();
        
        helpFunctionNames   = hs.helpFunctionNames;
        helpForFunctions    = hs.helpForFunctions;
        helpTypeNames       = hs.helpTypeNames;
        helpForMethods      = hs.helpForMethods;
        
        for ( std::map<std::string, RbHelpType*>::const_iterator it = hs.helpForTypes.begin(); it != hs.helpForTypes.end(); ++it)
        {
            helpForTypes.insert( std::pair<std::string, RbHelpType*>( it->first, it->second->clone() ) );
        }
    }
    
    return *this;
}


const std::set<std::string>& RbHelpSystem::getFunctionEntries( void ) const
{
    // return a constant reference to the internal value
    return helpFunctionNames;
}


const std::set<std::string>& RbHelpSystem::getTypeEntries( void ) const
{
    // return a constant reference to the internal value
    return helpTypeNames;
}


/** Retrieve the help entry */
const RbHelpEntry& RbHelpSystem::getHelp(const std::string &qs)
{
    
    std::map<std::string, RbHelpFunction>::iterator itFunction = helpForFunctions.find( qs );
    std::map<std::string, RbHelpType*>::iterator itType = helpForTypes.find( qs );
    if ( itFunction != helpForFunctions.end() )
    {
        return itFunction->second;
    }
    else if ( itType != helpForTypes.end() )
    {
        return *(itType->second);
    }
    else
    {
        throw RbException("Could not find help for '" + qs + "'.");
    }
    
}


/** Retrieve the help entry */
const RbHelpEntry& RbHelpSystem::getHelp(const std::string &baseQuery, const std::string &qs)
{
    
    // find the corresponding base type
    std::map<std::string, std::map<std::string, RbHelpFunction> >::iterator itMethods = helpForMethods.find( baseQuery );
    if ( itMethods != helpForMethods.end() )
    {
        const std::map<std::string, RbHelpFunction> &methods = itMethods->second;
        
        // find the corresponding method
        std::map<std::string, RbHelpFunction>::const_iterator itFunction = methods.find( qs );
        
        // check if we found it
        if ( itFunction != methods.end() )
        {
            return itFunction->second;
        }
        
    }

    throw RbException("Could not find help for '" + baseQuery + "." + qs + "'.");
    
}



/** Initialize the help from an XML file */
void RbHelpSystem::initializeHelp(const std::string &helpDir)
{
    
    // find the path to the directory containing the help files
    RevBayesCore::RbFileManager fMngr = RevBayesCore::RbFileManager();
    
    fMngr.setFilePath(helpDir);
    if (fMngr.testDirectory() == false)
    {
        throw RbException("Warning: Cannot find directory containing help files. User help is unavailable. Path = " + helpDir);
    }
    
    // get a help parser instance
    RbHelpParser parser = RbHelpParser();
    
    // get the files contained in the directory

    // gather all xml files in help dir, filtered by '.ext'
    std::string ext = "xml";
    std::vector<std::string> files;
    std::vector<std::string> fileNames;
    fMngr.setStringWithNamesOfFilesInDirectory( files );
    for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
    {
        RevBayesCore::RbFileManager tmpFM = RevBayesCore::RbFileManager( *it );
        if ( tmpFM.getFileExtension() == ext)
        {
            fileNames.push_back( *it );
        }
    }

    for (std::vector<std::string>::iterator it = fileNames.begin(); it != fileNames.end(); ++it)
    {
        
        if ( parser.testHelpEntry( *it ) == RbHelpParser::FUNCTION )
        {
            
            RbHelpFunction h = parser.parseHelpFunction( *it );
            helpForFunctions.insert( std::pair<std::string,RbHelpFunction>( h.getName() , h) );
            helpFunctionNames.insert( h.getName() );
            
            
            // also add all aliases
            const std::vector<std::string>& aliases = h.getAliases();
            for (std::vector<std::string>::const_iterator alias = aliases.begin(); alias != aliases.end(); ++alias)
            {
                helpForFunctions.insert( std::pair<std::string,RbHelpFunction>( *alias , h) );
            }
            
        }
        
        
        
        if ( parser.testHelpEntry( *it ) == RbHelpParser::TYPE || parser.testHelpEntry( *it ) == RbHelpParser::DISTRIBUTION || parser.testHelpEntry( *it ) == RbHelpParser::MONITOR || parser.testHelpEntry( *it ) == RbHelpParser::MOVE )
        {
            
            RbHelpType* h = NULL;
            if ( parser.testHelpEntry( *it ) == RbHelpParser::TYPE )
            {
                h = parser.parseHelpType( *it );
            }
            else if ( parser.testHelpEntry( *it ) == RbHelpParser::DISTRIBUTION )
            {
                h = parser.parseHelpDistribution( *it );
            }
            else if ( parser.testHelpEntry( *it ) == RbHelpParser::MONITOR )
            {
                h = parser.parseHelpMonitor( *it );
            }
            else if ( parser.testHelpEntry( *it ) == RbHelpParser::MOVE )
            {
                h = parser.parseHelpMove( *it );
            }

            helpForTypes.insert( std::pair<std::string,RbHelpType*>( h->getName() , h) );
            helpTypeNames.insert( h->getName() );
            
            
            // create a map for all methods for this type
            std::map<std::string, RbHelpFunction> methodsHelp;
            const std::vector<RbHelpFunction>& method = h->getMethods();
            for (std::vector<RbHelpFunction>::const_iterator m = method.begin(); m != method.end(); ++m)
            {
                methodsHelp.insert( std::pair<std::string,RbHelpFunction>( m->getName() , *m) );
            }
            
            // add the methods to our global map
            helpForMethods.insert( std::pair<std::string, std::map<std::string,RbHelpFunction> >(h->getName(),methodsHelp) );
            
            
            // also add all aliases
            const std::vector<std::string>& aliases = h->getAliases();
            for (std::vector<std::string>::const_iterator alias = aliases.begin(); alias != aliases.end(); ++alias)
            {
                helpForTypes.insert( std::pair<std::string,RbHelpType*>( *alias , h->clone() ) );
                helpForMethods.insert( std::pair<std::string, std::map<std::string,RbHelpFunction> >(*alias,methodsHelp) );
            }
        }
        
        
    }
    
    
}


bool RbHelpSystem::isHelpAvailableForQuery(const std::string &query)
{
    // test if we have a help entry for this query string
    return helpForFunctions.find( query ) != helpForFunctions.end() || helpForTypes.find( query ) != helpForTypes.end();
}


bool RbHelpSystem::isHelpAvailableForQuery(const std::string &baseQuery, const std::string &query)
{
    // test if we have a help entry for this query string
    return helpForTypes.find( baseQuery ) != helpForTypes.end();
}
