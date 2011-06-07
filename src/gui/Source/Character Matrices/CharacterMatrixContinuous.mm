#include <iomanip>
#include <iostream>
#include "CharacterMatrixContinuous.h"



CharacterMatrixContinuous::CharacterMatrixContinuous(void) {

	matrix = NULL;
	dataType = "continuous";
	isDiscrete = false;
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

bool CharacterMatrixContinuous::getIsAmbig(int taxaId, int charId) {

	return false;
}

void CharacterMatrixContinuous::print(void) {

    for (int i=0; i<numTaxa; i++)
        {
        std::string tf;
        if (isTaxonExcluded[i] == true)
            tf = "excluded ";
        else 
            tf = "included ";
        std::cout << std::setw(4) << i << " " << tf << taxonNames[i] << std::endl;
        }

	for (int j=0; j<numCharacters; j++)
		{
        std::string tf;
        if (isCharacterExcluded[j] == true)
            tf = " excluded -- ";
        else 
            tf = " included -- ";
		std::cout << std::setw(5) << j << tf;
		for (int i=0; i<numTaxa; i++)
            std::cout << std::fixed << std::setprecision(4) << matrix[i][j] << " ";
		std::cout << std::endl;
		}
}

void CharacterMatrixContinuous::setState(int taxaId, int charId, double x) {

    matrix[taxaId][charId] = x;
}

