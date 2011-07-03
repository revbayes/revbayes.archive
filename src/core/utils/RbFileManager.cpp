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
#include <iostream>
#include <string>
#include <sys/stat.h>

#ifdef WIN32
#   include <windows.h>
#   include <tchar.h> 
#   include <stdio.h>
#   include <strsafe.h>
#else
#	include <dirent.h>
#endif




/** Default constructor, creating a file manager object with the file 
    path equal to the current (default) directory and an empty file name */
RbFileManager::RbFileManager(void) {

	setFileName("");
	setFilePath("");
	setCurDirectory( findCurrentDirectory() );
	setFilePath( getCurDirectory() );
}


/** Constructor taking as an argument a string containing a file path and (maybe) a file name */
RbFileManager::RbFileManager(std::string s) {

	setFileName("");
	setFilePath("");
	parsePathFileNames(s);
	setCurDirectory( findCurrentDirectory() );
	if ( getFilePath() == "" )
		setFilePath( getCurDirectory() );
}


/** Divides a string into the path and file name components */
bool RbFileManager::parsePathFileNames(std::string s) {

	if ( s.length() == 0 )
		{
		/* the string that is supposed to hold the
		   path/file information is empty. */
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
		   means that no file name has been provided. */
		s.erase( location );
		fileName = "";
		filePath = s;
		return false;
		}
	else
		{
		/* We can divide the path into the path and the file. */
		fileName = s.substr( location+1, s.length()-location-1 );
		s.erase( location );
		filePath = s;
		}

	return true;
}


/** Finds the current (default) directory of the process */
#define	MAX_DIR_PATH	2048
std::string RbFileManager::findCurrentDirectory(void) {

#	ifdef WIN32

    LPSTR buf = new char[MAX_DIR_PATH];
    GetCurrentDirectory(MAX_DIR_PATH, buf);
    return std::string(buf);

#	else

	char cwd[MAX_DIR_PATH+1];
	if ( !getcwd(cwd, MAX_DIR_PATH+1) )
		return "";
	std::string curdir = cwd;
	
	if ( curdir.at( curdir.length()-1 ) == '/' )
		curdir.erase( curdir.length()-1 );
	
	return curdir;
    
#	endif
}


/** Tests whether the directory (passed in as an argument) is present */
bool RbFileManager::isDirectoryPresent(const std::string mp) {

#	ifdef WIN32

    WIN32_FIND_DATA data;
    HANDLE handle = FindFirstFile(mp.c_str(), &data);
    bool bFoundFile = handle != INVALID_HANDLE_VALUE;
    FindClose(handle);
    return bFoundFile;

#	else

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
    
#	endif
}


/** Checks whether the file, passed in as its path and file name components, is present */
bool RbFileManager::isFilePresent(const std::string mp, const std::string mf) {

#	ifdef WIN32

    WIN32_FIND_DATA data;
    std::string fullPath; 
    if (mp.length() > 1 && (mp[mp.length()-2] != '\\' || mp[mp.length()-2] != '/'))
        fullPath = mp + "\\" + mf;
    else
        fullPath = mp + mf;
    return isFilePresent(f);

#	else
    
    std::string f = mp + "/" + mf;
    return isFilePresent(f);
    
#	endif
}


bool RbFileManager::isFilePresent(const std::string fn) {

#	ifdef WIN32

    WIN32_FIND_DATA data;
    std::string fullPath = fn; 
    HANDLE handle = FindFirstFile(fullPath.c_str(), &data);
    bool bFoundFile = handle != INVALID_HANDLE_VALUE;
    FindClose(handle);
    return bFoundFile;

#	else

	DIR* d = opendir( fn.c_str() );
	if ( !d )
		return false;
    struct stat fInfo;
    if ( !stat(fn.c_str(), &fInfo) )
        {
        if ( S_ISDIR(fInfo.st_mode) )
            return false;
        else
            return true;
        }
    closedir(d);

    return false;
    
#	endif
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

#	ifdef WIN32

   WIN32_FIND_DATA ffd;
   TCHAR szDir[MAX_PATH];
   size_t length_of_arg;
   HANDLE hFind = INVALID_HANDLE_VALUE;
   DWORD dwError=0;

   // Code below is largely copied from msdn.com -- Fredrik

   // Check that the input path plus 3 is not longer than MAX_PATH.
   // Three characters are for the "\*" plus NULL appended below.
   // If input path is too long, then we just return false for now
   StringCchLength( dirpath.c_str(), MAX_PATH, &length_of_arg );

   if ( length_of_arg > (MAX_PATH - 3) )
       return false;

   // Prepare string for use with FindFile functions.  First, copy the
   // string to a buffer, then append '\*' to the directory name.
   StringCchCopy( szDir, MAX_PATH, dirpath.c_str() );
   StringCchCat ( szDir, MAX_PATH, TEXT("\\*") );

   // Find the first file in the directory.
   hFind = FindFirstFile( szDir, &ffd );

   // Just return false if we did not find a file
   if ( hFind == INVALID_HANDLE_VALUE )
       return false;
  
   // List all the files in the directory with some info about them.
   do {

        std::string entryName = ffd.cFileName;
        std::string entryPath = dirpath + "\\" + entryName;

       // If directory, we recurse into the directory
       if ( ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
         
            if      ( entryName == ".." ) {
                ;
            }
            else if ( entryName == "."  ) {
                std::cout << dirpath + "/" << std::endl;
                //result.push_back( dirpath + "/" );
            }
            else {
                listDirectoryContents( entryPath );
            }
        }

        // If file, just list file path
        else {
            std::cout << entryPath << std::endl;
            //result.push_back( entryPath );
        }
    } while ( FindNextFile( hFind, &ffd ) != 0 );
 
    dwError = GetLastError();

    FindClose( hFind );

    return dwError == ERROR_NO_MORE_FILES;

#	else

    DIR* dir = opendir( dirpath.c_str() );
    if (dir)
        {
        struct dirent* entry;
        while ((entry = readdir( dir )))
            {
            struct stat entryinfo;
            std::string entryname = entry->d_name;
            std::string entrypath = dirpath + "/" + entryname;
            if (!stat( entrypath.c_str(), &entryinfo ))
                {
                if (S_ISDIR( entryinfo.st_mode ))
                    {
                    if      (entryname == "..")
                        ;
                    else if (entryname == "." ) 
                        std::cout << dirpath + "/" << std::endl;
                        //result.push_back( dirpath + "/" );
                    else
                        listDirectoryContents( entrypath );
                    }
                else
                    {
                    std::cout << entrypath << std::endl;
                    //result.push_back( entrypath );
                    }
                }
            }
        closedir( dir );
        }
    return true;
#   endif
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

#	ifdef WIN32

   WIN32_FIND_DATA ffd;
   TCHAR szDir[MAX_PATH];
   size_t length_of_arg;
   HANDLE hFind = INVALID_HANDLE_VALUE;
   DWORD dwError=0;

   // Code below is largely copied from msdn.com -- Fredrik

   // Check that the input path plus 3 is not longer than MAX_PATH.
   // Three characters are for the "\*" plus NULL appended below.
   // If input path is too long, then we just return false for now
   StringCchLength( dirpath.c_str(), MAX_PATH, &length_of_arg );

   if ( length_of_arg > (MAX_PATH - 3) )
       return false;

   // Prepare string for use with FindFile functions.  First, copy the
   // string to a buffer, then append '\*' to the directory name.
   StringCchCopy( szDir, MAX_PATH, dirpath.c_str() );
   StringCchCat ( szDir, MAX_PATH, TEXT("\\*") );

   // Find the first file in the directory.
   hFind = FindFirstFile( szDir, &ffd );

   // Just return false if we did not find a file
   if ( hFind == INVALID_HANDLE_VALUE )
       return false;
  
   // List all the files in the directory with some info about them.
   do {

        std::string entryName = ffd.cFileName;
        std::string entryPath = dirpath + "\\" + entryName;

       // If directory, we recurse into the directory
       if ( ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
         
            if      ( entryName == ".." ) {
                ;
            }
            else if ( entryName == "."  ) {
                ;
            }
            else {
                setStringWithNamesOfFilesInDirectory( entryPath, sv );
            }
        }

        // If file, just push back file path
        else {
            sv.push_back( entryPath );
        }
    } while ( FindNextFile( hFind, &ffd ) != 0 );
 
    dwError = GetLastError();

    FindClose( hFind );

    return dwError == ERROR_NO_MORE_FILES;
    
#	else

    DIR* dir = opendir( dirpath.c_str() );
    if (dir)
        {
        struct dirent* entry;
        while ((entry = readdir( dir )))
            {
            struct stat entryinfo;
            std::string entryname = entry->d_name;
            std::string entrypath = dirpath + "/" + entryname;
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
#   endif
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





