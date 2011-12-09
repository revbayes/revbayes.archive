#import <Cocoa/Cocoa.h>
@class Inlet;
@class Outlet;



@interface Connection : NSObject <NSCoding> {

    Inlet*         inlet;
    Outlet*        outlet;
    BOOL           isSelected;
    NSBezierPath*  path1;           // keep track of my drawing information between the two tools
    NSBezierPath*  path2;
}

@property (assign) Inlet* inlet;
@property (assign) Outlet* outlet;
@property (readwrite) BOOL isSelected;
@property (assign) NSBezierPath* path1;
@property (assign) NSBezierPath* path2;

- (void)flipSelected;

@end
