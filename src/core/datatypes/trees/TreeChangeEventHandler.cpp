//
//  TreeChangeEventHandler.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/27/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "TreeChangeEventHandler.h"
#include "TreeChangeEventListener.h"

#include <iostream>

using namespace RevBayesCore;

TreeChangeEventHandler::TreeChangeEventHandler(void) {
}


TreeChangeEventHandler::~TreeChangeEventHandler(void) {
    
}


void TreeChangeEventHandler::addListener(TreeChangeEventListener *l) 
{
    listeners.insert( l );
    
}



void TreeChangeEventHandler::fire(const TopologyNode &n) 
{

    for (std::set<TreeChangeEventListener*>::iterator it = listeners.begin(); it != listeners.end(); ++it) 
    {
        TreeChangeEventListener *l = *it;
        l->fireTreeChangeEvent( n );
    }
}


const std::set<TreeChangeEventListener*>& TreeChangeEventHandler::getListeners( void ) const 
{

    return listeners;
}


void TreeChangeEventHandler::removeListener(TreeChangeEventListener *l) {
    std::set<TreeChangeEventListener*>::iterator pos = listeners.find( l );
    if ( pos != listeners.end() ) {
        listeners.erase( l );
    }
}
