    fvMesh interconnectTopMesh
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
//     labelIOList interconnectTopFaceMap
//     (
//         IOobject
//         (
//             "faceRegionAddressing",
//             interconnectTopMesh.time().findInstance
//             (
//                 interconnectTopMesh.meshDir(),
//                 "faces"
//             ),
//             polyMesh::meshSubDir,
//             interconnectTopMesh,
//             IOobject::MUST_READ
//         )
//     );

    labelIOList interconnectTopCellMap
    (
        IOobject
        (
            "cellRegionAddressing",
            interconnectTopMesh.time().findInstance
            (
                interconnectTopMesh.meshDir(),
                "faces"
            ),
            polyMesh::meshSubDir,
            interconnectTopMesh,
            IOobject::MUST_READ
        )
    );

    labelIOList interconnectTopPatchesMap
    (
        IOobject
        (
            "boundaryRegionAddressing",
            interconnectTopMesh.time().findInstance
            (
                interconnectTopMesh.meshDir(),
                "faces"
            ),
            polyMesh::meshSubDir,
            interconnectTopMesh,
            IOobject::MUST_READ
        )
    );
