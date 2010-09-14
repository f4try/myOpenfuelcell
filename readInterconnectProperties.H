    Info<< "Reading interconnect properties\n" << endl;

    IOdictionary interconnectProperties
    (
        IOobject
        (
            "interconnectProperties",
            runTime.constant(),
            interconnectMesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    dimensionedScalar rhoInterconnect(interconnectProperties.lookup("rho"));
    dimensionedScalar CpInterconnect(interconnectProperties.lookup("Cp"));
    //dimensionedScalar kI(interconnectProperties.lookup("k"));
