//
//  TraitData.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 3/13/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "TraitData.h"
#include "RbException.h"

using namespace RevBayesCore;


TraitData::TraitData() {
    
}


TraitData::~TraitData() {
    
}


void TraitData::addParameterName(std::string const &n) {
    
    bool exists = false;
    for (std::vector<std::string>::iterator it = paramNames.begin(); it != paramNames.end(); ++it) {
        if ( *it == n ) {
            exists = true;
            break;
        }
    }
    
    if ( exists ) {
        throw RbException("Parameter with name \"" + n + "\" already exists.");
    }
    
    paramNames.push_back( n );
}



void TraitData::addParameterValue(std::string const &n, std::string const &p, double v) {
    size_t parIndex = paramNames.size();
    for (size_t i = 0; i < paramNames.size(); ++i) {
        if ( paramNames[i] == p ) {
            parIndex = i;
            break;
        }
    }
    size_t taxonIndex = taxonNames.size();
    for (size_t i = 0; i < taxonNames.size(); ++i) {
        if ( taxonNames[i] == n ) {
            taxonIndex = i;
            break;
        }
    }
    
    if ( parIndex == paramNames.size() || taxonIndex == taxonNames.size() ) {
        throw RbException("Could not find taxon or parameter.");
    }
    
    values[taxonIndex][parIndex] = v;
}


void TraitData::addTaxon(std::string const &n) {
    
    bool exists = false;
    for (std::vector<std::string>::iterator it = taxonNames.begin(); it != taxonNames.end(); ++it) {
        if ( *it == n ) {
            exists = true;
            break;
        }
    }
    
    if ( exists ) {
        throw RbException("Taxon with name \"" + n + "\" already exists.");
    }
    
    taxonNames.push_back( n );
}





