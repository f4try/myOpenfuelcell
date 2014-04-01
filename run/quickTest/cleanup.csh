#!/bin/csh

# remove field files from regions (including global region) that don't use them
# set noGlobal is removed from global, etc.

set FIELDS =   ( diffH2 diffO2 k p rho T U YH2 YH2O YN2 YO2 )
set noGlobal = ( diffH2 diffO2   p rho   U YH2 YH2O YN2 YO2 )
set noAir =    ( diffH2 k YH2 YH2O )
set noFuel =   ( diffO2 k YN2 YO2 )

@ i = 0
while ( $i < $NPROCS )
    cd processor$i
    cd 0; rm $noGlobal; cd ..
    cd 0/air; rm $noAir; cd ../..
    cd 0/fuel; rm $noFuel; cd ../..
    #
    cd ..
    echo "processor"$i "done"
    @ i++
end
