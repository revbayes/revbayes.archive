#import <Cocoa/Cocoa.h>
#import "ParmRandomVariable.h"



@interface ParmIntegerPos : ParmRandomVariable <NSCoding> {

}

- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t;

@end
