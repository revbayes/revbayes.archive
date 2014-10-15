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
RbSettings::RbSettings(void) {

	initializeUserSettings();
	
    // read a file containing the user's alternate default values
}


/** Constructor that takes a file containing the user settings. The
 * default settings are first read, and then potentially overwritten by
 * values contained in a file. */
RbSettings::RbSettings(std::string& defaultFileName) {

	initializeUserSettings();
	
    // read the 'defaultFileName' file containing the user's alternate default values
}



bool RbSettings::getPrintNodeIndex( void ) const
{
    
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

    tolerance = 10E-10;         // set default value for tolerance comparing doubles
    printNodeIndex = false;     // print node indices of tree nodes as comments

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
}


void RbSettings::setPrintNodeIndex(bool tf)
{
    
    printNodeIndex = tf;
    
}


void RbSettings::setTolerance(double t)
{
    
    tolerance = t;
    
}


void RbSettings::setWorkingDirectory(const std::string &wd)
{
    
    RevBayesCore::RbFileManager fm = RevBayesCore::RbFileManager(wd);
    
    if ( !fm.isDirectory() )
    {
        throw RbException("Cannot set the current directory to '" + wd + "'.");
    }
    
    workingDirectory = fm.getFullFilePath();
    
}
