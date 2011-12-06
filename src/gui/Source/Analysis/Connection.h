#import <Cocoa/Cocoa.h>
@class Inlet;
@class Outlet;



@interface Connection : NSObject <NSCoding> {

    Inlet*         inlet;
    Outlet*        outlet;
    BOOL           isSelected;
}

@property (assign) Inlet* inlet;
@property (assign) Outlet* outlet;
@property (readwrite) BOOL isSelected;

- (void)flipSelected;

@end
