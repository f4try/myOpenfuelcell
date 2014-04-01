    fvMesh interconnectMesh
    (
        IOobject
        (
            "interconnect",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ
        )
    );


    // Read maps to global mesh
    // No mapping of face fields in solids.  HJ, 25/May/2011

    labelIOList interconnectCellMap
    (
        IOobject
        (
            "cellRegionAddressing",
            interconnectMesh.time().findInstance
            (
                interconnectMesh.meshDir(),
                "faces"
            ),
            polyMesh::meshSubDir,
            interconnectMesh,
            IOobject::MUST_READ
        )
    );

    labelIOList interconnectPatchesMap
    (
        IOobject
        (
            "boundaryRegionAddressing",
            interconnectMesh.time().findInstance
            (
                interconnectMesh.meshDir(),
                "faces"
            ),
            polyMesh::meshSubDir,
            interconnectMesh,
            IOobject::MUST_READ
        )
    );

