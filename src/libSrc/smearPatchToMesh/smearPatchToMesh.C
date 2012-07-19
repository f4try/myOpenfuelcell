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
    smearPatchToMesh

Description
    Smear interpolation

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

\*----------------------------------------------------------------------------*/

#include "smearPatchToMesh.H"
#include "MeshWave.H"
#include "regionInfo.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::smearPatchToMesh::calcAddressing() const
{
    if (addressingPtr_)
    {
        FatalErrorIn("void smearPatchToMesh::calcAddressing() const")
            << "Addressing already calculated"
            << abort(FatalError);
    }

    // Create initial lists
    const label patchStart = mesh_.boundaryMesh()[patchIndex_].start();
    const label patchSize = mesh_.boundaryMesh()[patchIndex_].size();

    labelList patchFaces(patchSize);
    List<regionInfo> patchValues(patchSize);

    forAll (patchFaces, i)
    {
        patchFaces[i] = patchStart + i;
        patchValues[i]= regionInfo(i);
    }

    MeshWave<regionInfo> wave(mesh_, patchFaces, patchValues, mesh_.nCells());

    // Copy index from region into a list
    addressingPtr_ = new labelList(mesh_.nCells());
    labelList& addr = *addressingPtr_;

    const List<regionInfo> ri = wave.allCellInfo();

    forAll (addr, i)
    {
        addr[i] = ri[i].region();
    }

    //Info << "cell info: " << addr << endl;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
Foam::smearPatchToMesh::smearPatchToMesh
(
    const fvMesh& mesh,
    const word& patchName
)
:
    mesh_(mesh),
    patchIndex_(mesh.boundaryMesh().findPatchID(patchName)),
    addressingPtr_(NULL)
{
    if (patchIndex_ < 0)
    {
        FatalErrorIn
        (
            "smearPatchToMesh::smearPatchToMesh\n"
            "(\n"
            "    const fvMesh& mesh,\n"
            "    const word& patchName\n"
            ")"
        )   << "Patch " << patchName << "not found"
            << abort(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::smearPatchToMesh::~smearPatchToMesh()
{
    deleteDemandDrivenData(addressingPtr_);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const Foam::labelList& Foam::smearPatchToMesh::addressing() const
{
    if (!addressingPtr_)
    {
        calcAddressing();
    }

    return *addressingPtr_;
}


// ************************************************************************* //
