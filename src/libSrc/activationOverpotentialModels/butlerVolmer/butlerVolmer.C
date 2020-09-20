/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/

#include "butlerVolmer.H"

#include "addToRunTimeSelectionTable.H"
#include "volFields.H"

#include "testFunction.H"
#include "RiddersRoot.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(butlerVolmer, 0);
    addToRunTimeSelectionTable(activationOverpotentialModel, butlerVolmer, dictionary);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

butlerVolmer::butlerVolmer
(
    const speciesTable& speciesNames,
    const dictionary& dict
)
:
    activationOverpotentialModel(speciesNames, dict)
{}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const tmp<scalarField> butlerVolmer::exchangeCurrentDensity
(
    const scalarField& electrodeT,
    const scalarField& pPatch,
    const PtrList<scalarField> molFraction
) const
{
    tmp<scalarField> pexchangeCurrentDensity
    (
        new scalarField (electrodeT.size(), 1)
    );
    scalarField& exCurrentDensity = pexchangeCurrentDensity.ref();

    // Compute local exchange current density
    forAll(stoichiometricNumbers(), fsi)
    {
        if(activeSwitch()[fsi])
        {
            exCurrentDensity *= Foam::pow
            (
                (pPatch*molFraction[fsi]/physicalConstant::pAtm.value()),
                stoichiometricNumbers()[fsi]
            );
        }
    }

    exCurrentDensity *= gamma()*electrodeT;
    exCurrentDensity *= Foam::exp
    (
        -1.*E().value()/(physicalConstant::Rgas.value()*electrodeT)
    );

    return pexchangeCurrentDensity;
}

scalar butlerVolmer::localCurrentDensity
(
    scalar i0,
    scalar T,
    scalar eta
) const
{
    scalar currentDensity;

    scalar A = 2*alpha()*physicalConstant::F.value()/(physicalConstant::Rgas.value()*T);
    scalar B = -2*(1-alpha())*physicalConstant::F.value()/(physicalConstant::Rgas.value()*T);

    currentDensity = Foam::exp
    (
        A*eta
    );
    currentDensity -= Foam::exp
    (
        B*eta
    );

    currentDensity *= i0;

    return currentDensity;
}


tmp<scalarField> butlerVolmer::currentDensity// SZ and SBB
(
    const scalarField& electrodeT,
    const scalarField& pPatch,
    const PtrList<scalarField> molFraction,
    const scalarField& overPotential
) const
{
    tmp<scalarField> pcurrentDensity
    (
        new scalarField (electrodeT.size(), 1)
    );

    scalarField& cCurrentDensity = pcurrentDensity.ref();

    scalarField i0 = exchangeCurrentDensity(electrodeT, pPatch, molFraction).ref();

    // Loop
    forAll(electrodeT, cellI)
    {
        cCurrentDensity[cellI] = localCurrentDensity(i0[cellI], electrodeT[cellI], overPotential[cellI]);
    }

    return pcurrentDensity;
}

tmp<scalarField> butlerVolmer::overPotential// SZ and SBB
(
    const scalarField& electrodeT,
    const scalarField& pPatch,
    const PtrList<scalarField> molFraction,
    const scalarField& currentDensity
) const
{
    tmp<scalarField> pOverPotential
    (
        new scalarField(electrodeT.size(), 0.0)
    );

    scalarField& cOverPotential = pOverPotential.ref();

    scalarField i0 = exchangeCurrentDensity(electrodeT, pPatch, molFraction).ref();

    forAll(electrodeT, cellI)
    {
       testFunction tf(currentDensity[cellI], electrodeT[cellI], i0[cellI], *this);

       cOverPotential[cellI] = RiddersRoot<testFunction>(tf, 1e-5).root(0., 1.2);  
    }

    return pOverPotential;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //

