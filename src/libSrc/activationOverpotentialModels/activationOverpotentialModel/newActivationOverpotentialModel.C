/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/

#include "error.H"
#include "activationOverpotentialModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

autoPtr<activationOverpotentialModel> activationOverpotentialModel::New
(
    const speciesTable& speciesNames,
    const dictionary& dict
)
{
    word diffTypeName = dict.lookup("type");

    Info<< "Selecting activation overpotential model..." << diffTypeName << endl;

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(diffTypeName);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "activationOverpotentialModel::New(const speciesTable& speciesNames, const dictionary& dict)"
        )   << "Unknown activationOverpotentialModel type " << diffTypeName
            << endl << endl
            << "Valid activationOverpotentialModel types are :" << endl
            << dictionaryConstructorTablePtr_->toc()
            << exit(FatalError);
    }

    return autoPtr<activationOverpotentialModel>(cstrIter()(speciesNames, dict));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
