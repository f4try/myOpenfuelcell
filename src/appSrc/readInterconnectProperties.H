    Info<< "Reading interconnect0 & interconnect1 properties\n" << endl;

    IOdictionary interconnectProperties
    (
        IOobject
        (
            "interconnectProperties",
            runTime.constant(),
            interconnectBottomMesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    dimensionedScalar rhointerconnectBottom
        (interconnectProperties.lookup("rho"));

    dimensionedScalar CpinterconnectBottom
        (interconnectProperties.lookup("Cp"));

    dimensionedScalar kinterconnectBottom
        (interconnectProperties.lookup("k"));


    dimensionedScalar rhointerconnectTop
        (interconnectProperties.lookup("rho"));

    dimensionedScalar CpinterconnectTop
        (interconnectProperties.lookup("Cp"));

    dimensionedScalar kinterconnectTop
        (interconnectProperties.lookup("k"));


    Info<< "  bottom" << nl
        << "    rhointerconnectBottom = " << rhointerconnectBottom << nl
        << "    CpinterconnectBottom  = " << CpinterconnectBottom << nl
        << "    kinterconnectBottom   = " << kinterconnectBottom << nl
        << "  top" << nl
        << "    rhointerconnectTop = " << rhointerconnectTop << nl
        << "    CpinterconnectTop  = " << CpinterconnectTop << nl
        << "    kinterconnectTop   = " << kinterconnectTop << nl
        << endl;

