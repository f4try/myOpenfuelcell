    fvMesh interconnectBottomMesh
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
//     labelIOList interconnectBottomFaceMap
//     (
//         IOobject
//         (
//             "faceRegionAddressing",
//             interconnectBottomMesh.time().findInstance
//             (
//                 interconnectBottomMesh.meshDir(),
//                 "faces"
//             ),
//             polyMesh::meshSubDir,
//             interconnectBottomMesh,
//             IOobject::MUST_READ
//         )
//     );

    labelIOList interconnectBottomCellMap
    (
        IOobject
        (
            "cellRegionAddressing",
            interconnectBottomMesh.time().findInstance
            (
                interconnectBottomMesh.meshDir(),
                "faces"
            ),
            polyMesh::meshSubDir,
            interconnectBottomMesh,
            IOobject::MUST_READ
        )
    );

    labelIOList interconnectBottomPatchesMap
    (
        IOobject
        (
            "boundaryRegionAddressing",
            interconnectBottomMesh.time().findInstance
            (
                interconnectBottomMesh.meshDir(),
                "faces"
            ),
            polyMesh::meshSubDir,
            interconnectBottomMesh,
            IOobject::MUST_READ
        )
    );
