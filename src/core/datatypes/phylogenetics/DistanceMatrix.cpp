//
//  DistanceMatrix.cpp
//
//  Created by Bastien Boussau on 4/8/15.
//  Copyright (c) 2015 Bastien Boussau. All rights reserved.
//

#include "DistanceMatrix.h"
#include "DistanceMatrixReader.h"
#include <sstream>
#include <string>

using namespace RevBayesCore;

DistanceMatrix::DistanceMatrix(DistanceMatrixReader* tadr) : filename(tadr->getFilename())
{
    names = tadr->getNames();
	matrix = tadr->getMatrix();
	numTips = names.size();
}

DistanceMatrix::DistanceMatrix(const DistanceMatrix& a)
{
    *this = a;
}

DistanceMatrix& DistanceMatrix::operator=(const DistanceMatrix& a)
{
    if (this != &a)
    {
        names = a.names;
		matrix = a.matrix;
		filename = a.filename;
		numTips = a.numTips;
    }
    
    return *this;
}

DistanceMatrix* DistanceMatrix::clone(void) const
{
    return new DistanceMatrix(*this);
}

std::vector<std::string> DistanceMatrix::getNames(void) const
{
    return names;
}

MatrixReal DistanceMatrix::getMatrix(void) const
{
    return matrix;
}

unsigned DistanceMatrix::getSize(void) const
{
	return numTips;
}


std::string DistanceMatrix::getFilename(void) const
{
    return filename;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const DistanceMatrix& x) {
    
    std::stringstream s;
    
    // Generate nice header
    o << std::endl;
    s << "DistanceMatrix with " << x.getSize() << " tips. " << std::endl;
    o << s.str();
	std::vector<std::string> names = x.getNames();
	for ( size_t i = 0; i < x.getSize(); ++i ) {
		o << names[i] ;
		for ( size_t j = 0; j < x.getSize(); ++j ) {
        	o << "\t" << x.getMatrix()[i][j] ;
		}
		o << std::endl;
	}
    o << std::endl;
    
    return o;
}
