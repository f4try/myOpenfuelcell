###################################################################################################
EXE=sofcFoam
test: clear mesh run view
mesh: bmesh patchBC regionMesh control
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
regionMesh:
	(rm -rf constant/polyMesh/sets constant/polyMesh/*Zones);
	(setSet -batch ./config/make.setSet -noVTK);
	(rm -rf constant/polyMesh/sets/*_old);
	(setsToZones -noFlipMap);
	(splitMeshRegions -cellZones);
	(cp -rf 0.001/* ./constant/);
	(rm -rf 0.001);
	(setSet -batch ./config/make.setAir -region air -noVTK);
	(rm -rf constant/air/polyMesh/sets/*_old);
	(setsToZones -noFlipMap -region air);
	(setSet -batch ./config/make.setFuel -region fuel -noVTK);
	(rm -rf constant/fuel/polyMesh/sets/*_old);
	(setsToZones -noFlipMap -region fuel);
control:
	(cp -rf ./system/controlDict.run ./system/controlDict);
#############
run:
	(make clean);
	($(EXE) | tee srun.log);	
view:
	(foamToVTK -latestTime);
	(foamToVTK -latestTime -region air);
	(foamToVTK -latestTime -region fuel);
	(foamToVTK -latestTime -region electrolyte);
	(foamToVTK -latestTime -region interconnect0);
	(foamToVTK -latestTime -region interconnect1);
viewAll:
	(foamToVTK);
	(foamToVTK -region air);
	(foamToVTK -region fuel);
	(foamToVTK -region electrolyte);
	(foamToVTK -region interconnect0);
	(foamToVTK -region interconnect1);
###################################################################################################
