#import <Cocoa/Cocoa.h>
@class Distribution;
@class Parm;



@interface Parameter : NSObject <NSCoding> {

	NSString*                parameterName;
	NSString*                parameterDescription;
	Distribution*            myDist;
	int                      type;
	Parm*                    refParm;
}

@property (retain) NSString* parameterName;
@property (retain) NSString* parameterDescription;
@property (readwrite,assign) Parm* refParm;
@property (readwrite) int type;
@property (readwrite,assign) Distribution* myDist;

- (id)initWithDist:(Distribution*)d;
- (id)initWithParameter:(Parameter*)p;

@end
