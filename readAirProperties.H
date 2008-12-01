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

    dimensionedScalar nuAir(airProperties.lookup("nu"));
    dimensionedScalar rhoAir(airProperties.lookup("rho"));
    dimensionedScalar CpAir(airProperties.lookup("Cp"));
    dimensionedScalar kAir(airProperties.lookup("k"));
    dimensionedScalar gammaAir(airProperties.lookup("gamma"));
