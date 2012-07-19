/*-------------------------------------------------------------------------*\
scalarField tau = T*1e-3;
int n = 7;
aH2O = (37.373, -41.205, 146.01, -217.08, 181.54, -79.409, 14.015);
\*-------------------------------------------------------------------------*/

#ifndef polyToddYoung_H
#define polyToddYoung_H

#include "scalar.H"
#include "scalarField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class polyToddYoung Declaration
\*---------------------------------------------------------------------------*/

class polyToddYoung
  
{

private:

    //private types and data

    // order and degree of polynomial
    //static const int polySize = 7;             // order (number of coeffs)
    //static const int polyDeg = polySize - 1;   // degree

    // - coefficients
    scalar a0_, a1_, a2_, a3_, a4_, a5_, a6_;

    //Sp      a0      a1      a2       a3      a4       a5      a6      Cp1473K
    //H2    21.157, 56.036, -150.55, 199.29, -136.15, 46.903, -6.4725    32.146
    //H2O   37.373, -41.205, 146.01, -217.08, 181.54, -79.409, 14.015    46.831
    //O2    34.850  -57.975  203.68  -300.37  231.72  -91.821  14.776    36.469
    //N2    29.027  4.8987  -38.040  105.17  -113.56  55.554  -10.350    34.756


public:

    // Constructors

    // Construct from coefficients
    polyToddYoung
    (
        scalar a0, scalar a1, scalar a2,
        scalar a3, scalar a4, scalar a5, scalar a6
    );

    //- Construct from Istream
    polyToddYoung(Istream& is);


    // Destructor

    ~polyToddYoung()
    {}


    // Member Functions

    //- Evaluate the polynomial function and return the result
    scalar polyVal(scalar T);

    //- Evaluate the polynomial function and return the result
    scalarField polyVal(scalarField T);


    //- Evaluate the integral of the polynomial function and return the result
    scalar polyInt(scalar T);

    //- Evaluate the integral of the polynomial function and return the result
    scalarField polyInt(scalarField T);


    //- Write the function coefficients
    void writeData(Ostream& os);
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
