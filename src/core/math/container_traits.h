//
//  container_traits.h
//  revbayes
//
//  Created by Sebastian Hoehna on 3/10/15.
//  Copyright (c) 2015 Sebastian Hoehna. All rights reserved.
//

#ifndef revbayes_container_traits_h
#define revbayes_container_traits_h

template< class Container >
struct container_traits
{
    typedef Container container_type;
    typedef typename container_type::value_type value_type;
    typedef typename container_type::iterator iterator;
    typedef typename container_type::const_iterator const_iterator;
    
    static void resize( const container_type &x , container_type &dxdt )
    { dxdt.resize( x.size() ); }
    static bool same_size( const container_type &x1 , const container_type &x2 )
    { return (x1.size() == x2.size()); }
    static void adjust_size( const container_type &x1 , container_type &x2 )
    { if( !same_size( x1 , x2 ) ) resize( x1 , x2 ); }
    
    static iterator begin( container_type &x ) { return x.begin(); }
    static const_iterator begin( const container_type &x ) { return x.begin(); }
    static iterator end( container_type &x ) { return x.end(); }
    static const_iterator end( const container_type &x ) { return x.end(); }
};

#endif
