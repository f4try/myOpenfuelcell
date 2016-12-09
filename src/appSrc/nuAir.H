// obtain nu(air mixture) from mu(air mixture) and rho(air mixture)
{
    scalarField& nu = nuAir.internalField();
    nu = 0;
    nu = muAir/rhoAir;
    nuAir.correctBoundaryConditions();
   Info << "min,mean,max(nuAir): "
        << gMin(nu)      << "   "
        << gAverage(nu)  << "   "
        << gMax(nu)      << endl;
}

