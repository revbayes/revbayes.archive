#import <Cocoa/Cocoa.h>
@class Inlet;
@class Outlet;



@interface ConnectionPair : NSObject {

	Outlet*   outlet;
	Inlet*    inlet;
}

@property (nonatomic,strong) Outlet* outlet;
@property (nonatomic,strong) Inlet*  inlet;

@end
