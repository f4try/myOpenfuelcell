// Current density

    scalarField& idensityIn = idensity.primitiveFieldRef();
    smearPatchToMesh smearIdensity(electrolyteMesh, electrolyteAnodeName);
    idensityIn = smearIdensity.smear(i);
    idensity.correctBoundaryConditions();

// Nernst Potential

    scalarField& NernstPotIn = NernstPot.primitiveFieldRef();
    smearPatchToMesh smearNernstPot(electrolyteMesh, electrolyteAnodeName);
    NernstPotIn = smearNernstPot.smear(Nernst);
    NernstPot.correctBoundaryConditions();


