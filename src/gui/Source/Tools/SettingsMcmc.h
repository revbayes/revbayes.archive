#import <Cocoa/Cocoa.h>



@interface SettingsMcmc : NSObject <NSCoding> {

    int                     mcmcType;
    int                     numRuns;
    int                     numChains;
    int                     numCycles;
    int                     statusFreq;
    int                     sampleFreq;
    double                  temperature;
    long int                initSeed;
}

@property (readwrite) int mcmcType;
@property (readwrite) int numRuns;
@property (readwrite) int numChains;
@property (readwrite) int numCycles;
@property (readwrite) int statusFreq;
@property (readwrite) int sampleFreq;
@property (readwrite) double temperature;
@property (readwrite) long int initSeed;

- (void)encodeWithCoder:(NSCoder *)aCoder;
- (id)initWithCoder:(NSCoder*)aDecoder;

@end
