    Info<< "Reading fuel properties\n" << endl;

    IOdictionary fuelProperties
    (
        IOobject
        (
            "fuelProperties",
            runTime.constant(),
            fuelMesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    dimensionedScalar muF(fuelProperties.lookup("mu"));
    dimensionedScalar rhoF(fuelProperties.lookup("rho"));
    dimensionedScalar CpF(fuelProperties.lookup("Cp"));
    dimensionedScalar kFuel(fuelProperties.lookup("k"));

    // hydraulic diameter for Reynolds number calculation
    dimensionedScalar dHydFuel(fuelProperties.lookup("dHyd"));

    // echo input to output:
    Info << "    muF       = " << muF       << nl
         << "    rhoF      = " << rhoF      << nl
         << "    CpF       = " << CpF       << nl
         << "    kFuel     = " << kFuel     << nl
         << "    dHydFuel  = " << dHydFuel  << nl
         << endl;

