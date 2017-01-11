#include "BranchHistory.h"
#include "CharacterEvent.h"
#include "CharacterEventCompare.h"

#include <iostream>
#include <iomanip>

using namespace RevBayesCore;


//BranchHistory::BranchHistory(void) : n_characters(0), n_states(0), isTip(false), isRoot(false) { }

BranchHistory::BranchHistory(size_t nc, size_t ns, size_t idx) :
    n_characters(nc),
    n_states(ns),
    branch_index(idx)
{
    parentCharacters.resize(n_characters);
    childCharacters.resize(n_characters);

    for (size_t i = 0; i < n_characters; i++)
    {
        parentCharacters[i] = new CharacterEvent(i,0,0.0);
        childCharacters[i] = new CharacterEvent(i,0,1.0);
    }
}

BranchHistory::BranchHistory(size_t nc, size_t ns, size_t idx, std::set<int> sc) :
    n_characters(nc),
    n_states(ns),
    branch_index(idx)
{
    parentCharacters.resize(n_characters);
    childCharacters.resize(n_characters);

    for (size_t i = 0; i < n_characters; i++)
    {
        parentCharacters[i] = new CharacterEvent(i,0,0.0);
        childCharacters[i] = new CharacterEvent(i,0,1.0);
    }

}



BranchHistory::BranchHistory(const BranchHistory& m)
{

    if (this != &m)
    {

        n_states                = m.n_states;
        n_characters            = m.n_characters;
        parentCharacters        = m.parentCharacters;
        childCharacters         = m.childCharacters;
        history                 = m.history;
        branch_index            = m.branch_index;
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

BranchHistory& BranchHistory::operator=(const BranchHistory &bh)
{

    if (this != &bh)
    {

        n_states                = bh.n_states;
        n_characters            = bh.n_characters;
        parentCharacters        = bh.parentCharacters;
        childCharacters         = bh.childCharacters;
        history                 = bh.history;
        branch_index            = bh.branch_index;
    }

    return *this;
}



bool BranchHistory::operator<(const BranchHistory& m) const
{
    return (this < &m);
}


void BranchHistory::addEvent(CharacterEvent* evt)
{
    history.insert(evt);
}

bool BranchHistory::areEventTimesValid(const TopologyNode &node) const
{
    
    double lower_boundary = node.getAge();
    double upper_boundary = lower_boundary + node.getBranchLength();
    double event_age;
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it;
    for (it = history.begin(); it != history.end(); it++)
    {
        event_age = (*it)->getAge();
        if ( event_age > upper_boundary || event_age < lower_boundary)
        {
            return false;
        }
    }
    
    return true;
}

BranchHistory* BranchHistory::clone(void) const
{
    return new BranchHistory(*this);
}

const std::vector<CharacterEvent*>& BranchHistory::getChildCharacters(void) const
{
    return childCharacters;
}

std::vector<CharacterEvent*>& BranchHistory::getChildCharacters(void)
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

const size_t BranchHistory::getNumberCharacters(void) const
{
    return n_characters;
}

const size_t BranchHistory::getNumberStates(void) const
{
    return n_states;
}

const size_t BranchHistory::getNumberEvents(void) const
{
    return history.size();
}


std::vector<CharacterEvent*>& BranchHistory::getParentCharacters(void)
{
    return parentCharacters;
}


const std::vector<CharacterEvent*>& BranchHistory::getParentCharacters(void) const
{
    return parentCharacters;
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
        if ( indexSet.find( (*it_h)->getSiteIndex() ) != indexSet.end() )
        {
            it_tmp = it_h;
            ++it_tmp;
            history.erase(it_h);
            //delete *it_h;
            it_h = it_tmp;
        }
        else
        {
            ++it_h;
        }
    }
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
        parentCharacters[ (*it_idx)->getSiteIndex() ] = *it_idx;
    for (it_idx = childSet.begin(); it_idx != childSet.end(); it_idx++)
        childCharacters[ (*it_idx)->getSiteIndex() ] = *it_idx;

}

void BranchHistory::updateHistory(const std::multiset<CharacterEvent*,CharacterEventCompare>& updateSet, const std::set<size_t>& indexSet)
{
    // erase events on branchHistory for indices in indexSet
    clearEvents(indexSet);

    // insert elements into history
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
    for (it_h = updateSet.begin(); it_h != updateSet.end(); it_h++)
    {
        history.insert(*it_h);
    }

}

void BranchHistory::updateHistory(const std::multiset<CharacterEvent*,CharacterEventCompare>& updateSet)
{
    // erase events on branchHistory for indices in indexSet
    clearEvents();

    // insert elements into history
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
    for (it_h = updateSet.begin(); it_h != updateSet.end(); it_h++)
    {
        history.insert(*it_h);
    }

}

void BranchHistory::setChildCharacters(const std::vector<CharacterEvent*>& s)
{
    std::vector<CharacterEvent*>::const_iterator it;
    for (it = s.begin(); it != s.end(); it++)
    {
        childCharacters[ (*it)->getSiteIndex() ] = *it;//new CharacterEvent(**it);
    }

}

void BranchHistory::setChildCharacters(const std::set<CharacterEvent*>& s)
{
    std::set<CharacterEvent*>::iterator it;
    for (it = s.begin(); it != s.end(); it++)
    {
        childCharacters[ (*it)->getSiteIndex() ] = *it;//new CharacterEvent(**it);
    }

}

void BranchHistory::setParentCharacters(const std::vector<CharacterEvent*>& s)
{
    std::vector<CharacterEvent*>::const_iterator it;
    for (it = s.begin(); it != s.end(); it++)
    {
        parentCharacters[ (*it)->getSiteIndex() ] = *it;//new CharacterEvent(**it);
    }

}

void BranchHistory::setParentCharacters(const std::set<CharacterEvent*>& s)
{
    std::set<CharacterEvent*>::iterator it;
    for (it = s.begin(); it != s.end(); it++)
    {
        parentCharacters[ (*it)->getSiteIndex() ] = *it;//new CharacterEvent(**it);
    }

}


void BranchHistory::setHistory(const std::set<CharacterEvent*,CharacterEventCompare>& s)
{
    history.clear();
    for (std::set<CharacterEvent*,CharacterEventCompare>::iterator it = s.begin(); it != s.end(); it++)
    {
        history.insert(*it);
    }

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

    std::cout << "BranchHistory " << branch_index << " size=" << history.size() << "  " << this << "\n";
    std::cout << "                             ";
    for (size_t i = 0; i < n_characters; i++)
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
        std::cout << std::setw(12) << std::setprecision(6) << (*it_h)->getAge() << " : ";
        tmp[ (*it_h)->getSiteIndex() ] = *it_h;
        for (size_t i = 0; i < n_characters; i++)
        {
            if (i != (*it_h)->getSiteIndex())
            {
                std::cout << " ";
            }
            else
            {
                std::cout << (*it_h)->getStateStr();
            }

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
    for (size_t i = 0; i < n_characters; i++)
    {
        if (i % 10 == 0)
            std::cout << ".";
        else
            std::cout << " ";
    }
    std::cout << "\n";
    ;
}


const size_t BranchHistory::getBranchIndex(void) const
{
    return branch_index;
}


CharacterEvent* BranchHistory::getEvent(size_t i)
{
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it = history.begin();
    for (size_t j=0; j<i; ++j)
    {
        ++it;
    }

    return (*it);
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const BranchHistory& x)
{

    o << x.getNumberEvents();

    return o;
}
