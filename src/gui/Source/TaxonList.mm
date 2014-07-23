#import "TaxonList.h"



@implementation TaxonList

@synthesize taxonName;

-(id)init {

     if ( (self = [super init]) ) 
		{
        [self setTaxonName:@""];
		}
    return self;
}
 
- (NSString*)modelName { 

	return taxonName; 
}

@end
