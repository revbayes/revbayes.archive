/*
 *  NaturalNumberTSVDataReader.h
 *  revbayes-proj
 *
 *  For reading in positive integer character data in tab separated files.
 *
 *  Created by Will Freyman on 9/8/14.
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
		std::vector<int> getData(void);
		
        
		
	protected:	
		
        std::vector<std::string> names;
		std::vector<int> data;
		std::string filename;
		
    };
	
}

#endif 