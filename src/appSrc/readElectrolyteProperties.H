    Info<< "Reading electrolyte properties\n" << endl;

    IOdictionary electrolyteProperties
    (
        IOobject
        (
            "electrolyteProperties",
            runTime.constant(),
            electrolyteMesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );


    // Read electrolyte properties
    dimensionedScalar rhoElectrolyte(electrolyteProperties.lookup("rho"));
    dimensionedScalar CpElectrolyte(electrolyteProperties.lookup("Cp"));
    dimensionedScalar kElectrolyte(electrolyteProperties.lookup("k"));


    //label IDs for electrolyte anode and cathode patches
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


    // electrolye height (thickness) is now calaculated
    #include "electrolyteThickness.H"


    // initial ohmic heating
    dimensionedScalar HsrcElectrolyte(electrolyteProperties.lookup("Hsrc"));

    // echo inputs from dictionary to stdout
    Info
        << "    rhoElectrolyte  = " << rhoElectrolyte  << nl
        << "    CpElectrolyte   = " << CpElectrolyte   << nl
        << "    kElectrolyte    = " << kElectrolyte    << nl
        << "    hE (calculated) = " << hE              << nl
        << "    HsrcElectrolyte = " << HsrcElectrolyte << nl
        << endl;
