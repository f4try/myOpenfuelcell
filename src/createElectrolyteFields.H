    Info<< "Reading electrolyte fields\n" << endl;

    // thermal conductivity
    volScalarField kElectrolyte
    (
        IOobject
        (
            "k",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh
    );

    // temperature
    volScalarField Telectrolyte
    (
        IOobject
        (
            "T",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh
    );

    //volumetric ohmic Heating

    scalarField ohmicHeating
    (
        electrolyteMesh.nCells(),
        HsrcElectrolyte.value()
    );

    volScalarField Qsrc
    (
        IOobject
        (
            "Qsource",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        electrolyteMesh,
        dimensionedScalar
        (
            "qs",
            dimensionSet(1, -1, -3, 0, 0, 0, 0),
            HsrcElectrolyte.value()
        )
    );
