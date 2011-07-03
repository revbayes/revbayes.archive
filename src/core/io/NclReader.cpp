/**
 * @file
 * This file contains the declaration of NclReader, which is
 * a singleton class that interfaces with NCL to read data
 * in Fasta, Nexus, and Phylip formats.
 *
 * @brief Declaration of NclReader
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#include "AminoAcidState.h"
#include "CharacterContinuous.h"
#include "CharacterMatrix.h"
#include "DnaState.h"
#include "NclReader.h"
#include "RbNames.h"
#include "RnaState.h"
#include "StandardState.h"
#include "StringUtilities.h"
#include "Tree.h"
#include "TreeNode.h"
#include "TreeSimple.h"
#include "UserInterface.h"
#include "VectorAminoAcidStates.h"
#include "VectorCharacterContinuous.h"
#include "VectorDnaStates.h"
#include "VectorRnaStates.h"
#include "VectorStandardStates.h"



/** Constructs a tree from NCL */
void NclReader::constructTreefromNclRecursively(TreeNode* tn, const NxsSimpleNode* tnNcl) {

    // get the children
    std::vector<NxsSimpleNode*> children = tnNcl->GetChildren();
    
    // iterate over all children
    for (std::vector<NxsSimpleNode*>::iterator it = children.begin(); it!=children.end(); it++) 
        {
        // create a new tree node with given name
        const std::string& name = (*it)->GetName(); 
        TreeNode* child = new TreeNode(name);
        
        // add new node as child
        tn->addChild(child);
        
        // add parent to new node
        child->setParent(tn);
        
        // recursive call for the child to parse the tree
        constructTreefromNclRecursively(child, *it);
        }
}


/** Reads the blocks stored by NCL and converts them to RevBayes character matrices */
std::vector<CharacterMatrix*> NclReader::convertFromNcl(std::vector<std::string>& fnv) {
    
	std::vector<CharacterMatrix*> cmv;
    
	int numTaxaBlocks = nexusReader.GetNumTaxaBlocks();
	int k = 0;
	for (int tBlck=0; tBlck<numTaxaBlocks; tBlck++)
        {
		NxsTaxaBlock* taxaBlock = nexusReader.GetTaxaBlock(tBlck);
		std::string taxaBlockTitle = taxaBlock->GetTitle();
		const unsigned nCharBlocks = nexusReader.GetNumCharactersBlocks(taxaBlock);
		for (unsigned cBlck=0; cBlck<nCharBlocks; cBlck++)
            {
			CharacterMatrix* m = NULL;
			NxsCharactersBlock* charBlock = nexusReader.GetCharactersBlock(taxaBlock, cBlck);
			std::string charBlockTitle = taxaBlock->GetTitle();
			int dt = charBlock->GetDataType();
			if (dt == NxsCharactersBlock::dna || dt == NxsCharactersBlock::nucleotide)
                {
                m = createDnaMatrix(charBlock);
                }
			else if (dt == NxsCharactersBlock::rna)
                {
                m = createRnaMatrix(charBlock);
                }
			else if (dt == NxsCharactersBlock::protein)
                {
                m = createAminoAcidMatrix(charBlock);
                }
			else if (dt == NxsCharactersBlock::standard)
                {
                m = createStandardMatrix(charBlock);
                }
			else if (dt == NxsCharactersBlock::continuous)
                {
                m = createContinuousMatrix(charBlock);
                }
			else if (dt == NxsCharactersBlock::mixed)
                {
                addWarning("Mixed data types are not allowed");
                }
            else 
                {
                addWarning("Unknown data type");
                }
                        
			if (m != NULL)
                {
                m->setFileName( fnv[k++] );
				cmv.push_back( m );
                }
            }
        }
    
	return cmv;
}


/** Create an object to hold amino acid data */
CharacterMatrix* NclReader::createAminoAcidMatrix(NxsCharactersBlock* charblock) {
 
    // check that the character block is of the correct type
	if (charblock->GetDataType() != NxsCharactersBlock::protein)
        return NULL;
    
    // get the set of characters (and the number of taxa)
    NxsUnsignedSet charset;
    for (int i=0; i<charblock->GetNumChar(); i++)
        charset.insert(i);
	int numOrigTaxa = charblock->GetNTax();
    
	// get the set of excluded characters
	NxsUnsignedSet excluded = charblock->GetExcludedIndexSet();
    
    // instantiate the character matrix
	CharacterMatrix* cMat = new CharacterMatrix();
    cMat->setDataType("Amino Acid");
    
	// read in the data, including taxon names
	for (int origTaxIndex=0; origTaxIndex<numOrigTaxa; origTaxIndex++) 
        {
        // add the taxon name
        NxsString tLabel = charblock->GetTaxonLabel(origTaxIndex);
        std::string tn = NxsString::GetEscaped(tLabel).c_str();
        cMat->addTaxonName(tn);
        
        // check if the taxon is excluded
		if ( !charblock->IsActiveTaxon(origTaxIndex) )
            cMat->excludeTaxon(origTaxIndex);
        
        // allocate a vector of Standard states
        VectorAminoAcidStates* dataVec = new VectorAminoAcidStates();

        for (NxsUnsignedSet::const_iterator cit = charset.begin(); cit != charset.end();cit++)
            {	
            AminoAcidState aaState;
            if (charblock->IsGapState(origTaxIndex, *cit) == true) 
                aaState.setState('n');
            else if (charblock->IsMissingState(origTaxIndex, *cit) == true) 
                aaState.setState('n');
            else
                {
                int nStates = charblock->GetNumStates(origTaxIndex, *cit);
                aaState.setState( charblock->GetState(origTaxIndex, *cit, 0) );
                for(int s=1; s<nStates; s++)
                    aaState.addState( charblock->GetState(origTaxIndex, *cit, s) );
                }
            dataVec->push_back( aaState );
                
            // check if the site is excluded
            NxsUnsignedSet::iterator it = excluded.find(*cit);
            if (it != excluded.end())
                cMat->excludeCharacter(*cit);
            }

        // add the vector of nucleotide sequences for this taxon to the character matrix
        cMat->addTaxonObservations( dataVec );
        }
    
    return cMat;
}


/** Create an object to hold continuous data */
CharacterMatrix* NclReader::createContinuousMatrix(NxsCharactersBlock* charblock) {
 
    // check that the character block is of the correct type
	if (charblock->GetDataType() != NxsCharactersBlock::continuous)
        return NULL;
    
    // get the set of characters (and the number of taxa)
    NxsUnsignedSet charset;
    for (int i=0; i<charblock->GetNumChar(); i++)
        charset.insert(i);
	int numOrigTaxa = charblock->GetNTax();
    
	// get the set of excluded characters
	NxsUnsignedSet excluded = charblock->GetExcludedIndexSet();
    
    // instantiate the character matrix
	CharacterMatrix* cMat = new CharacterMatrix();
    cMat->setDataType("Continuous");
    
	// read in the data, including taxon names
	for (int origTaxIndex=0; origTaxIndex<numOrigTaxa; origTaxIndex++) 
        {
        // add the taxon name
        NxsString tLabel = charblock->GetTaxonLabel(origTaxIndex);
        std::string tn = NxsString::GetEscaped(tLabel).c_str();
        cMat->addTaxonName(tn);
        
        // check if the taxon is excluded
		if ( !charblock->IsActiveTaxon(origTaxIndex) )
            cMat->excludeTaxon(origTaxIndex);
        
        // allocate a vector of Standard states
        VectorCharacterContinuous* dataVec = new VectorCharacterContinuous();

        // add the real-valued observation
        for (NxsUnsignedSet::const_iterator cit = charset.begin(); cit != charset.end();cit++)
            {	
            CharacterContinuous contObs;
            const std::vector<double>& x = charblock->GetContinuousValues( origTaxIndex, *cit, std::string("AVERAGE") );
            contObs.setValue(x[0]);
            dataVec->push_back( contObs );
            
            // check if the character is excluded
            NxsUnsignedSet::iterator it = excluded.find(*cit);
            if (it != excluded.end())
                cMat->excludeCharacter(*cit);
            }

        // add the vector of nucleotide sequences for this taxon to the character matrix
        cMat->addTaxonObservations( dataVec );
        }
    
    return cMat;
}


/** Create an object to hold DNA data */
CharacterMatrix* NclReader::createDnaMatrix(NxsCharactersBlock* charblock) {

    // check that the character block is of the correct type
	if ( charblock->GetDataType() != NxsCharactersBlock::dna )
        return NULL;

    // get the set of characters (and the number of taxa)
    NxsUnsignedSet charset;
    for (int i=0; i<charblock->GetNumChar(); i++)
        charset.insert(i);
	int numOrigTaxa = charblock->GetNTax();

	// get the set of excluded characters
	NxsUnsignedSet excluded = charblock->GetExcludedIndexSet();

    // instantiate the character matrix
	CharacterMatrix* cMat = new CharacterMatrix();
    cMat->setDataType("DNA");
    
	// read in the data, including taxon names
	for (int origTaxIndex=0; origTaxIndex<numOrigTaxa; origTaxIndex++) 
        {
        // add the taxon name
        NxsString tLabel = charblock->GetTaxonLabel(origTaxIndex);
        std::string tn = NxsString::GetEscaped(tLabel).c_str();
        cMat->addTaxonName(tn);
        
        // check if the taxon is excluded
		if ( !charblock->IsActiveTaxon(origTaxIndex) )
            cMat->excludeTaxon(origTaxIndex);
        
        // allocate a vector of DNA states
        VectorDnaStates* dataVec = new VectorDnaStates();
        
        // add the sequence information for the sequence associated with the taxon
        for (NxsUnsignedSet::iterator cit = charset.begin(); cit != charset.end(); cit++)
            {	
            // add the character state to the matrix
            DnaState dnaState;
            if ( charblock->IsGapState(origTaxIndex, *cit) == true ) 
                dnaState.setState('N');
            else if (charblock->IsMissingState(origTaxIndex, *cit) == true) 
                dnaState.setState('N');
            else
                {
                dnaState.setState( charblock->GetState(origTaxIndex, *cit, 0) );                
                for (int s=1; s<charblock->GetNumStates(origTaxIndex, *cit); s++)
                    dnaState.addState( charblock->GetState(origTaxIndex, *cit, s) );
                }
            dataVec->push_back( dnaState );

            // check if the site is excluded
            NxsUnsignedSet::iterator it = excluded.find(*cit);
            if (it != excluded.end())
                cMat->excludeCharacter(*cit);
            }
            
        // add the vector of nucleotide sequences for this taxon to the character matrix
        cMat->addTaxonObservations( dataVec );
        }

    return cMat;
}


/** Create an object to hold RNA data */
CharacterMatrix* NclReader::createRnaMatrix(NxsCharactersBlock* charblock) {

    // check that the character block is of the correct type
	if ( charblock->GetDataType() != NxsCharactersBlock::rna )
        return NULL;

    // get the set of characters (and the number of taxa)
    NxsUnsignedSet charset;
    for (int i=0; i<charblock->GetNumChar(); i++)
        charset.insert(i);
	int numOrigTaxa = charblock->GetNTax();

	// get the set of excluded characters
	NxsUnsignedSet excluded = charblock->GetExcludedIndexSet();

    // instantiate the character matrix
	CharacterMatrix* cMat = new CharacterMatrix();
    cMat->setDataType("RNA");
    
	// read in the data, including taxon names
	for (int origTaxIndex=0; origTaxIndex<numOrigTaxa; origTaxIndex++) 
        {
        // add the taxon name
        NxsString tLabel = charblock->GetTaxonLabel(origTaxIndex);
        std::string tn = NxsString::GetEscaped(tLabel).c_str();
        cMat->addTaxonName(tn);
        
        // check if the taxon is excluded
		if ( !charblock->IsActiveTaxon(origTaxIndex) )
            cMat->excludeTaxon(origTaxIndex);
        
        // allocate a vector of RNA states
        VectorRnaStates* dataVec = new VectorRnaStates();
        
        // add the sequence information for the sequence associated with the taxon
        for (NxsUnsignedSet::iterator cit = charset.begin(); cit != charset.end(); cit++)
            {	
            // add the character state to the matrix
            RnaState rnaState;
            if ( charblock->IsGapState(origTaxIndex, *cit) == true ) 
                rnaState.setState('N');
            else if (charblock->IsMissingState(origTaxIndex, *cit) == true) 
                rnaState.setState('N');
            else
                {
                rnaState.setState( charblock->GetState(origTaxIndex, *cit, 0) );                
                for (int s=1; s<charblock->GetNumStates(origTaxIndex, *cit); s++)
                    rnaState.addState( charblock->GetState(origTaxIndex, *cit, s) );
                }
            dataVec->push_back( rnaState );

            // check if the site is excluded
            NxsUnsignedSet::iterator it = excluded.find(*cit);
            if (it != excluded.end())
                cMat->excludeCharacter(*cit);
            }
            
        // add the vector of nucleotide sequences for this taxon to the character matrix
        cMat->addTaxonObservations( dataVec );
        }

    return cMat;
}


/** Create an object to hold standard data */
CharacterMatrix* NclReader::createStandardMatrix(NxsCharactersBlock* charblock) {
 
    // check that the character block is of the correct type
	if (charblock->GetDataType() != NxsCharactersBlock::standard)
        return NULL;
    
    // get the set of characters (and the number of taxa)
    NxsUnsignedSet charset;
    for (int i=0; i<charblock->GetNumChar(); i++)
        charset.insert(i);
	int numOrigTaxa = charblock->GetNTax();
    
	// get the set of excluded characters
	NxsUnsignedSet excluded = charblock->GetExcludedIndexSet();
    
    // get the number of states
    const NxsDiscreteDatatypeMapper* mapper = charblock->GetDatatypeMapperForChar(0);
    std::string sym = charblock->GetSymbols();
    int nStates = mapper->GetNumStates();
    if (nStates > 10)
        return NULL;
    
    // instantiate the character matrix
	CharacterMatrix* cMat = new CharacterMatrix();
    cMat->setDataType("Standard");
    
	// read in the data, including taxon names
	for (int origTaxIndex=0; origTaxIndex<numOrigTaxa; origTaxIndex++) 
        {
        // add the taxon name
        NxsString tLabel = charblock->GetTaxonLabel(origTaxIndex);
        std::string tn = NxsString::GetEscaped(tLabel).c_str();
        cMat->addTaxonName(tn);
        
        // check if the taxon is excluded
		if ( !charblock->IsActiveTaxon(origTaxIndex) )
            cMat->excludeTaxon(origTaxIndex);
        
        // allocate a vector of Standard states
        VectorStandardStates* dataVec = new VectorStandardStates();
        
        // add the character information for the data associated with the taxon
        for (NxsUnsignedSet::iterator cit = charset.begin(); cit != charset.end(); cit++)
            {	
            // add the character state to the matrix
            StandardState stdState(sym);
            for(int s=0; s<charblock->GetNumStates(origTaxIndex, *cit); s++)
                {
                char c = charblock->GetState(origTaxIndex, *cit, s);
                for (int i=0; i<nStates; i++)
                    {
                    if ( sym[i] == c )
                        stdState.addState(i);
                        //cMat->setState(origTaxIndex, *cit, i);
                    }
                }
            dataVec->push_back( stdState );

            // check if the site is excluded
            NxsUnsignedSet::iterator it = excluded.find(*cit);
            if (it != excluded.end())
                cMat->excludeCharacter(*cit);
            }

        // add the vector of nucleotide sequences for this taxon to the character matrix
        cMat->addTaxonObservations( dataVec );
        }
        
    return cMat;
}


/** Returns whether a file exists */
bool NclReader::fileExists(const char* fn) const {
    
	bool exists = false;
	FILE *fp = fopen(fn, "r");
	if (fp != NULL)
        {
		fclose(fp);
		exists = true;
        }
	return exists;
}


/** Get a reference to this singleton object */
NclReader& NclReader::getInstance(void) {
    
	static NclReader rb;
	return rb;
}


/** Attempt to determine the type of data this is being read */
std::string NclReader::intuitDataType(std::string& s) {

    // set up some strings containing the valid states for the different data types we are interested
    // in distiguishing
    static std::string dnaStates = "acgtmgrsvwyhkdbn-.?";
    static std::string rnaStates = "acgumgrsvwyhkdbn-.?";
    static std::string aaStates  = "arndcqeghilkmfpstwyv-.?";
    static std::string stdStates = "0123456789n-.?abcdefghijklmnopqrstuvwxyz()";
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
bool NclReader::isFastaFile(std::string& fn, std::string& dType) {

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
bool NclReader::isNexusFile(std::string& fn) {

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


/** Try to determine if the file is likely to be in Phylip format */
bool NclReader::isPhylipFile(std::string& fn, std::string& dType, bool& isInterleaved) {

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
            
        if (lineNum == 0 && wordNum == 0 && StringUtilities::isNumber(word) == true)
            {
            std::istringstream buf(word);
            buf >> numTaxa;
            foundNumTaxa = true;
            }
        else if (lineNum == 0 && wordNum == 1 && StringUtilities::isNumber(word) == true)
            foundNumChar = true;
        else if (lineNum > 0 && wordNum == 0 && word != "")
            taxonNames.push_back( word );
        else if (lineNum > 0 && wordNum > 0)
            seqStr += word;
            
        wordNum++;
        ch = fStrm.get();
        if (ch == '\n' || ch == '\r' || ch == EOF)
            {
            lineNum++;
            wordNum = 0;
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

/** Read a list of file names contained in a map (with file format info too) */
std::vector<CharacterMatrix*> NclReader::readMatrices(const std::map<std::string,std::string>& fileMap) {

    // allocate a vector of matrices
    std::vector<CharacterMatrix*> cmv;

    // We loop through the map that contains as the key the path and name of the file to be read and as the
    // value the data type. The data types are as follows: Nexus, Phylip+DNA/RNA/AA/Standard+Interleaved/Noninterleaved,
    // Fasta+DNA/RNA/AA.
    for (std::map<std::string,std::string>::const_iterator p = fileMap.begin(); p != fileMap.end(); p++)
        {
        // Check that the file exists. It is likely that this has been already checked during the formation of
        // the map that is passed into the function, but it never hurts to be safe...
        if ( !fileExists(p->first.c_str()) )	
            {
            addWarning("Data file not found");
            continue;
            }
            
        // Extract information on the file format from the value of the key/value pair. Note that we expect the 
        // fileFmt string to be in the format file_type|data_type|interleave_type with pipes ('|') separating
        // the format components. It might be better to make an object value in the key/value pair that contains
        // this information.
        std::vector<std::string> fileFmt;
        StringUtilities::stringSplit( p->second, "|", fileFmt );
        std::string ff = fileFmt[0];
        std::string dt = fileFmt[1];
        bool il = false;
        if ( fileFmt[2] == "interleaved" )
            il = true;

        // read the file
        std::vector<CharacterMatrix*> v = readMatrices( p->first.c_str(), ff, dt, il );
		if (v.size() > 0)
            {
			for (std::vector<CharacterMatrix*>::iterator m = v.begin(); m != v.end(); m++)
				cmv.push_back( (*m) );
            }
		nexusReader.ClearContent();
        }
        
    return cmv;
}


/** Read a list of file names contained in a vector of strings */
std::vector<CharacterMatrix*> NclReader::readMatrices(const std::vector<std::string> fn, const std::string fileFormat, const std::string dataType, const bool isInterleaved) {
    
	// instantiate a vector of matrices
	std::vector<CharacterMatrix*> cmv;

	// check that the files exist
    for (std::vector<std::string>::const_iterator f = fn.begin(); f != fn.end(); f++)
        {
        if ( !fileExists((*f).c_str()) )	
            {
            addWarning("Data file not found");
            return cmv;
            }
        }
        
    // read the data files
    for (std::vector<std::string>::const_iterator f = fn.begin(); f != fn.end(); f++)
        {
        std::vector<CharacterMatrix*> v = readMatrices( (*f).c_str(), fileFormat, dataType, isInterleaved );
		if (v.size() > 0)
            {
			for (std::vector<CharacterMatrix*>::iterator m = v.begin(); m != v.end(); m++)
				cmv.push_back( (*m) );
            }
		nexusReader.ClearContent();
        }
    
    return cmv;
}


/** Reads a single file using NCL */
std::vector<CharacterMatrix*> NclReader::readMatrices(const char* fileName, const std::string fileFormat, const std::string dataType, const bool isInterleaved) {
    
	// check that the file exists
	if ( !fileExists(fileName) )	
        {
        addWarning("Data file not found");
        std::vector<CharacterMatrix*> dummy;
        return dummy;
        }
	try
        {
		if (fileFormat == "nexus")
            {
			// NEXUS file format
			nexusReader.ReadFilepath(fileName, MultiFormatReader::NEXUS_FORMAT);
            }
		else if (fileFormat == "fasta")
            {
			// fasta file format
			if (dataType == "dna")
				nexusReader.ReadFilepath(fileName, MultiFormatReader::FASTA_DNA_FORMAT);
			else if (dataType == "rna")
				nexusReader.ReadFilepath(fileName, MultiFormatReader::FASTA_RNA_FORMAT);
			else if (dataType == "protein")
				nexusReader.ReadFilepath(fileName, MultiFormatReader::FASTA_AA_FORMAT);
            }
		else if (fileFormat == "phylip")
            {
			// phylip file format
			if (isInterleaved == false)
                {
				if (dataType == "dna")
					nexusReader.ReadFilepath(fileName, MultiFormatReader::RELAXED_PHYLIP_DNA_FORMAT);
				else if (dataType == "rna")
					nexusReader.ReadFilepath(fileName, MultiFormatReader::RELAXED_PHYLIP_RNA_FORMAT);
				else if (dataType == "protein")
					nexusReader.ReadFilepath(fileName, MultiFormatReader::RELAXED_PHYLIP_AA_FORMAT);
				else if (dataType == "standard")
					nexusReader.ReadFilepath(fileName, MultiFormatReader::RELAXED_PHYLIP_DISC_FORMAT);
                }
			else 
                {
				if (dataType == "dna")
					nexusReader.ReadFilepath(fileName, MultiFormatReader::INTERLEAVED_RELAXED_PHYLIP_DNA_FORMAT);
				else if (dataType == "rna")
					nexusReader.ReadFilepath(fileName, MultiFormatReader::INTERLEAVED_RELAXED_PHYLIP_RNA_FORMAT);
				else if (dataType == "protein")
					nexusReader.ReadFilepath(fileName, MultiFormatReader::INTERLEAVED_RELAXED_PHYLIP_AA_FORMAT);
				else if (dataType == "standard")
					nexusReader.ReadFilepath(fileName, MultiFormatReader::INTERLEAVED_RELAXED_PHYLIP_DISC_FORMAT);
                }
            }
        }
	catch(NxsException err)
        {
        std::string fns = fileName;
        addWarning("Nexus error in file \"" + StringUtilities::getLastPathComponent(fns) + "\"");
		//std::cout << pad << "Nexus Error: " << err.msg << " (" << err.pos << ", " << err.line << ", " << err.col << ")" << std::endl;
        std::vector<CharacterMatrix*> dummy;
		return dummy;
        }
    
	std::vector<std::string> fileNameVector;
	std::string str = fileName;
	fileNameVector.push_back( str );
	std::vector<CharacterMatrix*> cvm = convertFromNcl(fileNameVector);
	return cvm;
}


/** Read trees */
std::vector<Tree*>* NclReader::readTrees(const std::string fn, const std::string fileFormat) {
	
	// check that the file exist
    if ( !fileExists(fn.c_str()) ) 
        {
        addWarning("Data file not found");
        return NULL;
        }
    
	// allocate a vector of trees
	std::vector<Tree*>* trees = new std::vector<Tree*>();
	
    // TODO @Tracy: Why do we read a vector of trees, then copy every single tree into a new vector; instead of returning the vector straight away?!? (Sebastian)
    
    // read the data files
 	std::vector<Tree*>* f = readTrees( fn.c_str(), fileFormat);
	if (f != NULL) {
		for (std::vector<Tree*>::iterator m = f->begin(); m != f->end(); m++)
			trees->push_back( (*m) );
		delete f;
	}
	else 
		return NULL;
    
	nexusReader.ClearContent();
	
    return trees;
}


/** Read trees */
std::vector<Tree*>* NclReader::readTrees(const char* fileName, const std::string fileFormat) {
	
	// check that the file exists
	if ( !fileExists(fileName) ) 
        {
        addWarning("Data file not found");
        return NULL;
        }
	
	try 
        {
		if (fileFormat == "nexus") 
            {
			// NEXUS file format
			nexusReader.ReadFilepath(fileName, MultiFormatReader::NEXUS_FORMAT);
            }
		else if (fileFormat == "phylip") 
            {
			// phylip file format with long taxon names
			nexusReader.ReadFilepath(fileName, MultiFormatReader::RELAXED_PHYLIP_TREE_FORMAT);
            }
        }
	catch(NxsException err) 
        {
		std::cout << "Nexus Error: " << err.msg << " (" << err.pos << ", " << err.line << ", " << err.col << ")" << std::endl;
		return NULL;
        }
	
	std::vector<std::string> fileNameVector;
	std::string str = fileName;
	fileNameVector.push_back( str );
	
	std::vector<Tree*>* cvm = convertTreesFromNcl();
	return cvm;
}


/** Converts trees stored by NCL into RevBayes formatted trees */
std::vector<Tree*>* NclReader::convertTreesFromNcl(void) {
	
	const unsigned nTaxaBlocks = nexusReader.GetNumTaxaBlocks();
	std::vector<Tree*>* rbTreesFromFile;
	for (unsigned t = 0; t < nTaxaBlocks; ++t) 
        {
		const NxsTaxaBlock * tb = nexusReader.GetTaxaBlock(t);
		const unsigned nTreesBlocks = nexusReader.GetNumTreesBlocks(tb);
		if (nTreesBlocks == 0)
			continue;
        
		for (unsigned i = 0; i < nTreesBlocks; ++i) 
            {
			const NxsTreesBlock * trb = nexusReader.GetTreesBlock(tb, i);
			trb->ProcessAllTrees();
			for (unsigned j = 0; j < trb->GetNumTrees(); ++j) 
                {
				const NxsFullTreeDescription & ftd = trb->GetFullTreeDescription(j);
				NxsSimpleTree tree(ftd, -1, -1.0);
				Tree *rbTree = translateNclSimpleTreeToTree(tree);
                //! @todo Tracy: Make sure rbTreesFromFile is properly initialized before being used -- Fredrik
                rbTreesFromFile = new std::vector<Tree*>(); // Temporary fix making the compiler happy
				rbTreesFromFile->push_back( rbTree );
                }
            }
        }
    
	return rbTreesFromFile;
}


/** Translate a single NCL tree into a RevBayes tree */
Tree* NclReader::translateNclSimpleTreeToTree(NxsSimpleTree& nTree) {
    
    // get the root from the ncl tree
    const NxsSimpleNode* rn = nTree.GetRootConst();
    
    // create a new tree root node
    const std::string& name = rn->GetName();
    TreeNode* root = new TreeNode(name);
    
	// construct tree recursively
    constructTreefromNclRecursively(root, rn);
    
    // create a new simple tree
    Tree* myTreeFromNcl = new TreeSimple(root);
    
	return myTreeFromNcl;
}
