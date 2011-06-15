#include <iomanip>
#include <iostream>
#include <sstream>


#include "CharacterMatrixContinuous.h"
#include "RbNames.h"
#include "RbException.h"
#include "VectorString.h"



CharacterMatrixContinuous::CharacterMatrixContinuous(void) {
    
	matrix = NULL;
	dataType = "continuous";
	isDiscrete = false;
}

CharacterMatrixContinuous::CharacterMatrixContinuous(const CharacterMatrixContinuous& cmc) {
    
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

CharacterMatrixContinuous::CharacterMatrixContinuous(int nt, int nc) {
    
    numTaxa = nt;
    numCharacters = nc;
    matrix = NULL;
	dataType = "continuous";
	isDiscrete = false;
    allocateMatrix();
}

CharacterMatrixContinuous::~CharacterMatrixContinuous(void) {
    
    freeMatrix();
}

void CharacterMatrixContinuous::allocateMatrix(void) {
    
    freeMatrix();
	if (numTaxa > 0 && numCharacters > 0)
    {
        matrix = new double*[numTaxa];
        matrix[0] = new double[numTaxa*numCharacters];
        for (int i=1; i<numTaxa; i++)
            matrix[i] = matrix[i-1] + numCharacters;
        for (int i=0; i<numTaxa; i++)
            for (int j=0; j<numCharacters; j++)
                matrix[i][j] = 0.0;
        isTaxonExcluded.resize(numTaxa);
        isCharacterExcluded.resize(numCharacters);
        for (int i=0; i<numTaxa; i++)
            isTaxonExcluded[i] = false;
        for (int i=0; i<numCharacters; i++)
            isCharacterExcluded[i] = false;
    }
}

void CharacterMatrixContinuous::freeMatrix(void) {
    
    if (matrix != NULL)
    {
        isTaxonExcluded.clear();
        isCharacterExcluded.clear();
        delete [] matrix[0];
        delete [] matrix;
        matrix = NULL;
    }
}

/** Overloaded container clear function */
void CharacterMatrixContinuous::clear( void ) {
    
//    matrix.clear();
    freeMatrix();
//    length[0] = 0;
//    length[1] = 0;
}


/** Clone function */
CharacterMatrixContinuous* CharacterMatrixContinuous::clone(void) const {
    
    return new CharacterMatrixContinuous(*this);
}


/** Get class vector describing type of object */
const VectorString& CharacterMatrixContinuous::getClass(void) const {
    
    static VectorString rbClass = VectorString(CharacterMatrixContinuous_name) + CharacterMatrix::getClass();
    return rbClass;
}


bool CharacterMatrixContinuous::getIsAmbig(int taxaId, int charId) {
    
	return false;
}

void CharacterMatrixContinuous::printValue(std::ostream &o) const {
    
    for (int i=0; i<numTaxa; i++)
    {
        std::string tf;
        if (isTaxonExcluded[i] == true)
            tf = "excluded ";
        else 
            tf = "included ";
        o << std::setw(4) << i << " " << tf << taxonNames[i] << std::endl;
    }
    
	for (int j=0; j<numCharacters; j++)
    {
        std::string tf;
        if (isCharacterExcluded[j] == true)
            tf = " excluded -- ";
        else 
            tf = " included -- ";
		o << std::setw(5) << j << tf;
		for (int i=0; i<numTaxa; i++) {
            o << std::fixed << std::setprecision(4) << matrix[i][j] << " ";
        }
        
		o << std::endl;
    }
}

/** Complete info about object */
std::string CharacterMatrixContinuous::richInfo(void) const {
    
    std::ostringstream o;
    o <<  "CharacterMatrixContinuous: value = " ;
    printValue(o);
    
    return o.str();
}

void CharacterMatrixContinuous::setState(int taxaId, int charId, double x) {
    
    matrix[taxaId][charId] = x;
}


DAGNode* CharacterMatrixContinuous::getElement(const VectorInteger& index) {
    
    throw RbException( "GetElement of a continuous character matrix is not supported!" );
}


void CharacterMatrixContinuous::setElement(const VectorNatural& index, DAGNode* var) {
    
    throw RbException( "SetElement of a continuous character matrix is not supported!" );
}


/** Overloaded container resize method */
void CharacterMatrixContinuous::resize( const std::vector<size_t>& len ) {
    
    throw RbException( "Resize of a continuous character matrix is not supported!" );
}


/** Overloaded container setLength method */
void CharacterMatrixContinuous::setLength( const std::vector<size_t>& len) {
    
    throw RbException( "setLength of a continuous character matrix is not supported!" );
}


/** Overloaded container size method */
size_t CharacterMatrixContinuous::size( void ) const {
    
    return numCharacters * numTaxa;
}


/** Transpose the matrix */
void CharacterMatrixContinuous::transpose( void ) {
    
    throw RbException( "Transpose of a continuous character matrix is not supported!" );
}

