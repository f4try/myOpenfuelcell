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
// Added SBB
    Info<< "Creating fuel field nu" << endl;
    volScalarField nuFuel
    (
        IOobject
        (
            "nu",
            runTime.timeName(),
            fuelMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        fuelMesh,
        muF/rhoF,
        zeroGradientFvPatchScalarField::typeName
    );
    nuFuel.correctBoundaryConditions();


//End added SBB 


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

    Info<< "Creating fuel field k" << endl;
    volScalarField kFuel
    (
        IOobject
        (
            "k",
            runTime.timeName(),
            fuelMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        fuelMesh,
        kF,
        zeroGradientFvPatchScalarField::typeName
    );
    kFuel.correctBoundaryConditions();


    Info<< "Reading fuel porous zones" << endl;
/*    porousZones anodeZones(fuelMesh);Deleted SBB*/
/* Lines added SBB */
    IOdictionary fuelPorousZones
    (
        IOobject
        (
            "porousZones",
            runTime.constant(),
            fuelMesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );
    porousZoneList anodeZones(fuelMesh, fuelPorousZones); 
/* End lines added SBB */
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




