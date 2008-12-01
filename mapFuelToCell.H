    // Mapping fuel subdomain.  Memory management
    {
        // Create constant fields from fuel side
        scalarField rhoCpF
        (
            fuelMesh.nCells(),
            rhoFuel.value()*CpFuel.value()
        );

        scalarField kF(fuelMesh.nCells(), kFuel.value());

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpF, fuelCellMap);
        kCellIn.rmap(kF, fuelCellMap);

        // Map fuel fluxes
        labelList fuelInternalFaceMap
        (
            SubList<label>(fuelFaceMap, fuelMesh.nInternalFaces())
        );

        scalarField rhoCpPhiF =
            rhoFuel.value()*CpFuel.value()*phiFuel.internalField();

        phiCellIn.rmap(phiFuel.internalField(), fuelInternalFaceMap);
        rhoCpPhiCellIn.rmap(rhoCpPhiF, fuelInternalFaceMap);

        // Do flux boundary conditions

        forAll (fuelPatchesMap, patchI)
        {
            if (fuelPatchesMap[patchI] > -1)
            {
                // Patch maps
                labelField curFpm
                (
                    labelField::subField
                    (
                        fuelFaceMap,
                        fuelMesh.boundary()[patchI].size(),
                        fuelMesh.boundary()[patchI].patch().start()
                    )
                );

                curFpm -= mesh.boundary()
                    [fuelPatchesMap[patchI]].patch().start();

                phiCell.boundaryField()[fuelPatchesMap[patchI]].rmap
                (
                    phiFuel.boundaryField()[patchI],
                    curFpm
                );

                rhoCpPhiCell.boundaryField()[fuelPatchesMap[patchI]].rmap
                (
                    phiFuel.boundaryField()[patchI],
                    curFpm
                );

                rhoCpPhiCell.boundaryField()[fuelPatchesMap[patchI]] *=
                    rhoFuel.value()*CpFuel.value();
            }
        }
    }
