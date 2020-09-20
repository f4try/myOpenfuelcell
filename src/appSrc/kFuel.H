// obtain k(fuel mixture) from k(species components)

{
    scalarField& kFIn = kFuel.primitiveFieldRef();
    kFIn = 0;

    // initially using a simple linear combination weighted by mass fraction

    forAll(fuelSpecies, I)
    {
        scalarField kI = kFuelL[I].polyVal(Tfuel.primitiveFieldRef());
        kFIn += kI*Yfuel[I].primitiveFieldRef();
    }
    kFIn *= 1e-2;   // k=0.01*(ToddYoung polynomial value)
                    // Table 3, J. Power Sources 110 (2002) 186-200 (page 190)

    kFuel.correctBoundaryConditions();

    Info << "min,mean,max(kFuel): "
         << gMin(kFIn)      << "   "
         << gAverage(kFIn)  << "   "
         << gMax(kFIn)      << endl;
}
