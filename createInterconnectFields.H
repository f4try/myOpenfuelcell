    Info<< "reading interconnect fields" << endl;

    // thermal conductivity
    volScalarField kInterconnect
    (
        IOobject
        (
            "k",
            runTime.timeName(),
            interconnectMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        interconnectMesh
    );

    // temperature
    volScalarField Tinterconnect
    (
        IOobject
        (
            "T",
            runTime.timeName(),
            interconnectMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        interconnectMesh
    );

