//
//  ContinuousTraitDataReader.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 3/13/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "ContinuousTraitDataReader.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "StringUtilities.h"
#include "TraitData.h"

#include <cstdlib>


using namespace RevBayesCore;

ContinuousTraitDataReader::ContinuousTraitDataReader() {
    
}


TraitData* ContinuousTraitDataReader::readTraitData(std::string const &fileName) {
    // open file
    std::ifstream inFile( fileName.c_str() );
    
    if ( !inFile )
        throw RbException( "Could not open file \"" + fileName + "\"" );
    
    
    TraitData* data = new TraitData();
    
    std::vector<std::string> parameterNames;
    
    bool hasHeaderBeenRead = false;
    /* Command-processing loop */
    while ( inFile.good() ) {
        
        // Read a line
        std::string line;
        getline( inFile, line );
        
        // skip empty lines
        //line = StringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        if (line.length() == 0) 
        {
            continue;
        }
        
        // splitting every line into its columns
        std::vector<std::string> columns;
        // first, getting the file delimmiter
        std::string delimitter = "\t";
        
        // we should provide other delimiters too
        StringUtilities::stringSplit(line, delimitter, columns);
        
        // we assume a header at the first line of the file
        if (!hasHeaderBeenRead) {
            
            for (size_t j=1; j<columns.size(); j++) {
                
                std::string parmName = columns[j];
                data->addParameterName( parmName );
                parameterNames.push_back( parmName );
            }
            
            hasHeaderBeenRead = true;
            
            continue;
        }
        
        std::string taxonName = columns[0];
        
        data->addTaxon(taxonName);
        
        for (size_t j=1; j<columns.size(); j++) {
            
            std::string str_value = columns[j];
            double value = atof(str_value.c_str());
            data->addParameterValue( taxonName, parameterNames[j], value );
        }
        
        
    }

    return data;
}





