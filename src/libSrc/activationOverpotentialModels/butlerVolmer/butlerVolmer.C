/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/

#include "butlerVolmer.H"
#include "addToRunTimeSelectionTable.H"

#include "volFields.H"

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



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //

