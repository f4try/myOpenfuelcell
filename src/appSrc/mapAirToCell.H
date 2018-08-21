    // Mapping air subdomain.  Memory management
    {
        // Create fields from air side

        // mass based heat capacity cp
        // cp [J/kg-K] = Cp/M where Cp = [J/mol-K], M = [kg/mol]

        scalarField& cp = cpAir.primitiveFieldRef();
        cp = 0;

        forAll(airSpecies, i)
        {
            scalar rMWi = 1.0/(airSpecies[i].MW()*1e-3);    // 1/[kg/mol]
            scalarField cpi = molarCpAir[i].polyVal(Tair.primitiveFieldRef())*rMWi;
            cp += Yair[i].primitiveFieldRef()*cpi;
        }
        cpAir.correctBoundaryConditions();

        scalarField rhoCpF = cp*rhoAir.primitiveFieldRef();

        Info << nl;
        Info<< "min mean max rhoCpF(air) = " << gMin(rhoCpF) << " "
            << gAverage(rhoCpF) << " " << gMax(rhoCpF) << nl;

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpF, airCellMap);


        //scalarField kIn(airMesh.nCells(), kA.value());
        scalarField& kIn = kAir.primitiveFieldRef();

        forAll(cathodeZones, iz)
        {
            label znId =
                airMesh.cellZones().findZoneID(cathodeZones[iz].zoneName());

            dimensionedScalar kZn(cathodeZones[iz].dict().lookup("k"));
            scalar porZn = cathodeZones[iz].porosity();

            labelList znCells(airMesh.cellZones()[znId]);

            forAll(znCells, cellI)
            {
                //kIn[znCells[cellI]] =
                //    kZn.value()*(1 - porZn) + kA.value()*porZn;
                    
                kIn[znCells[cellI]] *= porZn;
                kIn[znCells[cellI]] += (1 - porZn)*kZn.value();
            }
        }

        // Perform reverse mapping
        kCellIn.rmap(kIn, airCellMap);


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
            //CpAir.value()*phiAir.primitiveFieldRef();
            linearInterpolate(cpAir)*phiAir.primitiveFieldRef();

        phiCellIn.rmap
        (
            phiAir.primitiveFieldRef()*airInternalFaceMask,
            airInternalFaceMap
        );

        rhoCpPhiCellIn.rmap
        (
            rhoCpPhiF*airInternalFaceMask,
            airInternalFaceMap
        );


        // Do electrolyte interface    (hkr: 2010 Dec 03)
        scalarField cathodePhi = phiAir.boundaryFieldRef()[cathodeID];
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

                phiCell.boundaryFieldRef()[airPatchesMap[patchI]].
                    scalarField::rmap
                    (
                        phiAir.boundaryFieldRef()[patchI]*curMask,
                        curFpm
                    );

                rhoCpPhiCell.boundaryFieldRef()[airPatchesMap[patchI]].
                    scalarField::rmap
                    (
                        (
                            cpAir.boundaryFieldRef()[patchI]
                            *phiAir.boundaryFieldRef()[patchI]
                        )*curMask,
                        curFpm
                    );

                //rhoCpPhiCell.boundaryFieldRef()[airPatchesMap[patchI]] *=
                //(
                //    CpAir.value()
                //);
            }
        }
    }
