// Reynolds number @ airInlet

if(ReA)
{ 
    scalar ReAir= dHydAir.value()*Foam::mag
    (
        Foam::gAverage
        (
            rhoAir.boundaryField()[airInletID]
            *Uair.boundaryField()[airInletID]
            /muAir.boundaryField()[airInletID]
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
            rhoFuel.boundaryField()[fuelInletID]
            *Ufuel.boundaryField()[fuelInletID]
            /muFuel.boundaryField()[fuelInletID]
        )
    );

    Info<< "ReFuel = "<<ReFuel<<endl;
}
