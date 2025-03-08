    Info<< "Creating air field p" << endl;
    volScalarField pAir
    (
        IOobject
        (
            "p",
            runTime.timeName(),
            airMesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        airMesh
    );

    Info<< "Creating air field rho" << endl;
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

    Info<< "Creating air field mu" << endl;
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

// Added SBB
    Info<< "Creating air field nu" << endl;
    volScalarField nuAir
    (
        IOobject
        (
            "nu",
            runTime.timeName(),
            airMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        airMesh,
        muA/rhoA,
        zeroGradientFvPatchScalarField::typeName
    );
    nuAir.correctBoundaryConditions();


//End added SBB 

    Info<< "Creating air field U" << endl;
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

    Info<< "Creating air face flux field phi" << endl;
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
    Info<< "Creating air field Tair" << endl;
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

    Info<< "Creating air field cp" << endl;
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

    Info<< "Creating air field k" << endl;
    volScalarField kAir
    (
        IOobject
        (
            "k",
            runTime.timeName(),
            airMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        airMesh,
        kA,
        zeroGradientFvPatchScalarField::typeName
    );
    kAir.correctBoundaryConditions();

    Info<< "Reading air field phis\n" << endl;
    volScalarField phisAir
    (
        IOobject
        (
            "phisAir",
            runTime.timeName(),
            airMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        airMesh
    );


    Info<< "Creating air porous zones" << endl;
/*    porousZones cathodeZones(airMesh);  Deleted SBB*/
/* Lines added SBB */
    IOdictionary airPorousZones
    (
        IOobject
        (
            "porousZones",
            runTime.constant(),
            airMesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );
    porousZoneList cathodeZones(airMesh, airPorousZones); 
/* End lines added SBB */

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

