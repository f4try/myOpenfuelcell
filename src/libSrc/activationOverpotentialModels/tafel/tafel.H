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
    A Tafel overpotential model.

Class
    Foam::tafel

Description
//    i = i0[exp(2*alpha*F*eta/RT)]
//    eta = Activation overpotential obtained by numerical iversion of BV eqn.
//    i = local current density
//    i0 = exchange current density
//    alpha = transfer coefficient 
//    R = universal gas constant
//    F = Faraday constant
//    i0 = gamma*(Pi*pj^m)_j*exp(EAct/(R*T))
//    where
//    gamma = pre-exponent
//    pj = partial pressure, species j
//    pi = Product
//    m  = reaction orders 
//    EAct = Actuation energy
//

SourceFiles
    activationOverpotentialModel.C
    newDiffusivityModel.C

\*---------------------------------------------------------------------------*/

#ifndef tafel_H
#define tafel_H

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
                           Class tafel Declaration
\*---------------------------------------------------------------------------*/

class tafel
:
    public activationOverpotentialModel
{

public:

    //- Runtime type information
    TypeName("tafel");


    // Constructors

        //- construct from mesh, diffsivity field labelList and dictionary
        tafel
        (
            const speciesTable& speciesNames,
            const dictionary& dict
        );


    // Destructor

        ~tafel()
        {}


    // Member functions

	    //- Returns the exhange current density
        virtual const tmp<scalarField> exchangeCurrentDensity
        (
            const scalarField& electrodeT,
            const scalarField& pPatch,
            const PtrList<scalarField> molFraction
        ) const;

	    //- Returns the current density as a function of overpotential
        virtual tmp<scalarField> currentDensity// Added SZ and SBB
        (
            const scalarField& electrodeT,
            const scalarField& pPatch,
            const PtrList<scalarField> molFraction,
            const scalarField& overPotential
        ) const;

        scalar localCurrentDensity(scalar i0, scalar T, scalar eta) const;

	    //- Returns the overpotential as a function of current density
        virtual tmp<scalarField> overPotential// Added SZ and SBB
        (
            const scalarField& electrodeT,
            const scalarField& pPatch,
            const PtrList<scalarField> molFraction,
            const scalarField& currentDensity
        ) const;


};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

