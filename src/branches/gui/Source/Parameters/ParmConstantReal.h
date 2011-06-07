#import <Foundation/Foundation.h>
#import "ParmConstant.h"



@interface ParmConstantReal : ParmConstant <NSCoding> {
    
}

- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t;

@end
