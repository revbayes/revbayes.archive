#import <Cocoa/Cocoa.h>
@class MatrixRowInformation;
@class RbDataCell;



@interface RbTaxonData : NSObject <NSCoding> {

	NSMutableArray*      data;
    int                  dataType;
	NSString*            taxonName;
}

@property (readwrite) int dataType;

- (void)cleanName:(NSString*)nameStr;
- (RbDataCell*)dataCellIndexed:(int)idx;
- (id)initWithRbTaxonData:(RbTaxonData*)d;
- (id)initWithMatrixRowInformation:(MatrixRowInformation*)mr;
- (void)addObservation:(RbDataCell*)dc;
- (int)numCharacters;
- (void)print;
- (void)setTaxonName:(NSString*)tn;
- (NSString*)taxonName;

@end
