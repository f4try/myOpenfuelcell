{
    // total air mass rate at inlet
    scalarField  rateInlet =
    (
        rhoAir.boundaryFieldRef()[airInletID]
        *(
             Uair.boundaryFieldRef()[airInletID]
             &
             airMesh.Sf().boundaryField()[airInletID]
         )
    );

    // total air mass rate at outlet
    scalarField  rateOutlet =
    (
        rhoAir.boundaryFieldRef()[airOutletID]
        *(
             Uair.boundaryFieldRef()[airOutletID]
             &
             airMesh.Sf().boundaryField()[airOutletID]
         )
    );

    // total air mass rate at cathode/electrolyte interface
    scalarField  rateCathode =
    (
        rhoAir.boundaryFieldRef()[cathodeID]
        *(
             Uair.boundaryFieldRef()[cathodeID]
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
                rateInlet*Yair[I].boundaryFieldRef()[airInletID]
            );

            scalar rateOutletI = gSum
            (
                rateOutlet*Yair[I].boundaryFieldRef()[airOutletID]
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

