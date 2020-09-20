{
    // rho = (p/RT)*Sum(xi*Mi);     xi = (yi/Mi)/Sum(yj/Mj)
    //                           xi*Mi = (yi)/Sum(yj/Mj)
    //                      Sum(xi*Mi) = Sum_i(yi/Sum_j(yj/Mj)
    //                                 = Sum_i(yi)/Sum_j(yj/Mj))
    //                                 = 1/Sum_j(yj/Mj)
    // rho = (p/RT)/Sum(yj/Mj)

    //const scalar Rgas = physicalConstant::Rgas.value();
    const scalar RR = physicalConstant::RR.value();

    // air
    {
        // sumAirYIonMI was calculated in getXair.H

        scalarField& rho = rhoAir.primitiveFieldRef();
        rho = pAir/(RR*Tair)/sumAirYIonMI;
        rhoAir.correctBoundaryConditions();

        Info << "min,mean,max(rhoAir): "
             << gMin(rho)      << "   "
             << gAverage(rho)  << "   "
             << gMax(rho)      << endl;
    }
}
