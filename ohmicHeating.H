    // calculate volumetric ohmic heating

    Info << nl << "Calculating ohmic heating" << endl;

    // thickness of electrolyte, hE, is read from electrolyte properties

    // interpolate anode current density i from fuel side to electrolyte side
    scalarField iEA = 
       fuelAnodeToElectrolyteAnode.faceInterpolate(i);

    // combine with overpotential and thickness of electrolyte
    iEA *= (E-V.value())/hE.value();

    // smear the surface heating field into electrolyte volume
    smearPatchToMesh smearInterpolation(electrolyteMesh, anodeName);
    ohmicHeating = smearInterpolation.smear(iEA);

    Info << "min,max(ohmicHeating): "
         << Foam::min(ohmicHeating) <<" , " 
         << Foam::max(ohmicHeating) << endl;

