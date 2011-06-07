#import "SettingsMcmc.h"


@implementation SettingsMcmc

@synthesize mcmcType;
@synthesize numRuns;
@synthesize numChains;
@synthesize numCycles;
@synthesize statusFreq;
@synthesize sampleFreq;
@synthesize temperature;
@synthesize initSeed;

- (void)encodeWithCoder:(NSCoder *)aCoder {

    [aCoder encodeInt:mcmcType forKey:@"mcmcType"];
    [aCoder encodeInt:numRuns forKey:@"numRuns"];
    [aCoder encodeInt:numChains forKey:@"numChains"];
    [aCoder encodeInt:numCycles forKey:@"numCycles"];
    [aCoder encodeInt:statusFreq forKey:@"statusFreq"];
    [aCoder encodeInt:sampleFreq forKey:@"sampleFreq"];
    [aCoder encodeDouble:temperature forKey:@"temperature"];
    [aCoder encodeInt:(int)initSeed forKey:@"initSeed"];
}

- (id)init {

    if ( (self = [super init]) ) 
		{
        mcmcType    = 0;
        numRuns     = 2;
        numChains   = 4;
        numCycles   = 1000000;
        statusFreq  = 100;
        sampleFreq  = 100;
        temperature = 0.2;
        initSeed    = (long int)time(NULL);
        }
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super init]) ) 
		{
        mcmcType    = [aDecoder decodeIntForKey:@"mcmcType"];
        numRuns     = [aDecoder decodeIntForKey:@"numRuns"];
        numChains   = [aDecoder decodeIntForKey:@"numChains"];
        numCycles   = [aDecoder decodeIntForKey:@"numCycles"];
        statusFreq  = [aDecoder decodeIntForKey:@"statusFreq"];
        sampleFreq  = [aDecoder decodeIntForKey:@"sampleFreq"];
        temperature = [aDecoder decodeDoubleForKey:@"temperature"];
        initSeed    = [aDecoder decodeIntForKey:@"initSeed"];
		}
	return self;
}

@end
