#import <Cocoa/Cocoa.h>
#import "ParmRandomVariable.h"



@interface ParmReal : ParmRandomVariable <NSCoding> {

}

- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t;

@end
