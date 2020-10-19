    //volumetric electrochemical Heating

    volScalarField electrochemicalHeating
    (
        IOobject
        (
            "electrochemicalHeating",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        0,
        zeroGradientFvPatchScalarField::typeName
    );

    Info<< "creating current density field\n" << endl;

    volScalarField idensity
    (
        IOobject
        (
            "i",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        dimensionedScalar
        (
            "i",
            dimensionSet(0, -2, 0, 0, 0, 1, 0),
            ibar0.value()
        ),
        zeroGradientFvPatchScalarField::typeName
    );
// Added SBB 02-02-14
 
    volScalarField etaA
    (
        IOobject
        (
            "etaA",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        dimensionedScalar
        (
            "etaA",
            dimensionSet(1, 2, -3, 0, 0, -1, 0),
            0
        ),
        zeroGradientFvPatchScalarField::typeName
    );

    volScalarField etaC
    (
        IOobject
        (
            "etaC",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        dimensionedScalar
        (
            "etaC",
            dimensionSet(1, 2, -3, 0, 0, -1, 0),
            0
        ),
        zeroGradientFvPatchScalarField::typeName
    );

    Info<< "creating voltage field\n" << endl;
    volScalarField voltage
    (
        IOobject
        (
            "V",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        V,
        zeroGradientFvPatchScalarField::typeName
    );

    Info<< "creating Nernst potential field\n" << endl;
    volScalarField NernstPot
    (
        IOobject
        (
            "NernstPot",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        1,
        zeroGradientFvPatchScalarField::typeName
    );

    Info<< "creating phil field\n" << endl;
    volScalarField phil
    (
        IOobject
        (
            "phil",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh
    );