#include "CharacterMatrix.h"
#include "RbNames.h"
#include "VectorString.h"



CharacterMatrix::CharacterMatrix(void) : Matrix(CharacterState_name) {

	numTaxa = 0;
	numCharacters = 0;
	name = "";
}

CharacterMatrix::CharacterMatrix(int nt, int nc) : Matrix(CharacterState_name)  {

	numTaxa = nt;
	numCharacters = nc;
	name = "";
}

/** Get class vector describing type of object */
const VectorString& CharacterMatrix::getClass(void) const {
    
    static VectorString rbClass = VectorString(CharacterMatrix_name) + Matrix::getClass();
    return rbClass;
}

int CharacterMatrix::getIndexOfTaxonNamed(std::string s) {

    for (size_t i=0; i<taxonNames.size(); i++)
        {
        if (taxonNames[i] == s)
            return i;
        }
    return -1;
}
