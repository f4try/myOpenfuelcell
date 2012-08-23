    // Mapping air subdomain.  Memory management
    {
        // Create fields from air side

        //scalarField rhoCpF_old
        //(
        //    airMesh.nCells(),
        //    CpAir.value()
        //);
        //rhoCpF_old *= rhoAir.internalField();
        //
        //Info << nl;
        //Info<< "min mean max rhoCpF_old = " << gMin(rhoCpF_old) << " "
        //    << gMin(rhoCpF_old) << " " << gMin(rhoCpF_old) << nl;


        // mass based heat capacity cp
        // cp [J/kg-K] = Cp/M where Cp = [J/mol-K], M = [kg/mol]

        scalarField& cp = cpAir.internalField();
        cp = 0;

        forAll(airSpecies, i)
        {
            scalar rMWi = 1.0/(airSpecies[i].MW()*1e-3);    // 1/[kg/mol]
            scalarField cpi = molarCpAir[i].polyVal(Tair.internalField())*rMWi;
            cp += Yair[i].internalField()*cpi;
        }
        cpAir.correctBoundaryConditions();

        scalarField rhoCpF = cp*rhoAir.internalField();

        Info << nl;
        Info<< "min mean max rhoCpF(air) = " << gMin(rhoCpF) << " "
            << gAverage(rhoCpF) << " " << gMax(rhoCpF) << nl;

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpF, airCellMap);


        scalarField kF(airMesh.nCells(), kAir.value());

        forAll(cathodeZones, iz)
        {
            label znId =
                airMesh.cellZones().findZoneID(cathodeZones[iz].zoneName());

            dimensionedScalar CpZn(cathodeZones[iz].dict().lookup("Cp"));
            dimensionedScalar kZn(cathodeZones[iz].dict().lookup("k"));
            scalar porZn = cathodeZones[iz].porosity();

            labelList znCells(airMesh.cellZones()[znId]);

            forAll(znCells, cellI)
            {
                kF[znCells[cellI]] =
                    kZn.value()*(1 - porZn) + kAir.value()*porZn;
            }
        }

        // Perform reverse mapping
        kCellIn.rmap(kF, airCellMap);


        // Map air fluxes
        labelList airInternalFaceMap
        (
            SubList<label>(airFaceMap, airMesh.nInternalFaces())
        );

        scalarField airInternalFaceMask
        (
            scalarField::subField(airFaceMask, airMesh.nInternalFaces())
        );

        //
        // ** recall phi already incorporates rho **
        //

        scalarField rhoCpPhiF =
            //CpAir.value()*phiAir.internalField();
            linearInterpolate(cpAir)*phiAir.internalField();

        phiCellIn.rmap
        (
            phiAir.internalField()*airInternalFaceMask,
            airInternalFaceMap
        );

        rhoCpPhiCellIn.rmap
        (
            rhoCpPhiF*airInternalFaceMask,
            airInternalFaceMap
        );


        // Do electrolyte interface    (hkr: 2010 Dec 03)
        scalarField cathodePhi = phiAir.boundaryField()[cathodeID];
        forAll(cathodePhi, faceI)
        {
            phiCell[cathodeMask[faceI]] = cathodePhi[faceI];
            //rhoCpPhiCell[cathodeMask[faceI]]= CpAir.value()*cathodePhi[faceI];
            rhoCpPhiCell[cathodeMask[faceI]]=
            (
                cpAir[faceI]*cathodePhi[faceI]
            );
        }

        // Do flux boundary conditions
        forAll (airPatchesMap, patchI)
        {
            // Map only real matching patches.  HJ, 22/May/2011
            if
            (
                airPatchesMap[patchI] > -1
             && airPatchesMap[patchI] < mesh.boundary().size()
            )
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

                scalarField curMask
                (
                    scalarField::subField
                    (
                        airFaceMask,
                        airMesh.boundary()[patchI].size(),
                        airMesh.boundary()[patchI].patch().start()
                    )
                );

                curFpm -= mesh.boundary()
                    [airPatchesMap[patchI]].patch().start();

                phiCell.boundaryField()[airPatchesMap[patchI]].
                    scalarField::rmap
                    (
                        phiAir.boundaryField()[patchI]*curMask,
                        curFpm
                    );

                rhoCpPhiCell.boundaryField()[airPatchesMap[patchI]].
                    scalarField::rmap
                    (
                        (
                            cpAir.boundaryField()[patchI]
                            *phiAir.boundaryField()[patchI]
                        )*curMask,
                        curFpm
                    );

                //rhoCpPhiCell.boundaryField()[airPatchesMap[patchI]] *=
                //(
                //    CpAir.value()
                //);
            }
        }
    }
