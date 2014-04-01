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
