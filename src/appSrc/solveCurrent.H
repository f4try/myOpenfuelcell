{


	label znId1 = electrolyteMesh.cellZones().findZoneID("layer");

    	Info << nl << "Solve current" << endl;

	scalar stackVoltage = 0.0;

	for (int counter1=1; counter1<=numCells.value(); counter1++)
	{
	    	Info << nl << "layer " << counter1 << endl;

		char rLayer[16]; // string which will contain the number

		sprintf (rLayer, "layer%d", counter1 ); // %d makes the result be a decimal integer

		znId1 = electrolyteMesh.cellZones().findZoneID(rLayer);
		labelList zn1(electrolyteMesh.cellZones()[znId1]);

		{


		    dimensionedScalar ibar ("ibar", dimensionSet(0, -2, 0, 0, 0, 1, 0), 0.0);
		    dimensionedScalar temp ("temp", dimensionSet(0, 0, 0, 1, 0, 0, 0), 0.0);

			ibar.value() = 0.0;
		    scalar rCurrentDV = 0.0;
		    scalar volume	= 0.0;
		    scalar sumVolume = 0.0;
		    scalar counter2 = 0.0;


		    forAll(zn1, j)
		    {    		    
			volume = mag(electrolyteMesh.V()[zn1[j]]);
			rCurrentDV += idensityIn[zn1[j]]*volume;
			sumVolume += volume;
			counter2 += 1;
			V.value() = voltage[zn1[j]];

		    }

		reduce(sumVolume,plusOp<scalar>());
		reduce(rCurrentDV,plusOp<scalar>());
		reduce(counter2,plusOp<scalar>());


		    ibar.value() = rCurrentDV/sumVolume;
		    Info << nl << "sumVolume" << sumVolume << endl;
//		    Info << nl << "rCurrentDV" << rCurrentDV << endl;
//		    Info << nl << "counter2" << counter2 << endl;


		    Info << "min,max(Nernst): "
			 << Foam::min(Nernst) <<" , "
			 << Foam::max(Nernst) << endl;

//		    Info << "min,mean,max(i): "
//			 << Foam::min(idensityIn) <<" , " << ibar.value() << " , "
//			 << Foam::max(idensityIn) << endl;

		    // voltage correction
		    if (galvanostatic)
		    {

			V += Rhat*(ibar - ibar0);

			Info << "ibar0 = " << ibar0.value()
			     << "    ibar = " << ibar.value()
			     << "    V = " << V.value() << endl;

			forAll(zn1, j)
			{
			voltage[zn1[j]] = V.value();
			}
		    }
		    else
		    {
			Info << "    ibar = " << ibar.value()
			     << "    V = " << V.value() << endl;
		    }



		}
		stackVoltage += V.value();
	}
		Info << nl << "stack Voltage =" << stackVoltage << endl;
}






