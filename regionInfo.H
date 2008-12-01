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
    regionInfo

Description
    Holds information regarding region of cell. Used in 'regioning' a mesh.

SourceFiles
    regionInfoI.H
    regionInfo.C

\*---------------------------------------------------------------------------*/

#ifndef regionInfo_H
#define regionInfo_H

#include "point.H"
#include "label.H"
#include "scalar.H"
#include "tensor.H"
#include "polyMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class polyPatch;

/*---------------------------------------------------------------------------*\
                           Class regionInfo Declaration
\*---------------------------------------------------------------------------*/

class regionInfo
{
    // Private data

        label region_;
        
    // Private Member Functions

        //- Update current cell/face type with neighbouring
        //  type. Return true if information needs to propagate,
        //  false otherwise.
        inline bool update
        (
            const regionInfo& w2,
            const label thisFaceI,
            const label thisCellI,
            const label neighbourFaceI,
            const label neighbourCellI
        );

public:

    // Constructors

        //- Construct null
        inline regionInfo();

        //- Construct from cType
        explicit inline regionInfo(const label);

        //- Construct as copy
        inline regionInfo(const regionInfo&);


    // Member Functions

        // Access

            inline label region() const
            {
                return region_;
            }


        // Needed by meshWave

            //- Check whether origin has been changed at all or
            //  still contains original (invalid) value.
            inline bool valid() const;

            //- Check for identical geometrical data. Used for cyclics checking.
            inline bool sameGeometry
            (
                const polyMesh&,
                const regionInfo&,
                const scalar
            ) const;

            //- Convert any absolute coordinates into relative to (patch)face
            //  centre
            inline void leaveDomain
            (
                const polyMesh&,
                const polyPatch& patch,
                const label patchFaceI,
                const point& faceCentre
            );

            //- Reverse of leaveDomain
            inline void enterDomain
            (
                const polyMesh&,
                const polyPatch& patch,
                const label patchFaceI,
                const point& faceCentre
            );

            //- Apply rotation matrix to any coordinates
            inline void transform
            (
                const polyMesh&,
                const tensor& rotTensor
            );

            //- Influence of neighbouring face.
            inline bool updateCell
            (
                const polyMesh&,
                const label thisCellI,
                const label neighbourFaceI,
                const regionInfo& neighbourInfo,
                const scalar tol
            );

            //- Influence of neighbouring cell.
            inline bool updateFace
            (
                const polyMesh&,
                const label thisFaceI,
                const label neighbourCellI,
                const regionInfo& neighbourInfo,
                const scalar tol
            );

            //- Influence of different value on same face.
            inline bool updateFace
            (
                const polyMesh&,
                const label thisFaceI,
                const regionInfo& neighbourInfo,
                const scalar tol
            );

    // Member Operators

        // Note: Used to determine whether to call update.
        inline bool operator==(const regionInfo&) const;

        inline bool operator!=(const regionInfo&) const;


    // IOstream Operators

        friend Ostream& operator<<(Ostream&, const regionInfo&);
        friend Istream& operator>>(Istream&, regionInfo&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "regionInfoI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
