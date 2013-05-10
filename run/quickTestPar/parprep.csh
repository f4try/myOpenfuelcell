#!/bin/csh

## edit system/decomposeParDict for the desired decomposition
## set environment variable NPROCS to number of processors.  e.g.,
## setenv NPROCS 2
## make mesh
##
## then:


echo "NPROCS = " $NPROCS


# To reconstruct and visualize the regions, we need the *ProcAddressing files
# created by decomposePar -region <region name>
# Only the fluid subregions have fields, so we restrict this decomp to fluids
# After the fluid region decomp, we rename the processor* directories as proc_*
# to (a) allow the parallel decomp to proceed 
# while (b) saving the *ProcAddressing files for later copy
#
cp system/decomposeParDict system/air
cp system/decomposeParDict system/fuel
decomposePar -region air
decomposePar -region fuel
rename processor proc_ processor*


# decompose the global mesh
rm -r processor*
decomposePar -force >& log.decompose

mpirun -np $NPROCS splitMeshRegions -cellZones -parallel >& log.split


# copy the new region/polyMesh dirs to the constant dir and
# copy the new region/fields dirs to the 0 dir
# both copies carry both sets of data; the dirs will be cleaned up later

@ i = 0
while ( $i < $NPROCS )
    cd processor$i
    #
    # copy the new region/polyMesh dirs to the constant dir
    cp -r 1/* ./constant
    #
    # copy the new region fields to the 0 dir
    cp -rf 1/air ./0
    cp -rf 1/fuel ./0
    cd ..
    echo "processor"$i "done"
    @ i++
end
# remove mesh files from the processor 0 directories
find processor*/0 -name polyMesh -exec rm -r {} \;
find processor*/0 -name cellToRegion -exec rm {} \;
# remove field files from the processor constant directories
find processor*/constant -name 'diff*' -exec rm {} \;
find processor*/constant -name '[kpTU]' -exec rm {} \;
find processor*/constant -name 'rho' -exec rm {} \;
find processor*/constant -name 'Y*' -exec rm {} \;


## patch subdictionaries named <regionA>_to_<regionB> need to be redefined
## from
##      type     calculated;  to  type     fixedValue;      (in field U)
## and from
##      type     calculated;  to  type     fixedGradient;
##      value    uniform 0;       gradient uniform 0;       (all others)
##
sed -i -e 's/calculated/fixedValue/' processor*/0/[af]*/U
sed -i -f calcType.sed processor*/0/[af]*/*


# region sets are lost in decomposition: replace them now
#
rm -rf processor*/constant/polyMesh/sets processor*/constant/polyMesh/*Zones
mpirun -np $NPROCS setSet -batch ./config/make.setSet -noVTK -parallel
mpirun -np $NPROCS setsToZones -noFlipMap -parallel


# remove the 1 directory and create the fluid zones
#
rm -r processor*/1

# air zones
#
mpirun -np $NPROCS setSet -batch ./config/make.setAir -region air -constant -noVTK -parallel
mpirun -np $NPROCS setsToZones -noFlipMap -region air -constant -parallel

# fuel zones
#
mpirun -np $NPROCS setSet -batch ./config/make.setFuel -region fuel -constant -noVTK -parallel
mpirun -np $NPROCS setsToZones -noFlipMap -region fuel -constant -parallel


# cleanup
./cleanup.csh


# copy *ProcAddressing files from earlier fluid region decomposition
# to processor fluid region meshes
@ I = 0
while ( $I < $NPROCS )
    foreach FLUID ( air fuel )
        echo $FLUID
        set SRCD = proc_$I/constant/$FLUID/polyMesh
	set DESTD = processor$I/constant/$FLUID/polyMesh
        cp $SRCD/*ProcAddressing $DESTD
    end
    echo "processor"$I "done"
    @ I++
end
