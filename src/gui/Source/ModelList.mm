#import "ModelList.h"



@implementation ModelList

@synthesize modelName;

-(id)init {

     if ( (self = [super init]) ) 
		{
        [self setModelName:@"New Model"];
		}
    return self;
}

@end
