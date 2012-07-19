
// O2 mass rate at inlet
{
label inletID = airMesh.boundaryMesh().findPatchID("airInlet");
scalarField  mO2rateInlet =
(
    rhoAir.boundaryField()[inletID]
    *yO2air.boundaryField()[inletID]
    *(
         Uair.boundaryField()[inletID]
         &
         airMesh.Sf().boundaryField()[inletID]
     )
);

// O2 mass rate at active area
// recall mass flux mO2 is calculated on anode

scalarField mO2cat = anodeToCathode.faceInterpolate(mO2);
scalarField mO2rateElec = mO2cat*airMesh.magSf().boundaryField()[cathodeID];
//scalarField mO2rateElec = mO2*fuelMesh.magSf().boundaryField()[anodeID];

Info<< "O2 inRate = " << Foam::mag(Foam::sum(mO2rateInlet))
    << "   chemRate = " << Foam::mag(Foam::sum(mO2rateElec))
    << "   utilization = "
    << Foam::mag(Foam::sum(mO2rateElec)/Foam::sum(mO2rateInlet))
    << endl;
}
