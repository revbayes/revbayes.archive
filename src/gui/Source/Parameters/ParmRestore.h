#import <Cocoa/Cocoa.h>
@class Parm;



@interface ParmRestore : NSObject {

    Parm*                           parameter;
    Parm*                           parameter1;
    Parm*                           parameter2;
    Parm*                           parameter3;
    Parm*                           parameter4;
}

@property (nonatomic,strong) Parm*  parameter;
@property (nonatomic,strong) Parm*  parameter1;
@property (nonatomic,strong) Parm*  parameter2;
@property (nonatomic,strong) Parm*  parameter3;
@property (nonatomic,strong) Parm*  parameter4;

@end
