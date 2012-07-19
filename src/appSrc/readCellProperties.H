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


    // Read anode and cathode patch names
    word anodeName(cellProperties.lookup("anodePatch"));
    word cathodeName(cellProperties.lookup("cathodePatch"));


    word electrolyteAnodeName
    (
        cellProperties.lookup("electrolyteAnodePatch")
    );

    word electrolyteCathodeName
    (
        cellProperties.lookup("electrolyteCathodePatch")
    );


    // echo inputs from dictionary to stdout
    Info<< "    V     = " << V     << nl
        << "    ibar0 = " << ibar0 << nl
        << "    Rhat  = " << Rhat  << nl
        << "    Tinit = " << Tinit << nl
        << "    anodeName              = " << anodeName << nl
        << "    cathodeName            = " << cathodeName << nl
        << "    electrolyteAnodeName   = " << electrolyteAnodeName << nl
        << "    electrolyteCathodeName = " << electrolyteCathodeName << nl
        << endl;


    // Calculate molecular weights [kg/mol] and valencies
    const scalar MH2 = (2*atomicWeights["H"])/1000.0;
    const scalar MH2O= (2*atomicWeights["H"] + atomicWeights["O"])/1000.0;
    const scalar MO2 = (2*atomicWeights["O"])/1000.0;
    const scalar MN2 = (2*atomicWeights["N"])/1000.0;

    const scalar vH2 = 2;
    const scalar vH2O= 2;
    const scalar vO2 = 4;

    //echo to stdout
    Info<< "    MH2  = " << MH2   << nl
        << "    MH2O = " << MH2O  << nl
        << "    MO2  = " << MO2   << nl
        << "    MN2  = " << MN2   << nl
        << "    vH2  = " << vH2   << nl
        << "    vH2O = " << vH2O  << nl
        << "    vO2  = " << vO2   << nl
        << endl;


    // Faraday's constant (charge of electron)
    const scalar F = physicalConstant::F.value();

    // Universal gas constant
    const scalar Rgas = physicalConstant::Rgas.value();

