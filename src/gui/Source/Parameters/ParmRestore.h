#import <Cocoa/Cocoa.h>
@class Parm;



@interface ParmRestore : NSObject {

    Parm*        parameter;
    Parm*        parameter1;
    Parm*        parameter2;
    Parm*        parameter3;
    Parm*        parameter4;
}

@property (readwrite,retain) Parm* parameter;
@property (readwrite,retain) Parm* parameter1;
@property (readwrite,retain) Parm* parameter2;
@property (readwrite,retain) Parm* parameter3;
@property (readwrite,retain) Parm* parameter4;

@end
