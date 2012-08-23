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

    Info<< "Reading fuel field rho\n" << endl;
    volScalarField rhoFuel
    (
        IOobject
        (
            "rho",
            runTime.timeName(),
            fuelMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        fuelMesh,
        rhoF,
        zeroGradientFvPatchScalarField::typeName
    );
    rhoFuel.correctBoundaryConditions();

    Info<< "Creating fuel field mu\n" << endl;
    volScalarField muFuel
    (
        IOobject
        (
            "mu",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        fuelMesh,
        muF,
        zeroGradientFvPatchScalarField::typeName
    );
    muFuel.correctBoundaryConditions();


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

    Info<< "Reading/calculating fuel face flux field phi\n" << endl;
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
        linearInterpolate(rhoFuel*Ufuel) & fuelMesh.Sf()
    );

    // inverse matrix diagonal field rUA
    volScalarField rUAfuel
    (
        IOobject
        (
            "(1|A(U))",
            runTime.timeName(),
            fuelMesh
        ),
        fuelMesh,
        dimensionedScalar("rUA", dimTime/dimDensity, 1),
        zeroGradientFvPatchScalarField::typeName
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


    // regional field obtained from Tcell
    volScalarField Tfuel
    (
        IOobject
        (
            "T",
            runTime.timeName(),
            fuelMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        fuelMesh,
        Tinit,
        zeroGradientFvPatchScalarField::typeName
    );

    Info<< "Reading fuel field cp\n" << endl;
    volScalarField cpFuel
    (
        IOobject
        (
            "cp",
            runTime.timeName(),
            fuelMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        fuelMesh,
        CpF,
        zeroGradientFvPatchScalarField::typeName
    );
    cpFuel.correctBoundaryConditions();


    // Read fuel scalars (mass fractions)
/******************** delete v v v ******************************************
    multivariateSurfaceInterpolationScheme<scalar>::fieldTable fuelFields;

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


    // mole fraction fields are calculated from mass fraction fields
    Info<< "Reading fuel field moleH2\n" << endl;
    volScalarField moleH2fuel
    (
        IOobject
        (
            "xH2",
            runTime.timeName(),
            fuelMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        fuelMesh,
        dimensionedScalar("xH2", dimensionSet(0, 0, 0, 0, 0, 0, 0), 1),
        zeroGradientFvPatchScalarField::typeName
    );

    Info<< "Reading fuel field moleH2O\n" << endl;
    volScalarField moleH2Ofuel
    (
        IOobject
        (
            "xH2O",
            runTime.timeName(),
            fuelMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        fuelMesh,
        dimensionedScalar("xH2O", dimensionSet(0, 0, 0, 0, 0, 0, 0), 1),
        zeroGradientFvPatchScalarField::typeName
    );
******************** delete ^ ^ ^ ******************************************/


    Info<< "Reading fuel porous zones" << endl;
    porousZones anodeZones(fuelMesh);

    Info<< "    number of porous zones = " << anodeZones.size() << nl;
    forAll(anodeZones, i)
    {
        label iz = fuelMesh.cellZones().findZoneID(anodeZones[i].zoneName());
        Info<< "    zone " << anodeZones[i].zoneIds()
            << " " << anodeZones[i].zoneName()
            << " : size = " << fuelMesh.cellZones()[iz].size()
            << "; porosity = " << anodeZones[i].porosity() << nl;
    }
    Info<< endl;


    Info<< "Reading fuel diffusion field diffFuel\n" << endl;
    volScalarField diffFuel
    (
        IOobject
        (
            "diff",
            runTime.timeName(),
            fuelMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        fuelMesh
//        fuelMesh,
//        dimensionedScalar("D", dimensionSet(0, 2, -1, 0, 0, 0, 0), 1),
//        zeroGradientFvPatchScalarField::typeName
    );


    Info<< "creating current density field\n" << endl;
    volScalarField idensity
    (
        IOobject
        (
            "i",
            runTime.timeName(),
            fuelMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        fuelMesh,
        dimensionedScalar("i", dimensionSet(0, -2, 0, 0, 0, 1, 0), 0),
        zeroGradientFvPatchScalarField::typeName
    );
