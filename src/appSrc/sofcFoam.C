/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           |
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
    Steady solver for the idealised fuel cell model under project work for
    NRC Canada, Feb/2007-

Developed by
    Hrvoje Jasak (h.jasak@wikki.co.uk)
    Dong Hyup Jeon (DongHyup.Jeon@nrc-cnrc.gc.ca)
    Helmut Roth (helmut.roth@nrc-cnrc.gc.ca)
    Hae-won Choi (haewon1972@gmail.com)

\*---------------------------------------------------------------------------*/

#include <iostream>
#include <iomanip>

#include "fvCFD.H"
#include "atomicWeights.H"
#include "physicalConstants.H"
#include "specie.H"
#include "speciesTable.H"
#include "sofcSpecie.H"

#include "patchToPatchInterpolation.H"
#include "continuityErrs.H"
#include "initContinuityErrs.H"
#include "fixedGradientFvPatchFields.H"
#include "smearPatchToMesh.H"

#include "diffusivityModels.H"
#include "porousZones.H"
#include "polyToddYoung.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
#   include "setRootCase.H"
#   include "createTime.H"

    // Complete cell components
#   include "createMesh.H"
#   include "readCellProperties.H"
#   include "createCellFields.H"

    // Interconnect0 components
#   include "createInterconnect0Mesh.H"

     // Air-related components
#   include "createAirMesh.H"
#   include "readAirProperties.H"
#   include "createAirFields.H"
#   include "createAirSpecies.H"

    // Electrolyte components
#   include "createElectrolyteMesh.H"
#   include "readElectrolyteProperties.H"
#   include "createElectrolyteFields.H"

    // Fuel-related components
#   include "createFuelMesh.H"
#   include "readFuelProperties.H"
#   include "createFuelFields.H"
#   include "createFuelSpecies.H"

    // Interconnect1 components
#   include "createInterconnect1Mesh.H"
#   include "readInterconnectProperties.H"

#   include "readRxnProperties.H"

#   include "setGlobalPatchIds.H"

    // calculate electrolye thickness, hE
#   include "electrolyteThickness.H"

    // Cathode & Anode interpolation
#   include "createPatchToPatchInterpolation.H"

    // Gas diffusivity models
#   include "createDiffusivityModels.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    bool firstTime = true;

    for (runTime++; !runTime.end(); runTime++)
    {
        Info<< "Time = " << runTime.timeName() << nl << endl;

    #   include "mapFromCell.H"    // map global T to fluid regions

    #   include "rhoAir.H"
    #   include "rhoFuel.H"

    #   include "muAir.H"
    #   include "muFuel.H"

    #   include "kAir.H"
    #   include "kFuel.H"

    #   include "solveFuel.H"
    #   include "solveAir.H"
    #   include "ReynoldsNumber.H"

    #   include "diffusivityAir.H"
    #   include "diffusivityFuel.H"

    #   include "YfuelEqn.H"
    #   include "YairEqn.H"

    #   include "solveElectrochemistry.H"

    #   include "mapToCell.H"
    #   include "solveEnergy.H"

        runTime.write();

        if(firstTime)
        {
            firstTime = false;
        }

        Info<< "ExecutionTime = "
            << runTime.elapsedCpuTime()
            << " s\n\n" << endl;
    }

    Info<< "End\n" << endl;
    return(0);
}


// ************************************************************************* //
