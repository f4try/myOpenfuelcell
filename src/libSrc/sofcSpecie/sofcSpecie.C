/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Description
    Set of electrochemical properties for a sofc specie

\*---------------------------------------------------------------------------*/

#include "sofcSpecie.H"
#include "IOstreams.H"
#include "dimensionedConstants.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::sofcSpecie::sofcSpecie(Istream& is)
:
    name_(is),
    molWeight_(readScalar(is)),
    nElectrons_(readScalar(is)),
    rSign_(readLabel(is)),
    hForm_(readScalar(is)),
    sForm_(readScalar(is))
{
    is.check("sofcSpecie::sofcSpecie(Istream& is)");
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

Foam::Ostream& Foam::operator<<(Ostream& os, const sofcSpecie& st)
{
    os  << st.name_ << tab
        << st.molWeight_ << tab
        << st.nElectrons_ << tab
        << st.rSign_ << tab
	<< st.hForm_ << tab
	<< st.sForm_;

    os.check("Ostream& operator<<(Ostream& os, const sofcSpecie& st)");
    return os;
}


// ************************************************************************* //
