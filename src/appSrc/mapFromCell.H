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
         << Foam::gMin(Tair.primitiveFieldRef())     << "    "
         << Foam::gAverage(Tair.primitiveFieldRef()) << "    "
         << Foam::gMax(Tair.primitiveFieldRef())
         << endl;

    Info << "Tfuel min mean max = "
         << Foam::gMin(Tfuel.primitiveFieldRef())     << "    "
         << Foam::gAverage(Tfuel.primitiveFieldRef()) << "    "
         << Foam::gMax(Tfuel.primitiveFieldRef())
         << endl;
}

