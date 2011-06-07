#import "TaxonInfo.h"


@implementation TaxonInfo

-(id)init {

     if ( (self = [super init]) ) 
		{
        [self setTaxonName:@"New Taxon"];
        [self setTaxonData:@"AAA"];
		}
    return self;
}
 
- (NSString*)taxonName { 

	return taxonName; 
}

- (void)setTaxonName:(NSString*)aName {

    if (aName != taxonName) 
		{
        [taxonName release];
        [aName retain];
        taxonName = aName;
		}
}

- (NSString*)taxonData { 

	return taxonData; 
}

- (void)setTaxonData:(NSString*)aData {

    if (aData != taxonName) 
		{
        [taxonData release];
        [aData retain];
        taxonData = aData;
		}
}

@end
