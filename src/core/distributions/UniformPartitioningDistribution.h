#ifndef UniformPartitioningDistribution_H
#define UniformPartitioningDistribution_H

#include "MemberObject.h"
#include "RbVector.h"
#include "Simplex.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    
    /**
     * The uniform partitioning distribution samples uniformly distributed random partitions
     * of a set.
     *
     * The input is a vector of non-zero values that represents the most finely fragmented
     * partition of the set. There are B_n possible partitions, where n is the number of 
     * elements and B_n is the Bell number for n. Each of the B_n partitions have equal 
     * probability.
     *
     * Additionally, zero values can be added, so we now include all partitions of n elements
     * where each subset can either have one of the n non-zero values or a zero value. This means 
     * there are now B_{n+1} possible partitions, each of which have equal probability.
     *
     * Will Freyman 6/29/17
     *
     */
    template <class valueType>
    class UniformPartitioningDistribution : public TypedDistribution< RbVector<valueType> >, public MemberObject<int> {
        
    public:
        // constructor(s)
        UniformPartitioningDistribution(const TypedDagNode< RbVector<valueType> > *v, bool include_zero);
        UniformPartitioningDistribution(const UniformPartitioningDistribution &d);

        // public member functions
        UniformPartitioningDistribution*                      clone(void) const;                                                                      //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                executeMethod(const std::string &n, const std::vector<const DagNode*> &args, int &rv) const;     //!< Map the member methods to internal function calls
        const std::vector<valueType>&                       getParameterValues(void) const;
        size_t                                              getCurrentIndex(void) const;
        RbVector<valueType>*                                getPartition(void);

        void                                                redrawValue(void);
        void                                                setCurrentIndex(size_t i);
        void                                                setValue(valueType *v, bool f=false);
        
        // special handling of state changes
        //void                                                getAffected(RbOrderedSet<DagNode *>& affected, DagNode* affecter);                          //!< get affected nodes
        //void                                                keepSpecialization(DagNode* affecter);
        //void                                                restoreSpecialization(DagNode *restorer);
        //void                                                touchSpecialization(DagNode *toucher, bool touchAll);
        
    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Swap a parameter
        
        
    private:
        // helper methods
        RbVector<valueType>*                                simulate();
        
        // private members
        bool                                                include_zero;
        size_t                                              index;
        size_t                                              num_partitions;
        const TypedDagNode< RbVector<valueType> >*          parameter_values;
    };
    
}

#include "Assign.h"
#include "Assignable.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbMathCombinatorialFunctions.h"

#include <cmath>
#include <vector>


template <class valueType>
RevBayesCore::UniformPartitioningDistribution<valueType>::UniformPartitioningDistribution(const TypedDagNode< RbVector<valueType> > *v, bool include_zero) : TypedDistribution< RbVector<valueType> >( new RbVector<valueType>() ),
    include_zero( include_zero ),
    index( 0 ),
    parameter_values( v )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    this->addParameter( parameter_values );
    
    // calculate the number of partitions
    size_t num_elements = parameter_values->getValue().size();
    if ( include_zero == false )
    {
        num_partitions = RbMath::bell( num_elements ); 
    }
    else
    {
        num_partitions = RbMath::bell( num_elements + 1 );
    }
    
    delete this->value;
    this->value = simulate();
}


template <class valueType>
RevBayesCore::UniformPartitioningDistribution<valueType>::UniformPartitioningDistribution( const UniformPartitioningDistribution &d ) : TypedDistribution< RbVector<valueType> >( d ),
    include_zero( d.include_zero ),
    index( d.index ),
    parameter_values( d.parameter_values )
{

    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    this->addParameter( parameter_values );
    
    // calculate the number of partitions
    size_t num_elements = parameter_values->getValue().size();
    if ( include_zero == false )
    {
        num_partitions = RbMath::bell( num_elements ); 
    }
    else
    {
        num_partitions = RbMath::bell( num_elements + 1 );
    }
    
    delete this->value;
    this->value = simulate();

}


template <class valueType>
RevBayesCore::UniformPartitioningDistribution<valueType>* RevBayesCore::UniformPartitioningDistribution<valueType>::clone( void ) const
{

    return new UniformPartitioningDistribution<valueType>( *this );
}



template <class valueType>
double RevBayesCore::UniformPartitioningDistribution<valueType>::computeLnProbability( void )
{
    
    //double diff = max->getValue() - min->getValue() + 1.0;
    return -log( num_partitions );

}


template <class valueType>
void RevBayesCore::UniformPartitioningDistribution<valueType>::executeMethod(const std::string &n, const std::vector<const DagNode *> &args, int &rv) const
{
    
    if ( n == "getAllocationIndex" )
    {
        rv = int(index);
    }
    else
    {
        throw RbException("A uniform partitions distribution does not have a member method called '" + n + "'.");
    }
    
}


//template <class valueType>
//void RevBayesCore::UniformPartitioningDistribution<valueType>::getAffected(RbOrderedSet<DagNode *> &affected, DagNode* affecter)
//{
//    // only delegate when the toucher was our parameters
//    if ( affecter == parameter_values && this->dag_node != NULL )
//    {
//        this->dag_node->getAffectedNodes( affected );
//    }
//    
//}


template <class valueType>
size_t RevBayesCore::UniformPartitioningDistribution<valueType>::getCurrentIndex( void ) const
{

    return index;
}


template <class valueType>
const std::vector<valueType>& RevBayesCore::UniformPartitioningDistribution<valueType>::getParameterValues( void ) const
{
    return parameter_values->getValue();
}


template <class valueType>
RevBayesCore::RbVector<valueType>* RevBayesCore::UniformPartitioningDistribution<valueType>::getPartition( void )
{
    int num_elements = parameter_values->getValue().size(); 

    std::vector<int> partition_label;
    if ( include_zero == true )
    {
        for (int i = 0; i < (num_elements + 1); i++)
            partition_label.push_back(0);
    }
    else
    {
        for (int i = 0; i < num_elements; i++)
            partition_label.push_back(1);
    }

    // iterate through an ordered list of all partitions, stopping on the current index
    // TODO list of partitions should be generated just the first time and then cached....
    int current_partition = 0;
    while (current_partition < int(index))
    //while (current_partition < num_partitions)
    {
        int start_index;
        if ( include_zero == true )
        {
            start_index = num_elements;
        }
        else
        {
            start_index = num_elements - 1;
        }

        while (start_index >= 0)
        {
            int max_value = 0;
            for (int i = 0; i < start_index; i++)
            {
                if (partition_label[i] > max_value)
                    max_value = partition_label[i];
            }

            if (partition_label[start_index] > max_value or max_value > num_elements or partition_label[start_index] >= num_elements)
            {
                if ( include_zero == true )
                {
                    partition_label[start_index] = 0;
                }
                else
                {
                    partition_label[start_index] = 1;
                }
                start_index--;
            }
            else
            {
                partition_label[start_index] += 1;
                break;
            }
        }
        
//        if ( include_zero == true )
//        {
//            std::cout << "include_zero = TRUE!!!!\n";
//            for (int i = 0; i < (num_elements + 1); i++)
//            {
//                std::cout << partition_label[i] << "\t";
//            }
//            std::cout << "\n";
//        }
//        else
//        {
//            std::cout << "include_zero = false!!!!\n";
//            for (int i = 0; i < num_elements; i++)
//            {
//                std::cout << partition_label[i] << "\t";
//            }
//            std::cout << "\n";
//        }

        current_partition++;
    }
   
    // assign values to the selected partitions non-zero subsets
    RbVector<valueType>* rv = new RbVector<valueType>();
    RbVector<valueType>& temp = *rv;
    
    if ( include_zero == true )
    {
        for (int i = 1; i < (num_elements + 1); i++)
        {
            if (partition_label[i] == 0)
                temp.push_back(0.0);
            else
            {
                // TODO value assignments should be randomized!
                valueType val = parameter_values->getValue()[ partition_label[i] - 1 ];
                temp.push_back( val );
            }
        }
    }
    else
    {
        for (int i = 0; i < num_elements; i++)
        {
            if (partition_label[i] == 0)
                temp.push_back(0.0);
            else
            {
                // TODO value assignments should be randomized!
                valueType val = parameter_values->getValue()[ partition_label[i] - 1 ];
                temp.push_back( val );
            }
        }
    }

    return rv;
}


//template <class valueType>
//void RevBayesCore::UniformPartitioningDistribution<valueType>::keepSpecialization( DagNode* affecter )
//{
//    // only do this when the toucher was our parameters
//    if ( affecter == parameter_values && this->dag_node != NULL )
//    {
//        this->dag_node->keepAffected();
//    }
//    
//}


template <class valueType>
RevBayesCore::RbVector<valueType>* RevBayesCore::UniformPartitioningDistribution<valueType>::simulate()
{
    
    double weight = 1 / double(num_partitions);

    RandomNumberGenerator *rng = GLOBAL_RNG;
    double u = rng->uniform01();
    
    index = 0;
    while ( u > weight )
    {
        u -= weight;
        ++index;
    }
    
    //return parameter_values->getValue()[index];
    return getPartition();
}


template <class valueType>
void RevBayesCore::UniformPartitioningDistribution<valueType>::setCurrentIndex( size_t i )
{
    index = i;
    delete this->value;
    this->value = getPartition();
}


template <class valueType>
void RevBayesCore::UniformPartitioningDistribution<valueType>::redrawValue( void )
{
    delete this->value;
    this->value = simulate();
    //Assign<valueType, IsDerivedFrom<valueType, Assignable>::Is >::doAssign( (*this->value), simulate() );
}


/** Swap a parameter of the distribution */
template <class valueType>
void RevBayesCore::UniformPartitioningDistribution<valueType>::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
    if (oldP == parameter_values)
    {
        parameter_values = static_cast<const TypedDagNode< RbVector<valueType> >* >( newP );
    }
}


//template <class valueType>
//void RevBayesCore::UniformPartitioningDistribution<valueType>::restoreSpecialization( DagNode *restorer )
//{
//    
//    // only do this when the toucher was our parameters
//    if ( restorer == parameter_values )
//    {
//        const valueType &tmp = parameter_values->getValue()[index];
//        Assign<valueType, IsDerivedFrom<valueType, Assignable>::Is >::doAssign( (*this->value), tmp );
//
//        if ( this->dag_node != NULL )
//        {
//            this->dag_node->restoreAffected();
//        }
//        
//    }
//    
//}


template <class valueType>
void RevBayesCore::UniformPartitioningDistribution<valueType>::setValue(valueType *v, bool force)
{
    
    const RbVector<valueType> &vals = parameter_values->getValue();
    // we need to catch the value and increment the index
    for (index = 0; index < vals.size(); ++index)
    {
        if ( vals[index] == *v )
        {
            break;
        }
    }
    
    // delegate class
    TypedDistribution<valueType>::setValue( v, force );
}


//template <class valueType>
//void RevBayesCore::UniformPartitioningDistribution<valueType>::touchSpecialization( DagNode *toucher, bool touchAll )
//{
//    // only do this when the toucher was our parameters
//    if ( toucher == parameter_values )
//    {
//        const valueType &tmp = parameter_values->getValue()[index];
//        Assign<valueType, IsDerivedFrom<valueType, Assignable>::Is >::doAssign( (*this->value), tmp );
//        
//        if ( this->dag_node != NULL )
//        {
//            this->dag_node->touchAffected();
//        }
//        
//    }
//    
//}

#endif
