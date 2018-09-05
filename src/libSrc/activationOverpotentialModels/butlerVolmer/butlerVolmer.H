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

Description
    A Butler-Volmer overpotential model.

Class
    Foam::butlerVolmer

Description
    Base class for diffusivity models used for laminar incompressible
    flow with two component mixtures and/or porous media.

SourceFiles
    activationOverpotentialModel.C
    newDiffusivityModel.C

\*---------------------------------------------------------------------------*/

#ifndef butlerVolmer_H
#define butlerVolmer_H

#include "activationOverpotentialModel.H"
#include "dimensionedScalar.H"
#include "primitiveFieldsFwd.H"
#include "labelList.H"
#include "volFieldsFwd.H"
#include "fvMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

//class activationOverpotentialModel;

/*---------------------------------------------------------------------------*\
                           Class butlerVolmer Declaration
\*---------------------------------------------------------------------------*/

class butlerVolmer
:
    public activationOverpotentialModel
{

public:

    //- Runtime type information
    TypeName("butlerVolmer");


    // Constructors

        //- construct from mesh, diffsivity field labelList and dictionary
        butlerVolmer
        (
            const speciesTable& speciesNames,
            const dictionary& dict
        );


    // Destructor

        ~butlerVolmer()
        {}


    // Member functions

	    //- Returns the exhange current density
        virtual const tmp<scalarField> exchangeCurrentDensity
        (
            const scalarField& electrodeT,
            const scalarField& pPatch,
            const PtrList<scalarField> molFraction
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

