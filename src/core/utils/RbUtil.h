/*!
 * @file
 * This file contains global constants used in the program.
 *
 * @brief Names of variable types
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2010-09-09, version 1.0
 *
 * $Id$
 */

#ifndef RbUtil_H
#define RbUtil_H

#include "BranchLengthTree.h"
#include "Taxon.h"
#include "TimeTree.h"
#include "Trace.h"
#include "RateMatrix_GTR.h"
#include "RateMatrix_JC.h"
#include "RbVector.h"

#include <ostream>
#include <string>
#include <vector>


namespace RevBayesCore {
    
    namespace RbUtils {
        
        // Utility string constants
        const std::string EMPTY_STRING                                  = "";
        const std::string PAD                                           = "   ";
        
        
        // for testing if a (template) type is an stl-vector
        template <typename T>
        struct is_vector {
            static const bool value = false;
        };
        
        template <typename T,typename Alloc>
        struct is_vector<std::vector<T,Alloc> > {
            static const bool value = true;
        };
        
        
        template <typename T>
        class sub_vector {
        public:
            static const T&  getElement(const T &obj, size_t i) { return obj; }
            static size_t    size(const T &obj) { return 1; }
        };
        
        template <typename T>
        class sub_vector< RbVector<T> > {
        public:
            static const T&  getElement(const RbVector<T> &obj, size_t i) { return obj[i]; }
            static size_t    size(const RbVector<T> &obj) { return obj.size(); }
        };

        template <typename T,typename Alloc>
        class sub_vector<std::vector<T,Alloc> > {
        public:
            static const T&  getElement(const std::vector<T,Alloc> &obj, size_t i) { return obj[i]; }
            static size_t    size(const std::vector<T,Alloc> &obj) { return obj.size(); }
        };


        
        
        template <class T, class U>
        struct same_type
        {
            static const bool value = false;
        };
        
        //specialization for types that are the same
        template <class T>
        struct same_type<T, T>
        {
            static const bool value = true;
        };
    }
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const std::vector<bool>& x);                            //!< Overloaded output operator
    std::ostream&                       operator<<(std::ostream& o, const std::vector<double>& x);                          //!< Overloaded output operator
    std::ostream&                       operator<<(std::ostream& o, const std::vector<int>& x);                             //!< Overloaded output operator
    std::ostream&                       operator<<(std::ostream& o, const std::vector<unsigned int>& x);                    //!< Overloaded output operator
    std::ostream&                       operator<<(std::ostream& o, const std::vector<std::string>& x);                     //!< Overloaded output operator
    
    std::vector<int>                    operator+(const std::vector<int>&    x, const std::vector<int>&    y);              //!< Overloaded addition operator
    std::vector<double>                 operator+(const std::vector<double>& x, const std::vector<double>& y);              //!< Overloaded addition operator
    std::vector<int>                    operator-(const std::vector<int>&    x, const std::vector<int>&    y);              //!< Overloaded subtraction operator
    std::vector<double>                 operator-(const std::vector<double>& x, const std::vector<double>& y);              //!< Overloaded subtraction operator
    std::vector<int>                    operator*(const std::vector<int>&    x, const std::vector<int>&    y);              //!< Overloaded multiplication operator
    std::vector<double>                 operator*(const std::vector<double>& x, const std::vector<double>& y);              //!< Overloaded multiplication operator
    std::vector<double>                 operator/(const std::vector<int>&    x, const std::vector<int>&    y);              //!< Overloaded division operator
    std::vector<double>                 operator/(const std::vector<double>& x, const std::vector<int>&    y);              //!< Overloaded division operator
    std::vector<double>                 operator/(const std::vector<int>&    x, const std::vector<double>& y);              //!< Overloaded division operator
    std::vector<double>                 operator/(const std::vector<double>& x, const std::vector<double>& y);              //!< Overloaded division operator
    
}

#endif
