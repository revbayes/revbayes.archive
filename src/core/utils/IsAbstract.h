#ifndef IsAbstract_H
#define IsAbstract_H

template<class T>
struct IsAbstract
{
    // Inspired by boost/type_traits/is_abstract.hpp
    // Deduction fails if T is void, function type,
    // reference type (14.8.2/2)or an abstract class type
    // according to review status issue #337
    template<class U>
    static char check_sig(U (*)[1]);
    template<class U>
    static short check_sig(...);
    //
    enum { Is = sizeof(IsAbstract<T>::check_sig<T>(0)) - 1 };
};

#endif
