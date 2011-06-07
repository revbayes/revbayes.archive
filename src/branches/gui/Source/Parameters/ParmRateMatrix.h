#import <Cocoa/Cocoa.h>
#import "Parm.h"



@interface ParmRateMatrix : Parm <NSCoding> {

}

- (void)initializeImage;
- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t;

@end
