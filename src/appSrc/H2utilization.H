
// H2 mass rate at inlet

{
label inletID = fuelMesh.boundaryMesh().findPatchID("fuelInlet");
scalarField  mH2rateInlet =
(
    rhoFuel.boundaryField()[inletID]
    *yH2fuel.boundaryField()[inletID]
    *(
         Ufuel.boundaryField()[inletID]
         &
         fuelMesh.Sf().boundaryField()[inletID]
     )
);

// H2 mass rate at active area
scalarField  mH2rateElec = mH2*fuelMesh.magSf().boundaryField()[anodeID];

Info<< "H2 inRate = " << Foam::mag(Foam::sum(mH2rateInlet))
    << "   chemRate = " << Foam::mag(Foam::sum(mH2rateElec))
    << "   utilization = "
    << Foam::mag(Foam::sum(mH2rateElec)/Foam::sum(mH2rateInlet))
    << endl;
}
