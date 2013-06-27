// obtain k(air mixture) from k(species components)

{
    scalarField& kAIn = kAir.internalField();
    kAIn = 0;

    // initially using a simple linear combination weighted by mass fraction

    forAll(airSpecies, I)
    {
        scalarField kI = kAirL[I].polyVal(Tair.internalField());
        kAIn += kI*Yair[I].internalField();
    }
    kAIn *= 1e-2;   // k=0.01*(ToddYoung polynomial value)
                    // Table 3, J. Power Sources 110 (2002) 186-200 (page 190)

    kAir.correctBoundaryConditions();

    Info << "min,mean,max(kAir): "
         << gMin(kAIn)      << "   "
         << gAverage(kAIn)  << "   "
         << gMax(kAIn)      << endl;
}
