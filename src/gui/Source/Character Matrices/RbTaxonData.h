#import <Cocoa/Cocoa.h>
@class MatrixRowInformation;
@class RbDataCell;



@interface RbTaxonData : NSObject <NSCoding> {

	NSMutableArray*       data;
    int                   dataType;
	NSString*             taxonName;
}

@property (nonatomic)        int       dataType;
@property (nonatomic,strong) NSString* taxonName;

- (void)cleanName:(NSString*)nameStr;
- (RbDataCell*)dataCellIndexed:(int)idx;
- (id)initWithRbTaxonData:(RbTaxonData*)d;
- (id)initWithMatrixRowInformation:(MatrixRowInformation*)mr;
- (void)addObservation:(RbDataCell*)dc;
- (int)numCharacters;
- (void)print;

@end
