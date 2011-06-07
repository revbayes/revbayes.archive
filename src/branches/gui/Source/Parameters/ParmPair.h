#import <Cocoa/Cocoa.h>
@class Parm;



@interface ParmPair : NSObject {

    Parm*        parentParm;
    Parm*        childParm;
}

@property (readwrite,retain) Parm* parentParm;
@property (readwrite,retain) Parm* childParm;

@end
