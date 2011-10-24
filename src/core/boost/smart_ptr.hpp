//
//  smart_ptr.hpp
//
//  For convenience, this header includes the rest of the smart
//  pointer library headers.
//
//  Copyright (c) 2003 Peter Dimov  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//  http://www.boost.org/libs/smart_ptr/smart_ptr.htm
//

#include "config.hpp"

#include "scoped_ptr.hpp"
#include "scoped_array.hpp"
#include "shared_ptr.hpp"
#include "shared_array.hpp"

#if !defined(BOOST_NO_MEMBER_TEMPLATES) || defined(BOOST_MSVC6_MEMBER_TEMPLATES)
# include "weak_ptr.hpp"
# include "intrusive_ptr.hpp"
# include "enable_shared_from_this.hpp"
#endif
