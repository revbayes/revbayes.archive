#include <iomanip>
#include <iostream>
#include <vector>
#include "CharacterMatrixDiscrete.h"
#include "RbException.h"
#include "RbNames.h"
#include "VectorString.h"


#include <sstream>

CharacterMatrixDiscrete::CharacterMatrixDiscrete(const CharacterMatrixDiscrete& cmc) {
    
	dataType = cmc.dataType;
	isDiscrete = cmc.isDiscrete;
    numTaxa = cmc.numTaxa;
    numCharacters = cmc.numCharacters;
    
    allocateMatrix();
    for (int i=0; i<numTaxa; i++) {
        for (int j=0; j<numCharacters; j++) {
            matrix[i][j] = cmc.matrix[i][j];
        }
    }
    
}

CharacterMatrixDiscrete::CharacterMatrixDiscrete(int nt, int nc, std::string dt) : CharacterMatrix(nt, nc) {

    dataType = dt;
	matrix = NULL;
	isDiscrete = true;
	if (dataType == "dna" || dataType == "rna")
		createNucleotideMatrix();
	else if (dataType == "amino acid")
		createAminoAcidMatrix();
	else if (dataType == "standard")
		createStandardMatrix(2);
}

CharacterMatrixDiscrete::CharacterMatrixDiscrete(int nt, int nc, int ns, std::string dt) : CharacterMatrix(nt, nc) {

    dataType = dt;
	matrix = NULL;
	isDiscrete = true;
	if (dataType == "dna" || dataType == "rna")
		createNucleotideMatrix();
	else if (dataType == "amino acid")
		createAminoAcidMatrix();
	else if (dataType == "standard")
		createStandardMatrix(ns);
}

CharacterMatrixDiscrete::~CharacterMatrixDiscrete(void) {

	freeMatrix();
}

void CharacterMatrixDiscrete::allocateMatrix(void) {

	if (matrix != NULL)
		freeMatrix();
	if (numTaxa > 0 && numCharacters > 0)
		{
		matrix = new std::vector<bool>*[numTaxa];
		matrix[0] = new std::vector<bool>[numTaxa*numCharacters];
		for (int i=1; i<numTaxa; i++)
			matrix[i] = matrix[i-1] + numCharacters;
        isTaxonExcluded.resize(numTaxa);
        isCharacterExcluded.resize(numCharacters);
        for (int i=0; i<numTaxa; i++)
            isTaxonExcluded[i] = false;
        for (int i=0; i<numCharacters; i++)
            isCharacterExcluded[i] = false;
		}
}

/** Overloaded container clear function */
void CharacterMatrixDiscrete::clear( void ) {
    
    freeMatrix();
}


/** Clone function */
CharacterMatrixDiscrete* CharacterMatrixDiscrete::clone(void) const {
    
    return new CharacterMatrixDiscrete(*this);
}


/** Get class vector describing type of object */
const VectorString& CharacterMatrixDiscrete::getClass(void) const {
    
    static VectorString rbClass = VectorString(CharacterMatrixDiscrete_name) + CharacterMatrix::getClass();
    return rbClass;
}


void CharacterMatrixDiscrete::freeMatrix(void) {

	if (matrix != NULL)
		{
        isTaxonExcluded.clear();
        isCharacterExcluded.clear();
		delete [] matrix[0];
		delete [] matrix;
		}
	matrix = NULL;
}

bool CharacterMatrixDiscrete::checkMatrix(void) {

	for (int i=0; i<numTaxa; i++)
		{
		for (int j=0; j<numCharacters; j++)
			{
            std::vector<bool>& x = matrix[i][j];
            int nOn = 0;
            for (int s=0; s<x.size(); s++)
                {
                if (x[s] == true)
                    nOn++;
                }
            if (nOn == 0)
                return false;
			}
		}
    return true;
}

void CharacterMatrixDiscrete::createNucleotideMatrix(void) {
	
	allocateMatrix();
	numStates = 4;
	for (int i=0; i<numTaxa; i++)
		{
		for (int j=0; j<numCharacters; j++)
			{
			matrix[i][j].resize(4);
			for (int k=0; k<matrix[i][j].size(); k++)
				matrix[i][j][i] = false;
			}
		}
}

void CharacterMatrixDiscrete::createAminoAcidMatrix(void) {

	allocateMatrix();
	numStates = 20;
	for (int i=0; i<numTaxa; i++)
		{
		for (int j=0; j<numCharacters; j++)
			{
			matrix[i][j].resize(20);
			for (int k=0; k<matrix[i][j].size(); k++)
				matrix[i][j][i] = false;
			}
		}
}

void CharacterMatrixDiscrete::createStandardMatrix(int ns) {

	allocateMatrix();
	numStates = ns;
	for (int i=0; i<numTaxa; i++)
		{
		for (int j=0; j<numCharacters; j++)
			{
			matrix[i][j].resize(numStates);
			for (int k=0; k<matrix[i][j].size(); k++)
				matrix[i][j][i] = false;
			}
		}
}

char CharacterMatrixDiscrete::getState(int taxaId, int charId) {

	std::vector<bool>& x = matrix[taxaId][charId];
	if (dataType == "dna")
		{
		return getNucleotideChar(x);
		}
	else if (dataType == "rna")
		{
		char c = getNucleotideChar(x);
		if (c == 'T')
			c = 'U';
		return c;
		}
	else if (dataType == "amino acid")
		{
		int nOn = 0;
		for (int i=0; i<20; i++)
			{
			if (x[i] == true)
				nOn++;
			}
		if (nOn > 1)
			return 'N';
		char aaCode[20] = { 'A', 'R', 'N', 'D', 'C', 'Q', 'E', 'G', 'H', 'I', 'L', 'K', 'M', 'F', 'P', 'S', 'T', 'W', 'Y', 'V' };
		for (int i=0; i<20; i++)
			{
			if (x[i] == true)
				return aaCode[i];
			}
		}
	else if (dataType == "standard")
		{
		int nOn = 0;
		for (int i=0; i<x.size(); i++)
			if (x[i] == true)
				nOn++;
		if (nOn > 1)
			return 'N';
		for (int i=0; i<x.size(); i++)
			{
			if (x[i] == true)
				{
				char temp[5];
				sprintf(temp, "%d", i);
				return temp[0];
				}
			}
		}
	return '*';
}

unsigned CharacterMatrixDiscrete::getUnsignedValue(int taxaId, int charId) {

	std::vector<bool>& x = matrix[taxaId][charId];

	unsigned val = 0;
	for (int i=0; i<x.size(); i++)
		{
		if (x[i] == true)
			{
			unsigned mask = 1 << i ;
			val |= mask;
			}
		}
	return val;
}

bool CharacterMatrixDiscrete::getIsAmbig(int taxaId, int charId) {

	std::vector<bool>& x = matrix[taxaId][charId];

	int nOn = 0;
	for (int i=0; i<x.size(); i++)
		{
		if (x[i] == true)
			nOn++;
		}
	if (nOn > 1)
		return true;
	return false;
}

char CharacterMatrixDiscrete::getNucleotideChar(std::vector<bool>& x) {

	if (x[0] == true && x[1] == false && x[2] == false && x[3] == false)
		return 'A';
	else if (x[0] == false && x[1] == true && x[2] == false && x[3] == false)
		return 'C';
	else if (x[0] == true && x[1] == true && x[2] == false && x[3] == false)
		return 'M';
	else if (x[0] == false && x[1] == false && x[2] == true && x[3] == false)
		return 'G';
	else if (x[0] == true && x[1] == false && x[2] == true && x[3] == false)
		return 'R';
	else if (x[0] == false && x[1] == true && x[2] == true && x[3] == false)
		return 'S';
	else if (x[0] == true && x[1] == true && x[2] == true && x[3] == false)
		return 'V';
	else if (x[0] == false && x[1] == false && x[2] == false && x[3] == true)
		return 'T';
	else if (x[0] == true && x[1] == false && x[2] == false && x[3] == true)
		return 'W';
	else if (x[0] == false && x[1] == true && x[2] == false && x[3] == true)
		return 'Y';
	else if (x[0] == true && x[1] == true && x[2] == false && x[3] == true)
		return 'H';
	else if (x[0] == false && x[1] == false && x[2] == true && x[3] == true)
		return 'K';
	else if (x[0] == true && x[1] == false && x[2] == true && x[3] == true)
		return 'D';
	else if (x[0] == false && x[1] == true && x[2] == true && x[3] == true)
		return 'B';
	else if (x[0] == true && x[1] == true && x[2] == true && x[3] == true)
		return 'N';
	return '*';
}

int CharacterMatrixDiscrete::nucId(char nuc) {

	char		n;
	
	if (nuc == 'U' || nuc == 'u')
		n = 'T';
	else
		n = nuc;

	if (n == 'A' || n == 'a')
		{
		return 1;
		}
	else if (n == 'C' || n == 'c')
		{
		return 2;
		}
	else if (n == 'G' || n == 'g')
		{
		return 4;
		}
	else if (n == 'T' || n == 't')
		{
		return 8;
		}
	else if (n == 'R' || n == 'r')
		{
		return 5;
		}
	else if (n == 'Y' || n == 'y')
		{
		return 10;
		}
	else if (n == 'M' || n == 'm')
		{
		return 3;
		}
	else if (n == 'K' || n == 'k')
		{
		return 12;
		}
	else if (n == 'S' || n == 's')
		{
		return 6;
		}
	else if (n == 'W' || n == 'w')
		{
		return 9;
		}
	else if (n == 'H' || n == 'h')
		{
		return 11;
		}
	else if (n == 'B' || n == 'b')
		{
		return 14;
		}
	else if (n == 'V' || n == 'v')
		{
		return 7;
		}
	else if (n == 'D' || n == 'd')
		{
		return 13;
		}
	else if (n == 'N' || n == 'n')
		{
		return 15;
		}
	else if (n == '-')
		{
		return 15;
		}
	else if (n == '?')
		{
		return 15;
		}
	else
		{
		std::cerr << "Error: Unrecognized nucleotide " << n << std::endl;
		}
	return -1;
}

void CharacterMatrixDiscrete::printValue(std::ostream &o) const {

    for (int i=0; i<numTaxa; i++)
        {
        std::string tf;
        if (isTaxonExcluded[i] == true)
            tf = "excluded ";
        else 
            tf = "         ";
        std::cout << std::setw(4) << i << " " << tf << taxonNames[i] << std::endl;
        }
        
    std::cout << "Datatype = " << dataType << std::endl;
	for (int j=0; j<numCharacters; j++)
		{
        std::string tf;
        if (isCharacterExcluded[j] == true)
            tf = " excluded -- ";
        else 
            tf = "          -- ";
		std::cout << std::setw(5) << j << tf;
		for (int i=0; i<numTaxa; i++)
			{
			std::vector<bool>& x = matrix[i][j];
			for (int k=0; k<x.size(); k++)
				{
				if (x[k] == true)
					std::cout << "1";
				else 
					std::cout << "0";
				}
			std::cout << " ";
			}
		std::cout << std::endl;
		}
}

/** Complete info about object */
std::string CharacterMatrixDiscrete::richInfo(void) const {
    
    std::ostringstream o;
    o <<  "CharacterMatrixDiscrete: value = " ;
    printValue(o);
    
    return o.str();
}

void CharacterMatrixDiscrete::addAminoAcid(int taxaId, int charId, char charCode) {

    // get a reference to the cell in the character matrix
    std::vector<bool>& x = matrix[taxaId][charId];

    // missing character code
    if (charCode == '?')
        {
        for (int i=0; i<20; i++)
            x[i] = true;
        return;
        }
        
    // set the bool for the appropriate amino acid
	char aaCode[20] = { 'A', 'R', 'N', 'D', 'C', 'Q', 'E', 'G', 'H', 'I', 'L', 'K', 'M', 'F', 'P', 'S', 'T', 'W', 'Y', 'V' };
	for (int i=0; i<20; i++)
		{
		if ( charCode == aaCode[i] || charCode == tolower(aaCode[i]) )
			{
			x[i] = true;
			break;
			}
		}
}

void CharacterMatrixDiscrete::addNucleotide(int taxaId, int charId, char charCode) {

	int nucCode = nucId(charCode);
	std::vector<bool>& nuc = matrix[taxaId][charId];
	
	if (nucCode == 1)
		{
		nuc[0] = true;
		}
	else if (nucCode == 2)
		{
		nuc[1] = true;
		}
	else if (nucCode == 3)
		{
		nuc[0] = true;
		nuc[1] = true;
		}
	else if (nucCode == 4)
		{
		nuc[2] = true;
		}
	else if (nucCode == 5)
		{
		nuc[0] = true;
		nuc[2] = true;
		}
	else if (nucCode == 6)
		{
		nuc[1] = true;
		nuc[2] = true;
		}
	else if (nucCode == 7)
		{
		nuc[0] = true;
		nuc[1] = true;
		nuc[2] = true;
		}
	else if (nucCode == 8)
		{
		nuc[3] = true;
		}
	else if (nucCode == 9)
		{
		nuc[0] = true;
		nuc[3] = true;
		}
	else if (nucCode == 10)
		{
		nuc[1] = true;
		nuc[3] = true;
		}
	else if (nucCode == 11)
		{
		nuc[0] = true;
		nuc[1] = true;
		nuc[2] = false;
		nuc[3] = true;
		}
	else if (nucCode == 12)
		{
		nuc[2] = true;
		nuc[3] = true;
		}
	else if (nucCode == 13)
		{
		nuc[0] = true;
		nuc[2] = true;
		nuc[3] = true;
		}
	else if (nucCode == 14)
		{
		nuc[1] = true;
		nuc[2] = true;
		nuc[3] = true;
		}
	else if (nucCode == 15)
		{
		nuc[0] = true;
		nuc[1] = true;
		nuc[2] = true;
		nuc[3] = true;
		}
}

void CharacterMatrixDiscrete::setAminoAcid(int taxaId, int charId, char charCode) {

	std::vector<bool>& x = matrix[taxaId][charId];
	for (int i=0; i<x.size(); i++)
		x[i] = false;
	addAminoAcid(taxaId, charId, charCode);
}

void CharacterMatrixDiscrete::setNucleotide(int taxaId, int charId, char charCode) {

	int nucCode = nucId(charCode);
	std::vector<bool>& nuc = matrix[taxaId][charId];
	
	if (nucCode == 1)
		{
		nuc[0] = true;
		nuc[1] = false;
		nuc[2] = false;
		nuc[3] = false;
		}
	else if (nucCode == 2)
		{
		nuc[0] = false;
		nuc[1] = true;
		nuc[2] = false;
		nuc[3] = false;
		}
	else if (nucCode == 3)
		{
		nuc[0] = true;
		nuc[1] = true;
		nuc[2] = false;
		nuc[3] = false;
		}
	else if (nucCode == 4)
		{
		nuc[0] = false;
		nuc[1] = false;
		nuc[2] = true;
		nuc[3] = false;
		}
	else if (nucCode == 5)
		{
		nuc[0] = true;
		nuc[1] = false;
		nuc[2] = true;
		nuc[3] = false;
		}
	else if (nucCode == 6)
		{
		nuc[0] = false;
		nuc[1] = true;
		nuc[2] = true;
		nuc[3] = false;
		}
	else if (nucCode == 7)
		{
		nuc[0] = true;
		nuc[1] = true;
		nuc[2] = true;
		nuc[3] = false;
		}
	else if (nucCode == 8)
		{
		nuc[0] = false;
		nuc[1] = false;
		nuc[2] = false;
		nuc[3] = true;
		}
	else if (nucCode == 9)
		{
		nuc[0] = true;
		nuc[1] = false;
		nuc[2] = false;
		nuc[3] = true;
		}
	else if (nucCode == 10)
		{
		nuc[0] = false;
		nuc[1] = true;
		nuc[2] = false;
		nuc[3] = true;
		}
	else if (nucCode == 11)
		{
		nuc[0] = true;
		nuc[1] = true;
		nuc[2] = false;
		nuc[3] = true;
		}
	else if (nucCode == 12)
		{
		nuc[0] = false;
		nuc[1] = false;
		nuc[2] = true;
		nuc[3] = true;
		}
	else if (nucCode == 13)
		{
		nuc[0] = true;
		nuc[1] = false;
		nuc[2] = true;
		nuc[3] = true;
		}
	else if (nucCode == 14)
		{
		nuc[0] = false;
		nuc[1] = true;
		nuc[2] = true;
		nuc[3] = true;
		}
	else if (nucCode == 15)
		{
		nuc[0] = true;
		nuc[1] = true;
		nuc[2] = true;
		nuc[3] = true;
		}
}

void CharacterMatrixDiscrete::setState(int taxaId, int charId, int stateId) {

	std::vector<bool>& x = matrix[taxaId][charId];
	if (stateId > x.size())
		{
		std::cerr << "Error: problem setting the state" << std::endl;
		return;
		}
	x[stateId] = true;
}

DAGNode* CharacterMatrixDiscrete::getElement(const VectorInteger& index) {
    
    throw RbException( "GetElement of a discrete character matrix is not supported!" );
}


void CharacterMatrixDiscrete::setElement(const VectorNatural& index, DAGNode* var) {
    
    throw RbException( "SetElement of a discrete character matrix is not supported!" );
}


/** Overloaded container resize method */
void CharacterMatrixDiscrete::resize( const std::vector<size_t>& len ) {
    
    throw RbException( "Resize of a discrete character matrix is not supported!" );
}


/** Overloaded container setLength method */
void CharacterMatrixDiscrete::setLength( const std::vector<size_t>& len) {
    
    throw RbException( "setLength of a discrete character matrix is not supported!" );
}


/** Overloaded container size method */
size_t CharacterMatrixDiscrete::size( void ) const {
    
    return numCharacters * numTaxa;
}


/** Transpose the matrix */
void CharacterMatrixDiscrete::transpose( void ) {
    
    throw RbException( "Transpose of a discrete character matrix is not supported!" );
}



