    Info<< "Setting special patch IDs\n" << endl;

    // IDs of the named patches
    // ------------------------

    label anodeID = fuelMesh.boundaryMesh().findPatchID(anodeName);
    if (anodeID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << anodeName << " on the fuel mesh."
            << abort(FatalError);
    }

    label fuelInletID = fuelMesh.boundaryMesh().findPatchID(fuelInletName);
    if (fuelInletID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << fuelInletName << " on the fuel mesh."
            << abort(FatalError);
    }

    label fuelOutletID = fuelMesh.boundaryMesh().findPatchID(fuelOutletName);
    if (fuelOutletID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << fuelOutletName << " on the fuel mesh."
            << abort(FatalError);
    }


    label cathodeID = airMesh.boundaryMesh().findPatchID(cathodeName);
    if (cathodeID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << cathodeName << " on the air mesh."
            << abort(FatalError);
    }

    label airInletID = airMesh.boundaryMesh().findPatchID(airInletName);
    if (airInletID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << airInletName << " on the air mesh."
            << abort(FatalError);
    }

    label airOutletID = airMesh.boundaryMesh().findPatchID(airOutletName);
    if (airOutletID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << airOutletName << " on the air mesh."
            << abort(FatalError);
    }


    label electrolyteAnodeID =
        electrolyteMesh.boundaryMesh().findPatchID(electrolyteAnodeName);
    if (electrolyteAnodeID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << electrolyteAnodeName
            << " on the electrolyte mesh."
            << abort(FatalError);
    }

    label electrolyteCathodeID =
        electrolyteMesh.boundaryMesh().findPatchID(electrolyteCathodeName);
    if (electrolyteCathodeID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << electrolyteAnodeName
            << " on the electrolyte mesh."
            << abort(FatalError);
    }

