#include "RbException.h"
#include "RbFileManager.h"
#include "RbHelpSystem.h"
#include "RbHelpParser.h"
#include "RbHelpRenderer.h"
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
    
    initializeHelp("help");
}


RbHelpSystem::RbHelpSystem( const RbHelpSystem &hs) :
    helpFunctionNames( hs.helpFunctionNames ),
    helpForFunctions( hs.helpForFunctions )
{
    
}




RbHelpSystem& RbHelpSystem::operator=( const RbHelpSystem &hs )
{
    
    if ( this != &hs )
    {
        helpFunctionNames = hs.helpFunctionNames;
        helpForFunctions  = hs.helpForFunctions;
    }
    
    return *this;
}


const std::set<std::string>& RbHelpSystem::getFunctionEntries( void ) const
{
    // return a constant reference to the internal value
    return helpFunctionNames;
}


/** Retrieve the help entry */
const RbHelpFunction& RbHelpSystem::getHelp(const std::string &qs)
{
    
    HelpRenderer r = HelpRenderer();
    
    std::map<std::string, RbHelpFunction>::iterator it = helpForFunctions.find( qs );
    if ( it == helpForFunctions.end() )
    {
        throw RbException("Could not find help for '" + qs + "'.");
    }
    
    const RbHelpFunction& h = it->second;
    
    return h;
}



/** Initialize the help from an XML file */
void RbHelpSystem::initializeHelp(const std::string &helpDir)
{
    
    // find the path to the directory containing the help files
    RevBayesCore::RbFileManager fMngr = RevBayesCore::RbFileManager();
    //pathToHelpDir = fMngr.getCurrentDirectory();
    
    
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
        if ( tmpFM.getFileExtension() == ext) {
            fileNames.push_back( *it );
        }
    }

    for (std::vector<std::string>::iterator it = fileNames.begin(); it != fileNames.end(); ++it)
    {
    
        
//        std::cerr << *it << std::endl;
        if ( *it == "help/mcmc.xml" )
        {
            std::cerr << *it << std::endl;
        }
        
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
        
        
        
        if ( parser.testHelpEntry( *it ) == RbHelpParser::TYPE )
        {
            
            RbHelpType h = parser.parseHelpType( *it );
//            helpForFunctions.insert( std::pair<std::string,RbHelpFunction>( h.getName() , h) );
//            helpFunctionNames.insert( h.getName() );
//            
//            
//            // also add all aliases
//            const std::vector<std::string>& aliases = h.getAliases();
//            for (std::vector<std::string>::const_iterator alias = aliases.begin(); alias != aliases.end(); ++alias)
//            {
//                helpForFunctions.insert( std::pair<std::string,RbHelpFunction>( *alias , h) );
//            }
            
        }
        
        
    }
    
    
}


bool RbHelpSystem::isHelpAvailableForQuery(const std::string &query)
{
    // test if we have a help entry for this query string
    return helpForFunctions.find( query ) != helpForFunctions.end();
}
