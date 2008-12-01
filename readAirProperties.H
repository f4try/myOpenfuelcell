//------ readAirProperties -------------------------------------------------//
//
// This version creates uniform fields from constants read from a dictionary
//

    Info<< "Reading air properties and Initializng air fields\n" << endl;


// property dictionary
//--------------------

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


// property fields
//----------------

    Info<< "air field nuAir\n";

    dimensionedScalar nuA(airProperties.lookup("nu"));

    volScalarField nuAir
    (
        IOobject
        (
            "nuAir",
            runTime.timeName(),
            airMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        airMesh,
	nuA
    );


    Info<< "air field rhoAir\n";

    dimensionedScalar rhoA(airProperties.lookup("rho"));

    volScalarField rhoAir
    (
        IOobject
        (
            "rhoAir",
            runTime.timeName(),
            airMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        airMesh,
	rhoA
    );


    Info<< "air field CpAir\n";

    dimensionedScalar CpA(airProperties.lookup("Cp"));

    volScalarField CpAir
    (
        IOobject
        (
            "CpAir",
            runTime.timeName(),
            airMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        airMesh,
	CpA
    );


    Info<< "air field kAir\n";

    dimensionedScalar kA(airProperties.lookup("k"));

    volScalarField kAir
    (
        IOobject
        (
            "kAir",
            runTime.timeName(),
            airMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        airMesh,
	kA
    );


    Info<< "air field gammaAir\n";

    dimensionedScalar gammaA(airProperties.lookup("gamma"));

    volScalarField gammaAir
    (
        IOobject
        (
            "gammaAir",
            runTime.timeName(),
            airMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        airMesh,
	gammaA
    );


