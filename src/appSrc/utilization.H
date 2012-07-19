// O2 and H2 utilization

// O2
// --
{
    label inletID = airMesh.boundaryMesh().findPatchID("airInlet");
    label outletID = airMesh.boundaryMesh().findPatchID("airOutlet");

    scalar inRate = Foam::mag
    (
        Foam::gSum
        (
            yO2air.boundaryField()[inletID]
            *(
                 Uair.boundaryField()[inletID]
                 &
                 airMesh.Sf().boundaryField()[inletID]
             )
            *rhoAir.boundaryField()[inletID]
        )
    );

    scalar outRate = Foam::mag
    (
        Foam::gSum
        (
            yO2air.boundaryField()[outletID]
            *(
                 Uair.boundaryField()[outletID]
                 &
                 airMesh.Sf().boundaryField()[outletID]
             )
            *rhoAir.boundaryField()[outletID]
        )
    );

    Info<< "O2 inRate = " << inRate
        << "  outRate = " << outRate
        << "  O2 utilizn = " << (inRate - outRate)/inRate << endl;
}

// H2
// --
{
    label inletID = fuelMesh.boundaryMesh().findPatchID("fuelInlet");
    label outletID = fuelMesh.boundaryMesh().findPatchID("fuelOutlet");

    scalar inRate = Foam::mag
    (
        Foam::gSum
        (
            yH2fuel.boundaryField()[inletID]
            *(
                 Ufuel.boundaryField()[inletID]
                 &
                 fuelMesh.Sf().boundaryField()[inletID]
             )
            *rhoFuel.boundaryField()[inletID]
        )
    );

    scalar outRate = Foam::mag
    (
        Foam::gSum
        (
            yH2fuel.boundaryField()[outletID]
            *(
                 Ufuel.boundaryField()[outletID]
                 &
                 fuelMesh.Sf().boundaryField()[outletID]
             )
            *rhoFuel.boundaryField()[outletID]
        )
    );

    Info<< "H2 inRate = " << inRate
        << "  outRate = " << outRate
        << "  H2 utilizn = " << (inRate - outRate)/inRate << endl;
}

