//
//  RelativeNodeAgeConstraints.h
//
//  Created by Bastien Boussau on 4/8/15.
//  Copyright (c) 2015 Bastien Boussau. All rights reserved.
//

#ifndef __RelativeNodeAgeConstraints__
#define __RelativeNodeAgeConstraints__

#include "Cloneable.h"
#include "RelativeNodeAgeConstraintsReader.h"
#include <string>

namespace RevBayesCore {
    class RelativeNodeAgeConstraints : public Cloneable
    {
        
    public:
        RelativeNodeAgeConstraints();
        RelativeNodeAgeConstraints(RelativeNodeAgeConstraintsReader* tadr);
        RelativeNodeAgeConstraints(const RelativeNodeAgeConstraints& a);
        RelativeNodeAgeConstraints&                     operator=(const RelativeNodeAgeConstraints& a);
        virtual RelativeNodeAgeConstraints*             clone(void) const;
        
        const std::pair < std::pair<std::string, std::string>, std::pair<std::string, std::string> >& getConstraint(size_t i) const;
        const std::vector <std::pair < std::pair<std::string, std::string>, std::pair<std::string, std::string> > >& getConstraints( ) const;

        size_t                                    getNumberOfConstraints(void) const;
        std::string                                     getFilename(void) const;
       // std::string                                     getDatatype(void) const;
    protected:
        std::vector <std::pair < std::pair<std::string, std::string>, std::pair<std::string, std::string> > >								        olderYoungerConstraints;
        
    private:
        std::string                                     filename;
        
    };
    
    std::ostream&                                       operator<<(std::ostream& o, const RelativeNodeAgeConstraints& x);
}


#endif /* defined(__RelativeNodeAgeConstraints__) */
