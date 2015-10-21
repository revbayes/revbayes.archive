#include "RbException.h"
#include "RbFileManager.h"
#include "RbSettings.h"
#include "StringUtilities.h"


#include <iostream>
#include <string>
#include <cstring>
#include <sys/stat.h>
#include <cstdlib>

//#include <boost/filesystem.hpp>


#ifdef WIN32
#	include <dirent.h>
#   include <unistd.h>
#   include <windows.h>
#   include "Shlwapi.h"
#else
#	include <dirent.h>
#   include <unistd.h>
#endif


using namespace RevBayesCore;

/** Default constructor, creating a file manager object with the file 
    path equal to the current (default) directory and an empty file name */
RbFileManager::RbFileManager( void ) :
    fileName( "" ),
    filePath( "" ),
    fullFileName( "" ),
    pathSeparator( "" )
{
    
#	ifdef WIN32
    pathSeparator = "\\";
#	else
    pathSeparator = "/";
#   endif

    // make certain the current file/path information is empty
	setFileName("");
	setFilePath(".");
    
    fullFileName = filePath;
    if ( fullFileName != "")
    {
        fullFileName += pathSeparator;
    }
    
    fullFileName += fileName;
    
}


/** Constructor taking as an argument a string containing a file path and (maybe) a file name */
RbFileManager::RbFileManager(const std::string &fn) :
    fileName( fn ),
    filePath( "" ),
    fullFileName( "" ),
    pathSeparator( "" )
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
    if ( fullFileName != "")
    {
        fullFileName += pathSeparator;
    }
    
    fullFileName += fileName;
    
}


/** Constructor taking as an argument a string containing a file path and (maybe) a file name */
RbFileManager::RbFileManager(const std::string &pn, const std::string &fn) :
    fileName( fn ),
    filePath( pn ),
    fullFileName( "" ),
    pathSeparator( "" )
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
    std::string tmp = pn + pathSeparator + fn;
    parsePathFileNames( tmp );
    
    fullFileName = filePath;
    if ( fullFileName != "")
    {
        fullFileName += pathSeparator;
    }
    
    fullFileName += fileName;
    
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
    
    std::string dir_path = getStringByDeletingLastPathComponent( fullFileName );
    
    std::vector<std::string> pathComponents;
    StringUtilities::stringSplit(filePath, pathSeparator, pathComponents);
    
    std::string directoryName = "";
    for ( std::vector<std::string>::const_iterator it=pathComponents.begin(); it != pathComponents.end(); ++it)
    {
        directoryName += *it;
        
        if ( isDirectoryPresent( directoryName ) == false )
        {
            makeDirectory( directoryName );
        }
        
        directoryName += pathSeparator;
    }
    
}




/**
 * Portable code to get full path to user home directory.
 *
 * @param path
 * @return full path to user home directory
 */
std::string RbFileManager::expandUserDir(std::string path)
{
    if ( !path.empty() && path[0] == '~')
    {
        char const* home = getenv("HOME");
        
        if (home or ((home = getenv("USERPROFILE"))))
        {
            path.replace(0, 1, home);
        }
    }
    else
    {
        char const *hdrive = getenv("HOMEDRIVE"), *hpath = getenv("HOMEPATH");
        path.replace(0, 1, std::string(hdrive) + hpath);
    }
    
    return path;
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

std::string RbFileManager::getFileNameWithoutExtension( void ) const
{
    std::vector<std::string> tokens;
    StringUtilities::stringSplit(fileName,".",tokens);
    std::string name = "";
    
    for (size_t i = 0; i < tokens.size()-1; ++i)
    {
        name += tokens[i];
    }
    
    return name;
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
        
    std::string fullFilePath = filePath;
        
    // check if filePath is relative or absolute
    // add current working path only if relative
#	ifdef WIN32
        
    if(PathIsRelative(filePath))
    {
            
#	else
            
    if( filePath.size() > 0 && pathSeparator[0] != filePath[0] )
    {
                
#   endif
        fullFilePath = RbSettings::userSettings().getWorkingDirectory() + pathSeparator + filePath;
    }
            
    return fullFilePath;
    
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


const std::string& RbFileManager::getPathSeparator( void ) const
{
    return pathSeparator;
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


/** Is this a directory */
bool RbFileManager::isDirectory( void ) const
{
    
    return isDirectoryPresent(fullFileName);
}


/** Tests whether the directory (passed in as an argument) is present */
bool RbFileManager::isDirectoryPresent(const std::string &mp) const 
{

    // Sebastian (20150416): This was temporary code to solve the problem that sometimes this function errornously did see a directory.
    // I keep it here for a bit as a reference if the problem re-occurs.
//    if ( mp == "" )
//    {
//        return true;
//    }
//    
//    struct stat info;
//    
//    if( stat( mp.c_str(), &info ) != 0)
//    {
//        return false;
//    }
//    else if(info.st_mode & S_IFDIR)
//    {
//        return true;
//    }
//    else
//    {
//        return false;
//    }
    
    DIR* d = opendir( mp.c_str() );

	if ( d == NULL )
    {
		return false;
    }
    
    struct stat fInfo;
    if ( !stat(mp.c_str(), &fInfo) )
    {
        if ( S_ISDIR(fInfo.st_mode) )
        {
            closedir(d);

            return true;
        }
        else
        {
            closedir(d);
            
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
bool RbFileManager::isFileNamePresent(void) const
{
    
    if ( fileName == "" )
    {
        return false;
    }
    
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


/**
 * Make a directory for the given path.
 */
bool RbFileManager::makeDirectory(const std::string &dn)
{
    
#	ifdef WIN32
    
    std::string cmd = "mkdir " + dn;
    system( cmd.c_str() );
    
#	else
    
    std::string cmd = "mkdir " + dn;
    return ( system( cmd.c_str() ) == 0 );
    
#   endif
    
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
bool RbFileManager::parsePathFileNames(const std::string &input_string)
{
    std::string name = input_string;
    
    // check if the path is a good one
    bool isDPresent = isDirectoryPresent(name);
    bool isFPresent = isFilePresent(name);
    
    if ( name.length() > 0 && isDPresent == true && isFPresent == false)
    {
        fileName = "";
        size_t location = name.find_last_of( pathSeparator );
        if ( location == name.length() - 1 )
        {
            name.erase( location );
        }
        filePath = name;
        return true;
    }
    
    // the string that is supposed to hold the path/file information is empty.
	if ( name.length() == 0 )
    {
        filePath = ".";
		return false;
    }
    
	// Find the location of the last "/".
    //This is where we will divide the path/file string into two.
	size_t location = StringUtilities::findLastOf( name, pathSeparator[0] );

	if ( location == std::string::npos )
    {
		/* There is no path in this string. We 
         must have only the file name, and the
         file should be in our current directory. */
		fileName = name;
		filePath = ".";
    }
	else if ( location == name.length() - 1 )
    {
		// It looks like the last character is "/", which
        // means that no file name has been provided. However,
        // it also means that the directory that has been provided
        // is not valid, otherwise it would have tested as
        // being present (above).
		fileName = "";
		filePath = ".";
		return false;
    }
	else
    {
		// We can divide the path into the path and the file.
		fileName = name.substr( location+1, name.length()-location-1 );
		name.erase( location );
		filePath = name;
        
    }

	return true;
}


void RbFileManager::setFileName(std::string const &s)
{
    fileName = s;
    
    fullFileName = filePath;
    if ( fullFileName == "" )
    {
        fullFileName = ".";
    }
    fullFileName += pathSeparator + fileName;
    
}

void RbFileManager::setFilePath(std::string const &s)
{
    filePath = s;
    
    fullFileName = filePath;
    if ( fullFileName == "" )
    {
        fullFileName = ".";
    }
    fullFileName += pathSeparator + fileName;
    
}

/** Recursively fills in a vector with the names of the files in the directory filePath */
bool RbFileManager::setStringWithNamesOfFilesInDirectory(std::vector<std::string>& sv, bool recursive)
{
    
    return setStringWithNamesOfFilesInDirectory(filePath, sv, recursive);
}


/** Recursively fills in a vector with the names of the files in the directory passed in as an argument to the function */
bool RbFileManager::setStringWithNamesOfFilesInDirectory(const std::string& dirpath, std::vector<std::string>& sv, bool recursive)
{
    
    
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
                
                if (entryname == "..")
                {
                    ;
                }
                else if (entryname == "." )
                {
                    ;
                }
                else if ( recursive == true && S_ISDIR( entryinfo.st_mode ) )
                {
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


/** Tests whether the directory specified in the object is present */
bool RbFileManager::testDirectory(void)
{

	return isDirectoryPresent(filePath);
}


/** Tests whether the file specified in the object is present */
bool RbFileManager::testFile(void)
{

	return isFilePresent(filePath, fileName);
}





