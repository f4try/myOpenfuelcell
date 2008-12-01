    // Mapping air subdomain.  Memory management
    {
        // Create fields from air side
        scalarField rhoCpFld = rhoAir*CpAir;

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpFld, airCellMap);
        kCellIn.rmap(kAir, airCellMap);

        // Map air fluxes
        labelList airInternalFaceMap
        (
            SubList<label>(airFaceMap, airMesh.nInternalFaces())
        );

        scalarField rhoCpPhiFld =
            fvc::interpolate(rhoAir*CpAir)*phiAir.internalField();

        phiCellIn.rmap(phiAir.internalField(), airInternalFaceMap);
        rhoCpPhiCellIn.rmap(rhoCpPhiFld, airInternalFaceMap);

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
                    //rhoA.value()*CpA.value();
                    rhoAir.boundaryField()[patchI]
                    *CpAir.boundaryField()[patchI];

            }
        }
    }
