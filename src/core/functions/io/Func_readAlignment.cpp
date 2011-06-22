/**
 * @file
 * This file contains the implementation of Func_readAlignment.
 *
 * @brief Implementation of Func_readAlingment
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#include "Boolean.h"
#include "ContainerNode.h"
#include "Func_readAlignment.h"
#include "NclReader.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RbNames.h"
#include "RbString.h"
#include "StringUtilities.h"
#include "UserInterface.h"
#include "ValueContainer.h"
#include "ValueRule.h"
#include "VectorString.h"
#include <map>
#include <sstream>




/** Clone object */
Func_readAlignment* Func_readAlignment::clone( void ) const {
    
    return new Func_readAlignment( *this );
}


/** Execute function */
DAGNode* Func_readAlignment::execute( void ) {

    // get the information from the arguments for reading the file
    const RbString* fn       = static_cast<const RbString*>( args[0].getValue() );
    //const RbString* fileFormat   = static_cast<const RbString*>( args[1].getValue() );
    //const RbString* dataType     = static_cast<const RbString*>( args[2].getValue() );
    //const Boolean* isInterleaved = static_cast<const Boolean*>( args[3].getValue() );

    // check that the file/path name has been correctly specified
    RbFileManager myFileManager( fn->getValue() );
    if ( (myFileManager.isFileNamePresent() == false && myFileManager.testDirectory() == false) ||
         (myFileManager.isFileNamePresent() == true  && (myFileManager.testFile() == false || myFileManager.testDirectory() == false)) )
        {
        std::string errorStr = "";
        formatError(myFileManager, errorStr);
        throw( RbException(errorStr) );
        }

    // are we reading a single file or are we reading the contents of a directory?
    bool readingDirectory = false;
    if ( myFileManager.testDirectory() == true && myFileManager.testFile() == false )
        readingDirectory = true;
        
    // set up a vector of strings containing the name or names of the files to be read
    std::vector<std::string> vectorOfFileNames;
    if (readingDirectory == true)
        myFileManager.setStringWithNamesOfFilesInDirectory(vectorOfFileNames);
    else
        vectorOfFileNames.push_back( myFileManager.getFilePath() + "/" + myFileManager.getFileName() );
        
    // Set up a map with the file name to be read as the key and the file type as the value. Note that we may not
    // read all of the files in the string called "vectorOfFileNames" because some of them may not be in a format
    // that can be read.
    std::map<std::string,std::string> fileMap;
    for (std::vector<std::string>::iterator p = vectorOfFileNames.begin(); p != vectorOfFileNames.end(); p++)
        {
        bool isInterleaved = false;
        std::string myFileType = "unknown", dType = "unknown";
        if (isNexusFile(*p) == true)
            myFileType = "nexus";
        else if (isPhylipFile(*p, dType, isInterleaved) == true)
            myFileType = "phylip";
        else if (isFastaFile(*p, dType) == true)
            myFileType = "fasta";
            
        if (myFileType != "unknown")
            {
            std::string suffix = "|" + dType;
            if ( myFileType == "phylip" )
                {
                if (isInterleaved == true)
                    suffix += "|interleaved";
                else
                    suffix += "|noninterleaved";
                }
            else if ( myFileType == "fasta" )
                suffix += "|noninterleaved";
            else
                suffix += "|unknown";
            myFileType += suffix;
            fileMap.insert( std::make_pair(*p,myFileType) );
            }
        else
            {
            RBOUT("Warning: File \"" + *p + "\" not read (unknown file type)");            
            }
        }
        
    for (std::map<std::string,std::string>::iterator p = fileMap.begin(); p != fileMap.end(); p++)
        RBOUT("Reading file \"" + p->first + "\" (" + p->second + ")");
        
    // get the global instance of the NCL reader
    NclReader& reader = NclReader::getInstance();
    
    // read the files in the map containing the file names with the output being a vector of pointers to
    // the character matrices that have been read
    std::vector<CharacterMatrix*>* m = reader.readMatrices( fileMap );
    std::cout << "number on vector = " << m->size() << std::endl;
    
#   if 0    
    std::vector<RbObject*>* tmp = (std::vector<RbObject*>*) m;
    
    // create a value container with all matrices
    //! @Sebastian: Replaced call to unsafe constructor with call to safe constructor
    //  We might want to replace this call to the ValueContainer constructor with a call
    //  to a generic vector class constructor
    std::vector<size_t> length;
    length.push_back( tmp->size() );
    ValueContainer* vc = new ValueContainer( TypeSpec( CharacterMatrix_name, 1 ), length, *tmp );
    
    return new ContainerNode( vc );
#   endif
    return NULL;
}


/** Format the error exception string for problems specifying the file/path name */
void Func_readAlignment::formatError(RbFileManager& fm, std::string& errorStr) {

    bool fileNameProvided    = fm.isFileNamePresent();
    bool isFileNameGood      = fm.testFile();
    bool isDirectoryNameGood = fm.testDirectory();
    
    if ( fileNameProvided == false && isDirectoryNameGood == false )
        errorStr += "Could not read contents of directory \"" + fm.getFilePath() + "\" because the directory does not exist";
    else if (fileNameProvided == true && (isFileNameGood == false || isDirectoryNameGood == false))
        {
        errorStr += "Could not read file named \"" + fm.getFileName() + "\" in directory named \"" + fm.getFilePath() + "\" ";
        if (isFileNameGood == false && isDirectoryNameGood == true)
            errorStr += "because the file does not exist";
        else if (isFileNameGood == true && isDirectoryNameGood == false)
            errorStr += "because the directory does not exist";
        else
            errorStr += "because neither the directory nor the file exist";
        }
}


/** Get argument rules */
const ArgumentRules& Func_readAlignment::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules;
    static bool          rulesSet = false;
    
    if (!rulesSet) 
        {
        argumentRules.push_back( new ValueRule( "file", RbString_name ) );
        //argumentRules.push_back( new ValueRule( "format", RbString_name ) );
        //argumentRules.push_back( new ValueRule( "type", RbString_name ) );
        //argumentRules.push_back( new ValueRule( "interleaved", Boolean_name ) );
        rulesSet = true;
        }
            
    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_readAlignment::getClass( void ) const {
    
    static VectorString rbClass = VectorString( Func_readAlignment_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec Func_readAlignment::getReturnType( void ) const {
    
    return TypeSpec( CharacterMatrix_name );
}


/** Attempt to determine the type of data this is being read */
std::string Func_readAlignment::intuitDataType(std::string& s) {

    // set up some strings containing the valid states for the different data types we are interested
    // in distiguishing
    static std::string dnaStates = "acgtmgrsvwyhkdbn-.?";
    static std::string rnaStates = "acgumgrsvwyhkdbn-.?";
    static std::string aaStates  = "arndcqeghilkmfpstwyv-.?";
    static std::string stdStates = "0123456789n-.?";
    static std::string nucStates = "acgtu";
    
    // and intialize a few variables we'll be needing
    int nucCount = 0, nMissing = 0;
    bool notDna = false, notRna = false, notAa = false, notStd = false;
    
    // loop over the string (s) that contains the raw data we look at the state and try to determine if the
    // state rules out certain data types
    StringUtilities::toLower( s );
    for (size_t i=0; i<s.size(); i++)
        {        
        char c = s[i];
        
        if (c == 'n' || c == '-' || c == '.' || c == '?')
            nMissing++;
            
        if (notDna == false)
            {
            bool foundState = false;
            for (size_t j=0; j<19; j++)
                {
                if ( c == dnaStates[j] )
                    {
                    foundState = true;
                    break;
                    }
                }
            if (foundState == false)
                notDna = true;
            }

        if (notRna == false)
            {
            bool foundState = false;
            for (size_t j=0; j<19; j++)
                {
                if ( c == rnaStates[j] )
                    {
                    foundState = true;
                    break;
                    }
                }
            if (foundState == false)
                notRna = true;
            }
            
        if (notAa == false)
            {
            bool foundState = false;
            for (size_t j=0; j<23; j++)
                {
                if ( c == aaStates[j] )
                    {
                    foundState = true;
                    break;
                    }
                }
            if (foundState == false)
                notAa = true;
            }

        if (notStd == false)
            {
            bool foundState = false;
            for (size_t j=0; j<14; j++)
                {
                if ( c == stdStates[j] )
                    {
                    foundState = true;
                    break;
                    }
                }
            if (foundState == false)
                notStd = true;
            }
            
        for (size_t j=0; j<5; j++)
            {
            if ( c == nucStates[j] )
                nucCount++;
            }
        }
    
    // Try to determine which type of data it is. Note that using only the sequence information in the
    // string s that it is difficult or impossible to rule out certain data types. For example, if the
    // data is DNA, then the DNA states with the ambiguity codes do not rule out amino acid data. However,
    // we would expect a lot of A, C, G, and Ts in our data and not much else if the data is DNA, so we
    // use a bit of "fuzzy" logic to determine if the data is highly likely to be nucleotide, or not.
    if (notDna == false && notRna == true && notAa == true && notStd == true)
        return "dna";
    else if (notDna == true && notRna == false && notAa == true && notStd == true)
        return "rna";
    else if (notDna == true && notRna == true && notAa == false && notStd == true)
        return "protein";
    else if (notDna == true && notRna == true && notAa == true && notStd == false)
        return "standard";
    else if (notDna == false && notRna == true && notAa == false && notStd == true)
        {
        if ( (double)nucCount / s.size() > 0.8 )
            return "dna";
        else
            return "protein";
        }
    else if (notDna == true && notRna == false && notAa == false && notStd == true)
        {
        if ( (double)nucCount / (s.size()-nMissing) > 0.8 )
            return "rna";
        else
            return "protein";
        }

    return "";
}


/** Try to determine if the file is likely to be in Fasta format */
bool Func_readAlignment::isFastaFile(std::string& fn, std::string& dType) {

    // open file
	std::ifstream fStrm;
    fStrm.open(fn.c_str(), ios::in);

    // read the file token-by-token looking for Fasta things
    int ch = fStrm.get();
    fStrm.unget();
    std::string word = "";
    std::string seqStr = "";
    int wordNum = 0, lineNum = 0, lastCarotLine = -100;
    int numSequences = 0;
    while (ch != EOF)
        {
        word = "";
        fStrm >> word;
            
        if (wordNum == 0 && word[0] == '>')
            {
            if (lineNum - lastCarotLine > 1)
                {
                lastCarotLine = lineNum;
                numSequences++;
                }
            else
                return false;
            }
        else if (wordNum == 0 && word[0] == ';')
            {
            // comment
            }
        else if (lineNum > 0 && word[0] != '>' && word[0] != ';')
            seqStr += word;
            
        //std::cout << word << " (" << lineNum << " " << wordNum << ")" << std::endl;
        wordNum++;
        ch = fStrm.get();
        if (ch == '\n' || ch == '\r' || ch == EOF)
            {
            lineNum++;
            wordNum = 0;
            //std::cout << "EOL" << std::endl;
            }
        } 

    // close file
    fStrm.close();

    if (numSequences < 1)
        return false;
        
    dType = intuitDataType(seqStr);
        
    return true;
}

/** Try to determine if the file is likely to be in Nexus format. All we do is check for the presence of a few 
    key words that appear in pretty much every Nexus file. */
bool Func_readAlignment::isNexusFile(std::string& fn) {

    // open file
	std::ifstream fStrm;
    fStrm.open(fn.c_str(), ios::in);

    // read the file token-by-token looking for NEXUS things
    bool foundNexus = false, foundBegin = false, foundData = false, foundDimensions = false, foundMatrix = false, foundFormat = false;
    int ch;
    std::string word = "";
    do
        {
        fStrm >> word;
        StringUtilities::toLower( word );
            
        // check if the token matches any key words we expect to see in a NEXUS file
        if (word == "#nexus")
            foundNexus = true;
        else if (word == "begin")
            foundBegin = true;
        else if (word == "data;")
            foundData = true;
        else if (word == "dimensions")
            foundDimensions = true;
        else if (word == "matrix")
            foundMatrix = true;
        else if (word == "format")
            foundFormat = true;
        //std::cout << word << std::endl;
        } while ((ch = fStrm.get()) != EOF);

    // close file
    fStrm.close();

    // check how many of the elements we were expecting we found
    int numElementsFound = 0;
    if (foundNexus == true)
        numElementsFound++;
    if (foundBegin == true)
        numElementsFound++;
    if (foundData == true)
        numElementsFound++;
    if (foundDimensions == true)
        numElementsFound++;
    if (foundMatrix == true)
        numElementsFound++;
    if (foundFormat == true)
        numElementsFound++;
                
    // return true or false depending upon how many of the elements we were expecting that we found
    if (numElementsFound >= 5)
        return true;

    return false;
}


/** Determine if the string s represents a number */
bool Func_readAlignment::isNumber(std::string& s) {

    for (size_t i=0; i<s.size(); i++)
        {
        if (!isdigit(s[i]))
            return false;
        }
    return true;
}


/** Try to determine if the file is likely to be in Phylip format */
bool Func_readAlignment::isPhylipFile(std::string& fn, std::string& dType, bool& isInterleaved) {

    // open file
	std::ifstream fStrm;
    fStrm.open(fn.c_str(), ios::in);
    std::string seqStr = "";

    // read the file token-by-token looking for NEXUS things
    bool foundNumTaxa = false, foundNumChar = false;
    int numTaxa = 0;
    std::vector<std::string> taxonNames;
    int ch = fStrm.get();
    fStrm.unget();
    std::string word = "";
    int wordNum = 0, lineNum = 0;
    while (ch != EOF)
        {
        word = "";
        fStrm >> word;
        StringUtilities::toLower( word );
            
        if (lineNum == 0 && wordNum == 0 && isNumber(word) == true)
            {
            std::istringstream buf(word);
            buf >> numTaxa;
            foundNumTaxa = true;
            }
        else if (lineNum == 0 && wordNum == 1 && isNumber(word) == true)
            foundNumChar = true;
        else if (lineNum > 0 && wordNum == 0 && word != "")
            taxonNames.push_back( word );
        else if (lineNum > 0 && wordNum > 0)
            seqStr += word;
            
        //std::cout << word << " (" << lineNum << " " << wordNum << ")" << std::endl;
        wordNum++;
        ch = fStrm.get();
        if (ch == '\n' || ch == '\r' || ch == EOF)
            {
            lineNum++;
            wordNum = 0;
            //std::cout << "EOL" << std::endl;
            }
        } 

    // close file
    fStrm.close();
    
    if (foundNumTaxa == true && foundNumChar == true)
        {
        if (taxonNames.size() == 0)
            return false;
        if (taxonNames.size() % numTaxa != 0)
            return false;
            
        if (taxonNames.size() != numTaxa)
            isInterleaved = true;
        dType = intuitDataType(seqStr);
        return true;
        }

    return false;
}
