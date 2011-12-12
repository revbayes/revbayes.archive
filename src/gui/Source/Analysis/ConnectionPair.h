#import <Cocoa/Cocoa.h>
@class Inlet;
@class Outlet;



@interface ConnectionPair : NSObject {

	Outlet*                 outlet;
	Inlet*                  inlet;
}

@property (readwrite,assign) Outlet* outlet;
@property (readwrite,assign) Inlet* inlet;

@end
