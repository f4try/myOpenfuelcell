#!/bin/csh

## edit system/decomposeParDict for the desired decomposition
## set environment variable NPROCS to number of processors.
##     e.g., setenv NPROCS 2
## make mesh
##
## then:


echo "NPROCS = " $NPROCS


# To reconstruct and visualize the regions, we need the *ProcAddressing files
# created by decomposePar -region <region name>
# After the region decomp, we rename the processor* directories as proc_*
# to (a) allow the parallel decomp to proceed 
# while (b) saving the *ProcAddressing files for later copy
#
cp system/decomposeParDict system/air
cp system/decomposeParDict system/fuel
cp system/decomposeParDict system/electrolyte
cp system/decomposeParDict system/interconnect
decomposePar -region air
decomposePar -region fuel
decomposePar -region electrolyte
decomposePar -region interconnect

#rename processor proc_ processor*
@ i = 0
while ( $i < $NPROCS )
    mv processor$i proc_$i
    @ i++
end 

# decompose the global mesh
rm -rf processor*
decomposePar -force >& log.decompose

mpirun -np $NPROCS splitMeshRegions -cellZonesOnly -parallel >& log.split


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
    cp -rf 1/electrolyte ./0
    cp -rf 1/interconnect ./0
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
sed -i -e '/calculated/,/}/  s/value/gradient/; s/calculated/fixedGradient/' processor*/0/[af]*/*

# region sets are lost in decomposition: replace them now
#
rm -rf processor*/constant/polyMesh/sets processor*/constant/polyMesh/*Zones
mpirun -np $NPROCS setSet -batch ./config/make.setSet -noVTK -parallel -noZero
mpirun -np $NPROCS setsToZones -noFlipMap -parallel -noZero


# remove the 1 directory and create the fluid zones
#
rm -r processor*/1

# air zones
#
mpirun -np $NPROCS setSet -batch ./config/make.setAir -region air -constant -noVTK -parallel -noZero
mpirun -np $NPROCS setsToZones -noFlipMap -region air -constant -parallel -noZero

# fuel zones
#
mpirun -np $NPROCS setSet -batch ./config/make.setFuel -region fuel -constant -noVTK -parallel -noZero
mpirun -np $NPROCS setsToZones -noFlipMap -region fuel -constant -parallel -noZero

# electrolyte zones
#
mpirun -np $NPROCS setSet -batch ./config/make.setElectrolyte -region electrolyte -constant -noVTK -parallel -noZero
mpirun -np $NPROCS setsToZones -noFlipMap -region electrolyte -constant -parallel -noZero


# cleanup
./cleanup.csh


# copy *ProcAddressing files from earlier region decomposition
# to processor region meshes
@ I = 0
while ( $I < $NPROCS )
    foreach REGION ( air fuel electrolyte interconnect )
        echo $REGION
        set SRCD = proc_$I/constant/$REGION/polyMesh
	set DESTD = processor$I/constant/$REGION/polyMesh
        cp $SRCD/*ProcAddressing $DESTD
    end
    echo "processor"$I "done"
    @ I++
end
