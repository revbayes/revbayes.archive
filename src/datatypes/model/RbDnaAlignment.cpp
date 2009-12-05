#include "RbDnaAlignment.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbBitset.h"
#include "StringVector.h"
#include <istream>
#include <sstream>
#include <fstream>
#include <string>
#include <cmath>
#include "StringVector.h"

const StringVector RbDnaAlignment::rbClass = StringVector(RbNames::DnaAlignment::name) + RbCharacterMatrix::rbClass;


RbDnaAlignment::RbDnaAlignment(std::string fileName, std::string fileType) : RbCharacterMatrix(fileName, fileType, 4) {

}	

RbDnaAlignment::RbDnaAlignment(const RbDnaAlignment& a) : RbCharacterMatrix(a) {

}

RbDnaAlignment::~RbDnaAlignment(void) {

	freeCharMatrix();
}

RbObject* RbDnaAlignment::clone() const {
	RbDnaAlignment* x = new RbDnaAlignment(*this);
	return (RbObject*)x;
}

bool RbDnaAlignment::equals(const RbObject* obj) const {

}

void RbDnaAlignment::printValue(std::ostream& o) const {

}

std::string RbDnaAlignment::toString(void) const {

}

RbObject& RbDnaAlignment::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const RbDnaAlignment& x = dynamic_cast<const RbDnaAlignment&> (obj);

        RbDnaAlignment& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbDnaAlignment& x = dynamic_cast<const RbDnaAlignment&> (*(obj.convertTo(RbNames::DnaAlignment::name)));

            RbDnaAlignment& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to " + RbNames::DnaAlignment::name);
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbCharacterMatrix& RbDnaAlignment::operator=(const RbCharacterMatrix& obj) {

    try {
        // Use built-in fast down-casting first
        const RbDnaAlignment& x = dynamic_cast<const RbDnaAlignment&> (obj);

        RbDnaAlignment& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbDnaAlignment& x = dynamic_cast<const RbDnaAlignment&> (*(obj.convertTo(RbNames::DnaAlignment::name)));

            RbDnaAlignment& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to " + RbNames::DnaAlignment::name);
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbDnaAlignment& RbDnaAlignment::operator=(const RbDnaAlignment& obj) {

	
	freeCharMatrix();
	numTaxa = obj.numTaxa;
	numChar = obj.numChar;
	numStates = obj.numStates;
	taxonNames = obj.taxonNames;
	allocateCharMatrix(numTaxa, numChar, numStates);
			
	for (int i=0; i<numTaxa; i++)
		for (int j=0; j<numChar; j++)
			*charMatrix[i][j] = *obj.charMatrix[i][j];
}

char RbDnaAlignment::convertToChar(RbBitset* bs) {

	int nucCode = 0;
	for (int i=0; i<numStates; i++)
		{
		if (bs->isBitSet(i) == true)
			nucCode += (int)pow( 2.0, (double)i );
		}
	char c = getNucleotideChar(nucCode);
	return c;
}

char RbDnaAlignment::getNucleotideChar(int nucCode) {

	if (nucCode == 1)
		{
		return 'A';
		}
	else if (nucCode == 2)
		{
		return 'C';
		}
	else if (nucCode == 3)
		{
		return 'M';
		}
	else if (nucCode == 4)
		{
		return 'G';
		}
	else if (nucCode == 5)
		{
		return 'R';
		}
	else if (nucCode == 6)
		{
		return 'S';
		}
	else if (nucCode == 7)
		{
		return 'V';
		}
	else if (nucCode == 8)
		{
		return 'T';
		}
	else if (nucCode == 9)
		{
		return 'W';
		}
	else if (nucCode == 10)
		{
		return 'Y';
		}
	else if (nucCode == 11)
		{
		return 'H';
		}
	else if (nucCode == 12)
		{
		return 'K';
		}
	else if (nucCode == 13)
		{
		return 'D';
		}
	else if (nucCode == 14)
		{
		return 'B';
		}
	else if (nucCode == 15)
		{
		return 'N';
		}
	return '?';
}

/*-------------------------------------------------------------------
|
|   NucID: 
|
|   Take a character, nuc, and return an integer:
|
|       nuc        returns
|        A            1 
|        C            2     
|        G            4      
|        T U          8     
|        R            5      
|        Y           10       
|        M            3      
|        K           12   
|        S            6     
|        W            9      
|        H           11      
|        B           14     
|        V            7      
|        D           13  
|        N - ?       15       
|
-------------------------------------------------------------------*/
void RbDnaAlignment::initState(char nuc, RbBitset* bs) {
	
	char		n;
	
	if (nuc == 'U' || nuc == 'u')
		n = 'T';
	else
		n = nuc;

	if (n == 'A' || n == 'a')
		{
		bs->setBit(0);
		}
	else if (n == 'C' || n == 'c')
		{
		bs->setBit(1);
		}
	else if (n == 'G' || n == 'g')
		{
		bs->setBit(2);
		}
	else if (n == 'T' || n == 't')
		{
		bs->setBit(3);
		}
	else if (n == 'R' || n == 'r')
		{
		bs->setBit(0);
		bs->setBit(2);
		}
	else if (n == 'Y' || n == 'y')
		{
		bs->setBit(1);
		bs->setBit(3);
		}
	else if (n == 'M' || n == 'm')
		{
		bs->setBit(0);
		bs->setBit(1);
		}
	else if (n == 'K' || n == 'k')
		{
		bs->setBit(2);
		bs->setBit(3);
		}
	else if (n == 'S' || n == 's')
		{
		bs->setBit(1);
		bs->setBit(2);
		}
	else if (n == 'W' || n == 'w')
		{
		bs->setBit(0);
		bs->setBit(3);
		}
	else if (n == 'H' || n == 'h')
		{
		bs->setBit(0);
		bs->setBit(1);
		bs->setBit(3);
		}
	else if (n == 'B' || n == 'b')
		{
		bs->setBit(1);
		bs->setBit(2);
		bs->setBit(3);
		}
	else if (n == 'V' || n == 'v')
		{
		bs->setBit(0);
		bs->setBit(1);
		bs->setBit(2);
		}
	else if (n == 'D' || n == 'd')
		{
		bs->setBit(0);
		bs->setBit(2);
		bs->setBit(3);
		}
	else if (n == 'N' || n == 'n' || n == '-' || n == '?')
		{
		bs->setBit(0);
		bs->setBit(1);
		bs->setBit(2);
		bs->setBit(3);
		}
}

