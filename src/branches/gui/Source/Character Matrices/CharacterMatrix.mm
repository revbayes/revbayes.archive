#include "CharacterMatrix.h"



CharacterMatrix::CharacterMatrix(void) {

	numTaxa = 0;
	numCharacters = 0;
	name = "";
}

CharacterMatrix::CharacterMatrix(int nt, int nc) {

	numTaxa = nt;
	numCharacters = nc;
	name = "";
}

int CharacterMatrix::getIndexOfTaxonNamed(std::string s) {

    for (int i=0; i<taxonNames.size(); i++)
        {
        if (taxonNames[i] == s)
            return i;
        }
    return -1;
}
