#import <Foundation/Foundation.h>
@class Distribution;
@class Parm;



@interface DistributionList : NSObject {

    NSMutableArray*          distributionList;
}

- (Distribution*)getDistributionWithIndex:(int)idx;
- (void)initializeDistributionList;
- (Distribution*)newDistributionWithName:(NSString*)distName andParm:(Parm*)prm;
- (int)numDistributions;

@end
