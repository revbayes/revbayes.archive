#import <Foundation/Foundation.h>
#import "ParmConstant.h"



@interface ParmConstantInteger : ParmConstant <NSCoding> {
        
}
    
- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t;
    
@end
