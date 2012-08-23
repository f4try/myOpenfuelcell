{
    // total air mass rate at inlet
    scalarField  rateInlet =
    (
        rhoAir.boundaryField()[airInletID]
        *(
             Uair.boundaryField()[airInletID]
             &
             airMesh.Sf().boundaryField()[airInletID]
         )
    );

    // total air mass rate at outlet
    scalarField  rateOutlet =
    (
        rhoAir.boundaryField()[airOutletID]
        *(
             Uair.boundaryField()[airOutletID]
             &
             airMesh.Sf().boundaryField()[airOutletID]
         )
    );

    // total air mass rate at cathode/electrolyte interface
    scalarField  rateCathode =
    (
        rhoAir.boundaryField()[cathodeID]
        *(
             Uair.boundaryField()[cathodeID]
             &
             airMesh.Sf().boundaryField()[cathodeID]
         )
    );

    Info<< "air mass rates: " << nl
        << "   inlet: " << Foam::mag(Foam::gSum(rateInlet))
        << "   outlet: " << Foam::mag(Foam::gSum(rateOutlet))
        << "   cathode: " << Foam::mag(Foam::gSum(rateCathode))
        << nl;

    // --------------------------------------------------------------------

    forAll(airSpecies, I)
    {
        if(airSpecies[I].rSign() < 0)
        {
            scalar rateInletI = gSum
            (
                rateInlet*Yair[I].boundaryField()[airInletID]
            );

            scalar rateOutletI = gSum
            (
                rateOutlet*Yair[I].boundaryField()[airOutletID]
            );

            scalar rateElecI = gSum
            (
                anodeToCathode.faceInterpolate(mflux[I])
                *airMesh.magSf().boundaryField()[cathodeID]
            );

            Info<< airSpecies[I].name() << " mass rates: " << nl;
            Info<< "   inlet = " << Foam::mag(rateInletI)
                << "   outlet = " << Foam::mag(rateOutletI)
                << "   chem = " << Foam::mag(rateElecI)
                << nl;

            Info<< "      utilization by chemRate = "
                << Foam::mag(rateElecI/rateInletI)
                << nl
                << "      utilization by outRate  = "
                << 1.0 - Foam::mag(rateOutletI/rateInletI)
                << nl;

            Info<< endl;
        }
    }
}

