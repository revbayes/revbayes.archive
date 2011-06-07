#import "TaxonList.h"



@implementation TaxonList

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

- (void)setTaxonName:(NSString*)aName {

    if (aName != taxonName) 
		{
        [taxonName autorelease];
        taxonName = [aName retain];
		}
}

- (NSString*)taxonName {

    return taxonName;
}

@end
