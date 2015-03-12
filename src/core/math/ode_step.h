//
//  ode_step.h
//  revbayes
//
//  Created by Sebastian Hoehna on 3/10/15.
//  Copyright (c) 2015 Sebastian Hoehna. All rights reserved.
//

#ifndef revbayes_ode_step_h
#define revbayes_ode_step_h

template<
class Container ,
class Time = double ,
class Traits = container_traits< Container >
>
class ode_step
{
    // provide basic typedefs
    //
public:
    
    typedef unsigned short order_type;
    typedef Time time_type;
    typedef Traits traits_type;
    typedef typename traits_type::container_type container_type;
    typedef typename traits_type::value_type value_type;
    
    // public interface
public:
    
    // return the order of the stepper
    order_type order_step() const;
    
    // standard constructor
    ode_step( void );
    
    // adjust the size of the internal arrays
    void adjust_size( const container_type &x );
    
    // performs one step
    template< class DynamicalSystem >
    void do_step( DynamicalSystem &system ,
                 container_type &x ,
                 const container_type &dxdt ,
                 time_type t ,
                 time_type dt );
    
    // performs one step
    template< class DynamicalSystem >
    void do_step( DynamicalSystem &system ,
                 container_type &x ,
                 time_type t ,
                 time_type dt );
};


#endif
