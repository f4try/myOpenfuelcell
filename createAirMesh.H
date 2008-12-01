    fvMesh airMesh
    (
        IOobject
        (
            "air",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ
        )
    );

    // Read maps to global mesh

    labelIOList airFaceMap
    (
        IOobject
        (
            "faceMap",
            airMesh.time().findInstance(airMesh.meshDir(), "faces"),
            polyMesh::meshSubDir,
            airMesh,
            IOobject::MUST_READ
        )
    );

    labelIOList airCellMap
    (
        IOobject
        (
            "cellMap",
            airMesh.time().findInstance(airMesh.meshDir(), "faces"),
            polyMesh::meshSubDir,
            airMesh,
            IOobject::MUST_READ
        )
    );

    labelIOList airPatchesMap
    (
        IOobject
        (
            "patchMap",
            airMesh.time().findInstance(airMesh.meshDir(), "faces"),
            polyMesh::meshSubDir,
            airMesh,
            IOobject::MUST_READ
        )
    );
