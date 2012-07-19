/*-------------------------------------------------------------------------*\
scalarField tau = T*1e-3;
int n = 7;
aH2O = (37.373, -41.205, 146.01, -217.08, 181.54, -79.409, 14.015);
\*-------------------------------------------------------------------------*/

#include "polyToddYoung.H"

#include "scalar.H"
#include "scalarField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

    //defineTypeNameAndDebug(polyToddYoung, 0);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

    // Construct from coefficients
    polyToddYoung::polyToddYoung
    (
        scalar a0, scalar a1, scalar a2,
        scalar a3, scalar a4, scalar a5, scalar a6
    )
    :
        a0_(a0),
        a1_(a1),
        a2_(a2),
        a3_(a3),
        a4_(a4),
        a5_(a5),
        a6_(a6)
    {}
 

    //- Construct from Istream
    polyToddYoung::polyToddYoung(Istream& is)
    :
        a0_(readScalar(is)),
        a1_(readScalar(is)),
        a2_(readScalar(is)),
        a3_(readScalar(is)),
        a4_(readScalar(is)),
        a5_(readScalar(is)),
        a6_(readScalar(is))
    {}


    //H2     21.157, 56.036, -150.55, 199.29, -136.15, 46.903, -6.4725
    //H2O    37.373, -41.205, 146.01, -217.08, 181.54, -79.409, 14.015


    // Member Functions

    //- Evaluate the polynomial function and return the result
    scalar polyToddYoung::polyVal(scalar T)
    {
        scalar t = T*1.0e-3;
        return (((((a6_*t + a5_)*t + a4_)*t + a3_)*t + a2_)*t + a1_)*t + a0_;
    }

    //- Evaluate the polynomial function and return the result
    scalarField polyToddYoung::polyVal(scalarField T)
    {
        scalarField t = T*1.0e-3;
        return (((((a6_*t + a5_)*t + a4_)*t + a3_)*t + a2_)*t + a1_)*t + a0_;
    }


    //- Evaluate the integral of the polynomial function and return the result
    //  Assumes zero for constant of integration
    scalar polyToddYoung::polyInt(scalar T)
    {
        scalar t = T*1.0e-3;
        return ((((((
	(
            a6_/7.0*t
            + a5_/6.0)*t
            + a4_/5.0)*t
            + a3_/4.0)*t
            + a2_/3.0)*t
            + a1_/2.0)*t
            + a0_)*t
	 )*1.0e3;
    }

    //- Evaluate the integral of the polynomial function and return the result
    //  Assumes zero for constant of integration
    scalarField polyToddYoung::polyInt(scalarField T)
    {
        scalarField t = T*1.0e-3;
        return ((((((
	(
            a6_/7.0*t
            + a5_/6.0)*t
            + a4_/5.0)*t
            + a3_/4.0)*t
            + a2_/3.0)*t
            + a1_/2.0)*t
            + a0_)*t
	 )*1.0e3;
    }


    //- Write the function coefficients
    void polyToddYoung::writeData(Ostream& os)
    {
        os  << a0_ << token::SPACE
            << a1_ << token::SPACE
            << a2_ << token::SPACE
            << a3_ << token::SPACE
            << a4_ << token::SPACE
            << a5_ << token::SPACE
            << a6_;
    }

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam


// ************************************************************************* //
