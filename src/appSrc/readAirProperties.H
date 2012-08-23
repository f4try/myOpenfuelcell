    Info<< "Reading air properties\n" << endl;

    IOdictionary airProperties
    (
        IOobject
        (
            "airProperties",
            runTime.constant(),
            airMesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    dimensionedScalar muA(airProperties.lookup("mu"));
    dimensionedScalar rhoA(airProperties.lookup("rho"));
    dimensionedScalar CpA(airProperties.lookup("Cp"));
    dimensionedScalar kAir(airProperties.lookup("k"));

    // hydraulic diameter for Reynolds number calculation
    dimensionedScalar dHydAir(airProperties.lookup("dHyd"));

    // echo input to output:
    Info << "    muA   = "    << muA       << nl
         << "    rhoA  = "    << rhoA      << nl
         << "    CpA   = "    << CpA       << nl
         << "    kAir  = "    << kAir      << nl
         << "    dHydAir  = " << dHydAir   << nl
         << endl;

