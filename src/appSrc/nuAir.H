// obtain nu(air mixture) from mu(air mixture) and rho(air mixture)
{
    scalarField& nu = nuAir.primitiveFieldRef();
    nu = 0;
    nu = muAir/rhoAir;
    nuAir.correctBoundaryConditions();
   Info << "min,mean,max(nuAir): "
        << gMin(nu)      << "   "
        << gAverage(nu)  << "   "
        << gMax(nu)      << endl;
}

