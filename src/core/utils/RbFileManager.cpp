/**
 * @file
 * This file contains the implementation of RbFileManager, which 
 * handles the opening, closing, testing presence of, etc. files.
 *
 * @brief Declaration of RbFileManager
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

#include "RbException.h"
#include "RbFileManager.h"
#include "RlUserInterface.h"
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


using namespace RevBayesCore;

/** Default constructor, creating a file manager object with the file 
    path equal to the current (default) directory and an empty file name */
RbFileManager::RbFileManager(void) {

    // make certain the current file/path information is empty
	setFileName("");
	setFilePath("");
    
    // initialize the current directory to be the directory the binary is sitting in
    setCurDirectory( findCurrentDirectory() );
}


/** Constructor taking as an argument a string containing a file path and (maybe) a file name */
RbFileManager::RbFileManager(std::string s) {

    // make certain the current file/path information is empty
    setCurDirectory("");
	setFileName("");
	setFilePath("");

    // initialize the current directory to be the directory the binary is sitting in
    setCurDirectory( findCurrentDirectory() );
    
    // set the path and file for the string
    parsePathFileNames(s);
        
    if (isDirectoryPresent(s) == false)
        {
        }
}



/** Format the error exception string for problems specifying the file/path name */
void RbFileManager::formatError(std::string& errorStr) {
    
    bool fileNameProvided    = isFileNamePresent();
    bool isFileNameGood      = testFile();
    bool isDirectoryNameGood = testDirectory();
    
    if ( fileNameProvided == false && isDirectoryNameGood == false )
        errorStr += "Could not read contents of directory \"" + getFilePath() + "\" because the directory does not exist";
    else if (fileNameProvided == true && (isFileNameGood == false || isDirectoryNameGood == false))
    {
        errorStr += "Could not read file named \"" + getFileName() + "\" in directory named \"" + getFilePath() + "\" ";
        if (isFileNameGood == false && isDirectoryNameGood == true)
            errorStr += "because the file does not exist";
        else if (isFileNameGood == true && isDirectoryNameGood == false)
            errorStr += "because the directory does not exist";
        else
            errorStr += "because neither the directory nor the file exist";
    }
}


/** Divides a string into the path and file name components */
bool RbFileManager::parsePathFileNames(std::string s) {

    // check if the path is a good one
    bool isDPresent = isDirectoryPresent(s);
    bool isFPresent = isFilePresent(s);
    if (isDPresent == false && isFPresent == false)
        {
        fileName = "";
        filePath = "";
        return false;
        }
    if (isDPresent == true && isFPresent == false)
        {
        fileName = "";
        int location = (int)s.find_last_of( "/" );
        if ( location == (int)s.length() - 1 )
            s.erase( location );
        filePath = s;
        return true;
        }
    
    /* the string that is supposed to hold the
       path/file information is empty. */
	if ( s.length() == 0 )
		{
		return false;
		}
		
	/* Find the location of the last "/". This is where
	   we will divide the path/file string into two. */
	int location = (int)s.find_last_of( "/" );
	
	if ( location == -1 )
		{
		/* There is no path in this string. We 
		   must have only the file name, and the
		   file should be in our current directory. */
		fileName = s;
		filePath = "";
		}
	else if ( location == (int)s.length() - 1 )
		{
		/* It looks like the last character is "/", which
		   means that no file name has been provided. However,
           it also means that the directory that has been provided
           is not valid, otherwise it would have tested as 
           being present (above). */
		fileName = "";
		filePath = "";
		return false;
		}
	else
		{
		/* We can divide the path into the path and the file. */
		fileName = s.substr( location+1, s.length()-location-1 );
		s.erase( location );
		filePath = s;
        if ( isDirectoryPresent(filePath) == false )
            filePath = "";
		}

	return true;
}


/** Finds the current (default) directory of the process */
#define	MAX_DIR_PATH	2048
std::string RbFileManager::findCurrentDirectory(void) {

	char cwd[MAX_DIR_PATH+1];
	if ( !getcwd(cwd, MAX_DIR_PATH+1) )
		return "";
	std::string curdir = cwd;
	
	if ( curdir.at( curdir.length()-1 ) == '/' )
		curdir.erase( curdir.length()-1 );
	
	return curdir;
    
}


/** Tests whether the directory (passed in as an argument) is present */
bool RbFileManager::isDirectoryPresent(const std::string &mp) {

//#	ifdef WIN32
//
//    DWORD data = GetFileAttributes( mp.c_str() );
//    if ( data == INVALID_FILE_ATTRIBUTES )
//        return false;
//    bool bIsDirectory = ( data & FILE_ATTRIBUTE_DIRECTORY ) != 0;   
//    return bIsDirectory;
//
//#	else

    if ( mp == "" ) {
        return true;
    }
    
    DIR* d = opendir( mp.c_str() );

	if ( !d )
		return false;

    struct stat fInfo;
    if ( !stat(mp.c_str(), &fInfo) )
        {
        if ( S_ISDIR(fInfo.st_mode) )
            return true;
        else
            return false;
        }
    return false;
    
    closedir(d);
    
//#	endif
}

/** Checks whether the file, passed in as its path and file name components, is present */
bool RbFileManager::isFilePresent(const std::string &mp, const std::string &mf) {

//#	ifdef WIN32
//
//    std::string fullPath; 
//    if (mp.length() > 1 && (mp[mp.length()-2] != '\\' || mp[mp.length()-2] != '/'))
//        fullPath = mp + "\\" + mf;
//    else
//        fullPath = mp + mf;
//    return isFilePresent(mf);
//
//#	else
    
    std::string f = mp;
    if ( mp == "" ) {
        f = ".";
    }
# ifdef WIN32
    f += "\\" + mf;
# else
    f += "/" + mf;
#	endif
    return isFilePresent(f);
    
}


bool RbFileManager::isFilePresent(const std::string &fn) {

    struct stat fInfo;
    if ( !stat(fn.c_str(), &fInfo) )
        {
        if ( S_ISDIR(fInfo.st_mode) )
            return false;
        else
            return true;
        }

    return false;
    
}


/** Recursively lists the contents of the directory for the member variable filePath */
bool RbFileManager::listDirectoryContents(void) {

#	ifdef WIN32

    return listDirectoryContents(filePath);
    
#	else
    
    return listDirectoryContents(filePath);
    
#	endif
}


/** Recursively lists the contents of the directory passed in as an argument to the function */
bool RbFileManager::listDirectoryContents(const std::string& dirpath) {


    DIR* dir = opendir( dirpath.c_str() );
    if (dir)
        {
        struct dirent* entry;
        while ((entry = readdir( dir )))
            {
            struct stat entryinfo;
            std::string entryname = entry->d_name;
#	ifdef WIN32
            std::string entrypath = dirpath + "/" + entryname;
#	else
            std::string entrypath = dirpath + "/" + entryname;
#   endif
            if (!stat( entrypath.c_str(), &entryinfo ))
                {
                if (S_ISDIR( entryinfo.st_mode ))
                    {
                    if      (entryname == "..")
                        ;
                    else if (entryname == "." ) 
                        RBOUT( dirpath + "/" );
                        //result.push_back( dirpath + "/" );
                    else
                        listDirectoryContents( entrypath );
                    }
                else
                    {
                    RBOUT( entrypath );
                    //result.push_back( entrypath );
                    }
                }
            }
        closedir( dir );
        }
    return true;
}


/** Recursively fills in a vector with the names of the files in the directory filePath */
bool RbFileManager::setStringWithNamesOfFilesInDirectory(std::vector<std::string>& sv) {

#	ifdef WIN32

    return setStringWithNamesOfFilesInDirectory(filePath, sv);
    
#	else

    return setStringWithNamesOfFilesInDirectory(filePath, sv);
    
#	endif
}


/** Recursively fills in a vector with the names of the files in the directory passed in as an argument to the function */
bool RbFileManager::setStringWithNamesOfFilesInDirectory(const std::string& dirpath, std::vector<std::string>& sv) {


    DIR* dir = opendir( dirpath.c_str() );
    if (dir)
        {
        struct dirent* entry;
        while ((entry = readdir( dir )))
            {
            struct stat entryinfo;
            std::string entryname = entry->d_name;
#	ifdef WIN32
            std::string entrypath = dirpath + "/" + entryname;
#	else
            std::string entrypath = dirpath + "/" + entryname;
#   endif
            if (!stat( entrypath.c_str(), &entryinfo ))
                {
                if (S_ISDIR( entryinfo.st_mode ))
                    {
                    if (entryname == "..")
                        ;
                    else if (entryname == "." ) 
                        ;
                    else
                        setStringWithNamesOfFilesInDirectory( entrypath, sv );
                    }
                else
                    {
                    sv.push_back( entrypath );
                    }
                }
            }
        closedir( dir );
        }
    return true;
}


/** Opens a file for input */
bool RbFileManager::openFile(std::ifstream& strm) {
	
	/* concactenate path and file name */
	std::string filePathName = filePath + "/" + fileName;

	/* here we assume that the presence of the path/file has
	   been checked elsewhere */
	strm.open( filePathName.c_str(), std::ios::in );
	if ( !strm )
		return false;
	return true;
}


/** Opens a file for output */
bool RbFileManager::openFile(std::ofstream& strm) {
	
	/* concactenate path and file name */
	std::string filePathName = filePath + "/" + fileName;

	/* here we assume that the presence of the path/file has
	   been checked elsewhere */
	strm.open( filePathName.c_str(), std::ios::out );
	if ( !strm )
		return false;
	return true;
}


/** Closes an input file */
void RbFileManager::closeFile(std::ifstream& strm) {

	strm.close();
}


/** Closes an output file */
void RbFileManager::closeFile(std::ofstream& strm) {

	strm.close();
}


/** Is this a directory */
bool RbFileManager::isDirectory(std::string fn) {

    return isDirectoryPresent(fn);
}


/** Is this a file */
bool RbFileManager::isFile(std::string fn) {

    return isFilePresent(fn);
}


/** Checks whether the file name is empty */
bool RbFileManager::isFileNamePresent(void) {

    if ( fileName == "" )
        return false;
    return true;
}


/** Tests whether the directory specified in the object is present */
bool RbFileManager::testDirectory(void) {

	return isDirectoryPresent(filePath);
}


/** Tests whether the file specified in the object is present */
bool RbFileManager::testFile(void) {

	return isFilePresent(filePath, fileName);
}





