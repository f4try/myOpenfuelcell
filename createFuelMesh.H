    fvMesh fuelMesh
    (
        IOobject
        (
            "fuel",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ
        )
    );

    // Read maps to global mesh

    labelIOList fuelFaceMap
    (
        IOobject
        (
            "faceMap",
            fuelMesh.time().findInstance(fuelMesh.meshDir(), "faces"),
            polyMesh::meshSubDir,
            fuelMesh,
            IOobject::MUST_READ
        )
    );

    labelIOList fuelCellMap
    (
        IOobject
        (
            "cellMap",
            fuelMesh.time().findInstance(fuelMesh.meshDir(), "faces"),
            polyMesh::meshSubDir,
            fuelMesh,
            IOobject::MUST_READ
        )
    );

    labelIOList fuelPatchesMap
    (
        IOobject
        (
            "patchMap",
            fuelMesh.time().findInstance(fuelMesh.meshDir(), "faces"),
            polyMesh::meshSubDir,
            fuelMesh,
            IOobject::MUST_READ
        )
    );
