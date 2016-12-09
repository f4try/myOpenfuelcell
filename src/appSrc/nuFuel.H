// obtain nu(fuel mixture) from mu(fuel mixture) and rhoFuel(fuel mixture)
{
    scalarField& nu = nuFuel.internalField();
    nu = 0;
    nu = muFuel/rhoFuel;
    nuFuel.correctBoundaryConditions();
   Info << "min,mean,max(nuFuel): "
        << gMin(nu)      << "   "
        << gAverage(nu)  << "   "
        << gMax(nu)      << endl;
}

