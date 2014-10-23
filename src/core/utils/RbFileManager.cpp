#include "RbException.h"
#include "RbFileManager.h"
#include "RbSettings.h"
#include "StringUtilities.h"


#include <iostream>
#include <string>
#include <sys/stat.h>

//#include <boost/filesystem.hpp>


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
RbFileManager::RbFileManager( void ) 
{
    
#	ifdef WIN32
    pathSeparator = "\\";
#	else
    pathSeparator = "/";
#   endif

    // make certain the current file/path information is empty
	setFileName("");
	setFilePath("");
    
    fullFileName = filePath;
    if ( fullFileName == "") 
    {
        fullFileName = ".";
    }
    fullFileName += pathSeparator + fileName;
    
}


/** Constructor taking as an argument a string containing a file path and (maybe) a file name */
RbFileManager::RbFileManager(const std::string &fn)
{
    
#	ifdef WIN32
    pathSeparator = "\\";
#	else
    pathSeparator = "/";
#   endif

    // make certain the current file/path information is empty
//    setCurrentDirectory("");
	setFileName("");
	setFilePath("");

    // initialize the current directory to be the directory the binary is sitting in
//    setCurrentDirectory( findCurrentDirectory() );
    
    // set the path and file for the string
    parsePathFileNames( fn );
    
    fullFileName = filePath;
    if ( fullFileName == "") 
    {
        fullFileName = ".";
    }
    
    fullFileName += pathSeparator + fileName;
    
}


/** Constructor taking as an argument a string containing a file path and (maybe) a file name */
RbFileManager::RbFileManager(const std::string &pn, const std::string &fn)
{
    
#	ifdef WIN32
    pathSeparator = "\\";
#	else
    pathSeparator = "/";
#   endif
    
    // make certain the current file/path information is empty
    //    setCurrentDirectory("");
    setFileName("");
    setFilePath("");
    
    // initialize the current directory to be the directory the binary is sitting in
    //    setCurrentDirectory( findCurrentDirectory() );
    
    // set the path and file for the string
    parsePathFileNames( pn + pathSeparator + fn);
    
    fullFileName = filePath;
    if ( fullFileName == "")
    {
        fullFileName = ".";
    }
    
    fullFileName += pathSeparator + fileName;
    
}



/** Closes an input file */
void RbFileManager::closeFile(std::ifstream& strm)
{
    
	strm.close();
}


/** Closes an output file */
void RbFileManager::closeFile(std::ofstream& strm)
{
    
	strm.close();
}


/**
 * Create the directories in which the file exist.
 * We do this recursively.
 */
void RbFileManager::createDirectoryForFile( void )
{
    
    std::string dir_path = getLastPathComponent( fullFileName );
    
//    boost::filesystem::path dir(dir_path);
//	if(boost::filesystem::create_directory(dir))
//    {
//		std::cout << "Success" << "\n";
//	}
    
}



/** Format the error exception string for problems specifying the file/path name */
void RbFileManager::formatError(std::string& errorStr) 
{
    
    bool fileNameProvided    = isFileNamePresent();
    bool isFileNameGood      = testFile();
    bool isDirectoryNameGood = testDirectory();
    
    if ( fileNameProvided == false && isDirectoryNameGood == false )
    {
        errorStr += "Could not read contents of directory \"" + getFilePath() + "\" because the directory does not exist";
    }
    else if (fileNameProvided == true && (isFileNameGood == false || isDirectoryNameGood == false))
    {
        errorStr += "Could not read file named \"" + getFileName() + "\" in directory named \"" + getFilePath() + "\" ";
        if (isFileNameGood == false && isDirectoryNameGood == true)
        {
            errorStr += "because the file does not exist";
        }
        else if (isFileNameGood == true && isDirectoryNameGood == false)
        {
            errorStr += "because the directory does not exist";
        }
        else
        {
            errorStr += "because neither the directory nor the file exist";
        }
    }
    
}


const std::string& RbFileManager::getCurrentDirectory( void ) const 
{
    return RbSettings::userSettings().getWorkingDirectory();
}


std::string RbFileManager::getFileExtension( void ) const
{
    std::vector<std::string> tokens;
    StringUtilities::stringSplit(fileName,".",tokens);
    return tokens[tokens.size()-1];
}


const std::string& RbFileManager::getFileName( void ) const
{
    return fileName;
}


const std::string& RbFileManager::getFilePath( void ) const
{
    return filePath;
}


const std::string& RbFileManager::getFullFileName( void ) const
{
    return fullFileName;
}


std::string RbFileManager::getFullFilePath( void ) const
{
//    DIR* d = opendir( fullFileName.c_str() );
//    struct dirent *ent;
//    while((ent = readdir(d)) != NULL)
//    {
//        std::cout << (ent->d_name) << std::endl;
//    }
//    std::string tmp = d->d_name;
    return RbSettings::userSettings().getWorkingDirectory() + pathSeparator + filePath;
}


const std::string& RbFileManager::getPathSeparator( void ) const
{
    return pathSeparator;
}


/** Is this a directory */
bool RbFileManager::isDirectory( void ) const
{
    
    return isDirectoryPresent(fullFileName);
}


/** Tests whether the directory (passed in as an argument) is present */
bool RbFileManager::isDirectoryPresent(const std::string &mp) const 
{

    if ( mp == "" ) 
    {
        return true;
    }
    
    DIR* d = opendir( mp.c_str() );

	if ( !d )
    {
		return false;
    }
    
    struct stat fInfo;
    if ( !stat(mp.c_str(), &fInfo) )
    {
        if ( S_ISDIR(fInfo.st_mode) )
        {
            return true;
        }
        else
        {
            return false;
        }
        
    }
    
    closedir(d);
    
    return false;
}


/** Is this a file */
bool RbFileManager::isFile( void ) const
{
    
    return isFilePresent(filePath, fileName);
}


/** Checks whether the file name is empty */
bool RbFileManager::isFileNamePresent(void) const {
    
    if ( fileName == "" )
        return false;
    return true;
}


/** Checks whether the file, passed in as its path and file name components, is present */
bool RbFileManager::isFilePresent(const std::string &mp, const std::string &mf) const
{

    
    std::string f = mp;
    if ( mp == "" ) 
    {
        f = ".";
    }
    
    f += pathSeparator + mf;

    return isFilePresent(f);
    
}


bool RbFileManager::isFilePresent(const std::string &fn) const
{

    struct stat fInfo;
    if ( !stat(fn.c_str(), &fInfo) )
    {
        if ( S_ISDIR(fInfo.st_mode) )
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    return false;
    
}


/** Recursively lists the contents of the directory for the member variable filePath */
bool RbFileManager::listDirectoryContents(void) 
{

    return listDirectoryContents(filePath);
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
            std::string entrypath = dirpath + pathSeparator + entryname;
            if (!stat( entrypath.c_str(), &entryinfo ))
            {
                if (S_ISDIR( entryinfo.st_mode ))
                {
                    if      (entryname == "..")
                    {
                        ;
                    }
                    else if (entryname == "." ) 
                    {
                        ;
                        //RBOUT( dirpath + "/" );
                        //result.push_back( dirpath + "/" );
                    }
                    else
                    {
                        listDirectoryContents( entrypath );
                    }
                }
                else
                {
                    //RBOUT( entrypath );
                    //result.push_back( entrypath );
                }
            }
        }
        closedir( dir );
    }
    
    return true;
}


/** Opens a file for input */
bool RbFileManager::openFile(std::ifstream& strm) 
{
	
	/* concactenate path and file name */
	std::string filePathName = filePath + pathSeparator + fileName;

	/* here we assume that the presence of the path/file has
	   been checked elsewhere */
	strm.open( filePathName.c_str(), std::ios::in );
	if ( !strm )
    {
		return false;
	}
    
    return true;
}


/** Opens a file for output */
bool RbFileManager::openFile(std::ofstream& strm) 
{
	
	/* concactenate path and file name */
	std::string filePathName = filePath + pathSeparator + fileName;

	/* here we assume that the presence of the path/file has
	   been checked elsewhere */
	strm.open( filePathName.c_str(), std::ios::out );
	if ( !strm )
    {
		return false;
	}
    
    return true;
}


/** Divides a string into the path and file name components */
bool RbFileManager::parsePathFileNames(std::string s) 
{
    
    // check if the path is a good one
    bool isDPresent = isDirectoryPresent(s);
    bool isFPresent = isFilePresent(s);
    
    if (isDPresent == true && isFPresent == false)
    {
        fileName = "";
        int location = (int)s.find_last_of( pathSeparator );
        if ( location == (int)s.length() - 1 )
        {
            s.erase( location );
        }
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
	size_t location = s.find_last_of( pathSeparator );
	
	if ( location == std::string::npos )
    {
		/* There is no path in this string. We 
         must have only the file name, and the
         file should be in our current directory. */
		fileName = s;
		filePath = "";
    }
	else if ( location == s.length() - 1 )
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
        
    }
    
	return true;
}

std::string RbFileManager::getLastPathComponent(std::string& s)
{

    std::string tempS = s;
	size_t location = tempS.find_last_of( pathSeparator );
	if ( location == std::string::npos )
    {
		/* There is no path in this string. We 
         must have only the file name. */
		return tempS;
    }
	else if ( location == tempS.length() - 1 )
    {
		/* It looks like the last character is "/", which
         means that no file name has been provided. */
		return "";
    }
	else
    {
		/* We can divide the path into the path and the file. */
		std::string lpc = tempS.substr( location+1, tempS.length()-location-1 );
        return lpc;
    }
    
    return "";
}

std::string RbFileManager::getStringByDeletingLastPathComponent(std::string& s)
{

    std::string tempS = s;
	size_t location = tempS.find_last_of( pathSeparator );
	if ( location == std::string::npos )
    {
		/* There is no path in this string. We 
         must have only the file name. */
		return "";
    }
	else if ( location == tempS.length() - 1 )
    {
		/* It looks like the last character is "/", which
         means that no file name has been provided. */
		return tempS;
    }
	else
    {
		/* We can divide the path into the path and the file. */
		tempS.erase( location );
		return tempS;
    }
    
    return "";
}

void RbFileManager::setFileName(std::string const &s)
{
    fileName = s;
    
    fullFileName = filePath;
    if ( fullFileName == "") 
    {
        fullFileName = ".";
    }
    fullFileName += pathSeparator + fileName;
    
}

void RbFileManager::setFilePath(std::string const &s)
{
    filePath = s;
    
    fullFileName = filePath;
    if ( fullFileName == "") 
    {
        fullFileName = ".";
    }
    fullFileName += pathSeparator + fileName;
    
}

/** Recursively fills in a vector with the names of the files in the directory filePath */
bool RbFileManager::setStringWithNamesOfFilesInDirectory(std::vector<std::string>& sv) 
{
    
    return setStringWithNamesOfFilesInDirectory(filePath, sv);
}


/** Recursively fills in a vector with the names of the files in the directory passed in as an argument to the function */
bool RbFileManager::setStringWithNamesOfFilesInDirectory(const std::string& dirpath, std::vector<std::string>& sv) {
    
    
    DIR* dir = opendir( dirpath.c_str() );
    if (dir)
    {
        struct dirent* entry;
        while ( (entry = readdir( dir )) )
        {
            struct stat entryinfo;
            std::string entryname = entry->d_name;
            std::string entrypath = dirpath + pathSeparator + entryname;
            
            if (!stat( entrypath.c_str(), &entryinfo ))
            {
                if (S_ISDIR( entryinfo.st_mode ))
                {
                    if (entryname == "..")
                    {
                        ;
                    }
                    else if (entryname == "." )
                    {
                        ;
                    }
                    else
                    {
                        setStringWithNamesOfFilesInDirectory( entrypath, sv );
                    }
                    
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


/** Tests whether the directory specified in the object is present */
bool RbFileManager::testDirectory(void) {

	return isDirectoryPresent(filePath);
}


/** Tests whether the file specified in the object is present */
bool RbFileManager::testFile(void) {

	return isFilePresent(filePath, fileName);
}





