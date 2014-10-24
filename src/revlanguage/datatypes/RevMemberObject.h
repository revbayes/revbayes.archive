#ifndef RevMemberObject_H
#define RevMemberObject_H

#include <string>

namespace RevLanguage {
    
    
    /**
     * @brief Rev interface for member object which provide methods.
     *
     * This interface specifies that classes implementing the interface will provide member methods.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2014-10-23
     *
     */
    class RevMemberObject {
        
    public:
        
        virtual const MethodTable&              getMethods(void) const = 0;                 //!< Get member methods

    };
    
}

#endif

