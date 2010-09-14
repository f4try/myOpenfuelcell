    Info<< "Creating air fields\n" << endl;

    // thermal conductivity
    Info<< "    thermal conductivity k\n" << endl;
    volScalarField kAir
    (
        IOobject
        (
            "k",
            runTime.timeName(),
            airMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        airMesh
    );

    // temperature
    Info<< "    T\n" << endl;
    volScalarField Tair
    (
        IOobject
        (
            "T",
            runTime.timeName(),
            airMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        airMesh
    );


    Info<< "Reading air field p\n" << endl;
    volScalarField pAir
    (
        IOobject
        (
            "p",
            runTime.timeName(),
            airMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        airMesh
    );

    Info<< "Reading air field U\n" << endl;
    volVectorField Uair
    (
        IOobject
        (
            "U",
            runTime.timeName(),
            airMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        airMesh
    );

    Info<< "Reading/calculating face flux field phi\n" << endl;

    surfaceScalarField phiAir
    (
        IOobject
        (
            "phi",
            runTime.timeName(),
            airMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        linearInterpolate(Uair) & airMesh.Sf()
    );

    // Pressure reference point in air
    label pAirRefCell = 0;
    scalar pAirRefValue = 0.0;

    setRefCell
    (
        pAir,
        airMesh.solutionDict().subDict("PISO"),
        pAirRefCell,
        pAirRefValue
    );


    // Read air scalars
    multivariateSurfaceInterpolationScheme<scalar>::fieldTable airFields;

    Info<< "Reading air field yO2\n" << endl;
    volScalarField yO2air
    (
        IOobject
        (
            "yO2",
            runTime.timeName(),
            airMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        airMesh
    );

    airFields.add(yO2air);

    Info<< "Reading air field yN2\n" << endl;
    volScalarField yN2air
    (
        IOobject
        (
            "yN2",
            runTime.timeName(),
            airMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        airMesh
    );

    airFields.add(yN2air);
