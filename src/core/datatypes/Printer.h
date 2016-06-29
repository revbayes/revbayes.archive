/**
 * @file Printer.h
 *
 *
 * @brief Declaration of Printer. This class clones objects by either calling clone, if the
 * object is derived from Cloneable, or calling the copy constructor. Hence, the Printer can be
 * used in templates when it is unknown if either the copy constructor (basic classes)
 * or the clone function (abstract classes) exists.
 *
 * (c) Copyright 2009-
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 */

#ifndef Printer_H
#define Printer_H

#include <string>

namespace RevBayesCore {
    
    template <typename objType, int>
    // general case: T is not derived from Printable
    // calls copy constructor
    class Printer {
        
    public:
        //!< Printer the given object.
        static void                     printForUser( const objType &a, std::ostream &o, const std::string &sep, int l, bool left ) {
            
            long previousPrecision = o.precision();
            std::ios_base::fmtflags previousFlags = o.flags();
            
            std::fixed( o );
            o.precision( 3 );
            
            o << a;
            
            o.setf( previousFlags );
            o.precision( previousPrecision );
        }
        static void                     printForSimpleStoring( const objType &a, std::ostream &o, const std::string &sep, int l, bool left ) { o << a; }
        static void                     printForComplexStoring( const objType &a, std::ostream &o, const std::string &sep, int l, bool left ) { o << a; }
    };
    
    template <typename objType>
    // T is derived from Printable
    // calls clone
    class Printer<objType,1> {
        
    public:
        //!< Printer the given object.
        static void                     printForUser( const objType &a, std::ostream &o, const std::string &sep, int l, bool left ) { a.printForUser(o, sep, l, left); }
        static void                     printForSimpleStoring( const objType &a, std::ostream &o, const std::string &sep, int l, bool left ) { a.printForSimpleStoring(o, sep, l, left); }
        static void                     printForComplexStoring( const objType &a, std::ostream &o, const std::string &sep, int l, bool left ) { a.printForComplexStoring(o, sep, l, left); }

    };
    
}



#endif

