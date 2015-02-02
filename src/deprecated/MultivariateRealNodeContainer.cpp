/* 
 * File:   MultivariateRealNodeContainer.cpp
 * Author: nl
 * 
 * Created on 16 juillet 2014, 20:17
 */

#include "MultivariateRealNodeContainer.h"

#include "RbException.h"
#include "RbOptions.h"
#include "TypedDagNode.h"

#include <cmath>

#include <iomanip>

#include <sstream>

using namespace RevBayesCore;

// Declarations
std::ostream& operator<<(std::ostream& o, const MultivariateRealNodeContainer& x);


/* Default constructor */
MultivariateRealNodeContainer::MultivariateRealNodeContainer(void) : MatrixReal() {
    
}

/* constructor based on a timetree and a dimension for the process */
MultivariateRealNodeContainer::MultivariateRealNodeContainer(const TimeTree* intree, size_t indim) : MatrixReal(intree->getNumberOfNodes(), indim, 0.0), tree(intree), clampVector(intree->getNumberOfNodes(),std::vector<bool>(indim,false))  {
    
}


/* Copy constructor */
MultivariateRealNodeContainer::MultivariateRealNodeContainer(const MultivariateRealNodeContainer& p) : MatrixReal(p), tree(p.getTimeTree()), clampVector(p.clampVector) {
    
}


/* Destructor */
MultivariateRealNodeContainer::~MultivariateRealNodeContainer(void) {
}


MultivariateRealNodeContainer& MultivariateRealNodeContainer::operator=(const MultivariateRealNodeContainer &p) {
    
    if (this != &p) {
        MatrixReal::operator=(p);
        tree = p.getTimeTree();
        clampVector = p.clampVector;
    }
    
    return *this;
}


/* Clone function */
MultivariateRealNodeContainer* MultivariateRealNodeContainer::clone(void) const {
    
    return new MultivariateRealNodeContainer(*this);
}


void MultivariateRealNodeContainer::executeMethod(const std::string &n, const std::vector<const DagNode *> &args, double &rv) const
{
    
    if ( n == "mean" )
    {
        const TypedDagNode< int >* k = static_cast<const TypedDagNode<int> *>( args[0] );
        rv = getMean(k->getValue()-1);
    }
    else if ( n == "tipMean" )
    {
        const TypedDagNode< int >* k = static_cast<const TypedDagNode<int> *>( args[0] );
        rv = getMeanOverTips(k->getValue()-1);
    }
    else if ( n == "stdev" )
    {
        const TypedDagNode< int >* k = static_cast<const TypedDagNode<int> *>( args[0] );
        rv = getStdev(k->getValue()-1);
    }
    else if ( n == "rootVal" )
    {
        const TypedDagNode< int >* k = static_cast<const TypedDagNode<int> *>( args[0] );
        rv = getRootVal(k->getValue()-1);
    }
    else    {
        throw RbException("A MultivariateRealNodeContainer object does not have a member method called '" + n + "'.");
    }
}

bool MultivariateRealNodeContainer::isClamped(size_t index, size_t k) const   {
    return clampVector[index][k];
}

void MultivariateRealNodeContainer::clampAt(const ContinuousCharacterData* data, size_t k, size_t l) {

    recursiveClampAt(getTimeTree()->getRoot(),data,k-1,l-1);
}


void MultivariateRealNodeContainer::recursiveClampAt(const TopologyNode& from, const ContinuousCharacterData* data, size_t k, size_t l) {
 
    if (from.isTip())   {
        
        // get taxon index
        size_t index = from.getIndex();
        std::string taxon = getTimeTree()->getTipNames()[index];
        size_t dataindex = data->getIndexOfTaxon(taxon);
        
        if (data->getCharacter(dataindex,l) != -1000) {
           (*this)[index][k] = data->getCharacter(dataindex,l);
            clampVector[index][k] = true;
        }
        else    {
            std::cerr << "taxon : " << taxon << " is missing for trait " << l+1 << '\n';
        }
    }

    // propagate forward
    size_t numChildren = from.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i) {
        recursiveClampAt(from.getChild(i),data,k,l);
    }    
}


void MultivariateRealNodeContainer::printBranchContrasts(std::ostream& os) const  {

    MatrixRealSymmetric c(getDim());
        
    for (size_t i=0; i<getDim(); i++)   {
        for (size_t j=0; j<getDim(); j++)   {
            c[i][j] = 0;
        }
    }

    int n = 0;
    recursiveGetBranchContrasts(getTimeTree()->getRoot(),c,n);
    
    for (size_t i=0; i<getDim(); i++)   {
        for (size_t j=0; j<getDim(); j++)   {
            c[i][j] /= (getTimeTree()->getNumberOfNodes() - 1);
        }
    }

    for (size_t i=0; i<getDim(); i++)   {
        for (size_t j=i+1; j<getDim(); j++)   {
            os << c[i][j] << '\t';
        }
    }
    for (size_t i=0; i<getDim(); i++)   {
        os << c[i][i];
        if (i < getDim() -1)    {
            os << '\t';
        }
    }    
}

MatrixRealSymmetric MultivariateRealNodeContainer::getBranchContrasts(int& n) const  {

    MatrixRealSymmetric c(getDim());
        
    for (size_t i=0; i<getDim(); i++)   {
        for (size_t j=0; j<getDim(); j++)   {
            c[i][j] = 0;
        }
    }

    n = 0;
    recursiveGetBranchContrasts(getTimeTree()->getRoot(),c,n);

    return c;
}

void MultivariateRealNodeContainer::recursiveGetBranchContrasts(const TopologyNode& from, MatrixRealSymmetric& c, int& n)  const   {

    if (! from.isRoot())    {

        std::vector<double> tmp(getDim());

        for (size_t i = 0; i < getDim(); i++) {
            tmp[i] = ((*this)[from.getIndex()][i] - (*this)[from.getParent().getIndex()][i]) / sqrt(from.getBranchLength());
        }

        for (size_t i = 0; i < getDim(); i++) {
            for (size_t j = 0; j < getDim(); j++) {
                c[i][j] += tmp[i] * tmp[j];
            }
        }
        n++;
    }
    
    // propagate forward
    size_t numChildren = from.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i) {
        recursiveGetBranchContrasts(from.getChild(i),c,n);
    }    
}

double MultivariateRealNodeContainer::getRootVal(int k) const {
    
        return (*this)[getTimeTree()->getRoot().getIndex()][k];
}

double MultivariateRealNodeContainer::getMean(int k) const {
    
    int n = 0;
    double e1 = 0;
    double e2 = 0;
    recursiveGetStats(k, getTimeTree()->getRoot(), e1, e2, n);
    e1 /= n;
    e2 /= n;
    e2 -= e1 * e1;
    return e1;
}

double MultivariateRealNodeContainer::getMeanOverTips(int k) const {
    
    int n = 0;
    double e1 = 0;
    double e2 = 0;
    recursiveGetStatsOverTips(k, getTimeTree()->getRoot(), e1, e2, n);
    e1 /= n;
    e2 /= n;
    e2 -= e1 * e1;
    return e1;
}

double MultivariateRealNodeContainer::getStdev(int k) const {
    
    int n = 0;
    double e1 = 0;
    double e2 = 0;
    recursiveGetStats(k, getTimeTree()->getRoot(), e1, e2, n);
    e1 /= n;
    e2 /= n;
    e2 -= e1 * e1;
    return sqrt(e2);
}


void MultivariateRealNodeContainer::recursiveGetStats(int k, const TopologyNode& from, double& e1, double& e2, int& n) const {

    double tmp = (*this)[from.getIndex()][k];

    n++;
    e1 += tmp;
    e2 += tmp * tmp;
    
    // propagate forward
    size_t numChildren = from.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i) {
        recursiveGetStats(k,from.getChild(i),e1,e2,n);
    }
    
}


void MultivariateRealNodeContainer::recursiveGetStatsOverTips(int k, const TopologyNode& from, double& e1, double& e2, int& n) const {

    if(from.isTip())   {
        double tmp = (*this)[from.getIndex()][k];

        n++;
        e1 += tmp;
        e2 += tmp * tmp;
    }
    // propagate forward
    size_t numChildren = from.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i) {
        recursiveGetStatsOverTips(k,from.getChild(i),e1,e2,n);
    }
    
}

std::string MultivariateRealNodeContainer::getNewick(int k) const {

    return recursiveGetNewick(getTimeTree()->getRoot(),k-1);
}

std::string MultivariateRealNodeContainer::recursiveGetNewick(const TopologyNode& from, int k) const {

    std::ostringstream s;
    
    if (from.isTip())   {
        s << getTimeTree()->getTipNames()[from.getIndex()] << "_";
    }
    else    {
        s << "(";
        // propagate forward
        size_t numChildren = from.getNumberOfChildren();
        for (size_t i = 0; i < numChildren; ++i) {
            s << recursiveGetNewick(from.getChild(i),k);
            if (i < numChildren-1)  {
                s << ",";
            }
        }
        s << ")";
    }
    s << (*this)[from.getIndex()][k];
    if (!from.isRoot()) {
        s << ":";
        s << getTimeTree()->getBranchLength(from.getIndex());
    }
    
    return s.str();
}


std::string MultivariateRealNodeContainer::getNewick() const {

    std::ostringstream s;
    s << recursiveGetNewick(getTimeTree()->getRoot());
    s << ";";
    return s.str();
}

std::string MultivariateRealNodeContainer::recursiveGetNewick(const TopologyNode& from) const {

    std::ostringstream s;
    
    if (from.isTip())   {
        s << getTimeTree()->getTipNames()[from.getIndex()] << "_";
    }
    else    {
        s << "(";
        // propagate forward
        size_t numChildren = from.getNumberOfChildren();
        for (size_t i = 0; i < numChildren; ++i) {
            s << recursiveGetNewick(from.getChild(i));
            if (i < numChildren-1)  {
                s << ",";
            }
        }
        s << ")";
    }
    s << "[";
    for (size_t k=0; k<getDim(); k++)   {
        s << "&X" << k+1 << "=";
        s << (*this)[from.getIndex()][k];
        if (k < getDim() -1)    {
            s << ",";
        }
    }
    s << "]";
//    if (!from.isRoot()) {
        s << ":";
        s << getTimeTree()->getBranchLength(from.getIndex());
//    }
    
    return s.str();
}

std::ostream& RevBayesCore::operator<<(std::ostream& os, const MultivariateRealNodeContainer& x) {

    os << x.getNewick();
    return os;
}

