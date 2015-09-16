//
//  Trace.h
//  RevBayesGui
//
//  This class implements a single trace for a single parameter. It contains a mutable array of values
//  and additional meta information about the trace.
//
//  Created by Sebastian Hoehna on 3/24/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef ModelTrace_H
#define ModelTrace_H

#include <string>
#include <vector>

namespace RevBayesCore {
    
    class ModelTrace {
        
    public:
        
        ModelTrace(void);
        
        virtual                     ~ModelTrace(void) {}
        
        bool                        operator==(const ModelTrace &t) const                { return this == &t; }
        bool                        operator!=(const ModelTrace &t) const                { return !this->operator==( t ); }
        bool                        operator<(const ModelTrace &t) const                 { return this < &t; }
        
        // overloaded functions from RbObject
        ModelTrace*                 clone(void) const;                                              //!< Clone object
        
        void                        addObject(const std::string &d);
        const std::string&          objectAt(size_t index)                          { return values.at(index); }
        void                        removeLastObject();
        void                        removeObjectAtIndex(int index);
        size_t                      size() { return values.size(); }
        
        
        // getters and setters
        std::string                 getFileName()                                   { return fileName; }
        const std::string&          getParameterName() const                        { return parmName; }
        size_t                      getSamples()                                    { return values.size(); }
        size_t                      getStepSize()                                   { return stepSize; }
        std::vector<std::string>    getValues()                                     { return values; }
        const std::vector<std::string>&  getValues() const                               { return values; }
        
        void                        setFileName(std::string fn)                     { fileName = fn; }
        void                        setParameterName(std::string pm)                { parmName = pm; }
        void                        setStepSize( size_t s)                          { stepSize = s; }
        void                        setValues(const std::vector<std::string> &v)    { values = v; }
        
        
        
    private:
        
        std::vector<std::string>    values;                                     //!< the values of this trace
        
        std::string                 parmName;
        std::string                 fileName;
        
        size_t                      stepSize;                                   //!< the step size between samples
                
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const ModelTrace& x);                                //!< Overloaded output operator
    
}

#endif
