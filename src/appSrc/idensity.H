// Current density

    scalarField& idensityIn = idensity.internalField();
    smearPatchToMesh smearIdensity(electrolyteMesh, electrolyteAnodeName);
    idensityIn = smearIdensity.smear(i);
    idensity.correctBoundaryConditions();

// Nernst Potential

    scalarField& NernstPotIn = NernstPot.internalField();
    smearPatchToMesh smearNernstPot(electrolyteMesh, electrolyteAnodeName);
    NernstPotIn = smearNernstPot.smear(Nernst);
    NernstPot.correctBoundaryConditions();


