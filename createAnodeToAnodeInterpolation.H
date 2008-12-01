    // Find anode patches on fuel and electrolyte meshes and 
    // create interpolation
    // Electrochemistry will be solved on the anode
    // Ohmic heating will occur in thr electrolyte
    // For now we assume anode patch has same anodeName on both meshes

    label fuelAnodeID = fuelMesh.boundaryMesh().findPatchID(anodeName);

    if (fuelAnodeID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << anodeName << " on the fuel mesh."
            << abort(FatalError);
    }

    label eletrolyteAnodeID =
                       electrolyteMesh.boundaryMesh().findPatchID(anodeName);

    if (eletrolyteAnodeID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << anodeName << " on the eletrolyte mesh."
            << abort(FatalError);
    }


    patchToPatchInterpolation fuelAnodeToElectrolyteAnode
    (
        fuelMesh.boundaryMesh()[fuelAnodeID],
        electrolyteMesh.boundaryMesh()[eletrolyteAnodeID]
    );

