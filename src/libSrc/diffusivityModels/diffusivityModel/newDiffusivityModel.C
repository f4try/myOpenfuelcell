/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/

#include "error.H"
#include "diffusivityModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace diffusivityModels
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

autoPtr<diffusivityModel> diffusivityModel::New
(
    fvMesh& mesh,
    scalarField& diff,
    const List<int>& cells,
    const dictionary& dict
)
{
    word diffTypeName = dict.lookup("type");

    Info<< "Selecting diffusivity model " << diffTypeName << endl;

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(diffTypeName);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "diffusivityModel::New(fvMesh& mesh, scalarField&, "
            "const labelList&)"
        )   << "Unknown diffusivityModel type " << diffTypeName
            << endl << endl
            << "Valid diffusivityModel types are :" << endl
            << dictionaryConstructorTablePtr_->toc()
            << exit(FatalError);
    }

    return autoPtr<diffusivityModel>(cstrIter()(mesh, diff, cells, dict));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diffusivityModels
} // End namespace Foam

// ************************************************************************* //
