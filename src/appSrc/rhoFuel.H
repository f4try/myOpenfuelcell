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
      /**********************************************************
        scalarField sumYjOnMj(Yfuel[0].internalField().size(), 0);  //sum(yj/Mj)
        forAll(Yfuel, j)
        {
            sumYjOnMj += Yfuel[j].internalField()/fuelSpecies[j].MW();
        }

        scalarField& rho = rhoFuel.internalField();
        rho = pFuel/(RR*Tfuel)/sumYjOnMj;
        rhoFuel.correctBoundaryConditions();
      **********************************************************/

        scalarField& rho = rhoFuel.internalField();
        rho = pFuel/(RR*Tfuel)/sumFuelYIonMI;
        rhoFuel.correctBoundaryConditions();

        Info << "min,mean,max(rhoFuel): "
             << gMin(rho)      << "   "
             << gAverage(rho)  << "   "
             << gMax(rho)      << endl;
    }
}
