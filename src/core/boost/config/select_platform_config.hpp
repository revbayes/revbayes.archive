//  Boost compiler configuration selection header file

//  (C) Copyright John Maddock 2001 - 2002. 
//  (C) Copyright Jens Maurer 2001. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

// locate which platform we are on and define BOOST_PLATFORM_CONFIG as needed.
// Note that we define the headers to include using "header_name" not
// <header_name> in order to prevent macro expansion within the header
// name (for example "linux" is a macro on linux systems).

#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__) || defined(__GLIBC__) 
// linux, also other platforms (Hurd etc) that use GLIBC, should these really have their own config headers though?
#  define BOOST_PLATFORM_CONFIG "linux.hpp"

#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
// BSD:
#  define BOOST_PLATFORM_CONFIG "bsd.hpp"

#elif defined(sun) || defined(__sun)
// solaris:
#  define BOOST_PLATFORM_CONFIG "solaris.hpp"

#elif defined(__sgi)
// SGI Irix:
#  define BOOST_PLATFORM_CONFIG "irix.hpp"

#elif defined(__hpux)
// hp unix:
#  define BOOST_PLATFORM_CONFIG "hpux.hpp"

#elif defined(__CYGWIN__)
// cygwin is not win32:
#  define BOOST_PLATFORM_CONFIG "cygwin.hpp"

#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
// win32:
#  define BOOST_PLATFORM_CONFIG "win32.hpp"

#elif defined(__BEOS__)
// BeOS
#  define BOOST_PLATFORM_CONFIG "beos.hpp"

#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
// MacOS
#  define BOOST_PLATFORM_CONFIG "macos.hpp"

#elif defined(__IBMCPP__) || defined(_AIX)
// IBM
#  define BOOST_PLATFORM_CONFIG "aix.hpp"

#elif defined(__amigaos__)
// AmigaOS
#  define BOOST_PLATFORM_CONFIG "amigaos.hpp"

#elif defined(__QNXNTO__)
// QNX:
#  define BOOST_PLATFORM_CONFIG "qnxnto.hpp"

#elif defined(__VXWORKS__)
// vxWorks:
#  define BOOST_PLATFORM_CONFIG "vxworks.hpp"

#elif defined(__SYMBIAN32__) 
// Symbian: 
#  define BOOST_PLATFORM_CONFIG "symbian.hpp" 

#elif defined(__VMS) 
// VMS:
#  define BOOST_PLATFORM_CONFIG "vms.hpp" 
#else

#  if defined(unix) \
      || defined(__unix) \
      || defined(_XOPEN_SOURCE) \
      || defined(_POSIX_SOURCE)

   // generic unix platform:

#  ifndef BOOST_HAS_UNISTD_H
#     define BOOST_HAS_UNISTD_H
#  endif

#  include <posix_features.hpp>

#  endif

#  if defined (BOOST_ASSERT_CONFIG)
      // this must come last - generate an error if we don't
      // recognise the platform:
#     error "Unknown platform - please configure and report the results to boost.org"
#  endif

#endif



