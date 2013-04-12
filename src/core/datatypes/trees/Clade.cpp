//
//  Clade.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 3/10/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "Clade.h"

#include <iostream>


using namespace RevBayesCore;


Clade::Clade(const std::vector<std::string> &n) : taxonNames( n ) {
    
}


bool Clade::operator==(const Clade &c) const {
    
    if ( c.size() != taxonNames.size() )
        return false;
    
    for ( std::vector<std::string>::const_iterator it = taxonNames.begin(); it != taxonNames.end(); ++it) {
        bool found = false;
        for (std::vector<std::string>::const_iterator j = c.begin(); j != c.end(); ++j) {
            if ( *j == * it ) {
                found = true;
                break;
            }
        }
        
        if ( !found ) {
            return false;
        }
    }
    
    return true;
}


bool Clade::operator!=(const Clade &c) const {
    return !operator==( c );
}

bool Clade::operator<(const Clade &c) const {
    return taxonNames.size() < c.size();
}


std::vector<std::string>::const_iterator Clade::begin(void) const {
    return taxonNames.begin();
}


std::vector<std::string>::iterator Clade::begin(void) {
    return taxonNames.begin();
}


std::vector<std::string>::const_iterator Clade::end(void) const {
    return taxonNames.end();
}


std::vector<std::string>::iterator Clade::end(void) {
    return taxonNames.end();
}



Clade* Clade::clone(void) const {
    return new Clade(*this);
}


const std::string& Clade::getTaxonName(size_t i) const {
    return taxonNames[i];
}


size_t Clade::size(void) const {
    return taxonNames.size();
}



std::ostream& RevBayesCore::operator<<(std::ostream& o, const Clade& x) {
    o << "{";
    
    for (size_t i = 0; i < x.size(); ++i) {
        if ( i > 0 ) {
            o << ",";
        }
        o << x.getTaxonName(i);
    }
    o << "}";
    return o;
}

