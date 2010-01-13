#include "RbCharacterMatrix.h"
#include "RbBitset.h"
#include "RbNames.h"
#include "StringVector.h"
#include "RbException.h"
#include <iomanip>
#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>


RbCharacterMatrix::RbCharacterMatrix(std::string fileName, std::string fileType, int ns) {

	charMatrix = NULL;
	numTaxa    = 0;
	numChar    = 0;
	numStates  = ns;

	if (fileType == RbNames::AlignmentFileType::nexus)
		{
		// not supported yet
		}
	else if (fileType == RbNames::AlignmentFileType::phylip)
		{
		readPhylip(fileName);
		}
	else if (fileType == RbNames::AlignmentFileType::fasta)
		{
		readFasta(fileName);
		}
	else
		{
		throw RbException("Unknown file type when reading alignment");
		}

}

RbCharacterMatrix::RbCharacterMatrix(const RbCharacterMatrix& a) {

	numTaxa = a.numTaxa;
	numChar = a.numChar;
	numStates = a.numStates;
	taxonNames = a.taxonNames;
	allocateCharMatrix(numTaxa, numChar, numStates);
	for (int i=0; i<numTaxa; i++)
		for (int j=0; j<numChar; j++)
			*charMatrix[i][j] = *a.charMatrix[i][j];
}

void RbCharacterMatrix::allocateCharMatrix(int nt, int nc, int ns) {

	freeCharMatrix();
	charMatrix = new RbBitset**[nt];
	charMatrix[0] = new RbBitset*[nt * nc];
	for (int i=1; i<nt; i++)
		charMatrix[i] = charMatrix[i-1] + nc;
	for (int i=0; i<nt; i++)
		for (int j=0; j<nc; j++)
			charMatrix[i][j] = new RbBitset(ns);
}

void RbCharacterMatrix::freeCharMatrix(void) {

	if (charMatrix != NULL)
		{
		for (int i=0; i<numTaxa; i++)
			for (int j=0; j<numChar; j++)
				delete charMatrix[i][j];
		delete [] charMatrix[0];
		delete [] charMatrix;
		}
	charMatrix = NULL;
}

/** Get class vector describing type of object */
const StringVector& RbCharacterMatrix::getClass() const {

    static StringVector rbClass = StringVector(RbNames::CharacterMatrix::name) + RbComplex::getClass();
    return rbClass;
}

void RbCharacterMatrix::readFasta(std::string fileName) {

}

void RbCharacterMatrix::readPhylip(std::string fileName) {

	/* open the file */

	std::ifstream seqStream( fileName.c_str() );
	if (!seqStream) 
		{
		std::cerr << "Cannot open file \"" + fileName + "\"" << std::endl;
		exit(1);
		}
		
	std::string linestring = "";
	int line = 0;
	std::string theSequence = "";
	int taxonNum = 0;
	numTaxa = numChar = 0;
	while( getline(seqStream, linestring).good() )
		{
		std::istringstream linestream(linestring);
		int ch;
		std::string word = "";
		int wordNum = 0;
		int siteNum = 0;
		std::string cmdString = "";
		do
			{
			word = "";
			linestream >> word;
			wordNum++;
			//cout << "word(" << wordNum << ") = " << word << endl;
			if (line == 0)
				{
				/* read the number of taxa/chars from the first line */
				int x;
				std::istringstream buf(word);
				buf >> x;
				if (wordNum == 1)
					numTaxa = x;
				else
					numChar = x;
				if (numTaxa > 0 && numChar > 0 && charMatrix == NULL)
					{	
					allocateCharMatrix(numTaxa, numChar, numStates);
					}
				}
			else
				{
				if (wordNum == 1)
					{
					taxonNames.push_back(word);
					taxonNum++;
					}
				else
					{
					for (int i=0; i<word.length(); i++)
						{
						char site = word.at(i);
						initState( site, charMatrix[taxonNum-1][siteNum] );
						siteNum++;
						}
					}
				}
			} while ( (ch=linestream.get()) != EOF );
			
		//cout << linestring << endl;
		line++;
		}	
	
	/* close the file */
	seqStream.close();

}

void RbCharacterMatrix::print(std::ostream& o) {

	for (int i=0; i<numTaxa; i++)
		o << std::setw(6) << i << " -- " << taxonNames[i] << std::endl;
	o << std::endl;
	for (int j=0; j<numChar; j++)
		{
		o << std::setw(6) << j << " -- ";
		for (int i=0; i<numTaxa; i++)
			{
			o << std::setw(4) << convertToChar(charMatrix[i][j]);
			}
		o << std::endl;
		}
}
