//
//  PseudoGibbsMetaMove.cpp
//  
//
//  Created by Bastien Boussau on 05/07/13.
//  Copyright 2013 UC Berkeley. All rights reserved.
//

#include <iostream>
#include "PseudoGibbsMetaMove.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbStatisticsHelper.h"

using namespace RevBayesCore;

PseudoGibbsMetaMove::PseudoGibbsMetaMove(const Move* m, AbstractCharacterData* d, const int numOfSubsets) {
    
	move = m;
	data = d;
	int numChars = data->getNumberOfCharacters();
	int subsetSize = (int)numChars/numberOfSubsets;
	std::vector<int> temp (0, subsetSize);
	for (size_t i = 0 ; i < numberOfSubsets ; i++) {
		dataSubsets.push_back( temp );
	}
	return;

	
}

PseudoGibbsMetaMove::PseudoGibbsMetaMove ( const PseudoGibbsMetaMove& p ) {
	move = p.move;
	data = p.data;
	numberOfSubsets = p.numberOfSubsets;
	dataSubsets = p.dataSubsets;
}

PseudoGibbsMetaMove::~PseudoGibbsMetaMove() {
    // we own nothing
}


PseudoGibbsMetaMove* PseudoGibbsMetaMove::clone( void ) const {
    return new PseudoGibbsMetaMove(*this);
}


void PseudoGibbsMetaMove::buildSubsets() {
	RandomNumberGenerator* rng = GLOBAL_RNG;
	int numChars = data->getNumberOfCharacters();
	int subsetSize = (int)numChars/numberOfSubsets;
	std::vector<int> charIndices (0, numChars);
	for (size_t i = 0 ; i < numChars ; i++) {
		charIndices[i]=i;
	}
	std::vector<int> randomizedCharIndices;
	RbStatistics::Helper::randomlySelectFromVectorWithoutReplacement(charIndices, randomizedCharIndices, subsetSize * numberOfSubsets, *rng);
	for (size_t i = 0 ; i < numberOfSubsets ; i++) {
		int offset = i * subsetSize;
		for (size_t j = 0 ; j < subsetSize ; j++) {
			dataSubsets[i][j] = randomizedCharIndices[offset + j] ;
		}
	}
	return;
}





