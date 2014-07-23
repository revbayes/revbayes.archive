//
//  YRKSpinningProgressIndicator.h
//
//  Copyright 2009 Kelan Champagne. All rights reserved.
//


@interface YRKSpinningProgressIndicator : NSView {

    int         _position;
    int         _numFins;
    NSColor**   _finColors;
    
    BOOL        _isAnimating;
    BOOL        _isFadingOut;
    NSTimer*    _animationTimer;
	NSThread*   _animationThread;
    
    NSColor*    _foreColor;
    NSColor*    _backColor;
    BOOL        _drawsBackground;
    
    BOOL        _displayedWhenStopped;
    BOOL        _usesThreadedAnimation;

    BOOL        _isIndeterminate;
    double      _currentValue;
    double      _maxValue;
}

@property (atomic,strong)                        NSColor* color;
@property (atomic,strong)                        NSColor* backgroundColor;
@property (atomic)                               BOOL     drawsBackground;
@property (atomic,getter=isDisplayedWhenStopped) BOOL     displayedWhenStopped;
@property (atomic)                               BOOL     usesThreadedAnimation;
@property (atomic,getter=isIndeterminate)        BOOL     indeterminate;
@property (atomic)                               double   doubleValue;
@property (atomic)                               double   maxValue;

- (void)stopAnimation:(id)sender;
- (void)startAnimation:(id)sender;

@end
