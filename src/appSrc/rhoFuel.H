{
    // rho = (p/RT)*Sum(xi*Mi);     xi = (yi/Mi)/Sum(yj/Mj)
    //                           xi*Mi = (yi)/Sum(yj/Mj)
    //                      Sum(xi*Mi) = Sum_i(yi/Sum_j(yj/Mj)
    //                                 = Sum_i(yi)/Sum_j(yj/Mj))
    //                                 = 1/Sum_j(yj/Mj)
    // rho = (p/RT)/Sum(yj/Mj)

    //const scalar Rgas = physicalConstant::Rgas.value();
    const scalar RR = physicalConstant::RR.value();

    // fuel
    {
        // sumFuelYIonMI was calculated in getXfuel.H

        scalarField& rho = rhoFuel.internalField();
        rho = pFuel/(RR*Tfuel)/sumFuelYIonMI;
        rhoFuel.correctBoundaryConditions();

        Info << "min,mean,max(rhoFuel): "
             << gMin(rho)      << "   "
             << gAverage(rho)  << "   "
             << gMax(rho)      << endl;
    }
}
