    Info<< "Reading interconnect0 & interconnect1 properties\n" << endl;

    IOdictionary interconnectProperties
    (
        IOobject
        (
            "interconnectProperties",
            runTime.constant(),
            interconnect0Mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    dimensionedScalar rhoInterconnect0
        (interconnectProperties.lookup("rho"));

    dimensionedScalar CpInterconnect0
        (interconnectProperties.lookup("Cp"));

    dimensionedScalar kInterconnect0
        (interconnectProperties.lookup("k"));


    dimensionedScalar rhoInterconnect1
        (interconnectProperties.lookup("rho"));

    dimensionedScalar CpInterconnect1
        (interconnectProperties.lookup("Cp"));

    dimensionedScalar kInterconnect1
        (interconnectProperties.lookup("k"));


    Info<< "  interconnect0" << nl
        << "    rhoInterconnect0 = " << rhoInterconnect0 << nl
        << "    CpInterconnect0  = " << CpInterconnect0 << nl
        << "    kInterconnect0   = " << kInterconnect0 << nl
        << "  interconnect1" << nl
        << "    rhoInterconnect1 = " << rhoInterconnect1 << nl
        << "    CpInterconnect1  = " << CpInterconnect1 << nl
        << "    kInterconnect1   = " << kInterconnect1 << nl
        << endl;

