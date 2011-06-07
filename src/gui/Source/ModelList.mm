#import "ModelList.h"



@implementation ModelList

-(id)init {

     if ( (self = [super init]) ) 
		{
        [self setModelName:@"New Model"];
		}
    return self;
}
 
- (NSString*)modelName { 

	return modelName; 
}

- (void)setModelName:(NSString*)aName {

    if (aName != modelName) 
		{
        [modelName release];
        [aName retain];
        modelName = aName;
		}
}

@end
