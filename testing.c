#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

double Di[23] = {6.3, 8.7, 12.0, 15.3, 20.4, 26.1, 34.5, 40.4, 52.0, 62.1, 77.3, 89.4, 101.5, 127.4, 153.2, 201.7, 253.4, 302.0, 332.1, 379.5, 426.9, 476.1, 572.6};
double Do[23] = {10.3, 13.7, 17.1, 21.3, 26.7, 33.4, 42.2, 48.3, 60.3, 73.0, 88.9, 101.6, 114.3, 141.3, 168.3, 219.1, 273.1, 323.9, 355.6, 406.4, 457.2, 508.0, 609.6};

struct HotFluid
{
    double density;
    double spHeat;
    double thConductivity;
    double dynViscosity;
    double mFlowRate;
};

struct ColdFluid
{
    double density;
    double spHeat;
    double thConductivity;
    double dynViscosity;
    double mFlowRate;
};

int main()
{
    const double PI = 3.1416;

    struct HotFluid Steam;
    struct ColdFluid Water;

    int i=0, j=0, u=0, i_Umax = 0, j_Umax = 0;
    double Ds, Deq_friction, Deq_thermal;
    double Di_m, Do_m, Ds_m; //Diameters in m
    double Tci, Tco, Thi, Tho, LMTD;
    double Q, hi, ho, hio, U[100], Umax = 0;
    double Ret, Prt, Nut, Res_friction, Res_thermal, Prs, Nus;
    double Rft, Rfs;
    double ft, dPt, dPt_allow, fs, dPs, dPs_allow;
    double At, As, vc, vh;
    double mu_w = 0.001;
    double L_assumed = 1.5, NL, Ao, NL_optimized;

    printf("This is a test\n");
    //Enter HotFluid Properties
    printf("Enter Properties for HotFluid\n");
    printf("\nDensity = ");
    scanf("%lf",&Steam.density);
    printf("\nSpecific Heat Capacity = ");
    scanf("%lf",&Steam.spHeat);
    printf("\nThermal Conductivity = ");
    scanf("%lf",&Steam.thConductivity);
    printf("\nDynamic Viscosity = ");
    scanf("%lf",&Steam.dynViscosity);
    printf("\nMass Flow Rate = ");
    scanf("%lf",&Steam.mFlowRate);

    //Enter HotFluid Temperatures
    printf("\nEnter HotFluid Inlet Temperature (degree Celsius) = ");
    scanf("%lf",&Thi);
    printf("\nEnter HotFluid Outlet Temperature (degree Celsius) = ");
    scanf("%lf",&Tho);

    //Enter ColdFluid Properties
    printf("\nEnter Properties for ColdFluid\n");
    printf("\nDensity = ");
    scanf("%lf",&Water.density);
    printf("\nSpecific Heat Capacity = ");
    scanf("%lf",&Water.spHeat);
    printf("\nThermal Conductivity = ");
    scanf("%lf",&Water.thConductivity);
    printf("\nDynamic Viscosity = ");
    scanf("%lf",&Water.dynViscosity);
    printf("\nMass Flow Rate = ");
    scanf("%lf",&Water.mFlowRate);

    //Enter ColdFluid Temperatures
    printf("\nEnter ColdFluid Inlet Temperature (degree Celsius) = ");
    scanf("%lf",&Tci);
    printf("\nEnter ColdFluid Outlet Temperature (degree Celsius) = ");
    scanf("%lf",&Tco);

    //Enter Allowable Pressure Drop
    printf("\nEnter Allowable Pressure Drop\n");
    printf("\nFor Shell Side Flow, dP_allow(Pa) = ");
    scanf("%lf",&dPs_allow);
    printf("\nFor Tube Side Flow, dP_allow(Pa) = ");
    scanf("%lf",&dPt_allow);

    //Enter Fouling Resistance
    printf("\nEnter Fouling Resistance\n");
    printf("\nFor Shell Side Flow, Rfs = ");
    scanf("%lf",&Rfs);
    printf("\nFor Tube Side Flow, Rft = ");
    scanf("%lf",&Rft);

    //HeatLoad
    Q = Water.mFlowRate * Water.spHeat * (Tco - Tci);
    //LMTD for counter flow
    LMTD = ((Thi - Tco) - (Tho - Tci)) / log((Thi - Tco)/(Tho - Tci));

            i = 6;
            j = 12;

            Ds = Di[j];

            //Convert Every Diameter to SI unit i.e. meter
            Di_m = Di[i]/1000;
            Do_m = Do[i]/1000;
            Ds_m = Ds/1000;

            //Iterative Calculations
            At = (PI/4) * pow(Di_m,2);
            vc = Water.mFlowRate / (Water.density * At);

            As = (PI/4) * (pow(Ds_m,2) - pow(Do_m,2));
            vh = Steam.mFlowRate / (Steam.density * As);

            //For Tube Side Flow
            Ret = (Water.density * vc * Di_m) / Water.dynViscosity;
            Prt = (Water.dynViscosity * Water.spHeat) / Water.thConductivity;

            if(Ret > 10000)
            {
                Nut = 0.023 * pow(Ret,0.8) * pow(Prt,0.4) * pow((Water.dynViscosity / mu_w),0.14);
            }
            else if(Ret < 2100)
            {
                Nut = 1.86 * pow(((Ret * Prt)/(L_assumed/Di_m)),(1/3));
            }

            hi =  (Nut * Water.thConductivity) / Di_m;

            //For Equivalent Annular Shell Side Flow
            Deq_friction = Ds_m - Do_m;
            Deq_thermal = (pow(Ds_m,2) - pow(Do_m,2)) / Do_m;

            Res_friction = (Steam.density * vh * Deq_friction) / Steam.dynViscosity;
            Res_thermal = (Steam.density * vh * Deq_thermal) / Steam.dynViscosity;

            Prs = (Steam.dynViscosity * Steam.spHeat) / Steam.thConductivity;

            if(Res_thermal > 10000)
            {
                Nus = 0.023 * pow(Res_thermal,0.8) * pow(Prs,0.3) * pow((Steam.dynViscosity / mu_w),0.14);
            }
            else if(Res_thermal < 2100)
            {
                Nus = 1.86 * pow(((Res_thermal * Prs)/(L_assumed/Deq_thermal)),(1/3));
            }

            ho =  (Nus * Steam.thConductivity) / Deq_thermal;

            hio = (hi * Di_m) / Do_m;

            //Overall Heat Transfer Co-efficient for this iteration
            U[u] = 1/((1/ho) + (1/hio) + Rft + Rfs);

            //Heat Transfer Area
            Ao = Q / (U[u] * LMTD);

            //Length
            NL = Ao / (PI * Do_m);

            //Pressure Drop
            //Shell Side
            if(Res_friction < 2100)
            {
                fs = 16/Res_friction;
            }
            else
            {
                fs = 0.0014 + 0.125 * pow(Res_friction,-0.32);
            }

            dPs = 4 * fs * (NL/Deq_friction) * ((Steam.density * pow(vh,2)) / 2);

            //Tube Side
            if(Ret < 2100)
            {
                ft = 16/Ret;
            }
            else
            {
                ft = 0.0014 + 0.125 * pow(Ret,-0.32);
            }

            dPt = 4 * ft * (NL/Di_m) * ((Water.density * pow(vc,2)) / 2);

            //Pressure Drop Check
            printf("\ndpt = %lf\n\ndps = %lf\n",dPt, dPs);

            printf("\n U = %lf",U[u]);

            if(U[u]>Umax)
            {
                Umax = U[u];
                i_Umax = i;
                j_Umax = j;
                NL_optimized = NL;
            }

            u++; //Next U

    //final inference
    printf("\n\nTherefore, Maximum Possible Overall Heat Transfer Co-efficient = %lf",Umax);
    printf("\n\nTherefore, Most Optimized Design Dimensions:\n");
    printf("Di = %lf mm\nDo = %lf mm\nDs = %lf mm",Di[i_Umax],Do[i_Umax],Di[j_Umax]);
    printf("\nTotal Length = %lf m\n\n",NL_optimized);

    return 0;
}
