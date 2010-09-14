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

    dimensionedScalar nuFuel(fuelProperties.lookup("nu"));
    dimensionedScalar rhoFuel(fuelProperties.lookup("rho"));
    dimensionedScalar CpFuel(fuelProperties.lookup("Cp"));
    //dimensionedScalar kF(fuelProperties.lookup("k"));
    dimensionedScalar gammaFuel(fuelProperties.lookup("gamma"));
