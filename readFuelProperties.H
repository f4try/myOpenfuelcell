//------ readFuelProperties -------------------------------------------------//
//
// This version creates uniform fields from constants read from a dictionary
//

    Info<< "Reading fuel properties and Initializng fuel fields\n" << endl;


// property dictionary
//--------------------

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


// property fields
//----------------

    Info<< "fuel field nuFuel\n";

    dimensionedScalar nuF(fuelProperties.lookup("nu"));

    volScalarField nuFuel
    (
        IOobject
        (
            "nuFuel",
            runTime.timeName(),
            fuelMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        fuelMesh,
	nuF
    );


    Info<< "fuel field rhoFuel\n";

    dimensionedScalar rhoF(fuelProperties.lookup("rho"));

    volScalarField rhoFuel
    (
        IOobject
        (
            "rhoFuel",
            runTime.timeName(),
            fuelMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        fuelMesh,
	rhoF
    );


    Info<< "fuel field CpFuel\n";

    dimensionedScalar CpF(fuelProperties.lookup("Cp"));

    volScalarField CpFuel
    (
        IOobject
        (
            "CpFuel",
            runTime.timeName(),
            fuelMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        fuelMesh,
	CpF
    );


    Info<< "fuel field kFuel\n";

    dimensionedScalar kF(fuelProperties.lookup("k"));

    volScalarField kFuel
    (
        IOobject
        (
            "kFuel",
            runTime.timeName(),
            fuelMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        fuelMesh,
	kF
    );


    Info<< "fuel field gammaFuel\n";

    dimensionedScalar gammaF(fuelProperties.lookup("gamma"));

    volScalarField gammaFuel
    (
        IOobject
        (
            "gammaFuel",
            runTime.timeName(),
            fuelMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        fuelMesh,
	gammaF
    );


