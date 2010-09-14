/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2007 Hrvoje Jasak
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

Application
    sofcFoam

Description
    Steady solver for the idealised fuel cell model
    Initial code by H.Jasak under project work for NRC Canada, Feb/2007
    Revised for steady solution with coupled regions
    
\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "coupledFvMatrices.H"
#include "regionCouplePolyPatch.H"
#include "atomicWeights.H"
#include "physicalConstants.H"
#include "specie.H"

#include "continuityErrs.H"
#include "patchToPatchInterpolation.H"
#include "fixedGradientFvPatchFields.H"
#include "smearPatchToMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
#   include "setRootCase.H"
#   include "createTime.H"

    // Air-related components
#   include "createAirMesh.H"
#   include "readAirProperties.H"
#   include "createAirFields.H"

    // Fuel-related components
#   include "createFuelMesh.H"
#   include "readFuelProperties.H"
#   include "createFuelFields.H"

    // Electrolyte components
#   include "createElectrolyteMesh.H"
#   include "readElectrolyteProperties.H"
#   include "createElectrolyteFields.H"
#   include "readCellProperties.H"

    // Interconnect components
#   include "createInterconnectMesh.H"
#   include "readInterconnectProperties.H"
#   include "createInterconnectFields.H"

#   include "coupleConductivities.H"

#   include "createElectrodeInterpolation.H"
#   include "createAnodeToAnodeInterpolation.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    for (runTime++; !runTime.end(); runTime++)
    {
        Info<< "Time = " << runTime.timeName() << nl << endl;

    #   include "detachPatches.H"
    #   include "solveFuel.H"
    #   include "solveAir.H"

    #   include "attachPatches.H"
    #   include "solveEnergy.H"
    #   include "detachPatches.H"

    #   include "solveElectrochemistry.H"

    #   include "solveFuelScalars.H"
    #   include "solveAirScalars.H"

        runTime.write();

        Info<< "ExecutionTime = "
            << runTime.elapsedCpuTime()
            << " s\n\n" << endl;
    }

    Info<< "End\n" << endl;

    return(0);
}


// ************************************************************************* //
