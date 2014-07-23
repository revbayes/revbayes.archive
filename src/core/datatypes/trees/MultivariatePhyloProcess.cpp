/* 
 * File:   MultivariatePhyloProcess.cpp
 * Author: nl
 * 
 * Created on 16 juillet 2014, 20:17
 */

#include "MultivariatePhyloProcess.h"

#include "RbException.h"
#include "RbOptions.h"
#include "TypedDagNode.h"

#include <cmath>

#include <iomanip>

using namespace RevBayesCore;

// Declarations
std::ostream& operator<<(std::ostream& o, const MultivariatePhyloProcess& x);


/* Default constructor */
MultivariatePhyloProcess::MultivariatePhyloProcess(void) : MatrixReal() {
    
}

/* constructor based on a timetree and a dimension for the process */
MultivariatePhyloProcess::MultivariatePhyloProcess(const TimeTree* intree, size_t indim) : MatrixReal(intree->getNumberOfNodes(), indim, 0.0), tree(intree), clampVector(intree->getNumberOfNodes(),std::vector<bool>(indim,false))  {
    
}


/* Copy constructor */
MultivariatePhyloProcess::MultivariatePhyloProcess(const MultivariatePhyloProcess& p) : MatrixReal(p), tree(p.getTimeTree()), clampVector(p.clampVector) {
    
}


/* Destructor */
MultivariatePhyloProcess::~MultivariatePhyloProcess(void) {
}


MultivariatePhyloProcess& MultivariatePhyloProcess::operator=(const MultivariatePhyloProcess &p) {
    
    if (this != &p) {
        MatrixReal::operator=(p);
        tree = p.getTimeTree();
    }
    
    return *this;
}


/* Clone function */
MultivariatePhyloProcess* MultivariatePhyloProcess::clone(void) const {
    
    return new MultivariatePhyloProcess(*this);
}


void MultivariatePhyloProcess::executeMethod(const std::string &n, const std::vector<const DagNode *> &args, double &rv) const
{
    
    /*
    if ( n == "mean" )
    {
        const TypedDagNode< int >* k = static_cast<const TypedDagNode<int> *>( args[0] );
        rv = getMean(k->getValue());
    }
    */
    throw RbException("A MultivariatePhyloProcess object does not have a member method called '" + n + "'.");
    
}

bool MultivariatePhyloProcess::isClamped(size_t index, size_t k) const   {
    return clampVector[index][k];
}

void MultivariatePhyloProcess::clampAt(const ContinuousCharacterData* data, size_t k, size_t l) {

    std::cerr << "clamp\n";
    std::cerr << k << '\t' << l << '\n';
    recursiveClampAt(getTimeTree()->getRoot(),data,k-1,l-1);
    std::cerr << "clamp ok\n";
}


void MultivariatePhyloProcess::recursiveClampAt(const TopologyNode& from, const ContinuousCharacterData* data, size_t k, size_t l) {
 
    if (from.isTip())   {
        
        // get taxon index
        size_t index = from.getIndex();

        std::string taxon = data->getTaxonNameWithIndex(index);
        
        std::cerr << index << '\t' << taxon << '\t';
        std::cerr << data->getCharacter(index,l).getMean() << '\t' << data->getCharacter(index,l).getVariance() << '\n';
        
        if (data->getCharacter(index,l).getMean() != -1000) {
           (*this)[index][k] = data->getCharacter(index,l).getMean();
            clampVector[index][k] = true;
        }
    }
    else    {
        std::cerr << ".";
    }

    // propagate forward
    size_t numChildren = from.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i) {
        recursiveClampAt(from.getChild(i),data,k,l);
    }    
}


void MultivariatePhyloProcess::printBranchContrasts(std::ostream& os) const  {

    std::vector<std::vector<double> > c(getDim(), std::vector<double>(getDim(),0) );
        
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

void MultivariatePhyloProcess::recursiveGetBranchContrasts(const TopologyNode& from, std::vector<std::vector<double> >& c, int& n)  const   {

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

double MultivariatePhyloProcess::getRootVal(int k) const {
    
        return (*this)[getTimeTree()->getRoot().getIndex()][k];
}

double MultivariatePhyloProcess::getMean(int k) const {
    
    int n = 0;
    double e1 = 0;
    double e2 = 0;
    recursiveGetStats(k, getTimeTree()->getRoot(), e1, e2, n);
    e1 /= n;
    e2 /= n;
    e2 -= e1 * e1;
    return e1;
}

double MultivariatePhyloProcess::getStdev(int k) const {
    
    int n = 0;
    double e1 = 0;
    double e2 = 0;
    recursiveGetStats(k, getTimeTree()->getRoot(), e1, e2, n);
    e1 /= n;
    e2 /= n;
    e2 -= e1 * e1;
    return sqrt(e2);
}


void MultivariatePhyloProcess::recursiveGetStats(int k, const TopologyNode& from, double& e1, double& e2, int& n) const {

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


std::ostream& RevBayesCore::operator<<(std::ostream& os, const MultivariatePhyloProcess& x) {

    os << std::fixed;
    os << std::setprecision(4);
        
    x.printBranchContrasts(os);
    os << '\t';
    
    for (size_t i=0; i<x.getDim(); i++)   {
        os << x.getMean(i) << '\t';
    }
    
    for (size_t i=0; i<x.getDim(); i++)   {
        os << x.getStdev(i) << '\t';
    }    
    
    for (size_t i=0; i<x.getDim(); i++)   {
        os << x.getRootVal(i);
        if (i < x.getDim() -1)  {
            os << '\t';
        }
    }    
    
    return os;
}

