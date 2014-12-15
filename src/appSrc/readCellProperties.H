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

    word fuelCellType(cellProperties.lookup("fuelCellType"));		//Qing, 04.06.2014
    dimensionedScalar PreIterAct(cellProperties.lookup("PreIterAct"));	//Qing, 22.06.2014
    dimensionedScalar PreIterV(cellProperties.lookup("PreIterV"));	//Qing, 22.06.2014

    Switch galvanostatic(cellProperties.lookup("galvanostatic"));
    dimensionedScalar V(cellProperties.lookup("voltage"));
    dimensionedScalar ibar0(cellProperties.lookup("ibar0"));
    dimensionedScalar Tinit(cellProperties.lookup("Tinit"));
    dimensionedScalar numCells(cellProperties.lookup("numCells"));
    dimensionedScalar Rhat("Rhat", dimensionSet(1, 4, -3, 0, 0, -2, 0), 0);
    dimensionedScalar Rhat0("Rhat0", dimensionSet(1, 4, -3, 0, 0, -2, 0), 0);	//Qing, 22.06.2014
    // echo inputs from dictionary to stdout
    // -------------------------------------
    if (galvanostatic)
    {
        Info<< "galvanostatic run" << nl;
        Info<< "    V     = " << V << nl;
        Info<< "    ibar0 = " << ibar0 << nl;

        Rhat = cellProperties.lookup("Rhat");
        Info<< "    Rhat  = " << Rhat << nl;

        Rhat0 = cellProperties.lookup("Rhat0");
	Info<< "    Rhat0  = " << Rhat0 << nl;
    }
    else
    {
        Info<< "potentiostatic run" << nl;
        Info<< "    ibar0 = " << ibar0 << nl;
        Info<< "    V     = " << V << nl;
    }

    Info<< "    Tinit = " << Tinit << nl;
    Info<< endl;



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

