    Info<< "Reading activation parameters\n" << endl;

    IOdictionary activationParameters
    (
        IOobject
        (
            "activationParameters",
            runTime.constant(),
            electrolyteMesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    Switch activationOverpotential(activationParameters.lookup("activationOverpotential"));

    // Read electrode kinetic parameters
    dimensionedScalar gammaAnodeElectrolyte(activationParameters.lookup("gammaAnode"));
    dimensionedScalar aElectrolyte(activationParameters.lookup("a"));
    dimensionedScalar bElectrolyte(activationParameters.lookup("b"));
    dimensionedScalar EAnodeElectrolyte(activationParameters.lookup("EAnode"));
    dimensionedScalar alphaAnodeElectrolyte(activationParameters.lookup("alphaAnode"));

    dimensionedScalar gammaCathodeElectrolyte(activationParameters.lookup("gammaCathode"));
    dimensionedScalar mElectrolyte(activationParameters.lookup("m"));
    dimensionedScalar ECathodeElectrolyte(activationParameters.lookup("ECathode"));
    dimensionedScalar alphaCathodeElectrolyte(activationParameters.lookup("alphaCathode"));

    // echo inputs from dictionary to stdout
    Info<< "activation overpotential = " << activationOverpotential << nl;
    Info
        << "    gammaAnodeElectrolyte  = " << gammaAnodeElectrolyte  << nl
        << "    aElectrolyte   = " << aElectrolyte   << nl
        << "    bElectrolyte    = " << bElectrolyte    << nl
        << "    EAnodeElectrolyte   = " << EAnodeElectrolyte   << nl
        << "    alphaAnodeElectrolyte    = " << alphaAnodeElectrolyte    << nl
        << endl;
    Info
        << "    gammaCathodeElectrolyte  = " << gammaCathodeElectrolyte  << nl
        << "    mElectrolyte   = " << mElectrolyte   << nl
        << "    ECathodeElectrolyte   = " << ECathodeElectrolyte   << nl
        << "    alphaCathodeElectrolyte    = " << alphaCathodeElectrolyte    << nl
        << endl;
