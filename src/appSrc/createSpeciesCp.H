    // Establish Cp as Cp(T) for each fluid species

    // Molar heat capacity [kJ/kmol-K] = [J/mol-K]:
    // Following Cp(T) are 6th degree (7th order) polynomials in T[K]/1000, from
    // Todd & Young, Thermodynamic and transport properties of gases for use in
    // solid oxide fuel cell modelling, J. Power Sources 110 (2002) pp186-200.
    // Cp(T) = sum(a[k]*(T/1000)^k), k = 0,..,6

    polyToddYoung CpH2O
        (37.373, -41.205, 146.01, -217.08, 181.54, -79.409, 14.015);

    polyToddYoung CpH2
        (21.157, 56.036, -150.55, 199.29, -136.15, 46.903, -6.4725);

    polyToddYoung CpO2
        (34.850, -57.975, 203.68, -300.37, 231.72, -91.821, 14.776);

    polyToddYoung CpN2
        (29.027, 4.8987, -38.040, 105.17, -113.56, 55.554, -10.350);

