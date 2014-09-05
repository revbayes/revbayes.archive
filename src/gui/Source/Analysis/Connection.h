#import <Cocoa/Cocoa.h>
@class Inlet;
@class Outlet;



@interface Connection : NSObject <NSCoding> {

    Inlet*          inlet;
    Outlet*         outlet;
    BOOL            isSelected;
    NSBezierPath*   path1;           // keep track of my drawing information between the two tools
    NSBezierPath*   path2;
}

@property (nonatomic,strong) Inlet*        inlet;
@property (nonatomic,strong) Outlet*       outlet;
@property (nonatomic)        BOOL          isSelected;
@property (nonatomic,strong) NSBezierPath* path1;
@property (nonatomic,strong) NSBezierPath* path2;

- (void)flipSelected;

@end
