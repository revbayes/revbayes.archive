#import <Foundation/Foundation.h>
#import "ParmConstant.h"



@interface ParmConstantVector : ParmConstant <NSCoding> {
    
}

- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t;

@end
