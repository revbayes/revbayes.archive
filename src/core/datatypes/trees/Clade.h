/**
 * @file
 * This file contains the declaration of a Clade. A clade is simply a container of the taxon names.
 *
 * @brief Declaration of Clade
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-17 10:31:20 +0200 (Tue, 17 Jul 2012) $
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends MemberObject
 *
 * $Id: TopologyNode.h 1682 2012-07-17 08:31:20Z hoehna $
 */

#ifndef Clade_H
#define Clade_H

#include <vector>
#include <string>

namespace RevBayesCore {
    
    class Clade  {
        
    public:
        Clade(const std::vector<std::string> &n);                                                                          //!< Default constructor with optional index
//        Clade(const Clade &n);                                        //!< Copy constructor
//        virtual                                    ~Clade(void);                                                         //!< Destructor
//        Clade&                                      operator=(const Clade& n);
        std::vector<std::string>::const_iterator    begin(void) const;
        std::vector<std::string>::iterator          begin(void);
        std::vector<std::string>::const_iterator    end(void) const;
        std::vector<std::string>::iterator          end(void);
        // overloaded operators
        bool                                        operator==(const Clade &t) const;
        bool                                        operator!=(const Clade &t) const;
        bool                                        operator<(const Clade &t) const;

        
        // Basic utility functions
        Clade*                                      clone(void) const;                                                      //!< Clone object
        
        // public methods
        const std::string&                          getTaxonName(size_t i) const;
        size_t                                      size(void) const;
        
        // public TopologyNode functions
        
    private:    
        std::vector<std::string>                    taxonNames;
        
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const Clade& x);                                         //!< Overloaded output operator

}

#endif
