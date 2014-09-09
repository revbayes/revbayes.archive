/*
 *  ChromosomesDataReader.h
 *  revbayes-proj
 *
 *  Created by Will Freyman on 9/8/14.
 */


#ifndef ChromosomesDataReader_H
#define ChromosomesDataReader_H

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
    class ChromosomesDataReader : public DelimitedDataReader {
        
    public:
		
		ChromosomesDataReader(std::string fn, char d='\t');
		
		std::string getFilename(void);
		std::vector<std::string> getNames(void);
		std::vector<int> getCounts(void);
		
        
		
	protected:	
		
        std::vector<std::string> names;
		std::vector<int> counts;
		std::string filename;
		
    };
	
}

#endif 