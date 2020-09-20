/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2012 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::IOporousZoneList

Description
    List of porosity models with IO functionality

SourceFiles
    IOporousZoneList.C

\*---------------------------------------------------------------------------*/

#ifndef IOporousZoneList_H
#define IOporousZoneList_H

#include "IOdictionary.H"
#include "porousZoneList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class IOporousZoneList Declaration
\*---------------------------------------------------------------------------*/

class IOporousZoneList
:
    public IOdictionary,
    public porousZoneList
{
private:

    // Private Member Functions

        //- Create IO object if dictionary is present
        IOobject createIOobject(const fvMesh& mesh) const;

        //- Disallow default bitwise copy construct
        IOporousZoneList(const IOporousZoneList&);

        //- Disallow default bitwise assignment
        void operator=(const IOporousZoneList&);


public:

    // Constructors

        //- Construct from mesh
        IOporousZoneList(const fvMesh& mesh);


        //- Destructor
        virtual ~IOporousZoneList()
        {}


    // Member Functions

        //- Read dictionary
        virtual bool read();
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
