    Info<< "Reading cell properties\n" << endl;

    IOdictionary cellProperties
    (
        IOobject
        (
            "cellProperties",
            runTime.constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    // Read anode and cathode patch names
    word anodeName(cellProperties.lookup("anodePatch"));
    word cathodeName(cellProperties.lookup("cathodePatch"));

    dimensionedScalar V(cellProperties.lookup("voltage"));

    dimensionedScalar E0(cellProperties.lookup("E0"));

    dimensionedScalar ibar0(cellProperties.lookup("ibar0"));

    dimensionedScalar Rhat(cellProperties.lookup("Rhat"));

