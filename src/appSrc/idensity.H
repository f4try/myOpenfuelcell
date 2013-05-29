    scalarField& idensityIn = idensity.internalField();
    smearPatchToMesh smearIdensity(fuelMesh, anodeName);
    idensityIn = smearIdensity.smear(i);
    idensity.correctBoundaryConditions();
    idensity.boundaryField()[anodeID] == i;

