    // Area Specific Resistance function
    //
    // Choose an existing one or add your own


    // an empiracal lumped resistance
    //    range of validity = 550 to 1200 degrees Celsius = 823 to 1473 K
    //    original polynomial in r delivers R in units of ohm*cm^2
    //    we multiply by 1.0e-4 to convert to ohm*m^2

scalarField R (anodePatch.size(), 1);
if (fuelCellType == "SOFC")	//choose type, Qing, 22.06.2014
{
    scalar T0 = 273.15;         // 273.15 Kelvin = 0 Celsius
    scalarField r = 1000.0/(anodeT-T0) - 1.1463;
/* Changed ASR to Ohmic resistance model SBB 04-04-18 
    R = 1.0e-4*
       (0.3044 + 0.408*r + 0.8687*sqr(r) + 2.7861*pow(r,3) + 2.9285*pow(r,4));
*/
    R = 1.0e-4*
       (0.2064 + 0.3495*r + 0.3451*sqr(r) + 4.1293*pow(r,3) + 11.4228*pow(r,4));
}
    // FZJ ASR function
    //scalarField R = 2.9e-5*exp(62715/Rgas*(1.0/anodeT - 1.0/1073.15));
else if (fuelCellType == "HTPEFC")
{
    R = 1.0e-4 * (0.4025 - 0.0007*cathodeT);
}
else 
{
    FatalError
	<< "not available fuel cell type! (available==>SOFC / HTPEFC)" <<nl
	<< exit (FatalError);
}
