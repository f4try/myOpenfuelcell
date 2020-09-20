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

\*---------------------------------------------------------------------------*/

#include "sofcSpecie.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components without name
inline sofcSpecie::sofcSpecie
(
    const scalar molWeight,
    const scalar nElectrons,
    const label rSign,
    const scalar hForm,
    const scalar sForm
)
:
    //name_(),
    molWeight_(molWeight),
    nElectrons_(nElectrons),
    rSign_(rSign),
    hForm_(hForm),
    sForm_(sForm)
{}


// Construct from components
inline sofcSpecie::sofcSpecie
(
    const word& name,
    const scalar molWeight,
    const scalar nElectrons,
    const label rSign,
    const scalar hForm,
    const scalar sForm
)
:
    name_(name),
    molWeight_(molWeight),
    nElectrons_(nElectrons),
    rSign_(rSign),
    hForm_(hForm),
    sForm_(sForm)
{}


// Construct as copy
inline sofcSpecie::sofcSpecie(const sofcSpecie& st)
:
    name_(st.name_),
    molWeight_(st.molWeight_),
    nElectrons_(st.nElectrons_),
    rSign_(st.rSign_),
    hForm_(st.hForm_),
    sForm_(st.sForm_)
{}


// Construct as named copy
inline sofcSpecie::sofcSpecie(const word& name, const sofcSpecie& st)
:
    name_(name),
    molWeight_(st.molWeight_),
    nElectrons_(st.nElectrons_),
    rSign_(st.rSign_),
    hForm_(st.hForm_),
    sForm_(st.sForm_)
{}


// Clone
inline Foam::autoPtr<sofcSpecie>
Foam::sofcSpecie::clone() const
{
    return autoPtr<sofcSpecie>
    (
        new sofcSpecie(*this)
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Name
inline word sofcSpecie::name() const
{
    return name_;
}

//- Molecular weight [kg/kmol]
inline scalar sofcSpecie::MW() const
{
    return molWeight_;
}

//- No of electrons this sofcSpecies contributes tp electrochemistry
inline scalar sofcSpecie::ne() const
{
    return nElectrons_;
}

//- Is the specie consumed(-1), produced(+1), or inert(0)?
inline label sofcSpecie::rSign() const
{
    return rSign_;
}

//- enthalpy of formation [J/mol]
inline scalar sofcSpecie::hForm() const
{
    return hForm_;
}

//- standard entropy [J/mol]
inline scalar sofcSpecie::sForm() const
{
    return sForm_;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void sofcSpecie::operator=(const sofcSpecie& st)
{
    name_ = st.name_;
    molWeight_ = st.molWeight_;
    nElectrons_ = st.nElectrons_;
    rSign_ = st.rSign_;
    hForm_ = st.hForm_;
    sForm_ = st.sForm_;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
