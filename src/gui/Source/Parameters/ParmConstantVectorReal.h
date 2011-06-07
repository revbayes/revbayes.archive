#import <Foundation/Foundation.h>
#import "ParmConstantVector.h"



@interface ParmConstantVectorReal : ParmConstantVector <NSCoding> {
    
}

- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t;

@end
