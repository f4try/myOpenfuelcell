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
    A namespace for various activation overpotential implementations

Class
    Foam::activationOverpotentialModel

Description
    Base class for diffusivity models used for laminar incompressible
    flow with two component mixtures and/or porous media.

SourceFiles
    activationOverpotentialModel.C
    newDiffusivityModel.C

\*---------------------------------------------------------------------------*/

#ifndef activationOverpotentialModel_H
#define activationOverpotentialModel_H

#include "IOdictionary.H"
#include "typeInfo.H"
#include "runTimeSelectionTables.H"
#include "volFieldsFwd.H"
#include "dimensionedScalar.H"
#include "autoPtr.H"
#include "tmp.H"
#include "scalarField.H"
#include "labelList.H"
#include "speciesTable.H"
#include "physicalConstants.H"
#include "fvMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                Class activationOverpotentialModel Declaration
\*---------------------------------------------------------------------------*/

class activationOverpotentialModel
{
    // Private data

        // Stoichiometric numbers
        List<scalar> stoichiometricNumbers_;

        // Active species switch (false if the species is electrocehmically
        // inert)
        List<Switch> activeSwitch_;

        // Pre-exponent
        scalar gamma_;

        // Activation energy
        dimensionedScalar E_;

        // Transfer coefficient
        scalar alpha_;

        // Relaxation factor
        scalar relax_;


    // Private member functions

        //- Disallow copy construct
        activationOverpotentialModel(const activationOverpotentialModel&);

        //- Disallow default bitwise assignment
        void operator=(const activationOverpotentialModel&);

public:

    //- Runtime type information
    TypeName("activationOverpotentialModel");

    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            activationOverpotentialModel,
            dictionary,
            (
                const speciesTable& speciesNames,
                const dictionary& dict
            ),
            (speciesNames, dict)
        );


    // Selectors

        //- Return a reference to the selected diffusivity model
        static autoPtr<activationOverpotentialModel> New
        (
            const speciesTable& speciesNames,
            const dictionary& dict
        );


    // Constructors

        activationOverpotentialModel
        (
            const speciesTable& speciesNames,
            const dictionary& dict
        );

    // Destructor

        virtual ~activationOverpotentialModel();


    // Member Functions

	    //- Returns the exhange current density
        virtual const tmp<scalarField> exchangeCurrentDensity
        (
            const scalarField& electrodeT,
            const scalarField& pPatch,
            const PtrList<scalarField> molFraction
        ) const = 0;

	    //- Returns the current density as a function of overpotential
        virtual tmp<scalarField> currentDensity// Added SZ and SBB
        (
            const scalarField& electrodeT,
            const scalarField& pPatch,
            const PtrList<scalarField> molFraction,
            const scalarField& overPotential
        ) const = 0;

	    //- Returns the overpotential as a function of current density

        virtual tmp<scalarField> overPotential// Added SZ and SBB
        (
            const scalarField& electrodeT,
            const scalarField& pPatch,
            const PtrList<scalarField> molFraction,
            const scalarField& i
        ) const = 0;

        //- Acess to stoichiometricNumbers_
        const List<scalar>& stoichiometricNumbers() const
        {
            return stoichiometricNumbers_;
        }        

        //- Acess to activeSwitch_
        const List<Switch>& activeSwitch() const
        {
            return activeSwitch_;
        }

        //- Acess to alpha
        scalar alpha() const
        {
            return alpha_;
        }

        //- Acess to relax
        scalar relax() const
        {
            return relax_;
        }

        //- Acess to gamma
        scalar gamma() const
        {
            return gamma_;
        }

        //- Acess to E
        dimensionedScalar E() const
        {
            return E_;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
