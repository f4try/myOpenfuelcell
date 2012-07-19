{
    scalar Rgas = physicalConstant::Rgas.value();

    // rho = (p/RT)*Sum(xi*Mi);  xi = (yi/Mi)/Sum(yj/Mj)

    // air
    {
        scalarField xO2 = (yO2air/MO2)/(yO2air/MO2 + yN2air/MN2);
        scalarField xN2 = (yN2air/MN2)/(yO2air/MO2 + yN2air/MN2);

        //Info << "min,mean,max(Tair): "
        //     << Foam::min(Tair.internalField())      << "   "
        //     << Foam::average(Tair.internalField())  << "   "
        //     << Foam::max(Tair.internalField())      << endl;

        scalarField& rho = rhoAir.internalField();

        rho = pAir/(Rgas*Tair)*(xO2*MO2 + xN2*MN2);
        rhoAir.correctBoundaryConditions();

        Info << "min,mean,max(rhoAir): "
             << Foam::gMin(rho)      << "   "
             << Foam::gAverage(rho)  << "   "
             << Foam::gMax(rho)      << endl;
    }

    // fuel
    {
        scalarField xH2  =   (yH2fuel/MH2)/(yH2fuel/MH2 + yH2Ofuel/MH2O);
        scalarField xH2O = (yH2Ofuel/MH2O)/(yH2fuel/MH2 + yH2Ofuel/MH2O);

        scalarField& rho = rhoFuel.internalField();

        rho = pFuel/(Rgas*Tfuel)*(xH2*MH2 + xH2O*MH2O);
        rhoFuel.correctBoundaryConditions();

        Info << "min,mean,max(rhoFuel): "
             << Foam::gMin(rho)       << "   "
             << Foam::gAverage(rho)   << "   "
             << Foam::gMax(rho)       << endl;
    }
}
