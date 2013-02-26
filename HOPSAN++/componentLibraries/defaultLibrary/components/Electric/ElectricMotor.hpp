#ifndef ELECTRICMOTOR_HPP_INCLUDED
#define ELECTRICMOTOR_HPP_INCLUDED

#include <iostream>
#include "ComponentEssentials.h"
#include "ComponentUtilities.h"
#include "math.h"


//!
//! @file ElectricMotor.hpp
//! @author Petter Krus <petter.krus@liu.se>
//! @date Tue 31 Jan 2012 15:03:08
//! @brief Electric motor with inertia load
//! @ingroup ElectricComponents
//!
//This component is generated by COMPGEN for HOPSAN-NG simulation 
//from 
/*{, C:, Documents and Settings, petkr14, My Documents, \
CompgenNG}/ElectricNG.nb*/

using namespace hopsan;

class ElectricMotor : public ComponentQ
{
private:
     double mKe;
     double mRa;
     double mTm0;
     double mwc;
     double mBm;
     double mJm;
     Port *mpPel1;
     Port *mpPel2;
     Port *mpPmr1;
     Port *mpPtormg;
     Port *mpPPin;
     Port *mpPPout;
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
     //inputVariables pointers
     //outputVariables pointers
     double *mpND_tormg;
     double *mpND_Pin;
     double *mpND_Pout;
     Delay mDelayedPart10;
     Delay mDelayedPart11;
     Delay mDelayedPart20;
     Delay mDelayedPart21;
     Delay mDelayedPart30;

     EquationSystemSolver *pSolver;

public:
     static Component *Creator()
     {
        return new ElectricMotor();
     }

     void configure()
     {
        const double Ke = 0.13;
        const double Ra = 0.04;
        const double Tm0 = 0.;
        const double wc = 1.;
        const double Bm = 0.0012;
        const double Jm = 1;

        mNstep=9;
        jacobianMatrix.create(6,6);
        systemEquations.create(6);
        delayedPart.create(7,6);
        mNoiter=2;
        jsyseqnweight[0]=1;
        jsyseqnweight[1]=0.67;
        jsyseqnweight[2]=0.5;
        jsyseqnweight[3]=0.5;

        mKe = Ke;
        mRa = Ra;
        mTm0 = Tm0;
        mwc = wc;
        mBm = Bm;
        mJm = Jm;

        //Add ports to the component
        mpPel1=addPowerPort("Pel1","NodeElectric");
        mpPel2=addPowerPort("Pel2","NodeElectric");
        mpPmr1=addPowerPort("Pmr1","NodeMechanicRotational");

        //Add inputVariables ports to the component

        //Add outputVariables ports to the component
        mpPtormg=addWritePort("Ptormg","NodeSignal", Port::NOTREQUIRED);
        mpPPin=addWritePort("PPin","NodeSignal", Port::NOTREQUIRED);
        mpPPout=addWritePort("PPout","NodeSignal", Port::NOTREQUIRED);

        //Register changable parameters to the HOPSAN++ core
        registerParameter("Ke", "emf constant", "V/rpm", mKe);
        registerParameter("Ra", "motor resistance", "Ohm", mRa);
        registerParameter("Tm0", "zero movement friction of motor", "Nm", mTm0);
        registerParameter("wc", "Friction movement", "rad/s", mwc);
        registerParameter("Bm", "Visc. fric. coeff., motor", "Ns/m", mBm);
        registerParameter("Jm", "Moment of inertia, motor", "kg m^2", mJm);

        pSolver = new EquationSystemSolver(this, 6);
     }

    void initialize()
     {
        //Read port variable pointers from nodes
        //Port Pel1
        mpND_uel1=getSafeNodeDataPtr(mpPel1, NodeElectric::VOLTAGE);
        mpND_iel1=getSafeNodeDataPtr(mpPel1, NodeElectric::CURRENT);
        mpND_cel1=getSafeNodeDataPtr(mpPel1, NodeElectric::WAVEVARIABLE);
        mpND_Zcel1=getSafeNodeDataPtr(mpPel1, NodeElectric::CHARIMP);
        //Port Pel2
        mpND_uel2=getSafeNodeDataPtr(mpPel2, NodeElectric::VOLTAGE);
        mpND_iel2=getSafeNodeDataPtr(mpPel2, NodeElectric::CURRENT);
        mpND_cel2=getSafeNodeDataPtr(mpPel2, NodeElectric::WAVEVARIABLE);
        mpND_Zcel2=getSafeNodeDataPtr(mpPel2, NodeElectric::CHARIMP);
        //Port Pmr1
        mpND_tormr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::TORQUE);
        mpND_thetamr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::ANGLE);
        mpND_wmr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::ANGULARVELOCITY);
        mpND_cmr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::WAVEVARIABLE);
        mpND_Zcmr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::CHARIMP);
        mpND_eqInertiamr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::EQINERTIA);
        //Read inputVariables pointers from nodes
        //Read outputVariable pointers from nodes
        mpND_tormg=getSafeNodeDataPtr(mpPtormg, NodeSignal::VALUE);
        mpND_Pin=getSafeNodeDataPtr(mpPPin, NodeSignal::VALUE);
        mpND_Pout=getSafeNodeDataPtr(mpPPout, NodeSignal::VALUE);

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

        //Read outputVariables from nodes
        tormg = mpPtormg->getStartValue(NodeSignal::VALUE);
        Pin = mpPPin->getStartValue(NodeSignal::VALUE);
        Pout = mpPPout->getStartValue(NodeSignal::VALUE);



        //Initialize delays
        delayParts1[1] = (-(iel2*mKe*mTimestep) + mTimestep*tormr1 - \
2*mJm*wmr1 + mBm*mTimestep*wmr1 + \
mTimestep*mTm0*limit(wmr1/mwc,-1.,1.))/(2*mJm + mBm*mTimestep);
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
          systemEquations[0] =wmr1 + (mTimestep*(-(iel2*mKe) + tormr1 + \
mTm0*limit(wmr1/mwc,-1.,1.)))/(2*mJm + mBm*mTimestep) + delayedPart[1][1];
          systemEquations[1] =thetamr1 - (mTimestep*wmr1)/2. + \
delayedPart[2][1];
          systemEquations[2] =iel2 + (-uel1 + uel2 + mKe*wmr1)/mRa;
          systemEquations[3] =-cel1 + uel1 + iel2*Zcel1;
          systemEquations[4] =-cel2 + uel2 - iel2*Zcel2;
          systemEquations[5] =-cmr1 + tormr1 - wmr1*Zcmr1;

          //Jacobian matrix
          jacobianMatrix[0][0] = 1 + \
(mTimestep*mTm0*dxLimit(wmr1/mwc,-1.,1.))/((2*mJm + mBm*mTimestep)*mwc);
          jacobianMatrix[0][1] = 0;
          jacobianMatrix[0][2] = -((mKe*mTimestep)/(2*mJm + mBm*mTimestep));
          jacobianMatrix[0][3] = 0;
          jacobianMatrix[0][4] = 0;
          jacobianMatrix[0][5] = mTimestep/(2*mJm + mBm*mTimestep);
          jacobianMatrix[1][0] = -mTimestep/2.;
          jacobianMatrix[1][1] = 1;
          jacobianMatrix[1][2] = 0;
          jacobianMatrix[1][3] = 0;
          jacobianMatrix[1][4] = 0;
          jacobianMatrix[1][5] = 0;
          jacobianMatrix[2][0] = mKe/mRa;
          jacobianMatrix[2][1] = 0;
          jacobianMatrix[2][2] = 1;
          jacobianMatrix[2][3] = -(1/mRa);
          jacobianMatrix[2][4] = 1/mRa;
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

          //Solving equation using LU-faktorisation
          pSolver->solve(jacobianMatrix, systemEquations, stateVark, iter);

          wmr1=stateVark[0];
          thetamr1=stateVark[1];
          iel2=stateVark[2];
          uel1=stateVark[3];
          uel2=stateVark[4];
          tormr1=stateVark[5];
          //Expressions
          iel1 = -iel2;
          tormg = iel2*mKe - mBm*wmr1 - mTm0*limit(wmr1/mwc,-1.,1.);
          Pin = iel2*(uel1 - uel2);
          Pout = tormr1*wmr1;
        }

        //Calculate the delayed parts
        delayParts1[1] = (-(iel2*mKe*mTimestep) + mTimestep*tormr1 - \
2*mJm*wmr1 + mBm*mTimestep*wmr1 + \
mTimestep*mTm0*limit(wmr1/mwc,-1.,1.))/(2*mJm + mBm*mTimestep);
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
        (*mpND_tormg)=tormg;
        (*mpND_Pin)=Pin;
        (*mpND_Pout)=Pout;

        //Update the delayed variabels
        mDelayedPart11.update(delayParts1[1]);
        mDelayedPart21.update(delayParts2[1]);

     }
};
#endif // ELECTRICMOTOR_HPP_INCLUDED
