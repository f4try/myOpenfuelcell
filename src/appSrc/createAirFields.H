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


    // Read air scalars
/******************* delete v v v ******************************************
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


    // mole fraction fields are calculated from mass fraction fields
    Info<< "Reading air field moleO2\n" << endl;
    volScalarField moleO2air
    (
        IOobject
        (
            "xO2",
            runTime.timeName(),
            airMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        airMesh,
        dimensionedScalar("xO2", dimensionSet(0, 0, 0, 0, 0, 0, 0), 1),
        zeroGradientFvPatchScalarField::typeName
    );

    Info<< "Reading air field moleN2\n" << endl;
    volScalarField moleN2air
    (
        IOobject
        (
            "xN2",
            runTime.timeName(),
            airMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        airMesh,
        dimensionedScalar("xN2", dimensionSet(0, 0, 0, 0, 0, 0, 0), 1),
        zeroGradientFvPatchScalarField::typeName
    );
******************* delete ^ ^ ^ ******************************************/


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


    Info<< "Reading air diffusion field diffAir\n" << endl;

    volScalarField diffAir
    (
        IOobject
        (
            "diff",
            runTime.timeName(),
            airMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        airMesh
//        airMesh,
//        dimensionedScalar("D", dimensionSet(0, 2, -1, 0, 0, 0, 0), 1),
//        zeroGradientFvPatchScalarField::typeName
    );
