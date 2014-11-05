/*
 *  NaturalNumberTSVDataReader.cpp
 *  revbayes-proj
 *
 *  Created by Will Freyman on 9/8/14.
 *
 */


#include "NaturalNumberTSVDataReader.h"
#include "RbFileManager.h"

#include <boost/foreach.hpp>
#include "RbConstants.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <set>
#include <string>
#include <boost/lexical_cast.hpp>


using namespace RevBayesCore;


NaturalNumberTSVDataReader::NaturalNumberTSVDataReader(std::string fn, char d) : DelimitedDataReader(fn, d) {
	
   filename = fn;
	int i = 0;
	while (i < DelimitedDataReader::getChars().size()) {
		
		std::string name = DelimitedDataReader::getChars()[i][0];
		
		names.push_back( name );
		int d = boost::lexical_cast<int>(DelimitedDataReader::getChars()[i][1]);
		
		data.push_back( d );
		i++;
	}
}


std::vector<std::string> NaturalNumberTSVDataReader::getNames(void)
{
    return names;
}

std::vector<int> NaturalNumberTSVDataReader::getData(void)
{
    return data;
}

std::string NaturalNumberTSVDataReader::getFilename(void)
{
    return filename;
}