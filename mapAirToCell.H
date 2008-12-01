    // Mapping air subdomain.  Memory management
    {
        // Create constant fields from air side
        scalarField rhoCpF
        (
            airMesh.nCells(),
            rhoAir.value()*CpAir.value()
        );

        scalarField kF(airMesh.nCells(), kAir.value());

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpF, airCellMap);
        kCellIn.rmap(kF, airCellMap);

        // Map air fluxes
        labelList airInternalFaceMap
        (
            SubList<label>(airFaceMap, airMesh.nInternalFaces())
        );

        scalarField rhoCpPhiF =
            rhoAir.value()*CpAir.value()*phiAir.internalField();

        phiCellIn.rmap(phiAir.internalField(), airInternalFaceMap);
        rhoCpPhiCellIn.rmap(rhoCpPhiF, airInternalFaceMap);

        // Do flux boundary conditions

        forAll (airPatchesMap, patchI)
        {
            if (airPatchesMap[patchI] > -1)
            {
                // Patch maps
                labelField curFpm
                (
                    labelField::subField
                    (
                        airFaceMap,
                        airMesh.boundary()[patchI].size(),
                        airMesh.boundary()[patchI].patch().start()
                    )
                );

                curFpm -= mesh.boundary()
                    [airPatchesMap[patchI]].patch().start();

                phiCell.boundaryField()[airPatchesMap[patchI]].rmap
                (
                    phiAir.boundaryField()[patchI],
                    curFpm
                );

                rhoCpPhiCell.boundaryField()[airPatchesMap[patchI]].rmap
                (
                    phiAir.boundaryField()[patchI],
                    curFpm
                );

                rhoCpPhiCell.boundaryField()[airPatchesMap[patchI]] *=
                    rhoAir.value()*CpAir.value();

            }
        }
    }
