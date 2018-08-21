// Reynolds number @ airInlet

if(ReA)
{ 
    scalar ReAir= dHydAir.value()*Foam::mag
    (
        Foam::gAverage
        (
            rhoAir.boundaryFieldRef()[airInletID]
            *Uair.boundaryFieldRef()[airInletID]
            /muAir.boundaryFieldRef()[airInletID]
        )
    );
     
    Info<< "ReAir  = "<<ReAir<<endl;
}


// Reynolds number @ fuelInlet

if(ReF)
{
    scalar ReFuel= dHydFuel.value()*Foam::mag
    (
        Foam::gAverage
        (
            rhoFuel.boundaryFieldRef()[fuelInletID]
            *Ufuel.boundaryFieldRef()[fuelInletID]
            /muFuel.boundaryFieldRef()[fuelInletID]
        )
    );

    Info<< "ReFuel = "<<ReFuel<<endl;
}
