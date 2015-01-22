/**
 * @file
 * This file contains the implementation of RbSettings, which 
 * contains the settings for many of the variables that are
 * potentially tunable by the user.
 *
 * @brief Declaration of RbSettings
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since version 1.0 2009-09-02
 *
 * $Id$
 */

#include "RbSettings.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "StringUtilities.h"

#include <iostream>
#include <string>
#include <sys/stat.h>

#ifdef WIN32
#	include <dirent.h>
#   include <unistd.h>
#else
#	include <dirent.h>
#   include <unistd.h>
#endif



/** Default constructor: The default settings are first read, and 
 * then potentially overwritten by values contained in a file.  */
RbSettings::RbSettings(void)
{

	initializeUserSettings();
}


const std::string& RbSettings::getHelpDir( void ) const
{
    
    return helpDir;
}


const std::string& RbSettings::getModuleDir( void ) const
{
    
    return moduleDir;
}


size_t RbSettings::getLineWidth( void ) const
{
    // return the internal value
    return lineWidth;
}


bool RbSettings::getPrintNodeIndex( void ) const
{
    // return the internal value
    return printNodeIndex;
}


double RbSettings::getTolerance( void ) const
{
    
    return tolerance;
}


const std::string& RbSettings::getWorkingDirectory( void ) const
{
    
    return workingDirectory;
}


/** Initialize the user settings */
#define	MAX_DIR_PATH	2048
void RbSettings::initializeUserSettings(void)
{
    
    std::string userDir = RevBayesCore::RbFileManager::expandUserDir("~");
    
    // read the ini file
    RevBayesCore::RbFileManager fm = RevBayesCore::RbFileManager(userDir,".RevBayes.ini");
    if ( fm.isFile() )
    {
        std::ifstream readStream;
        fm.openFile( readStream );
        std::string readLine = "";
        while (std::getline(readStream,readLine))
        {
            std::vector<std::string> tokens;
            StringUtilities::stringSplit(readLine, "=", tokens);
            
            setOption(tokens[0], tokens[1], false);
            
        }
        
        fm.closeFile(readStream);
    }
    else
    {

        helpDir   = "help";         // the default help directory
        moduleDir = "modules";      // the default module directory
        lineWidth = 100;            // the default line width
        tolerance = 10E-10;         // set default value for tolerance comparing doubles
        printNodeIndex = true;      // print node indices of tree nodes as comments

    }
    

    // initialize the current directory to be the directory the binary is sitting in
    char cwd[MAX_DIR_PATH+1];
	if ( getcwd(cwd, MAX_DIR_PATH+1) )
    {
#	ifdef WIN32
        std::string pathSeparator = "\\";
#	else
        std::string pathSeparator = "/";
#   endif
        
        std::string curdir = cwd;
        
        if ( curdir.at( curdir.length()-1 ) == pathSeparator[0] )
        {
            curdir.erase( curdir.length()-1 );
        }
        
        workingDirectory = curdir;
	}
    else
    {
        workingDirectory = "";
    }
    
    // save the current settings for the future.
    writeUserSettings();
}


void RbSettings::setHelpDir(const std::string &hd)
{
    
    RevBayesCore::RbFileManager fm = RevBayesCore::RbFileManager(hd);
    
    if ( !fm.isDirectory() )
    {
        throw RbException("Cannot set the help directory to '" + hd + "'.");
    }
    
    helpDir = fm.getFullFilePath();
    
    // save the current settings for the future.
    writeUserSettings();
}


void RbSettings::setModuleDir(const std::string &md)
{
    
    RevBayesCore::RbFileManager fm = RevBayesCore::RbFileManager(md);
    
    if ( !fm.isDirectory() )
    {
        throw RbException("Cannot set the help directory to '" + md + "'.");
    }
    
    moduleDir = fm.getFullFilePath();
    
    // save the current settings for the future.
    writeUserSettings();
}


void RbSettings::setLineWidth(size_t w)
{
    // replace the internal value with this new value
    lineWidth = w;
    
    // save the current settings for the future.
    writeUserSettings();
}


void RbSettings::setOption(const std::string &key, const std::string &value, bool write)
{
    
    if ( key == "helpdir" )
    {
        helpDir = value;
    }
    else if ( key == "moduledir" )
    {
        moduleDir = value;
    }
    else if ( key == "printNodeIndex" )
    {
        printNodeIndex = value == "TRUE";
    }
    else if ( key == "tolerance" )
    {
        std::string::size_type sz;     // alias of size_t
        tolerance = std::stod (value,&sz);
    }
    else if ( key == "linewidth" )
    {
        std::string::size_type sz;     // alias of size_t
        lineWidth = std::stoi (value,&sz);
    }
    else
    {
        std::cerr << "Unknown user setting with key '" << key << "'." << std::endl;
    }
    
    if ( write == true )
    {
        writeUserSettings();
    }
    
}


void RbSettings::setPrintNodeIndex(bool tf)
{
    // replace the internal value with this new value
    printNodeIndex = tf;
}


void RbSettings::setTolerance(double t)
{
    // replace the internal value with this new value
    tolerance = t;
    
    // save the current settings for the future.
    writeUserSettings();
}


void RbSettings::setWorkingDirectory(const std::string &wd)
{
    
    RevBayesCore::RbFileManager fm = RevBayesCore::RbFileManager(wd);
    
    if ( !fm.isDirectory() )
    {
        throw RbException("Cannot set the current directory to '" + wd + "'.");
    }
    
    workingDirectory = fm.getFullFilePath();
    
    // save the current settings for the future.
    writeUserSettings();
}


void RbSettings::writeUserSettings( void )
{
    std::string userDir = RevBayesCore::RbFileManager::expandUserDir("~");
    
    // open the ini file
    RevBayesCore::RbFileManager fm = RevBayesCore::RbFileManager(userDir,".RevBayes.ini");

    std::ofstream writeStream;
    fm.openFile( writeStream );
    writeStream << "helpdir=" << helpDir << std::endl;
    writeStream << "moduledir=" << moduleDir << std::endl;
    writeStream << "printNodeIndex=" << (printNodeIndex ? "TRUE" : "FALSE") << std::endl;
    writeStream << "tolerance=" << tolerance << std::endl;
    writeStream << "linewidth=" << lineWidth << std::endl;
    fm.closeFile( writeStream );

}
