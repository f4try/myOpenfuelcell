    Info<< "Reading cell field T\n" << endl;
    volScalarField Tcell
    (
        IOobject
        (
            "T",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    // Info<< "Reading cell field phis\n" << endl;
    // volScalarField phis
    // (
    //     IOobject
    //     (
    //         "phis",
    //         runTime.timeName(),
    //         mesh,
    //         IOobject::MUST_READ,
    //         IOobject::AUTO_WRITEphisFuel
    //     ),
    //     mesh
    // );

    // volScalarField phil
    // (
    //     IOobject
    //     (
    //         "phil",
    //         runTime.timeName(),
    //         mesh,
    //         IOobject::MUST_READ,
    //         IOobject::AUTO_WRITE
    //     ),
    //     mesh
    // );

    volScalarField rhoCpCell
    (
        IOobject
        (
            "rhoCp",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("rc", dimDensity*dimSpecificHeatCapacity, 0),
        zeroGradientFvPatchScalarField::typeName
    );

    volScalarField kCell
    (
        IOobject
        (
            "k",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    surfaceScalarField phiCell
    (
        IOobject
        (
            "phi",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar
        (
            "rp",
            dimVelocity*dimArea,
            0
        )
    );

    surfaceScalarField rhoCpPhiCell
    (
        IOobject
        (
            "rhoCpPhi",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar
        (
            "rp",
            dimVelocity*dimDensity*dimSpecificHeatCapacity*dimArea,
            0
        )
    );

    // Volumetric heat source
    volScalarField TsourceCell
    (
        IOobject
        (
            "Tsource",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionedScalar("ts", dimensionSet(1, -1, -3, 0, 0, 0, 0), 0)
    );


