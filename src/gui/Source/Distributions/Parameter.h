#import <Cocoa/Cocoa.h>
@class DistributionGui;
@class Parm;



@interface Parameter : NSObject <NSCoding> {

	NSString*                parameterName;
	NSString*                parameterDescription;
	DistributionGui*         myDist;
	int                      type;
	Parm*                    refParm;
}

@property (retain) NSString* parameterName;
@property (retain) NSString* parameterDescription;
@property (readwrite,assign) Parm* refParm;
@property (readwrite) int type;
@property (readwrite,assign) DistributionGui* myDist;

- (id)initWithDist:(Distribution*)d;
- (id)initWithParameter:(Parameter*)p;

@end
