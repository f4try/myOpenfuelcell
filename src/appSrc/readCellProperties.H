    Info<< "Reading cell properties\n" << endl;

    IOdictionary cellProperties
    (
        IOobject
        (
            "cellProperties",
            runTime.constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );


    dimensionedScalar V(cellProperties.lookup("voltage"));
    dimensionedScalar ibar0(cellProperties.lookup("ibar0"));
    dimensionedScalar Rhat(cellProperties.lookup("Rhat"));
    dimensionedScalar Tinit(cellProperties.lookup("Tinit"));

    // echo inputs from dictionary to stdout
    Info<< "    V     = " << V     << nl
        << "    ibar0 = " << ibar0 << nl
        << "    Rhat  = " << Rhat  << nl
        << "    Tinit = " << Tinit << nl
        << endl;


    // special patch names
    // -------------------
    word anodeName(cellProperties.lookup("anodePatch"));
    word fuelInletName(cellProperties.lookup("fuelInletPatch"));
    word fuelOutletName(cellProperties.lookup("fuelOutletPatch"));

    word cathodeName(cellProperties.lookup("cathodePatch"));
    word airInletName(cellProperties.lookup("airInletPatch"));
    word airOutletName(cellProperties.lookup("airOutletPatch"));

    word electrolyteAnodeName
        (cellProperties.lookup("electrolyteAnodePatch"));

    word electrolyteCathodeName
        (cellProperties.lookup("electrolyteCathodePatch"));


    // echo inputs from dictionary to stdout
    Info
        << "    anodeName              = " << anodeName << nl
        << "    fuelInletName          = " << fuelInletName << nl
        << "    fuelOutletName         = " << fuelOutletName << nl
        << "    cathodeName            = " << cathodeName << nl
        << "    airInletName           = " << airInletName << nl
        << "    airOutletName          = " << airOutletName << nl
        << "    electrolyteAnodeName   = " << electrolyteAnodeName << nl
        << "    electrolyteCathodeName = " << electrolyteCathodeName << nl
        << endl;

