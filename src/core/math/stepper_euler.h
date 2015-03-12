//
//  stepper_euler.h
//  revbayes
//
//  Created by Sebastian Hoehna on 3/10/15.
//  Copyright (c) 2015 Sebastian Hoehna. All rights reserved.
//

#ifndef revbayes_stepper_euler_h
#define revbayes_stepper_euler_h

#include "container_traits.h"

template<
class Container ,
class Time = double ,
class Traits = container_traits< Container >
>
class stepper_euler
{
    // provide basic typedefs
public:
    
    typedef unsigned short order_type;
    typedef Time time_type;
    typedef Traits traits_type;
    typedef typename traits_type::container_type container_type;
    typedef typename traits_type::value_type value_type;
    
    // private members
private:
    
    container_type m_dxdt;
    
    // public interface
public:
    
    // return the order of the stepper
    order_type order_step() const { return 1; }
    
    // standard constructor, m_dxdt is not adjusted
    stepper_euler( void )
    {
    }
    
    // contructor, which adjusts m_dxdt
    stepper_euler( const container_type &x )
    {
        adjust_size( x );
    }
    
    // adjust the size of m_dxdt
    void adjust_size( const container_type &x )
    {
        traits_type::adjust_size( x , m_dxdt );
    }
    
    // performs one step with the knowledge of dxdt(t)
    template< class DynamicalSystem >
    void do_step( DynamicalSystem &system ,
                 container_type &x ,
                 const container_type &dxdt ,
                 time_type t ,
                 time_type dt )
    {
        detail::it_algebra::increment( traits_type::begin(x) ,
                                      traits_type::end(x) ,
                                      traits_type::begin(dxdt) ,
                                      dt );
    }
    
    // performs one step
    template< class DynamicalSystem >
    void do_step( DynamicalSystem &system ,
                 container_type &x ,
                 time_type t ,
                 time_type dt )
    {
        system( x , m_dxdt , t );
        do_step( system , x , m_dxdt , t , dt );
    }
};


#endif
