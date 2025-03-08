// obtain mu(air mixture) from mu(species components)

{
    scalarField& mu = muAir.primitiveFieldRef();
    mu = 0;

    // initially using a simple linear combination weighted by mass fraction

    forAll(airSpecies, I)
    {
        scalarField muI = muAirL[I].polyVal(Tair.primitiveFieldRef());
        mu += muI*Yair[I].primitiveFieldRef();
    }
    mu *= 1e-7;    //convert from micro-Poise to kg/(m s)
    muAir.correctBoundaryConditions();

   Info << "min,mean,max(muAir): "
        << gMin(mu)      << "   "
        << gAverage(mu)  << "   "
        << gMax(mu)      << endl;
}
