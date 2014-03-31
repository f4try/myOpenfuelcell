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

    dimensionedScalar rhointerconnect
        (interconnectProperties.lookup("rho"));

    dimensionedScalar Cpinterconnect
        (interconnectProperties.lookup("Cp"));

    dimensionedScalar kinterconnect
        (interconnectProperties.lookup("k"));


    Info<< "  interconnect" << nl
        << "    rhointerconnect = " << rhointerconnect << nl
        << "    Cpinterconnect  = " << Cpinterconnect << nl
        << "    kinterconnect   = " << kinterconnect << nl
        << endl;

