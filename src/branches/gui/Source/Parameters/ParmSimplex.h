#import <Foundation/Foundation.h>
#import "ParmRandomVariable.h"



@interface ParmSimplex : ParmRandomVariable <NSCoding> {
        
}
    
- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t;

@end
