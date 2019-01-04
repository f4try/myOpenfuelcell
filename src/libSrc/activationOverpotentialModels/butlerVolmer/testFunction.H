namespace Foam
{

class testFunction
{
    const scalar i_, T_, i0_;

    const butlerVolmer& bv_;

    
public: 

    testFunction(scalar i, scalar T, scalar i0, const butlerVolmer& bv)
    :
    i_(i),
    T_(T),
    i0_(i0),
    bv_(bv)
    {}

    scalar operator()(const scalar& eta) const
    //scalar evaluate(const scalar& eta) const
    {
        return i_ - bv_.localCurrentDensity(i0_, T_, eta);
    }
};

}
