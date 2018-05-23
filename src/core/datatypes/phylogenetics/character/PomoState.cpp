#include "PomoState.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMathCombinatorialFunctions.h"

#include <assert.h>
#include <sstream>
#include <iostream>

#include <boost/lexical_cast.hpp>

using namespace RevBayesCore;

/** Default constructor */
PomoState::PomoState(void) : DiscreteCharacterState( 4 + 6 * (10 - 1) ),
is_gap( false ),
is_missing( false ),
index_single_state( 0 ),
num_observed_states( 0 ),
state(4 + 6 * (10 - 1)),
chromosome_ (""),
position_(0),
virtualPopulationSize_ ( 10 ),
stringValue_ ("")
{
  std::cout << "IN CONSTRUCTOR 1" <<std::endl;
    setWeighted(true);
    weights_.assign(4 + 6 * (virtualPopulationSize_ - 1), 0.0);

}

/** Constructor with virtual population size */
PomoState::PomoState(size_t vps): DiscreteCharacterState( 4 + 6 * (vps - 1) ),
is_gap( false ),
is_missing( false ),
index_single_state( 0 ),
num_observed_states( 0 ),
state(4 + 6 * (10 - 1)),
chromosome_ (""),
position_(0),
virtualPopulationSize_( vps ),
stringValue_ ("")
{
  std::cout << "IN CONSTRUCTOR 2" <<std::endl;

    setWeighted(true);
    weights_.assign(4 + 6 * (virtualPopulationSize_ - 1), 0.0);

}


/** Constructor that sets the observation */
PomoState::PomoState(const std::string &s) : DiscreteCharacterState( 4 + 6 * (10 - 1) ),
is_gap( false ),
is_missing( false ),
index_single_state( 0 ),
num_observed_states( 0 ),
state(4 + 6 * (10 - 1)),
chromosome_( "" ),
position_( 0 ),
virtualPopulationSize_( 10 ),
stringValue_ (s)
{
  std::cout << "IN CONSTRUCTOR 3" <<std::endl;

    //assert( s <= 15 );
    setWeighted(true);
    weights_.assign(4 + 6 * (virtualPopulationSize_ - 1), 0.0);
    setState(s);
}



/** Constructor that sets the observation and the other fields */
PomoState::PomoState(const std::string &s, const std::string chromosome, const size_t position, const size_t virtualPopulationSize, std::vector<double> weights ) : DiscreteCharacterState( 4 + 6 * (virtualPopulationSize - 1) ),
is_gap( false ),
is_missing( false ),
index_single_state( 0 ),
num_observed_states( 0 ),
state(4 + 6 * (virtualPopulationSize - 1)),
chromosome_( chromosome ),
position_( position ),
virtualPopulationSize_( virtualPopulationSize ),
stringValue_ (s)
{
  std::cout << "IN CONSTRUCTOR 4" <<std::endl;

    setWeighted(true);
    weights_.assign(4 + 6 * (virtualPopulationSize_ - 1), 0.0);
    setState(s);
    std::cout << "in PomoState::COPY CONSTRUCTOR 4 END state: "<<this->getStringValue()<<" ; " << state <<std::endl;

    std::cout << "in PomoState::COPY CONSTRUCTOR 4 END state.getNumberSetBits(): " << this->getStringValue()<<" ; " << state.getNumberSetBits() <<std::endl;
}


/** Constructor that sets the observation and the other fields */
PomoState::PomoState(const std::string &s, const std::string chromosome, const size_t position, const size_t virtualPopulationSize) : DiscreteCharacterState( 4 + 6 * (virtualPopulationSize - 1) ),
is_gap( false ),
is_missing( false ),
index_single_state( 0 ),
num_observed_states( 0 ),
state(4 + 6 * (virtualPopulationSize - 1)),
chromosome_( chromosome ),
position_( position ),
virtualPopulationSize_( virtualPopulationSize ),
stringValue_ (s)
{
  std::cout << "IN CONSTRUCTOR 5: " << stringValue_<<" ; " <<std::endl;

    setWeighted(true);
    weights_.assign(4 + 6 * (virtualPopulationSize_ - 1), 0.0);
    setState(s);
    std::cout << "in PomoState::COPY CONSTRUCTOR 5 END state: "<< this->getStringValue()<<" ; " << state <<std::endl;

    std::cout << "in PomoState::COPY CONSTRUCTOR 5 END state.getNumberSetBits(): " << this->getStringValue()<<" ; " << state.getNumberSetBits() <<std::endl;
}


/* Copy constructor */
PomoState::PomoState(const PomoState& t) : DiscreteCharacterState( 4 + 6 * (t.virtualPopulationSize_ - 1) ), is_gap(t.is_gap), is_missing(t.is_missing), index_single_state(t.index_single_state), num_observed_states(t.num_observed_states), state(t.state), chromosome_(t.chromosome_), position_(t.position_), virtualPopulationSize_(t.virtualPopulationSize_), stringValue_(t.stringValue_)
 {
   std::cout << "IN COPY CONSTRUCTOR 6: "<< stringValue_ <<std::endl;

   setWeighted (t.weighted_);
   weights_.clear();
   for (size_t i = 0; i < t.weights_.size() ; ++i) {
     std::cout << "t.weights_.at(i): " << t.weights_.at(i) << std::endl;
     weights_.push_back(t.weights_.at(i));
     std::cout << "weights_.at(i): " << weights_.at(i) << std::endl;

   }
   state = t.state;
   std::cout << "in PomoState::COPY CONSTRUCTOR 6 END state: "<< this->getStringValue()<<" ; " << state <<std::endl;

   std::cout << "in PomoState::COPY CONSTRUCTOR 6 END state.getNumberSetBits(): " << this->getStringValue()<<" ; " << state.getNumberSetBits() <<std::endl;


 }



PomoState* PomoState::clone( void ) const
{

    return new PomoState( *this );
}


void PomoState::populateWeightedStatesForMonoallelicState(int id1, int sum) {
  // We do Pomo state averaging.
  double n = (double)sum;
  double p = 1.0;

  //weights_[id1] = RbStatistics::Binomial::pdf(n, p, (double)virtualPopulationSize_);
  //State id1 can only generate state id1

  std::cout << "BEFORE LOOP All weights: "<<std::endl;

  for (int i = 0; i<weights_.size(); ++i) {
    if (weights_[i] >0.0) {
      std::cout << i<< " : "<< weights_[i] << std::endl ;
    }
  }
  std::cout << "\n "<<std::endl;


  weights_[id1] = 1.0;
  state.set( id1 );
  std::cout << "monoallelic weights_[id1]: "<< id1 << " : " << weights_[id1] <<std::endl;


  int virtMinus1 = virtualPopulationSize_ - 1;


std::vector<double> nd (virtMinus1, 0.0);
std::vector<double> id (virtMinus1, 0.0);
for (size_t i = 0; i < virtMinus1; ++i) {
  nd[i] = (double)(i + 1) / (double)virtualPopulationSize_;
  id[i] = (double)(virtMinus1 - i) / (double)virtualPopulationSize_;
}

  if (id1 == 0) {
    // A, AC, AG, AT weights will be non 0.
    for (size_t numid1 = 0; numid1< virtMinus1; ++numid1)
    {
      //A1C9 and subsequent
//      std::cout << "RbStatistics::Binomial::pdf(n, p, (double)(numid1));: "<< RbStatistics::Binomial::pdf(n, p, (double)(numid1)) <<std::endl;
      weights_[ 4 + numid1 ] = pow(nd[numid1], (double)sum);//RbStatistics::Binomial::pdf(n, p, (double)(numid1));
      state.set( 4 + numid1 );
      //A1G9 and subsequent
      weights_[ 4 + virtMinus1 + numid1 ] = pow(nd[numid1], (double)sum);//RbStatistics::Binomial::pdf(n, p, (double)(numid1));
      state.set( 4 + virtMinus1 + numid1 );
      //A1T9 and subsequent
      weights_[ 4 + 2 * virtMinus1 + numid1 ] = pow(nd[numid1], (double)sum);//RbStatistics::Binomial::pdf(n, p, (double)(numid1));
      state.set( 4 + 2 * virtMinus1 + numid1 );
    }
  }
  else if (id1 == 1) {
    // C, AC, CG, CT weights will be non 0.
    for (size_t numid1 = 0; numid1< virtMinus1; ++numid1)
    {
      //A.C.
      weights_[ 4 + numid1 ] = pow(id[numid1], (double)sum);
      state.set( 4 + numid1 );
      //C.G.
      weights_[ 4 + 3 * virtMinus1 + numid1 ] = pow(nd[numid1], (double)sum);
      state.set( 4 + 3 * virtMinus1 + numid1 );
      //C.T.
      weights_[ 4 + 4 * virtMinus1 + numid1 ] = pow(nd[numid1], (double)sum);
      state.set( 4 + 4 * virtMinus1 + numid1 );
    }
  }
  else if (id1 == 2) {
    // G, AG, CG, GT weights will be non 0.
    for (size_t numid1 = 0; numid1< virtMinus1; ++numid1)
    {
      //A.G.
      weights_[ 4 + virtMinus1 + numid1 ] = pow(id[numid1], (double)sum);
      state.set( 4 + virtMinus1 + numid1 );
      //C.G.
      weights_[ 4 + 3 * virtMinus1 + numid1 ] = pow(id[numid1], (double)sum);
      state.set( 4 + 3 * virtMinus1 + numid1 );
      //G.T.
      weights_[ 4 + 5 * virtMinus1 + numid1 ] = pow(nd[numid1], (double)sum);
      state.set( 4 + 5 * virtMinus1 + numid1 );
    }
  }
  else if (id1 == 3) {
    // T, AT, CT, GT weights will be non 0.
    for (size_t numid1 = 0; numid1< virtMinus1; ++numid1)
    {
      //A.T.
      weights_[ 4 + 2 * virtMinus1 + numid1 ] = pow(id[numid1], (double)sum);
      state.set( 4 + 2 * virtMinus1 + numid1 );
      //C.T.
      weights_[ 4 + 4 * virtMinus1 + numid1 ] = pow(id[numid1], (double)sum);
      state.set( 4 + 4 * virtMinus1 + numid1 );
      //G.T.
      weights_[ 4 + 5 * virtMinus1 + numid1 ] = pow(id[numid1], (double)sum);
      state.set( 4 + 5 * virtMinus1 + numid1 );
    }
  }
  else  {
    throw RbException( "Pomo string state not correct. We found "+ id1  );
  }

  std::cout << "\n "<<std::endl;
  std::cout << "AFTER LOOP All weights: "<<std::endl;

  for (int i = 0; i<weights_.size(); ++i) {
    if (weights_[i] >0.0) {
      std::cout << i<< " : " << weights_[i] << std::endl ;
    }
  }
  std::cout << "\n "<<std::endl;

  return;
}


void PomoState::setState(const std::string &symbol)
{
    /* Example with only ten states:
     A C G T A10C90 A20C80 A30C70...A90C10 A10G90 A20G80...A10T90...C10G90...C10T90...G10T90
     The preferred format is that of counts: e.g.:
     0,1,4,0
     meaning 0 A, 1 C, 4 G, 0 T were sampled at that position.
     */
    std::cout << "in PomoState::setState: " << symbol <<std::endl;
    size_t index = 0;
    size_t virtMinus1 = virtualPopulationSize_-1;
    //Checking if we have the preferred format, i.e. counts.
    if ( symbol.find(",") != std::string::npos ) {
        std::stringstream ss(symbol);
        std::vector<int> vect;
        int j;
        while (ss >> j)
        {
            vect.push_back(j);
            if (ss.peek() == ',' || ss.peek() == ' ')
                ss.ignore();
        }
        if (vect.size() != 4)
            throw RbException( "Pomo string state not correctly formatted. We found "+ symbol +", but the preferred format is that of counts, e.g. 0,1,4,0 meaning 0 A, 1 C, 4 G, 0 T were sampled at that position." );
        //We have the counts, now we create the state.
        //code borrowed in part from IQ-Tree
        int sum = 0;
        int count = 0;
        int id1 = -1;
        int id2 = -1;
        int num = 0;
        // Sum over elements and count non-zero elements.
        for (std::vector<int>::iterator i = vect.begin(); i != vect.end(); ++i) {
          std::cout << "in PomoState::setState loop: " << *i <<std::endl;
            // `i` is an iterator object that points to some
            // element of `vect`.
            if (*i != 0) {
                // `i - vect.begin()` ranges from 0 to 3 and
                // determines the nucleotide or allele type.
                if (id1 == -1)
                {
                  id1 = i - vect.begin();
                  num = *i;
                }
                else id2 = i - vect.begin();
                count++;
                sum += *i;
                std::cout << "in PomoState::setState id1: " << id1 <<std::endl;
                std::cout << "in PomoState::setState id2: " << id2 <<std::endl;
            }
        }
        if ( count == 1 ) // monoallelic state
        {
          std::cout << "MONOALLELIC: " << id1 << " ; "<< id2 <<std::endl;
            index = id1 ; //+ 1;
            //weights_[id1] = 1.0;
            state.clear();
            state.set(id1);
            populateWeightedStatesForMonoallelicState(id1, sum);
        }
        else if ( count == 2 ) //biallelic state
        {
          std::cout << "BIALLELIC: " << id1 << " ; "<< id2 <<std::endl;
          //          index = 3 + id1*virtualPopulationSize_ + id2 ;
          int basicIndex = 4;
          if (id1==0) {
            if (id2 == 1) {
              basicIndex = 4;
            }
            else if (id2 == 2) {
              basicIndex = 4 + (virtMinus1);
            }
            else if (id2 == 3) {
              basicIndex = 4 + 2*(virtMinus1);
            }
            else  {
              throw RbException( "Pomo string state not correct. We found "+ symbol  );
            }
          }
          else if (id1==1) {
            if (id2 == 2) {
              basicIndex = 4 + 3*(virtMinus1);
            }
            else if (id2 == 3) {
              basicIndex = 4 + 4*(virtMinus1);
            }
            else  {
              throw RbException( "Pomo string state not correct. We found "+ symbol  );
            }
          }
          else if (id1==2) {
            if (id2 == 3) {
              basicIndex = 4 + 5*(virtMinus1);
            }
            else  {
              throw RbException( "Pomo string state not correct. We found "+ symbol  );
            }
          }
          else  {
            throw RbException( "Pomo string state not correct. We found "+ symbol  );
          }

          size_t situation = (id1+1)*2+(id2+1);

          std::cout <<"SITUATION "<<situation <<std::endl;
          state.clear();
          // index corresponds to the closest place where the pomo state is.
          // In case the virtual population size is inferior to the counts in the state,
          // we have to do some maths.
          // We have to get the closest numbers to the observed counts.
          // Basically, the lowest count has to be >=1, and as close as possible
          // to the observed count.
          if (sum > virtualPopulationSize_) {
            double obs_proportion = (double) (sum - num) / (double)sum ;
            size_t corrected_num = 0;
            if  (obs_proportion <= 1.0) {
              corrected_num = virtualPopulationSize_ - 1;
            }
            else {
              corrected_num = virtualPopulationSize_ - (size_t)round(obs_proportion);
            }
            index = basicIndex + virtualPopulationSize_ - corrected_num - 1;
          }
          else {
            index = basicIndex + virtualPopulationSize_ - num - 1;
          }
          std::cout << "INDEX: " << index <<std::endl;
          //index = 3 + id1*virtualPopulationSize_ + id2 ;
          // Let's try Pomo state averaging.
          // Basically all cells in the weight matrix that contain only id1, only id2, or a combination of both should have a non-zero weight.
          double n = (double)sum;
          double p = (double)num/(double)sum;

          std::vector<double> prob (virtualPopulationSize_ + 1);
          for (size_t j =0; j <= virtualPopulationSize_; ++j) {
            prob[j] = RbMath::choose(sum, num) *  pow( ( (double)j/double(virtualPopulationSize_)) , num) * pow( ( (double)(virtualPopulationSize_ - j)/double(virtualPopulationSize_) ) , (double)(sum-num)) ;
            //RbStatistics::Binomial::pdf(n, (double)j/double(virtualPopulationSize_), (double)(virtualPopulationSize_));
          }

          if (situation == 4) { //A.C.
            for (size_t j =0; j < virtMinus1; ++j) {
                weights_[j+4] = prob[j+1];
                state.set( j+4 );
            }
          }
          else if (situation == 5) { //A.G.
            for (size_t j =0; j < virtMinus1; ++j) {
                weights_[virtMinus1+j+4] = prob[j+1];
                state.set( virtMinus1+j+4 );
            }
          }
          else if (situation == 6) { //A.T.
            for (size_t j =0; j < virtMinus1; ++j) {
                weights_[4 + 2 * virtMinus1 + j] = prob[j+1];
                state.set( 4 + 2 * virtMinus1 + j );
            }
          }
          else if (situation == 7) { //C.G.
            for (size_t j =0; j < virtMinus1; ++j) {
                weights_[4 + 3 * virtMinus1 + j] = prob[j+1];
                state.set( 4 + 3 * virtMinus1 + j );
            }
          }
          else if (situation == 8) { //C.T.
            for (size_t j =0; j < virtMinus1; ++j) {
                weights_[4 + 4 * virtMinus1 + j] = prob[j+1];
                state.set( 4 + 4 * virtMinus1 + j );
            }
          }
          else if (situation == 10) { //G.T.
            for (size_t j =0; j < virtualPopulationSize_; ++j) {
                weights_[4 + 5 * virtMinus1 + j] = prob[j+1];
                state.set( 4 + 5 * virtMinus1 + j );
            }
          }
          else {
            throw RbException( "Pomo string state not correct. We found "+ symbol  );
          }

          /*
          // only id1:
          weights_[id1] = RbStatistics::Binomial::pdf(n, p, (double)virtualPopulationSize_);
          state.set( id1 );
          std::cout << "weights_[id1]: "<< id1 << " : " << weights_[id1] <<std::endl;
          // only id2:
          weights_[id2] = RbStatistics::Binomial::pdf(n, p, 0.0);
          state.set( id2 );
          std::cout << "weights_[id2]: "<< id2 << " : " << weights_[id2] <<std::endl;
          // Mixtures of id1 and id2:
          for (size_t numid1 = 1; numid1< virtualPopulationSize_; ++numid1)
          {
            weights_[basicIndex + virtualPopulationSize_ - numid1 - 1] = RbStatistics::Binomial::pdf(n, p, (double)(numid1));
            state.set( basicIndex + virtualPopulationSize_ - numid1 - 1 );
            std::cout << "weights_[basicIndex + virtualPopulationSize_ - numid1 - 1]: "<< basicIndex + virtualPopulationSize_ - numid1 - 1 << " : " << weights_[basicIndex + virtualPopulationSize_ - numid1 - 1] <<std::endl;
          }
          */
          //          d.pdf(n, p, (double)virtualPopulationSize_);
          std::cout << "INDEX: "<< index <<std::endl;
          std::cout << "n: "<<n << " p:"<< p <<std::endl;

          std::cout << "\n "<<std::endl;
          std::cout << "AFTER LOOP All weights: "<<std::endl;

          for (int i = 0; i<weights_.size(); ++i) {
            if (weights_[i] >0.0) {
              std::cout << i<< " : " << weights_[i] << std::endl ;
            }
          }
          std::cout << "\n "<<std::endl;

        }
        else if ( count == 0 )
        {
            setGapState( true ); //We say we have a gap
        }
        else
        {
            throw RbException( "Pomo string state not correct. We found "+ symbol +", but only 2 states can be non-0." );
        }
      }

    std::cout << "in PomoState::setState END index: " << index <<std::endl;

    //state.clear();
    //state.set( index );
    index_single_state = index;
    std::cout << "in PomoState::setState END state: " << state <<std::endl;
    std::cout << "in PomoState::setState END state.getNumberSetBits(): " << state.getNumberSetBits() <<std::endl;

    std::cout << "in PomoState::setState END index_single_state: " << index_single_state <<std::endl;

    num_observed_states = 1;

    std::cout << "\n "<<std::endl;
    std::cout << "END of PomoState::setState All weights: "<<std::endl;

    for (int i = 0; i<weights_.size(); ++i) {
      if (weights_[i] >0.0) {
        std::cout << i<< " : " << weights_[i] << std::endl ;
      }
    }
    std::cout << "\n "<<std::endl;


}


std::string PomoState::getDataType( void ) const
{

    return "Pomo";
}


std::string PomoState::getStateLabels( void ) const
{

    static std::string labels = "A C G T ";
    std::string acgt( "ACGT" );
    std::vector< size_t > frequencies;
    int stepSize = 100 / virtualPopulationSize_;
    for (size_t i = 1; i < virtualPopulationSize_; ++i)
    {
        frequencies.push_back(i*stepSize);
    }
    for ( size_t k = 0; k < acgt.size(); ++k )
    {
        char ch = acgt[k];

        for ( size_t j = k+1; j < acgt.size(); ++j )
        {
            char ch2 = acgt[j];
            for (size_t i = 0; i < virtualPopulationSize_-1; ++i)
            {
                labels += ch + boost::lexical_cast<std::string>(frequencies[i]) + ch2 + boost::lexical_cast<std::string>(frequencies[virtualPopulationSize_ - 2 - i]) + " ";
            }
        }
    }

    return labels;
}

std::string PomoState::getStringValue(void) const
{

    if ( isMissingState() )
    {
        return "?";
    }

    if ( isGapState() )
    {
        return "-";
    }

    return stringValue_;
    /*
    int stepSize = 100 / virtualPopulationSize_ - 1;
    size_t index = getStateIndex();
    if (index < 4)
    {
        switch ( index )
        {
            case 0:
                return "A";
            case 1:
                return "C";
            case 2:
                return "G";
            case 3:
                return "T";
        }
    }
    int stateMinus4 = int(index) - 4;
    int typeOfPomoState = stateMinus4 / stepSize;
    int typeOfFrequency = stateMinus4 % stepSize +1;
    int freqi = typeOfFrequency*virtualPopulationSize_;
    int freqj = 100 - freqi;
    switch ( typeOfPomoState )
    {
        case 0: //AC
            return "A"+ boost::lexical_cast<std::string>(freqi) + "C" + boost::lexical_cast<std::string>(freqj) ;
        case 1: //AG
            return "A"+ boost::lexical_cast<std::string>(freqi) + "G" + boost::lexical_cast<std::string>(freqj) ;
        case 2: //AT
            return "A"+ boost::lexical_cast<std::string>(freqi) + "T" + boost::lexical_cast<std::string>(freqj) ;
        case 3: //CG
            return "C"+ boost::lexical_cast<std::string>(freqi) + "G" + boost::lexical_cast<std::string>(freqj) ;
        case 4: //CT
            return "C"+ boost::lexical_cast<std::string>(freqi) + "T" + boost::lexical_cast<std::string>(freqj) ;
        case 5: //GT
            return "G"+ boost::lexical_cast<std::string>(freqi) + "T" + boost::lexical_cast<std::string>(freqj) ;
    }
    throw RbException( "getStringValue called on a non-standard state." );
    */
}

void PomoState::setVirtualPopulationSize(size_t populationSize)
{
    if (populationSize >= 100)
    {
        throw RbException( "The virtual population size should be < 100 and should be a divisor of 100." );
    }
    if (100 % populationSize != 0)
    {
        throw RbException( "The virtual population size should be a divisor of 100." );
    }
    virtualPopulationSize_ = populationSize;
}

void PomoState::setChromosome(std::string chromosome){
    chromosome_ = chromosome;
}

void PomoState::setPosition(size_t position){
    position_ = position;
}

const std::string PomoState::getChromosome( void )
{
    return chromosome_;
}

const size_t PomoState::getPosition( void )
{
    return position_;
}

// const std::vector<double> PomoState::getWeights( void ) const
// {
//   std::cout << "USING getWeights" <<std::endl;
//     return weights_;
// }

//
// bool PomoState::isWeighted( void ) const
// {
//     return weighted_;
// }
//
//
// void PomoState::setWeighted( bool tf )
// {
//     weighted_ = tf;
//     return;
// }



void PomoState::addState(const std::string &symbol)
{
    ++num_observed_states;

    std::string labels = getStateLabels();
    size_t pos = labels.find(symbol);

    state.set( pos );
    index_single_state = pos;
}


RbBitSet PomoState::getState(void) const
{
  std::cout << "in PomoState::getState1: " << stringValue_ << " "<< state << " and state.getNumberSetBits(): "<< state.getNumberSetBits() << std::endl;
    // we need to clear the bits first
    RbBitSet bs = RbBitSet( state.size());
std::cout << "in PomoState::getState2: index_single_state: " << index_single_state <<std::endl;
    bs.set(index_single_state);

    std::cout << "in PomoState::getState3: " << stringValue_ << " " << state<< " and state.getNumberSetBits(): "<< state.getNumberSetBits() <<std::endl;
    std::cout << state <<std::endl;
    return state;
//    return bs;
}


bool PomoState::isGapState( void ) const
{
    return is_gap;
}


bool PomoState::isMissingState( void ) const
{
    return is_missing;
}


void PomoState::setGapState( bool tf )
{
    is_gap = tf;
}


void PomoState::setMissingState( bool tf )
{
    is_missing = tf;
}


void PomoState::setToFirstState(void)
{
    num_observed_states = 1;
    index_single_state = 0;
    state.clear();
    state.set( 0 );
}


void PomoState::setStateByIndex(size_t index)
{

    num_observed_states = 1;
    index_single_state = index;
    state.clear();
    state.set( index );
}
