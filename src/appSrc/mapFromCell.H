{
    // Air
    {
       forAll (Tair, cellI)
        {
            Tair[cellI] = Tcell[airCellMap[cellI]];
        }
        Tair.correctBoundaryConditions();
    }

    // Fuel
    {
       forAll (Tfuel, cellI)
        {
            Tfuel[cellI] = Tcell[fuelCellMap[cellI]];
        }
        Tfuel.correctBoundaryConditions();
    }

    Info << "Tair min mean max  = "
         << Foam::gMin(Tair.internalField())     << "    "
         << Foam::gAverage(Tair.internalField()) << "    "
         << Foam::gMax(Tair.internalField())
         << endl;

    Info << "Tfuel min mean max = "
         << Foam::gMin(Tfuel.internalField())     << "    "
         << Foam::gAverage(Tfuel.internalField()) << "    "
         << Foam::gMax(Tfuel.internalField())
         << endl;
}

