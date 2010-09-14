{
    // air mesh
    const polyPatchList& airPatches = airMesh.boundaryMesh();

    forAll (airPatches, patchI)
    {
        if (isType<regionCouplePolyPatch>(airPatches[patchI]))
        {
            const regionCouplePolyPatch& rcpAir =
                refCast<const regionCouplePolyPatch>(airPatches[patchI]);

            // Attach it here
            rcpAir.attach();
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

            // Attach it here
            rcpFuel.attach();
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

            // Attach it here
            rcpElectrolyte.attach();
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

            // Attach it here
            rcpInterconnect.attach();
        }
    }

    // Force recalculation of weights
    electrolyteMesh.surfaceInterpolation::movePoints(); 
}
