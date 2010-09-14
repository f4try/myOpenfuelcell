    fvMesh interconnectMesh
    (
        IOobject
        (
            "interconnect",
            runTime.timeName(),
            runTime,
            IOobject::MUST_READ
        )
    );

