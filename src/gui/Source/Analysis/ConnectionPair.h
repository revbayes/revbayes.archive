#import <Cocoa/Cocoa.h>
@class Inlet;
@class Outlet;



@interface ConnectionPair : NSObject {

	Outlet*                        outlet;
	Inlet*                         inlet;
}

@property (nonatomic,weak) Outlet* outlet;
@property (nonatomic,weak) Inlet*  inlet;

@end
