/*
 *  ChromosomesDataReader.cpp
 *  revbayes-proj
 *
 *  Created by Will Freyman on 9/8/14.
 *
 */


#include "ChromosomesDataReader.h"
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


ChromosomesDataReader::ChromosomesDataReader(std::string fn, char d) : DelimitedDataReader(fn, d) {
	
   filename = fn;
	int i = 0;
	while (i < (DelimitedDataReader::getChars().size()-1)) {
		
		std::string name = DelimitedDataReader::getChars()[i][0];
		
		names.push_back( name );
		int count = boost::lexical_cast<int>(DelimitedDataReader::getChars()[i][1]);
		
		counts.push_back( count );
		i++;
	}
}


std::vector<std::string> ChromosomesDataReader::getNames(void)
{
    return names;
}

std::vector<int> ChromosomesDataReader::getCounts(void)
{
    return counts;
}

std::string ChromosomesDataReader::getFilename(void)
{
    return filename;
}