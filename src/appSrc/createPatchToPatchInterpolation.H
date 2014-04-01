    // Electrochemistry will be solved on the anode

    const polyPatch& anodePatch = fuelMesh.boundaryMesh()[anodeID];
    const polyPatch& cathodePatch = airMesh.boundaryMesh()[cathodeID];
    const polyPatch& electrolyteAnodePatch = electrolyteMesh.boundaryMesh()[electrolyteAnodeID];

    patchToPatchInterpolation anodeToCathode
    (
        anodePatch,
        cathodePatch
    );

    patchToPatchInterpolation cathodeToAnode
    (
        cathodePatch,
        anodePatch
    );

    // Create a mask from global faces to the anode surface on the fuel mesh
    labelList anodeMask(anodePatch.size());

    forAll (anodeMask, i)
    {
        anodeMask[i] = fuelFaceMap[anodePatch.start() + i];
    }

    // Create a mask from global faces to the cathode surface on the air mesh
    labelList cathodeMask(cathodePatch.size());

    forAll (cathodeMask, i)
    {
        cathodeMask[i] = airFaceMap[cathodePatch.start() + i];
    }

    // Create a mask from global faces to the anode surface on the fuel mesh
    labelList electrolyteAnodeMask(electrolyteAnodePatch.size());

    forAll (electrolyteAnodeMask, i)
    {
        electrolyteAnodeMask[i] = electrolyteFaceMap[electrolyteAnodePatch.start() + i];
    }



    // Ohmic heating will occur in the electrolyte
    // Interpolation is no longer required (R.Nishida, 01.04.2014)

    patchToPatchInterpolation fuelAnodeToElectrolyteAnode
    (
        fuelMesh.boundaryMesh()[anodeID],
        electrolyteMesh.boundaryMesh()[electrolyteAnodeID]
    );
