    fvMesh interconnect1Mesh
    (
        IOobject
        (
            "interconnect1",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ
        )
    );


    // Read maps to global mesh
    // No mapping of face fields in solids.  HJ, 25/May/2011

    labelIOList interconnect1CellMap
    (
        IOobject
        (
            "cellRegionAddressing",
            interconnect1Mesh.time().findInstance
            (
                interconnect1Mesh.meshDir(),
                "faces"
            ),
            polyMesh::meshSubDir,
            interconnect1Mesh,
            IOobject::MUST_READ
        )
    );

    labelIOList interconnect1PatchesMap
    (
        IOobject
        (
            "boundaryRegionAddressing",
            interconnect1Mesh.time().findInstance
            (
                interconnect1Mesh.meshDir(),
                "faces"
            ),
            polyMesh::meshSubDir,
            interconnect1Mesh,
            IOobject::MUST_READ
        )
    );

