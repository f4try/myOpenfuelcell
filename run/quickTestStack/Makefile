################################################################################
All: mesh parprep run reconstruct view
mesh: bmesh patchBC regionSets regionInterface regionMesh control
###############
clean:
	(rm -rf *.log [1-9]*);
clear:
	(./Allclean)
###############
bmesh:
	(cp -rf ./system/controlDict.mesh ./system/controlDict);
	(blockMesh);
patchBC:
	(rm -rf ./constant/polyMesh/sets);
	(setSet -batch ./config/make.faceSet -noVTK);
	(rm -rf ./constant/polyMesh/sets/*_old);
	(setSet -batch ./config/make.faceAir -noVTK);
	(rm -rf ./constant/polyMesh/sets/*_old);
	(setSet -batch ./config/make.faceFuel -noVTK);
	(rm -rf ./constant/polyMesh/sets/*_old);
	(setsToZones -noFlipMap);
	(createPatch -overwrite);
regionSets:
	(rm -rf constant/polyMesh/sets constant/polyMesh/*Zones);
	(setSet -batch ./config/make.setSet -noVTK);
	(rm -rf constant/polyMesh/sets/*_old);
	(setsToZones -noFlipMap);
regionInterface:
	(setSet -batch ./config/make.setIFaces -noVTK);
	(rm -rf constant/polyMesh/sets/*_old);
	(rm constant/polyMesh/sets/[abc]Faces);
	(setsToZones -noFlipMap);
regionMesh:
	(splitMeshRegions -cellZonesOnly);
	(cp -r 0.001/* ./constant);
	(rm -rf 0.001);
	(setSet -batch ./config/make.setAir -region air -noVTK);
	(setsToZones -noFlipMap -region air);
	(setSet -batch ./config/make.setFuel -region fuel -noVTK);
	(setsToZones -noFlipMap -region fuel);
	(setSet -batch ./config/make.setElectrolyte -region electrolyte -noVTK);
	(setsToZones -noFlipMap -region electrolyte);
	(rm -f constant/[aefi]*/[kpTU] constant/[aefi]*/rho);
	(rm -f constant/[aefi]*/Y* constant/[aefi]*/diff*);
control:
	(cp -rf ./system/controlDict.run ./system/controlDict);
#############
parprep:
	(./parprep.csh | tee log.parprep);
#############
run:
	(./prun | tee log.prun);
#############
srun:
	(fuelCellFoam | tee log.srun);
#############
reconstruct:
	(reconstructParMesh -region air -constant);
	(reconstructParMesh -region fuel -constant);
	(reconstructParMesh -region electrolyte -constant);
	(reconstructParMesh -region interconnect -constant);

	(reconstructPar -zeroTime);
	(reconstructPar -zeroTime -region air);
	(reconstructPar -zeroTime -region fuel);
	(reconstructPar -zeroTime -region electrolyte);
	(reconstructPar -zeroTime -region interconnect);
#############
view:
	(foamToVTK -latestTime);
	(foamToVTK -latestTime -region air);
	(foamToVTK -latestTime -region fuel);
	(foamToVTK -latestTime -region electrolyte);
	(foamToVTK -latestTime -region interconnect);
viewAll:
	(foamToVTK);
	(foamToVTK -region air);
	(foamToVTK -region fuel);
	(foamToVTK -region electrolyte);
	(foamToVTK -region interconnect);
################################################################################
