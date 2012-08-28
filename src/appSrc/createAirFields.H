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

    Info<< "Reading air field rho\n" << endl;
    volScalarField rhoAir
    (
        IOobject
        (
            "rho",
            runTime.timeName(),
            airMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        airMesh,
        rhoA,
        zeroGradientFvPatchScalarField::typeName
    );
    rhoAir.correctBoundaryConditions();

    Info<< "Creating air field mu\n" << endl;
    volScalarField muAir
    (
        IOobject
        (
            "mu",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        airMesh,
        muA,
        zeroGradientFvPatchScalarField::typeName
    );
    muAir.correctBoundaryConditions();

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

    Info<< "Reading/calculating air face flux field phi\n" << endl;
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
        linearInterpolate(rhoAir*Uair) & airMesh.Sf()
    );

    // inverse matrix diagonal field rUA
    volScalarField rUAair
    (
        IOobject
        (
            "(1|A(U))",
            runTime.timeName(),
            airMesh
        ),
        airMesh,
        dimensionedScalar("rUA", dimTime/dimDensity, 1),
        zeroGradientFvPatchScalarField::typeName
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


    // regional field obtained from Tcell
    volScalarField Tair
    (
        IOobject
        (
            "T",
            runTime.timeName(),
            airMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        airMesh,
        Tinit,
        zeroGradientFvPatchScalarField::typeName
    );

    Info<< "Reading air field cp\n" << endl;
    volScalarField cpAir
    (
        IOobject
        (
            "cp",
            runTime.timeName(),
            airMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        airMesh,
        CpA,
        zeroGradientFvPatchScalarField::typeName
    );
    cpAir.correctBoundaryConditions();


    Info<< "Reading air porous zones" << endl;
    porousZones cathodeZones(airMesh);

    Info<< "    number of porous zones = " << cathodeZones.size() << nl;
    forAll(cathodeZones, i)
    {
        label iz = airMesh.cellZones().findZoneID(cathodeZones[i].zoneName());
        Info<< "    zone " << cathodeZones[i].zoneIds()
            << " " << cathodeZones[i].zoneName()
            << " : size = " << airMesh.cellZones()[iz].size()
            << "; porosity = " << cathodeZones[i].porosity() << nl;
    }
    Info<< endl;

