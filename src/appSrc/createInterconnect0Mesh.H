    fvMesh interconnect0Mesh
    (
        IOobject
        (
            "interconnect0",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ
        )
    );


    // Read maps to global mesh
    // No mapping of face fields in solids.  HJ, 25/May/2011

    labelIOList interconnect0CellMap
    (
        IOobject
        (
            "cellRegionAddressing",
            interconnect0Mesh.time().findInstance
            (
                interconnect0Mesh.meshDir(),
                "faces"
            ),
            polyMesh::meshSubDir,
            interconnect0Mesh,
            IOobject::MUST_READ
        )
    );

    labelIOList interconnect0PatchesMap
    (
        IOobject
        (
            "boundaryRegionAddressing",
            interconnect0Mesh.time().findInstance
            (
                interconnect0Mesh.meshDir(),
                "faces"
            ),
            polyMesh::meshSubDir,
            interconnect0Mesh,
            IOobject::MUST_READ
        )
    );

