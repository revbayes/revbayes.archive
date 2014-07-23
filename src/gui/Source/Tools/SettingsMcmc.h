#import <Cocoa/Cocoa.h>



@interface SettingsMcmc : NSObject <NSCoding> {

    int                        mcmcType;
    int                        numRuns;
    int                        numChains;
    int                        numCycles;
    int                        statusFreq;
    int                        sampleFreq;
    double                     temperature;
    long int                   initSeed;
}

@property (nonatomic) int      mcmcType;
@property (nonatomic) int      numRuns;
@property (nonatomic) int      numChains;
@property (nonatomic) int      numCycles;
@property (nonatomic) int      statusFreq;
@property (nonatomic) int      sampleFreq;
@property (nonatomic) double   temperature;
@property (nonatomic) long int initSeed;

- (void)encodeWithCoder:(NSCoder *)aCoder;
- (id)initWithCoder:(NSCoder*)aDecoder;

@end
