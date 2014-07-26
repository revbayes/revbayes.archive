#import <Cocoa/Cocoa.h>
@class Parameter;
@class Parm;



@interface DistributionGui : NSObject <NSCoding> {

	NSString*                          distributionName;
	NSString*                          distributionDescription;
	int                                distributionType;
    int                                distributionDomain;
    BOOL                               appliesExclusivelyToNodesOfUnrootedTrees;
    BOOL                               appliesExclusivelyToBranchesOfUnrootedTrees;
    BOOL                               appliesExclusivelyToNodesOfRootedTrees;
    BOOL                               appliesExclusivelyToBranchsOfRootedTrees;
	NSMutableArray*                    parameters;
	Parm*                              myParm;
}

@property (nonatomic,strong) NSString* distributionName;
@property (nonatomic,strong) NSString* distributionDescription;
@property (nonatomic)        int       distributionDomain;
@property (nonatomic)        int       distributionType;
@property (nonatomic,strong) Parm*     myParm;
@property (nonatomic)        BOOL      appliesExclusivelyToNodesOfUnrootedTrees;
@property (nonatomic)        BOOL      appliesExclusivelyToBranchesOfUnrootedTrees;
@property (nonatomic)        BOOL      appliesExclusivelyToNodesOfRootedTrees;
@property (nonatomic)        BOOL      appliesExclusivelyToBranchsOfRootedTrees;

- (DistributionGui*)clone;
- (Parameter*)getParameterIndexed:(int)idx;
- (id)initWithDist:(DistributionGui*)d;
- (id)initWithParm:(Parm*)p;
- (int)numParameters;

@end
