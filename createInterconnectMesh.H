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

    labelIOList interconnectFaceMap
    (
        IOobject
        (
            "faceMap",
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

    labelIOList interconnectCellMap
    (
        IOobject
        (
            "cellMap",
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
            "patchMap",
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
