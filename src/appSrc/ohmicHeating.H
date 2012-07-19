    // calculate volumetric ohmic heating

    Info << nl << "Calculating ohmic heating @ ohmicHeating.H" << endl;

    // thickness of electrolyte, hE, is read from electrolyte properties

    // interpolate anode current density i from fuel side to electrolyte side
    scalarField iEA = 
       fuelAnodeToElectrolyteAnode.faceInterpolate(i);

    // combine with overpotential and thickness of electrolyte
    scalarField iv = iEA * (E-V.value())/hE.value();

    // formation enthalpy H2O 
//    scalarField Hform = (-238.0392-0.07369004*273.15+2.072838e-6
//       *pow(273.15,2)) / (1+0.0002495145*273.15);

    // enthalpy H2O
    scalarField Hh2o = 37.373*(anodeT-273.15)  
       - 41.205*(pow(anodeT,2)-pow(273.15,2))*1e-3/2 
       + 146.01*(pow(anodeT,3)-pow(273.15,3))*1e-6/3
       - 217.08*(pow(anodeT,4)-pow(273.15,4))*1e-9/4
       + 181.54*(pow(anodeT,5)-pow(273.15,5))*1e-12/5
       - 79.409*(pow(anodeT,6)-pow(273.15,6))*1e-15/6
       + 14.015*(pow(anodeT,7)-pow(273.15,7))*1e-18/7;

    //enthalpy H2
    scalarField Hh2 = 21.157*(anodeT-273.15)
       + 56.036*(pow(anodeT,2)-pow(273.15,2))*1e-3/2
       - 150.55*(pow(anodeT,3)-pow(273.15,3))*1e-6/3
       + 199.29*(pow(anodeT,4)-pow(273.15,4))*1e-9/4
       - 136.15*(pow(anodeT,5)-pow(273.15,5))*1e-12/5
       + 46.903*(pow(anodeT,6)-pow(273.15,6))*1e-15/6
       - 6.4725*(pow(anodeT,7)-pow(273.15,7))*1e-18/7;

    //enthapy O2
    scalarField Ho2 = 34.85*(anodeT-273.15)
       - 57.975*(pow(anodeT,2)-pow(273.15,2))*1e-3/2
       + 203.68*(pow(anodeT,3)-pow(273.15,3))*1e-6/3
       - 300.37*(pow(anodeT,4)-pow(273.15,4))*1e-9/4
       + 231.72*(pow(anodeT,5)-pow(273.15,5))*1e-12/5
       - 91.821*(pow(anodeT,6)-pow(273.15,6))*1e-15/6
       + 14.776*(pow(anodeT,7)-pow(273.15,7))*1e-18/7;

    //Heat source
    scalarField Tsource = -(-241.8349e3+Hh2o-Hh2-0.5*Ho2)*iEA/(2*F)/hE.value() - iEA*V.value()/hE.value();

    // smear the surface heating field into electrolyte volume
    smearPatchToMesh smearInterpolation(electrolyteMesh, electrode_anodeName);
    ohmicHeating = smearInterpolation.smear(Tsource);

    Info << "min,max(ohmicHeating): "
         << Foam::min(ohmicHeating) <<" , " 
         << Foam::max(ohmicHeating) << endl;

