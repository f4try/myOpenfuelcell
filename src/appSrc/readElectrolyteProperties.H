    Info<< "Reading electrolyte properties\n" << endl;

    IOdictionary electrolyteProperties
    (
        IOobject
        (
            "electrolyteProperties",
            runTime.constant(),
            electrolyteMesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );


    // Read electrolyte properties
    dimensionedScalar rhoElectrolyte(electrolyteProperties.lookup("rho"));
    dimensionedScalar CpElectrolyte(electrolyteProperties.lookup("Cp"));
    dimensionedScalar kElectrolyte(electrolyteProperties.lookup("k"));


    // calculate electrolye height thickness
    //include "electrolyteThickness.H"


    // initial ohmic heating
    dimensionedScalar HsrcElectrolyte(electrolyteProperties.lookup("Hsrc"));

    // echo inputs from dictionary to stdout
    Info
        << "    rhoElectrolyte  = " << rhoElectrolyte  << nl
        << "    CpElectrolyte   = " << CpElectrolyte   << nl
        << "    kElectrolyte    = " << kElectrolyte    << nl
        //<< "    hE (calculated) = " << hE              << nl
        << "    HsrcElectrolyte = " << HsrcElectrolyte << nl
        << endl;
