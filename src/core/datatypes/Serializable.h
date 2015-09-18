/**
 * @file Cloneable.h
 *
 *
 * @brief Declaration of template cloneable. The only method this interface defines
 * is T* clone(void) const. The clone function is required in e.g. DAG nodes if the
 * type is abstract (e.g. RateMatrix, Function, ...)
 *
 * (c) Copyright 2009-
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 */

#ifndef Serializable_H
#define Serializable_H

#include <string>

namespace RevBayesCore {
    
    class Serializable {
        
    public:
        virtual                         ~Serializable(void) {}
        
        virtual void                    initFromString( const std::string &s ) = 0;                             //!< Serialize (resurrect) the object from a string value
        virtual void                    initFromFile( const std::string &dir, const std::string &fn );          //!< Read and resurrect this object from a file in its default format.
        virtual void                    writeToFile( const std::string &dir, const std::string &fn ) const;     //!< Write this object into a file in its default format.
    };
    
}

#endif

