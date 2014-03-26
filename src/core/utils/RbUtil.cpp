//
//  RbUtil.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 7/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "RbUtil.h"
#include "Clade.h"


std::ostream& RevBayesCore::operator<<(std::ostream& o, const std::vector<bool>& x) {
    o << "(";
    for (std::vector<bool>::const_iterator it = x.begin(); it != x.end(); ++it) {
        if ( it != x.begin() ) {
            o << ", ";
        }
        if ( *it ) {
            o << "TRUE";
        } else {
            o << "FALSE";
        }
    }
    o << ")";
    
    return o;
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const std::vector<double>& x) {
    o << "(";
    for (std::vector<double>::const_iterator it = x.begin(); it != x.end(); ++it) {
        if ( it != x.begin() ) {
            o << ", ";
        }
        o << *it;
    }
    o << ")";
    
    return o;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const std::vector<int>& x) {
    o << "(";
    for (std::vector<int>::const_iterator it = x.begin(); it != x.end(); ++it) {
        if ( it != x.begin() ) {
            o << ", ";
        }
        o << *it;
    }
    o << ")";
    
    return o;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const std::vector<unsigned int>& x) {
    o << "(";
    for (std::vector<unsigned int>::const_iterator it = x.begin(); it != x.end(); ++it) {
        if ( it != x.begin() ) {
            o << ", ";
        }
        o << *it;
    }
    o << ")";
    
    return o;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const std::vector<std::string>& x) {
    o << "(";
    for (std::vector<std::string>::const_iterator it = x.begin(); it != x.end(); ++it) {
        if ( it != x.begin() ) {
            o << ", ";
        }
        o << *it;
    }
    o << ")";
    
    return o;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const std::vector<Clade>& x) {
    o << "(";
    for (std::vector<Clade>::const_iterator it = x.begin(); it != x.end(); ++it) {
        if ( it != x.begin() ) {
            o << ", ";
        }
        o << *it;
    }
    o << ")";
    
    return o;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const std::vector<Taxon>& x) {
    o << "(";
    for (std::vector<Taxon>::const_iterator it = x.begin(); it != x.end(); ++it) {
        if ( it != x.begin() ) {
            o << ", ";
        }
        o << *it;
    }
    o << ")";
    
    return o;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const std::vector<TimeTree>& x) {
    o << "(";
    for (std::vector<TimeTree>::const_iterator it = x.begin(); it != x.end(); ++it) {
        if ( it != x.begin() ) {
            o << ", ";
        }
        o << *it;
    }
    o << ")";
    
    return o;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const std::vector<Trace>& x) {
    o << "(";
    for (std::vector<Trace>::const_iterator it = x.begin(); it != x.end(); ++it) {
        if ( it != x.begin() ) {
            o << ", ";
        }
        o << *it;
    }
    o << ")";
    
    return o;
}




//std::ostream& RevBayesCore::operator<<(std::ostream& o, const std::vector<RateMatrix_GTR>& x) {
//    o << "(";
//    for (std::vector<RateMatrix_GTR>::const_iterator it = x.begin(); it != x.end(); ++it) {
//        if ( it != x.begin() ) {
//            o << ", ";
//        }
//        o << *it;
//    }
//    o << ")";
//    
//    return o;
//}
//
//
//std::ostream& RevBayesCore::operator<<(std::ostream& o, const std::vector<RateMatrix_JC>& x) {
//    o << "(";
//    for (std::vector<RateMatrix_JC>::const_iterator it = x.begin(); it != x.end(); ++it) {
//        if ( it != x.begin() ) {
//            o << ", ";
//        }
//        o << *it;
//    }
//    o << ")";
//    
//    return o;
//}
