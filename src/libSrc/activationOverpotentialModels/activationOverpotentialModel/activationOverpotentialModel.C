/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

\*---------------------------------------------------------------------------*/

#include "activationOverpotentialModel.H"
#include "volFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
  defineTypeNameAndDebug(activationOverpotentialModel, 0);
  defineRunTimeSelectionTable(activationOverpotentialModel, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

activationOverpotentialModel::activationOverpotentialModel
(
    const speciesTable& speciesNames,
    const dictionary& dict
)
:
   stoichiometricNumbers_(speciesNames.size()),
   activeSwitch_(speciesNames.size()),
   gamma_(dict.lookupOrDefault<scalar>("gamma", 1.0)),
   E_(dict.lookup("E")),
   alpha_(readScalar(dict.lookup("alpha"))),
   relax_(dict.lookupOrDefault<scalar>("relax", 1.0))
{
    forAll(speciesNames, apI)
    {
        stoichiometricNumbers_[apI] = scalar
        (
            readScalar
            (
                dict.subDict("entries").subDict
                (
                    speciesNames[apI]
                ).lookup("stoichiometricCoeff")
            )
        );
        activeSwitch_[apI] = Switch
        (
            dict.subDict("entries").subDict
            (
                speciesNames[apI]
            ).lookup("active")
        );
    }
}


// * * * * * * * * * * * * * * * * Destructors * * * * * * * * * * * * * * * //

activationOverpotentialModel::~activationOverpotentialModel()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

// none

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
