    // Find anode and cathode patch and create interpolation
    // Electrochemistry will be solved on the anode

    label anodeID = fuelMesh.boundaryMesh().findPatchID(anodeName);

    if (anodeID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << anodeName << " on the fuel mesh."
            << abort(FatalError);
    }

    label cathodeID = airMesh.boundaryMesh().findPatchID(cathodeName);

    if (cathodeID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << cathodeName << " on the air mesh."
            << abort(FatalError);
    }


    const polyPatch& anodePatch = fuelMesh.boundaryMesh()[anodeID];

    patchToPatchInterpolation anodeToCathode
    (
        anodePatch,
        airMesh.boundaryMesh()[cathodeID]
    );

    patchToPatchInterpolation cathodeToAnode
    (
        airMesh.boundaryMesh()[cathodeID],
        anodePatch
    );

/****************************************************************************\
    // Create a mask form global faces to the cathode surface on the fuel mesh
    labelList anodeMask(anodePatch.size());

    forAll (anodeMask, i)
    {
        anodeMask[i] = airFaceMap[anodePatch.start() + i];
    }
\****************************************************************************/
