#import <Cocoa/Cocoa.h>
@class DistributionGui;
@class Parm;



@interface Parameter : NSObject <NSCoding> {

	NSString*                                 parameterName;
	NSString*                                 parameterDescription;
	DistributionGui*                          myDist;
	int                                       type;
	Parm*                                     refParm;
}

@property (nonatomic,strong) NSString*        parameterName;
@property (nonatomic,strong) NSString*        parameterDescription;
@property (nonatomic,strong) Parm*            refParm;
@property (nonatomic)        int              type;
@property (nonatomic,strong) DistributionGui* myDist;

- (id)initWithDist:(DistributionGui*)d;
- (id)initWithParameter:(Parameter*)p;

@end
