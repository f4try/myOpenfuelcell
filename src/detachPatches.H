{
    // air mesh
    const polyPatchList& airPatches = airMesh.boundaryMesh();

    forAll (airPatches, patchI)
    {
        if (isType<regionCouplePolyPatch>(airPatches[patchI]))
        {
            const regionCouplePolyPatch& rcpAir =
                refCast<const regionCouplePolyPatch>(airPatches[patchI]);

            // Detach it here
            rcpAir.detach();
        }
    }

    // Force recalculation of weights
    airMesh.surfaceInterpolation::movePoints(); 



    // fuelmesh
    const polyPatchList& fuelPatches = fuelMesh.boundaryMesh();

    forAll (fuelPatches, patchI)
    {
        if (isType<regionCouplePolyPatch>(fuelPatches[patchI]))
        {
            const regionCouplePolyPatch& rcpFuel =
                refCast<const regionCouplePolyPatch>(fuelPatches[patchI]);

            // Detach it here
            rcpFuel.detach();
        }
    }

    // Force recalculation of weights
    fuelMesh.surfaceInterpolation::movePoints(); 



    // electrolytemesh
    const polyPatchList& electrolytePatches = electrolyteMesh.boundaryMesh();

    forAll (electrolytePatches, patchI)
    {
        if (isType<regionCouplePolyPatch>(electrolytePatches[patchI]))
        {
            const regionCouplePolyPatch& rcpElectrolyte =
                refCast<const regionCouplePolyPatch>(electrolytePatches[patchI]);

            // Detach it here
            rcpElectrolyte.detach();
        }
    }

    // Force recalculation of weights
    electrolyteMesh.surfaceInterpolation::movePoints(); 



    // interconnectmesh
    const polyPatchList& interconnectPatches = interconnectMesh.boundaryMesh();

    forAll (interconnectPatches, patchI)
    {
        if (isType<regionCouplePolyPatch>(interconnectPatches[patchI]))
        {
            const regionCouplePolyPatch& rcpInterconnect =
                refCast<const regionCouplePolyPatch>(interconnectPatches[patchI]);

            // Detach it here
            rcpInterconnect.detach();
        }
    }

    // Force recalculation of weights
    interconnectMesh.surfaceInterpolation::movePoints(); 
}
