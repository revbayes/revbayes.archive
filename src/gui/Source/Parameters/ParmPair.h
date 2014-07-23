#import <Cocoa/Cocoa.h>
@class Parm;



@interface ParmPair : NSObject {

    Parm*                           parentParm;
    Parm*                           childParm;
}

@property (nonatomic,strong) Parm*  parentParm;
@property (nonatomic,strong) Parm*  childParm;

@end
