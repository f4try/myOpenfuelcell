{
    // total mass rate at fuel inlet
    scalarField  rateInlet =
    (
        rhoFuel.boundaryFieldRef()[fuelInletID]
        *(
             Ufuel.boundaryFieldRef()[fuelInletID]
             &
             fuelMesh.Sf().boundaryField()[fuelInletID]
         )
    );
    
    // total mass rate at fuel outlet
    scalarField  rateOutlet =
    (
        rhoFuel.boundaryFieldRef()[fuelOutletID]
        *(
             Ufuel.boundaryFieldRef()[fuelOutletID]
             &
             fuelMesh.Sf().boundaryField()[fuelOutletID]
         )
    );
    
    // total mass rate at anode/electrolyte interface
    scalarField  rateAnode =
    (
        rhoFuel.boundaryFieldRef()[anodeID]
        *(
             Ufuel.boundaryFieldRef()[anodeID]
             &
             fuelMesh.Sf().boundaryField()[anodeID]
         )
    );

    Info<< "fuel mass rates: " << nl
        << "   inlet: " << Foam::mag(Foam::gSum(rateInlet))
        << "   outlet: " << Foam::mag(Foam::gSum(rateOutlet))
        << "   anode: " << Foam::mag(Foam::gSum(rateAnode))
        << nl;
    
    // --------------------------------------------------------------------
    
    forAll(fuelSpecies, I)
    {
        if(fuelSpecies[I].rSign() < 0)
        {
            scalar rateInletI = Foam::mag
            (
                gSum
                (
                    rateInlet*Yfuel[I].boundaryFieldRef()[fuelInletID]
                )
            );

            scalar rateOutletI = Foam::mag
            (
                gSum
                (
                    rateOutlet*Yfuel[I].boundaryFieldRef()[fuelOutletID]
                )
            );

            scalar rateElecI = Foam::mag
            (
                gSum
                (
                    mflux[I]*fuelMesh.magSf().boundaryField()[anodeID]
                )
            );

            Info<< fuelSpecies[I].name() << " mass rates: " << nl;
            Info<< "   inlet = " << rateInletI
                << "   outlet = " << rateOutletI
                << "   chem = " << rateElecI
                << nl;

            Info<< "      utilization by chemRate = "
                << rateElecI/rateInletI
                << nl;
            Info<< "      utilization by outRate  = "
                << (rateInletI - rateOutletI)/rateInletI
                << nl;

            Info<< endl;
        }
    }
}

