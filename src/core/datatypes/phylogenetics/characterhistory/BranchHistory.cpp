//
//  BranchHistory.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "BranchHistory.h"
#include "CharacterEvent.h"
#include "CharacterEventCompare.h"

#include <iostream>
#include <iomanip>

using namespace RevBayesCore;


//BranchHistory::BranchHistory(void) : numCharacters(0), numStates(0), isTip(false), isRoot(false) { }

BranchHistory::BranchHistory(size_t nc, size_t ns, size_t idx) : numCharacters(nc), numStates(ns), index(idx), redrawChildCharacters(true), redrawParentCharacters(true), redrawHistory(true), clampChildCharacters(false)
{
    parentCharacters.resize(numCharacters);
    childCharacters.resize(numCharacters);
    
    for (size_t i = 0; i < numCharacters; i++)
    {
        parentCharacters[i] = new CharacterEvent(i,0,0.0);
        childCharacters[i] = new CharacterEvent(i,0,1.0);
    }
}

BranchHistory::BranchHistory(size_t nc, size_t ns, size_t idx, std::set<int> sc) : numCharacters(nc), numStates(ns), index(idx), redrawChildCharacters(true), redrawParentCharacters(true), redrawHistory(true),  clampChildCharacters(false), sampleChildCharacters(sc)
{
    parentCharacters.resize(numCharacters);
    childCharacters.resize(numCharacters);
    
    for (size_t i = 0; i < numCharacters; i++)
    {
        parentCharacters[i] = new CharacterEvent(i,0,0.0);
        childCharacters[i] = new CharacterEvent(i,0,1.0);
    }
}



BranchHistory::BranchHistory(const BranchHistory& m)
{
    if (this != &m) {
        
        numStates = m.numStates;
        numCharacters = m.numCharacters;
        parentCharacters = m.parentCharacters;
        childCharacters = m.childCharacters;
        history = m.history;
        redrawChildCharacters = m.redrawChildCharacters;
        redrawParentCharacters = m.redrawParentCharacters;
        redrawHistory = m.redrawHistory;
        index = m.index;
        clampChildCharacters = m.clampChildCharacters;
        sampleChildCharacters = m.sampleChildCharacters;
    }
    
}

BranchHistory::~BranchHistory(void)
{
//    for (size_t i = 0; i < parentCharacters.size(); i++)
//        delete parentCharacters[i];
//    for (size_t i = 0; i < childCharacters.size(); i++)
//        delete childCharacters[i];
//    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it;

}

BranchHistory& BranchHistory::operator=(const BranchHistory &bh) {
    
    if (this != &bh) {
        
        numStates = bh.numStates;
        numCharacters = bh.numCharacters;
        parentCharacters = bh.parentCharacters;
        childCharacters = bh.childCharacters;
        history = bh.history;
        redrawChildCharacters = bh.redrawChildCharacters;
        redrawParentCharacters = bh.redrawParentCharacters;
        redrawHistory = bh.redrawHistory;
        index = bh.index;
        clampChildCharacters = bh.clampChildCharacters;
        sampleChildCharacters = bh.sampleChildCharacters;
    }
    
    return *this;
}



bool BranchHistory::operator<(const BranchHistory& m) const {
    return (this < &m);
}

BranchHistory* BranchHistory::clone(void) const
{
    return new BranchHistory(*this);
}

std::vector<CharacterEvent*>& BranchHistory::getParentCharacters(void)
{
    return parentCharacters;
}

std::vector<CharacterEvent*>& BranchHistory::getChildCharacters(void)
{
    return childCharacters;
}


const std::vector<CharacterEvent*>& BranchHistory::getParentCharacters(void) const
{
    return parentCharacters;
}

const std::vector<CharacterEvent*>& BranchHistory::getChildCharacters(void) const
{
    return childCharacters;
}

std::multiset<CharacterEvent*,CharacterEventCompare>& BranchHistory::getHistory(void)
{
    return history;
}

const std::multiset<CharacterEvent*,CharacterEventCompare>& BranchHistory::getHistory(void) const
{
    return history;
}

void BranchHistory::clearEvents(void)
{
    history.clear();
}

void BranchHistory::clearEvents(const std::set<size_t>& indexSet)
{
    std::set<size_t>::iterator it_idx;
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h, it_tmp;

    // for each event in history, delete if index matches indexSet
    for (it_h = history.begin(); it_h != history.end(); )
    {
        if ( indexSet.find( (*it_h)->getIndex() ) != indexSet.end() )
        {
//            std::cout << "found one! " << *it_h << " " << (*it_h)->getIndex() << " " << (*it_h)->getState() << " " << (*it_h)->getTime() << "\n";            
            
            it_tmp = it_h;
            ++it_tmp;
            history.erase(it_h);
            //delete *it_h;
            it_h = it_tmp;
            //std::cout << "found!\n";
        }
        else
        {
            ++it_h;
        }
        //std::cout << "sz " << history.size() << "\n";
    }
}

void BranchHistory::addEvent(CharacterEvent* evt)
{
    history.insert(evt);
}

void BranchHistory::removeEvent(CharacterEvent* evt)
{
    history.erase(evt);
}

void BranchHistory::updateHistory(const std::multiset<CharacterEvent*,CharacterEventCompare>& updateSet, const std::set<CharacterEvent*>& parentSet, const std::set<CharacterEvent*>& childSet, const std::set<size_t>& indexSet)
{
    
    /*
    // erase events on branchHistory for indices in indexSet
    clearEvents(indexSet);
    
    // insert elements into history
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
    for (it_h = updateSet.begin(); it_h != updateSet.end(); it_h++)
        history.insert(*it_h);
    */
    
    updateHistory(updateSet, indexSet);
    
    // update events on terminal vectors
    std::set<CharacterEvent*>::iterator it_idx;
    for (it_idx = parentSet.begin(); it_idx != parentSet.end(); it_idx++)
        parentCharacters[ (*it_idx)->getIndex() ] = *it_idx;
    for (it_idx = childSet.begin(); it_idx != childSet.end(); it_idx++)
        childCharacters[ (*it_idx)->getIndex() ] = *it_idx;
    
}

void BranchHistory::updateHistory(const std::multiset<CharacterEvent*,CharacterEventCompare>& updateSet, const std::set<size_t>& indexSet)
{
    // erase events on branchHistory for indices in indexSet
    clearEvents(indexSet);
    
    // insert elements into history
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
    for (it_h = updateSet.begin(); it_h != updateSet.end(); it_h++)
        history.insert(*it_h);

}


void BranchHistory::setChildCharacters(const std::vector<CharacterEvent*>& s)
{
    std::vector<CharacterEvent*>::const_iterator it;
    for (it = s.begin(); it != s.end(); it++)
        childCharacters[ (*it)->getIndex() ] = *it;//new CharacterEvent(**it);
}

void BranchHistory::setChildCharacters(const std::set<CharacterEvent*>& s)
{
    std::set<CharacterEvent*>::iterator it;
    for (it = s.begin(); it != s.end(); it++)
        childCharacters[ (*it)->getIndex() ] = *it;//new CharacterEvent(**it);
}

void BranchHistory::setParentCharacters(const std::vector<CharacterEvent*>& s)
{
    std::vector<CharacterEvent*>::const_iterator it;
    for (it = s.begin(); it != s.end(); it++)
        parentCharacters[ (*it)->getIndex() ] = *it;//new CharacterEvent(**it);
}

void BranchHistory::setParentCharacters(const std::set<CharacterEvent*>& s)
{
    std::set<CharacterEvent*>::iterator it;
    for (it = s.begin(); it != s.end(); it++)
        parentCharacters[ (*it)->getIndex() ] = *it;//new CharacterEvent(**it);
}


void BranchHistory::setHistory(const std::set<CharacterEvent*,CharacterEventCompare>& s)
{
    history.clear();
    for (std::set<CharacterEvent*,CharacterEventCompare>::iterator it = s.begin(); it != s.end(); it++)
        history.insert(*it);
}

void BranchHistory::setHistory(const std::multiset<CharacterEvent*,CharacterEventCompare>& s)
{
    history = s;
}


void BranchHistory::print(void) const
{
    std::set<CharacterEvent*,CharacterEventCompare>::iterator it_h;
    std::vector<CharacterEvent*>::iterator it_v;
    
//    std::cout << parentCharacters.size() << "\n";
//    for (size_t i = 0; i < parentCharacters.size(); i++)
//        std::cout << parentCharacters[i] << " ";
//    std::cout << "\n";
    
    std::vector<CharacterEvent*> tmp = parentCharacters;
    
    std::cout << "BranchHistory " << index << " size=" << history.size() << "  " << this << "\n";
    std::cout << "                             ";
    for (size_t i = 0; i < numCharacters; i++)
    {
        if (i % 10 == 0) std::cout << ".";
        else std::cout << " ";
    }
    std::cout << "\n";
    std::cout << "                       0.0 : ";
    for (it_v = parentCharacters.begin(); it_v != parentCharacters.end(); it_v++)
    {
//        std::cout << (*it_v)->getState();
        std::cout << (*it_v)->getStateStr();
    }
    std::cout << "\n";
    
    for (it_h = history.begin(); it_h != history.end(); it_h++)
    {
        std::cout << *it_h << "   ";
        std::cout << std::setw(12) << std::setprecision(6) << (*it_h)->getTime() << " : ";
        tmp[ (*it_h)->getIndex() ] = *it_h;
        for (size_t i = 0; i < numCharacters; i++)
        {
            if (i != (*it_h)->getIndex())
                std::cout << " ";
            else
                std::cout << (*it_h)->getStateStr();
//                std::cout << (*it_h)->getState();
            //std::cout << " ";
        }
        std::cout << "\n";
    }
    std::cout << "                       1.0 : ";
    for (it_v = childCharacters.begin(); it_v != childCharacters.end(); it_v++)
    {
//        std::cout << (*it_v)->getState();
        std::cout << (*it_v)->getStateStr();
    }
    std::cout << "\n";
    std::cout << "                             ";
    for (size_t i = 0; i < numCharacters; i++)
    {
        if (i % 10 == 0) std::cout << ".";
        else std::cout << " ";
    }
    std::cout << "\n";
    ;
}

const std::set<size_t>& BranchHistory::getDirtyCharacters(void)
{
    return dirtyCharacters;
}

void BranchHistory::setDirtyCharacters(const std::set<size_t>& s)
{
    dirtyCharacters = s;
}

bool BranchHistory::getRedrawChildCharacters(void)
{
    return redrawChildCharacters;
}

bool BranchHistory::getRedrawParentCharacters(void)
{
    return redrawParentCharacters;
}

bool BranchHistory::getRedrawHistory(void)
{
    return redrawHistory;
}

void BranchHistory::setRedrawChildCharacters(bool tf)
{
    redrawChildCharacters = tf;
}

void BranchHistory::setRedrawParentCharacters(bool tf)
{
    redrawParentCharacters = tf;
}

void BranchHistory::setRedrawHistory(bool tf)
{
    redrawHistory = tf;
}

const size_t BranchHistory::getNumCharacters(void) const
{
    return numCharacters;
}

const size_t BranchHistory::getNumStates(void) const
{
    return numStates;
}

const size_t BranchHistory::getNumEvents(void) const
{
    return history.size();
}

const size_t BranchHistory::getIndex(void) const
{
    return index;
}

bool BranchHistory::isClampedChildCharacters(void)
{
    return clampChildCharacters;
}

void BranchHistory::setClampChildCharacters(bool tf)
{
    clampChildCharacters = tf;
}

const std::set<int>& BranchHistory::getSampleChildCharacters(void)
{
    return sampleChildCharacters;
}

void BranchHistory::setSampleChildCharacters(std::set<int> sc)
{
    sampleChildCharacters = sc;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const BranchHistory& x) {
    
    o << x.getNumEvents();
    
    return o;
}
