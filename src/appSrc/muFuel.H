// obtain mu(fuel mixture) from mu(species components)

{
    scalarField& mu = muFuel.internalField();
    mu = 0;

    // initially using a simple linear combination weighted by mass fraction

    forAll(fuelSpecies, I)
    {
        scalarField muI = muFuelL[I].polyVal(Tfuel.internalField());
        mu += muI*Yfuel[I].internalField();
    }
    mu *= 1e-7;    //convert from micro-Poise to kg/(m s)
    muFuel.correctBoundaryConditions();

   Info << "min,mean,max(muFuel): "
        << gMin(mu)      << "   "
        << gAverage(mu)  << "   "
        << gMax(mu)      << endl;
}
