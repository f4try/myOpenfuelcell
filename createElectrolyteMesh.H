    fvMesh electrolyteMesh
    (
        IOobject
        (
            "electrolyte",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ
        )
    );


    // Read maps to global mesh

    labelIOList electrolyteFaceMap
    (
        IOobject
        (
            "faceMap",
            electrolyteMesh.time().findInstance
            (
                electrolyteMesh.meshDir(),
                "faces"
            ),
            polyMesh::meshSubDir,
            electrolyteMesh,
            IOobject::MUST_READ
        )
    );

    labelIOList electrolyteCellMap
    (
        IOobject
        (
            "cellMap",
            electrolyteMesh.time().findInstance
            (
                electrolyteMesh.meshDir(),
                "faces"
            ),
            polyMesh::meshSubDir,
            electrolyteMesh,
            IOobject::MUST_READ
        )
    );

    labelIOList electrolytePatchesMap
    (
        IOobject
        (
            "patchMap",
            electrolyteMesh.time().findInstance
            (
                electrolyteMesh.meshDir(),
                "faces"
            ),
            polyMesh::meshSubDir,
            electrolyteMesh,
            IOobject::MUST_READ
        )
    );
