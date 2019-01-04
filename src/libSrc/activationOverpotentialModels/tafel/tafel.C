/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/

#include "tafel.H"

#include "addToRunTimeSelectionTable.H"
#include "volFields.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(tafel, 0);
    addToRunTimeSelectionTable(activationOverpotentialModel, tafel, dictionary);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

tafel::tafel
(
    const speciesTable& speciesNames,
    const dictionary& dict
)
:
    activationOverpotentialModel(speciesNames, dict)
{}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const tmp<scalarField> tafel::exchangeCurrentDensity
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

scalar tafel::localCurrentDensity
(
    scalar i0,
    scalar T,
    scalar eta
) const
{
    scalar currentDensity;

    scalar A = 2*alpha()*physicalConstant::F.value()/(physicalConstant::Rgas.value()*T);

    currentDensity = i0*Foam::exp(A*eta);

    return currentDensity;
}


tmp<scalarField> tafel::currentDensity// SZ and SBB
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

tmp<scalarField> tafel::overPotential// SZ and SBB
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

    cOverPotential = Foam::max(0.0,Foam::log(currentDensity/i0));  

    return pOverPotential;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //

