//
//  CharacterEventCompare.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__CharacterEventCompare__
#define __rb_mlandis__CharacterEventCompare__


namespace RevBayesCore {

    class CharacterEvent;
    class CharacterEventCompare
    {
    public:
        bool operator()(CharacterEvent* lhs, CharacterEvent* rhs) const { return (*lhs > *rhs); }
    protected:
        // ...
    private:
        // ...

    };
}

#endif /* defined(__rb_mlandis__CharacterEventCompare__) */
