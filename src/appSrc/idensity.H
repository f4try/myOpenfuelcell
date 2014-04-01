    scalarField& idensityIn = idensity.internalField();
    smearPatchToMesh smearIdensity(electrolyteMesh, electrolyteAnodeName);
    idensityIn = smearIdensity.smear(i);
    idensity.correctBoundaryConditions();


