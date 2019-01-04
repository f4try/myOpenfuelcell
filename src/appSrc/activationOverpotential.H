//    Activation term prescribed by Butler-Volmer Equation:
//    initialize etaAnode

//    scalarField etaAnode(anodeT.size(), 0);
   // scalarField i0Anode(anodeT.size(), 1);
//    scalarField etaCathode(anodeT.size(), 0);
  //  scalarField i0Cathode(anodeT.size(), 1);

//    tmp<scalarField> i0Anode(fuelActivationOverpotential->exchangeCurrentDensity
//    (
//        anodeT,
//        pFuelPatch,
//        xFuel
//    ));

    tmp<scalarField> etaAnode(fuelActivationOverpotential->overPotential
    (
        anodeT,
        pFuelPatch,
        xFuel,
        i
    ));
   Info<< "min,mean,max calBV(etaAn) = " << gMin(etaAnode()) << "  "
       << gAverage(etaAnode()) << "  " << gMax(etaAnode()) << nl;
/*
   etaAnode =
       (1-fuelActivationOverpotential->relax()) * oldEtaA
     + fuelActivationOverpotential->relax() * etaAnode; //relax the etaAnode, Qing, 22.06.2014

   Info<< "min,mean,max newBV(etaAn) = " << gMin(etaAnode) << "  "
       << gAverage(etaAnode) << "  " << gMax(etaAnode) << nl;
*/


// Compute local cathode exchange current density
// SBB: Must add the air-side pressure
// SBB: Also Cathode overpotential computed with anode temperature

//    tmp<scalarField> i0Cathode(airActivationOverpotential->exchangeCurrentDensity
//    (
//        cathodeT,
//        pAirPatch,
//        xAir
//    ));

    tmp<scalarField> etaCathode(airActivationOverpotential->overPotential
    (
        cathodeT,
        pAirPatch,
        xAir,
        i
    ));
   Info<< "min,mean,max calBV(etaAn) = " << gMin(etaCathode()) << "  "
       << gAverage(etaAnode()) << "  " << gMax(etaCathode()) << nl;

// End activation calculation

//  Save data for output

    scalarField& etaAIn = etaA.primitiveFieldRef();
    smearPatchToMesh smearetaA(electrolyteMesh, electrolyteAnodeName);
    etaAIn = smearetaA.smear(etaAnode());
    etaA.correctBoundaryConditions();
    etaA.boundaryFieldRef()[electrolyteAnodeID] == etaAnode();

    scalarField& etaCIn = etaC.primitiveFieldRef();
    smearPatchToMesh smearetaC(electrolyteMesh, electrolyteAnodeName);
    etaCIn = smearetaC.smear(etaCathode());
    etaC.correctBoundaryConditions();
    etaC.boundaryFieldRef()[electrolyteCathodeID] == etaCathode();



