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

    // Calculate molecular weights [kg/mol] and valencies
    const scalar MH2 = (2*atomicWeights["H"])/1000.0;
    const scalar MH2O = (2*atomicWeights["H"] + atomicWeights["O"])/1000.0;
    const scalar MO2 = (2*atomicWeights["O"])/1000.0;
    const scalar MN2 = (2*atomicWeights["N"])/1000.0;

    const scalar vH2 = 2;
    const scalar vH2O = 2;
    const scalar vO2 = 4;

    // Faraday's constant (charge of electron)
    const scalar F = physicalConstant::F.value();

    // electrolye height (thickness)
    const dimensionedScalar hE(electrolyteProperties.lookup("hE"));

    // initial ohmic heating
    dimensionedScalar HsrcElectrolyte(electrolyteProperties.lookup("Hsrc"));

