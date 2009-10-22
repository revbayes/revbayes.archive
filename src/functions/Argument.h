/*
 * Argument.h
 *
 *  Created on: 21 okt 2009
 *      Author: Sebastian
 */

#ifndef ARGUMENT_H_
#define ARGUMENT_H_

#include <string>
#include "RbObject.h"
#include "DAGNode.h"

class Argument : public RbObject{
    public:
        Argument(std::string& l, DAGNode* n);
        Argument(const Argument& a);                                                  //!< copy constructor
        virtual ~Argument();

        // implemented abstract/virtual functions from base classes
        RbObject*           clone(void) const;                                  //!< clone this object
        void                print(std::ostream &c) const;                       //!< Print the value to ostream c
        void                dump(std::ostream& c);                              //!< Dump to ostream c
        void                resurrect(const RbDumpState& x);                    //!< Resurrect from dumped state
        bool                operator==(const RbObject& o) const;                //!< Comparison
        bool                operator==(const Argument& o) const;                //!< Comparison


        // getter and setter
        std::string&        getLabel() const;
        void                setLabel(std::string& l);
        DAGNode*            getDAGNode() const;                                       //!< whatever that is?!?

    private:
        std::string&    label;
        DAGNode*        value;
};

#endif /* ARGUMENT_H_ */
