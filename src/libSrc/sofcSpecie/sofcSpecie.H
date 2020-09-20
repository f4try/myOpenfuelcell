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

Class
    Foam::sofcSpecie

Description
    Set of electrochemical properties for a sofc specie

SourceFiles
    sofcSpecieI.H
    sofcSpecie.C

\*---------------------------------------------------------------------------*/

#ifndef sofcSpecie_H
#define sofcSpecie_H

#include "word.H"
#include "scalar.H"
#include "Switch.H"

#include "fvMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class Istream;
class Ostream;

/*---------------------------------------------------------------------------*\
                           Class sofcSpecie Declaration
\*---------------------------------------------------------------------------*/

class sofcSpecie
{
    // Private data

        //- Name of sofcSpecie
        word name_;

        //- Molecular weight of sofcSpecie [kg/kmol]
        scalar molWeight_;

        //- Number of electrons this specie contributes to electrochemistry
        scalar nElectrons_;

        //- Is the specie consumed(-1), produced(+1), or inert(0)?
        label rSign_;

        //- Enthalpy of formation at 298.15 K
        scalar hForm_;

        //- Standard entropy at 298.15 K
        scalar sForm_;

        //- Reference to the finite volume mesh this sofcSpecie is part of
	//const fvMesh& mesh_;


    // Private member functions

        //- Construct from components without name
        inline sofcSpecie
        (
            const scalar molWeight,
            const scalar nElectrons,
            const label rSign,
            const scalar hForm,
            const scalar sForm
        );


public:

    // Constructors

        //- Construct from components with name
        inline sofcSpecie
        (
            const word& name,
            const scalar molWeight,
            const scalar nElectrons,
            const label rSign,
            const scalar hForm,
            const scalar sForm
        );

        //- Construct as copy
        inline sofcSpecie(const sofcSpecie&);

        //- Construct as named copy
        inline sofcSpecie(const word& name, const sofcSpecie&);

	//- Construct and return a clone
	inline autoPtr<sofcSpecie> clone() const;

        //- Construct from Istream
        sofcSpecie(Istream&);


        //- Return pointer to new sofcSpecie created on freestore from Istream
	autoPtr<sofcSpecie> New(Istream& is);


    // Member Functions

        // Access

            //- Name
            inline word name() const;

            //- Molecular weight [kg/kmol]
            inline scalar MW() const;

            //- Number of electrons this specie contributes to electrochemistry
            inline scalar ne() const;

            //- Is the specie consumed(<0), produced(>0), or inert(0)?
            inline label rSign() const;

            //- Enthalpy of formation at 298.15 K
            inline scalar hForm() const;

            //- Standard entropy at 298.15 K
            inline scalar sForm() const;


    // Member operators

        inline void operator=(const sofcSpecie&);


    // Ostream Operator

        friend Ostream& operator<<(Ostream&, const sofcSpecie&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "sofcSpecieI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
