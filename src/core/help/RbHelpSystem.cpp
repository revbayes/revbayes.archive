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
    
}


/** This is what turns up in terminal window */
std::string RbHelpSystem::getHelp(const std::string &qs, size_t columnWidth)
{
    
    HelpRenderer r = HelpRenderer();
    
    std::map<std::string, RbHelpFunction>::iterator it = helpForFunctions.find( qs );
    if ( it == helpForFunctions.end() )
    {
        throw RbException("Could not find help for '" + qs + "'.");
    }
    
    const RbHelpFunction& h = it->second;
    
    return r.renderFunctionHelp( h );
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
    std::vector<std::string> fileNames;
    fMngr.setStringWithNamesOfFilesInDirectory( fileNames );
    for (std::vector<std::string>::iterator it = fileNames.begin(); it != fileNames.end(); ++it)
    {
        RbHelpFunction h = parser.parseHelpFunction( *it );
        helpForFunctions.insert( std::pair<std::string,RbHelpFunction>( h.getTitle() , h) );
        
        
        // also add all aliases
    }
    
    
}


bool RbHelpSystem::isHelpAvailableForQuery(const std::string &query)
{

    return helpForFunctions.find( query ) != helpForFunctions.end();
}
