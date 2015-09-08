#ifndef MECHANICSPRING_HPP_INCLUDED
#define MECHANICSPRING_HPP_INCLUDED

#include <iostream>
#include "ComponentEssentials.h"
#include "ComponentUtilities.h"
#include "math.h"

//!
//! @file MechanicSpring.hpp
//! @author Petter Krus <petter.krus@liu.se>
//  co-author/auditor **Not yet audited by a second person**
//! @date Mon 7 Sep 2015 14:40:08
//! @brief Linear spring
//! @ingroup MechanicComponents
//!
//==This code has been autogenerated using Compgen==
//from 
/*{, C:, HopsanTrunk, componentLibraries, defaultLibrary, Mechanic, \
Linear}/MechanicSpring.nb*/

using namespace hopsan;

class MechanicSpring : public ComponentC
{
private:
     double Ks;
     double alpha;
     Port *mpPm1;
     Port *mpPm2;
     int mNstep;
     //Port Pm1 variable
     double fm1;
     double xm1;
     double vm1;
     double cm1;
     double Zcm1;
     double eqMassm1;
     //Port Pm2 variable
     double fm2;
     double xm2;
     double vm2;
     double cm2;
     double Zcm2;
     double eqMassm2;
//==This code has been autogenerated using Compgen==
     //inputVariables
     //outputVariables
     //InitialExpressions variables
     double fak;
     double Zexpr;
     double cm1f;
     double cm2f;
     //LocalExpressions variables
     double cm10;
     double cm20;
     //Expressions variables
     //Port Pm1 pointer
     double *mpND_fm1;
     double *mpND_xm1;
     double *mpND_vm1;
     double *mpND_cm1;
     double *mpND_Zcm1;
     double *mpND_eqMassm1;
     //Port Pm2 pointer
     double *mpND_fm2;
     double *mpND_xm2;
     double *mpND_vm2;
     double *mpND_cm2;
     double *mpND_Zcm2;
     double *mpND_eqMassm2;
     //Delay declarations
//==This code has been autogenerated using Compgen==
     //inputVariables pointers
     //inputParameters pointers
     double *mpKs;
     double *mpalpha;
     //outputVariables pointers
     EquationSystemSolver *mpSolver;

public:
     static Component *Creator()
     {
        return new MechanicSpring();
     }

     void configure()
     {
//==This code has been autogenerated using Compgen==

        mNstep=9;

        //Add ports to the component
        mpPm1=addPowerPort("Pm1","NodeMechanic");
        mpPm2=addPowerPort("Pm2","NodeMechanic");
        //Add inputVariables to the component

        //Add inputParammeters to the component
            addInputVariable("Ks", "Spring constant", "N/m", 100.,&mpKs);
            addInputVariable("alpha", "numerical damping", "", 0.3,&mpalpha);
        //Add outputVariables to the component

//==This code has been autogenerated using Compgen==
        //Add constantParameters
     }

    void initialize()
     {
        //Read port variable pointers from nodes
        //Port Pm1
        mpND_fm1=getSafeNodeDataPtr(mpPm1, NodeMechanic::Force);
        mpND_xm1=getSafeNodeDataPtr(mpPm1, NodeMechanic::Position);
        mpND_vm1=getSafeNodeDataPtr(mpPm1, NodeMechanic::Velocity);
        mpND_cm1=getSafeNodeDataPtr(mpPm1, NodeMechanic::WaveVariable);
        mpND_Zcm1=getSafeNodeDataPtr(mpPm1, NodeMechanic::CharImpedance);
        mpND_eqMassm1=getSafeNodeDataPtr(mpPm1, \
NodeMechanic::EquivalentMass);
        //Port Pm2
        mpND_fm2=getSafeNodeDataPtr(mpPm2, NodeMechanic::Force);
        mpND_xm2=getSafeNodeDataPtr(mpPm2, NodeMechanic::Position);
        mpND_vm2=getSafeNodeDataPtr(mpPm2, NodeMechanic::Velocity);
        mpND_cm2=getSafeNodeDataPtr(mpPm2, NodeMechanic::WaveVariable);
        mpND_Zcm2=getSafeNodeDataPtr(mpPm2, NodeMechanic::CharImpedance);
        mpND_eqMassm2=getSafeNodeDataPtr(mpPm2, \
NodeMechanic::EquivalentMass);

        //Read variables from nodes
        //Port Pm1
        fm1 = (*mpND_fm1);
        xm1 = (*mpND_xm1);
        vm1 = (*mpND_vm1);
        cm1 = (*mpND_cm1);
        Zcm1 = (*mpND_Zcm1);
        eqMassm1 = (*mpND_eqMassm1);
        //Port Pm2
        fm2 = (*mpND_fm2);
        xm2 = (*mpND_xm2);
        vm2 = (*mpND_vm2);
        cm2 = (*mpND_cm2);
        Zcm2 = (*mpND_Zcm2);
        eqMassm2 = (*mpND_eqMassm2);

        //Read inputVariables from nodes

        //Read inputParameters from nodes
        Ks = (*mpKs);
        alpha = (*mpalpha);

        //Read outputVariables from nodes

//==This code has been autogenerated using Compgen==
        //InitialExpressions
        fak = 1/(1 - alpha);
        Zexpr = fak*Ks*mTimestep;
        cm1 = fm1 - vm1*Zexpr;
        cm2 = fm2 - vm2*Zexpr;
        cm1f = fm1;
        cm2f = fm2;

        //LocalExpressions
        fak = 1/(1 - alpha);
        Zexpr = 2*fak*Ks*mTimestep;
        cm10 = cm2 + 2*vm2*Zexpr;
        cm20 = cm1 + 2*vm1*Zexpr;

        //Initialize delays


        simulateOneTimestep();

     }
    void simulateOneTimestep()
     {
        //Read variables from nodes
        //Port Pm1
        fm1 = (*mpND_fm1);
        xm1 = (*mpND_xm1);
        vm1 = (*mpND_vm1);
        eqMassm1 = (*mpND_eqMassm1);
        //Port Pm2
        fm2 = (*mpND_fm2);
        xm2 = (*mpND_xm2);
        vm2 = (*mpND_vm2);
        eqMassm2 = (*mpND_eqMassm2);

        //Read inputVariables from nodes

        //Read inputParameters from nodes
        Ks = (*mpKs);
        alpha = (*mpalpha);

        //LocalExpressions
        fak = 1/(1 - alpha);
        Zexpr = 2*fak*Ks*mTimestep;
        cm10 = cm2 + 2*vm2*Zexpr;
        cm20 = cm1 + 2*vm1*Zexpr;

          //Expressions
          cm1 = cm1f;
          cm2 = cm2f;
          cm1f = (1 - alpha)*cm10 + alpha*cm1f;
          cm2f = (1 - alpha)*cm20 + alpha*cm2f;
          Zcm1 = Zexpr;
          Zcm2 = Zexpr;

        //Calculate the delayed parts


        //Write new values to nodes
        //Port Pm1
        (*mpND_cm1)=cm1;
        (*mpND_Zcm1)=Zcm1;
        //Port Pm2
        (*mpND_cm2)=cm2;
        (*mpND_Zcm2)=Zcm2;
        //outputVariables

        //Update the delayed variabels

     }
    void deconfigure()
    {
        delete mpSolver;
    }
};
#endif // MECHANICSPRING_HPP_INCLUDED
