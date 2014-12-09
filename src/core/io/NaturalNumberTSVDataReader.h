/**
 * @file
 * This file contains the declaration of the NaturalNumberTSVDataReader class.
 * For reading in positive integer character data in tab separated files.
 *
 * @brief Declaration of the NaturalNumberTSVDataReader.
 *
 * (c) Copyright 2014- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 */


#ifndef NaturalNumberTSVDataReader_H
#define NaturalNumberTSVDataReader_H

#include "DelimitedDataReader.h"
#include <string>
#include <vector>

namespace RevBayesCore {


    /**
	 *
     * Reader for chromosome tab delimited files.
     *
     *
     */
    class NaturalNumberTSVDataReader : public DelimitedDataReader {
        
    public:
		
		NaturalNumberTSVDataReader(std::string fn, char d='\t');
		
		std::string getFilename(void);
		std::vector<std::string> getNames(void);
		std::vector<std::string> getData(void);
		
        
		
	protected:	
		
        std::vector<std::string> names;
		std::vector<std::string> data;
		std::string filename;
		
    };
	
}

#endif 