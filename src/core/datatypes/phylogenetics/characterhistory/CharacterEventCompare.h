#ifndef CharacterEventCompare_H
#define CharacterEventCompare_H


namespace RevBayesCore {
    
    class CharacterEvent;
    class CharacterEventCompare
    {
    public:
        bool operator()(CharacterEvent* lhs, CharacterEvent* rhs) const { return (*lhs < *rhs); }
    protected:
        // ...
    private:
        // ...

    };
}

#endif /* defined(__rb_mlandis__CharacterEventCompare__) */
