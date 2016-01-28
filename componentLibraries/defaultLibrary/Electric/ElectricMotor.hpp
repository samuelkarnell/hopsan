/*-----------------------------------------------------------------------------
 This source file is a part of Hopsan

 Copyright (c) 2009 to present year, Hopsan Group

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

 For license details and information about the Hopsan Group see the files
 GPLv3 and HOPSANGROUP in the Hopsan source code root directory

 For author and contributor information see the AUTHORS file
-----------------------------------------------------------------------------*/

#ifndef ELECTRICMOTOR_HPP_INCLUDED
#define ELECTRICMOTOR_HPP_INCLUDED

#include <iostream>
#include "ComponentEssentials.h"
#include "ComponentUtilities.h"
#include "math.h"

//!
//! @file ElectricMotor.hpp
//! @author Petter Krus <petter.krus@liu.se>
//! @date Mon 7 Apr 2014 13:06:56
//! @brief Electric motor with inertia load
//! @ingroup ElectricComponents
//!
//==This code has been autogenerated using Compgen==
//from 
/*{, C:, HopsanTrunk, ComponentLibraries, defaultLibrary, \
Electric}/ElectricMotor.nb*/

using namespace hopsan;

class ElectricMotor : public ComponentQ
{
private:
     double Ke;
     double Ra;
     double Tm0;
     double wc;
     double Bm;
     double Jm;
     Port *mpPel1;
     Port *mpPel2;
     Port *mpPmr1;
     double delayParts1[9];
     double delayParts2[9];
     double delayParts3[9];
     double delayParts4[9];
     double delayParts5[9];
     double delayParts6[9];
     Matrix jacobianMatrix;
     Vec systemEquations;
     Matrix delayedPart;
     int i;
     int iter;
     int mNoiter;
     double jsyseqnweight[4];
     int order[6];
     int mNstep;
     //Port Pel1 variable
     double uel1;
     double iel1;
     double cel1;
     double Zcel1;
     //Port Pel2 variable
     double uel2;
     double iel2;
     double cel2;
     double Zcel2;
     //Port Pmr1 variable
     double tormr1;
     double thetamr1;
     double wmr1;
     double cmr1;
     double Zcmr1;
     double eqInertiamr1;
//==This code has been autogenerated using Compgen==
     //inputVariables
     //outputVariables
     double tormg;
     double Pin;
     double Pout;
     //Expressions variables
     //Port Pel1 pointer
     double *mpND_uel1;
     double *mpND_iel1;
     double *mpND_cel1;
     double *mpND_Zcel1;
     //Port Pel2 pointer
     double *mpND_uel2;
     double *mpND_iel2;
     double *mpND_cel2;
     double *mpND_Zcel2;
     //Port Pmr1 pointer
     double *mpND_tormr1;
     double *mpND_thetamr1;
     double *mpND_wmr1;
     double *mpND_cmr1;
     double *mpND_Zcmr1;
     double *mpND_eqInertiamr1;
     //Delay declarations
//==This code has been autogenerated using Compgen==
     //inputVariables pointers
     //inputParameters pointers
     double *mpKe;
     double *mpRa;
     double *mpTm0;
     double *mpwc;
     double *mpBm;
     double *mpJm;
     //outputVariables pointers
     double *mptormg;
     double *mpPin;
     double *mpPout;
     Delay mDelayedPart10;
     Delay mDelayedPart11;
     Delay mDelayedPart20;
     Delay mDelayedPart21;
     Delay mDelayedPart30;
     EquationSystemSolver *mpSolver;

public:
     static Component *Creator()
     {
        return new ElectricMotor();
     }

     void configure()
     {
//==This code has been autogenerated using Compgen==

        mNstep=9;
        jacobianMatrix.create(6,6);
        systemEquations.create(6);
        delayedPart.create(7,6);
        mNoiter=2;
        jsyseqnweight[0]=1;
        jsyseqnweight[1]=0.67;
        jsyseqnweight[2]=0.5;
        jsyseqnweight[3]=0.5;


        //Add ports to the component
        mpPel1=addPowerPort("Pel1","NodeElectric");
        mpPel2=addPowerPort("Pel2","NodeElectric");
        mpPmr1=addPowerPort("Pmr1","NodeMechanicRotational");
        //Add inputVariables to the component

        //Add inputParammeters to the component
            addInputVariable("Ke", "emf constant", "V s/rad", 0.13,&mpKe);
            addInputVariable("Ra", "motor resistance", "Ohm", 0.04,&mpRa);
            addInputVariable("Tm0", "zero speed friction of motor", "Nm", \
0.,&mpTm0);
            addInputVariable("wc", "Friction speed", "rad/s", 1.,&mpwc);
            addInputVariable("Bm", "Visc. fric. coeff., motor", "Nms/rad", \
0.0012,&mpBm);
            addInputVariable("Jm", "Moment of inertia, motor", "kg m^2", \
1,&mpJm);
        //Add outputVariables to the component
            addOutputVariable("tormg","Generated motor \
torque","Nm",0.,&mptormg);
            addOutputVariable("Pin","Input power","W",0.,&mpPin);
            addOutputVariable("Pout","Output power","W",0.,&mpPout);

//==This code has been autogenerated using Compgen==
        //Add constantParameters
        mpSolver = new EquationSystemSolver(this,6);
     }

    void initialize()
     {
        //Read port variable pointers from nodes
        //Port Pel1
        mpND_uel1=getSafeNodeDataPtr(mpPel1, NodeElectric::Voltage);
        mpND_iel1=getSafeNodeDataPtr(mpPel1, NodeElectric::Current);
        mpND_cel1=getSafeNodeDataPtr(mpPel1, NodeElectric::WaveVariable);
        mpND_Zcel1=getSafeNodeDataPtr(mpPel1, NodeElectric::CharImpedance);
        //Port Pel2
        mpND_uel2=getSafeNodeDataPtr(mpPel2, NodeElectric::Voltage);
        mpND_iel2=getSafeNodeDataPtr(mpPel2, NodeElectric::Current);
        mpND_cel2=getSafeNodeDataPtr(mpPel2, NodeElectric::WaveVariable);
        mpND_Zcel2=getSafeNodeDataPtr(mpPel2, NodeElectric::CharImpedance);
        //Port Pmr1
        mpND_tormr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::Torque);
        mpND_thetamr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::Angle);
        mpND_wmr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::AngularVelocity);
        mpND_cmr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::WaveVariable);
        mpND_Zcmr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::CharImpedance);
        mpND_eqInertiamr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::EquivalentInertia);

        //Read variables from nodes
        //Port Pel1
        uel1 = (*mpND_uel1);
        iel1 = (*mpND_iel1);
        cel1 = (*mpND_cel1);
        Zcel1 = (*mpND_Zcel1);
        //Port Pel2
        uel2 = (*mpND_uel2);
        iel2 = (*mpND_iel2);
        cel2 = (*mpND_cel2);
        Zcel2 = (*mpND_Zcel2);
        //Port Pmr1
        tormr1 = (*mpND_tormr1);
        thetamr1 = (*mpND_thetamr1);
        wmr1 = (*mpND_wmr1);
        cmr1 = (*mpND_cmr1);
        Zcmr1 = (*mpND_Zcmr1);
        eqInertiamr1 = (*mpND_eqInertiamr1);

        //Read inputVariables from nodes

        //Read inputParameters from nodes
        Ke = (*mpKe);
        Ra = (*mpRa);
        Tm0 = (*mpTm0);
        wc = (*mpwc);
        Bm = (*mpBm);
        Jm = (*mpJm);

        //Read outputVariables from nodes
        tormg = (*mptormg);
        Pin = (*mpPin);
        Pout = (*mpPout);

//==This code has been autogenerated using Compgen==


        //Initialize delays
        delayParts1[1] = (-(iel2*Ke*mTimestep) + mTimestep*tormr1 - 2*Jm*wmr1 \
+ Bm*mTimestep*wmr1 + mTimestep*Tm0*limit(wmr1/wc,-1.,1.))/(2*Jm + \
Bm*mTimestep);
        mDelayedPart11.initialize(mNstep,delayParts1[1]);
        delayParts2[1] = (-2*thetamr1 - mTimestep*wmr1)/2.;
        mDelayedPart21.initialize(mNstep,delayParts2[1]);

        delayedPart[1][1] = delayParts1[1];
        delayedPart[2][1] = delayParts2[1];
        delayedPart[3][1] = delayParts3[1];
        delayedPart[4][1] = delayParts4[1];
        delayedPart[5][1] = delayParts5[1];
        delayedPart[6][1] = delayParts6[1];
     }
    void simulateOneTimestep()
     {
        Vec stateVar(6);
        Vec stateVark(6);
        Vec deltaStateVar(6);

        //Read variables from nodes
        //Port Pel1
        cel1 = (*mpND_cel1);
        Zcel1 = (*mpND_Zcel1);
        //Port Pel2
        cel2 = (*mpND_cel2);
        Zcel2 = (*mpND_Zcel2);
        //Port Pmr1
        cmr1 = (*mpND_cmr1);
        Zcmr1 = (*mpND_Zcmr1);

        //Read inputVariables from nodes

        //LocalExpressions

        //Initializing variable vector for Newton-Raphson
        stateVark[0] = wmr1;
        stateVark[1] = thetamr1;
        stateVark[2] = iel2;
        stateVark[3] = uel1;
        stateVark[4] = uel2;
        stateVark[5] = tormr1;

        //Iterative solution using Newton-Rapshson
        for(iter=1;iter<=mNoiter;iter++)
        {
         //Motor
         //Differential-algebraic system of equation parts

          //Assemble differential-algebraic equations
          systemEquations[0] =wmr1 + (mTimestep*(-(iel2*Ke) + tormr1 + \
Tm0*limit(wmr1/wc,-1.,1.)))/(2*Jm + Bm*mTimestep) + delayedPart[1][1];
          systemEquations[1] =thetamr1 - (mTimestep*wmr1)/2. + \
delayedPart[2][1];
          systemEquations[2] =iel2 + (-uel1 + uel2 + Ke*wmr1)/Ra;
          systemEquations[3] =-cel1 + uel1 + iel2*Zcel1;
          systemEquations[4] =-cel2 + uel2 - iel2*Zcel2;
          systemEquations[5] =-cmr1 + tormr1 - wmr1*Zcmr1;

          //Jacobian matrix
          jacobianMatrix[0][0] = 1 + \
(mTimestep*Tm0*dxLimit(wmr1/wc,-1.,1.))/((2*Jm + Bm*mTimestep)*wc);
          jacobianMatrix[0][1] = 0;
          jacobianMatrix[0][2] = -((Ke*mTimestep)/(2*Jm + Bm*mTimestep));
          jacobianMatrix[0][3] = 0;
          jacobianMatrix[0][4] = 0;
          jacobianMatrix[0][5] = mTimestep/(2*Jm + Bm*mTimestep);
          jacobianMatrix[1][0] = -mTimestep/2.;
          jacobianMatrix[1][1] = 1;
          jacobianMatrix[1][2] = 0;
          jacobianMatrix[1][3] = 0;
          jacobianMatrix[1][4] = 0;
          jacobianMatrix[1][5] = 0;
          jacobianMatrix[2][0] = Ke/Ra;
          jacobianMatrix[2][1] = 0;
          jacobianMatrix[2][2] = 1;
          jacobianMatrix[2][3] = -(1/Ra);
          jacobianMatrix[2][4] = 1/Ra;
          jacobianMatrix[2][5] = 0;
          jacobianMatrix[3][0] = 0;
          jacobianMatrix[3][1] = 0;
          jacobianMatrix[3][2] = Zcel1;
          jacobianMatrix[3][3] = 1;
          jacobianMatrix[3][4] = 0;
          jacobianMatrix[3][5] = 0;
          jacobianMatrix[4][0] = 0;
          jacobianMatrix[4][1] = 0;
          jacobianMatrix[4][2] = -Zcel2;
          jacobianMatrix[4][3] = 0;
          jacobianMatrix[4][4] = 1;
          jacobianMatrix[4][5] = 0;
          jacobianMatrix[5][0] = -Zcmr1;
          jacobianMatrix[5][1] = 0;
          jacobianMatrix[5][2] = 0;
          jacobianMatrix[5][3] = 0;
          jacobianMatrix[5][4] = 0;
          jacobianMatrix[5][5] = 1;
//==This code has been autogenerated using Compgen==

          //Solving equation using LU-faktorisation
          mpSolver->solve(jacobianMatrix, systemEquations, stateVark, iter);
          wmr1=stateVark[0];
          thetamr1=stateVark[1];
          iel2=stateVark[2];
          uel1=stateVark[3];
          uel2=stateVark[4];
          tormr1=stateVark[5];
          //Expressions
          iel1 = -iel2;
          tormg = iel2*Ke - Bm*wmr1 - Tm0*limit(wmr1/wc,-1.,1.);
          Pin = iel2*(uel1 - uel2);
          Pout = tormr1*wmr1;
        }

        //Calculate the delayed parts
        delayParts1[1] = (-(iel2*Ke*mTimestep) + mTimestep*tormr1 - 2*Jm*wmr1 \
+ Bm*mTimestep*wmr1 + mTimestep*Tm0*limit(wmr1/wc,-1.,1.))/(2*Jm + \
Bm*mTimestep);
        delayParts2[1] = (-2*thetamr1 - mTimestep*wmr1)/2.;

        delayedPart[1][1] = delayParts1[1];
        delayedPart[2][1] = delayParts2[1];
        delayedPart[3][1] = delayParts3[1];
        delayedPart[4][1] = delayParts4[1];
        delayedPart[5][1] = delayParts5[1];
        delayedPart[6][1] = delayParts6[1];

        //Write new values to nodes
        //Port Pel1
        (*mpND_uel1)=uel1;
        (*mpND_iel1)=iel1;
        //Port Pel2
        (*mpND_uel2)=uel2;
        (*mpND_iel2)=iel2;
        //Port Pmr1
        (*mpND_tormr1)=tormr1;
        (*mpND_thetamr1)=thetamr1;
        (*mpND_wmr1)=wmr1;
        (*mpND_eqInertiamr1)=eqInertiamr1;
        //outputVariables
        (*mptormg)=tormg;
        (*mpPin)=Pin;
        (*mpPout)=Pout;

        //Update the delayed variabels
        mDelayedPart11.update(delayParts1[1]);
        mDelayedPart21.update(delayParts2[1]);

     }
    void deconfigure()
    {
        delete mpSolver;
    }
};
#endif // ELECTRICMOTOR_HPP_INCLUDED
