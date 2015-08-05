//
//  BiogeographicCladoEvent.h
//  revbayes-proj
//
//  Created by Michael Landis on 2/2/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__BiogeographicCladoEvent__
#define __revbayes_proj__BiogeographicCladoEvent__

namespace RevBayesCore {
    namespace BiogeographicCladoEvent {
        const unsigned SYMPATRY_NARROW     = 0;         // A    -> A    | A
        const unsigned SYMPATRY_WIDESPREAD = 1;         // ABCD -> ABCD | ABCD
        const unsigned SYMPATRY_SUBSET     = 2;         // ABCD -> ABCD | A
        const unsigned ALLOPATRY           = 3;         // ABCD -> AB   |   CD
        const unsigned PARAPATRY           = 4;         // ABCD -> ABC  |  BCD
        const unsigned JUMP_DISPERSAL      = 5;         // ABC  -> ABC  |    D
    };
};


#endif /* defined(__revbayes_proj__BiogeographicCladoEvent__) */
