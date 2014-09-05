#import "TaxonInfo.h"


@implementation TaxonInfo

@synthesize taxonData;
@synthesize taxonName;

-(id)init {

     if ( (self = [super init]) ) 
		{
        [self setTaxonName:@"New Taxon"];
        [self setTaxonData:@"AAA"];
		}
    return self;
}

@end
