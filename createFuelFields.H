    Info<< "Creating fuel fields\n" << endl;

    // thermal conductivity
    Info<< "   thermal conductivity\n" << endl;
    volScalarField kFuel
    (
        IOobject
        (
            "k",
            runTime.timeName(),
            fuelMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        fuelMesh
    );

    // temperature
    Info<< "Reading fuel field T\n" << endl;
    volScalarField Tfuel
    (
        IOobject
        (
            "T",
            runTime.timeName(),
            fuelMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        fuelMesh
    );


    Info<< "Reading fuel field p\n" << endl;
    volScalarField pFuel
    (
        IOobject
        (
            "p",
            runTime.timeName(),
            fuelMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        fuelMesh
    );


    Info<< "Reading fuel field U\n" << endl;
    volVectorField Ufuel
    (
        IOobject
        (
            "U",
            runTime.timeName(),
            fuelMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        fuelMesh
    );

    Info<< "Reading/calculating face flux field phi\n" << endl;

    surfaceScalarField phiFuel
    (
        IOobject
        (
            "phi",
            runTime.timeName(),
            fuelMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        linearInterpolate(Ufuel) & fuelMesh.Sf()
    );


    // Pressure reference point in fuel
    label pFuelRefCell = 0;
    scalar pFuelRefValue = 0.0;

    setRefCell
    (
        pFuel,
        fuelMesh.solutionDict().subDict("PISO"),
        pFuelRefCell,
        pFuelRefValue
    );


    // Read fuel scalars
    multivariateSurfaceInterpolationScheme<scalar>::fieldTable fuelFields;

    Info<< "Reading fuel field yN2\n" << endl;
    volScalarField yN2fuel
    (
        IOobject
        (
            "yN2",
            runTime.timeName(),
            fuelMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        fuelMesh
    );

    fuelFields.add(yN2fuel);

    Info<< "Reading fuel field yH2\n" << endl;
    volScalarField yH2fuel
    (
        IOobject
        (
            "yH2",
            runTime.timeName(),
            fuelMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        fuelMesh
    );

    fuelFields.add(yH2fuel);

    Info<< "Reading fuel field yH2O\n" << endl;
    volScalarField yH2Ofuel
    (
        IOobject
        (
            "yH2O",
            runTime.timeName(),
            fuelMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        fuelMesh
    );

    fuelFields.add(yH2Ofuel);
