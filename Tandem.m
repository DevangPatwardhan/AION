clc
clear all

%% Airplane and Path Parameters
AOA = [-90 -80 -70 -60 -40 -30 -20 -10 0 1 2 3 4 5 6 7 8 9 10 11];
CLdata = [-0.1015 -0.2266 -0.3658 -0.4903 -0.5500 -0.4326 -0.2433 -0.1816 0.66120 0.77510 0.88300 0.99170 1.09700 1.19830 1.29620 1.39160 1.48380 1.57350 1.64680 1.66520];
CDdata = [0.19151 0.29724 0.39650 0.45337 0.37460 0.27690 0.18555 0.11973 0.01723 0.01631 0.01718 0.01824 0.01960 0.02124 0.02310 0.02508 0.02703 0.02731 0.02753 0.02903];

Climb_Angle = 30;
Mass = 1.5;
V_fwd = 10;
Set_AOA = 6;
Distance_of_COG_Origin_X = 0.844/2; 
Distance_of_COG_Origin_Z = 0.03;

%% Front Wing Location and dimensions
Distance_of_LE_Wing1_Origin_X = 0.209;
Distance_of_COP1_Origin_Z = 0.006;
Distance_of_Rotor1_Origin_X = 0.265;
Distance_of_Rotor1_Origin_Z = 0.006;
MAC_Wing1 = 0.119;
Wing_Span_Wing1 = 0.714;

%% Rear Wing Location and dimensions
Distance_of_LE_Wing2_Origin_X = 0.589;
Distance_of_COP2_Origin_Z = 0.035;
Distance_of_Rotor2_Origin_X = 0.646;
Distance_of_Rotor2_Origin_Z = 0.035;

%% Calculations
Cl_Wing1 = interp1(AOA, CLdata, (Set_AOA - Climb_Angle));
Cd_Wing1 = interp1(AOA, CDdata, (Set_AOA - Climb_Angle));

MAC_Wing2 = MAC_Wing1; 
Wing_Span_Wing2 = Wing_Span_Wing1; 
Cl_Wing2 = Cl_Wing1; 
Cd_Wing2 = Cd_Wing1;

Climb_Angle = deg2rad(Climb_Angle);
V_up = V_fwd*(tan(Climb_Angle));

Weight = Mass*9.81;
Distance_of_COP1_Origin_X = Distance_of_LE_Wing1_Origin_X + (MAC_Wing1/4);
Distance_of_COP2_Origin_X = Distance_of_LE_Wing2_Origin_X + (MAC_Wing2/4);

Lift_Wing1 = 0.5*1.225*MAC_Wing1*Wing_Span_Wing1*Cl_Wing1*(V_up^2 + V_fwd^2);
Lift_Wing2 = 0.5*1.225*MAC_Wing2*Wing_Span_Wing2*Cl_Wing2*(V_up^2 + V_fwd^2);
Drag_Wing1 = 0.5*1.225*MAC_Wing1*Wing_Span_Wing1*Cd_Wing1*(V_up^2 + V_fwd^2);
Drag_Wing2 = 0.5*1.225*MAC_Wing2*Wing_Span_Wing2*Cd_Wing2*(V_up^2 + V_fwd^2);

Actual_Lift_Wing_1 = ((Lift_Wing1)*cos(Climb_Angle)) - ((Drag_Wing1)*sin(Climb_Angle));
Actual_Lift_Wing_2 = ((Lift_Wing2)*cos(Climb_Angle)) - ((Drag_Wing2)*sin(Climb_Angle));
Actual_Lift_Wing = Actual_Lift_Wing_1 + Actual_Lift_Wing_2;

Actual_Drag_Wing_1 = ((Lift_Wing1)*sin(Climb_Angle)) + ((Drag_Wing1)*cos(Climb_Angle));
Actual_Drag_Wing_2 = ((Lift_Wing2)*sin(Climb_Angle)) + ((Drag_Wing2)*cos(Climb_Angle));
Actual_Drag_Wing = Actual_Drag_Wing_1 + Actual_Drag_Wing_2;

syms Thrust_1 Thrust_2 Tilt_Angle_1 Tilt_Angle_2

%% Looping over different tilt angles
for i = 0:30
    Tilt_Angle_1 = deg2rad(i);
    
    Actual_Lift_Rotor_1 = (Thrust_1*cos(Tilt_Angle_1));
    Actual_Lift_Rotor_2 = (Thrust_2*cos(Tilt_Angle_2));
    Actual_Lift_Rotor = Actual_Lift_Rotor_1 + Actual_Lift_Rotor_2;

    Actual_Thrust_Rotor_1 = (Thrust_1*sin(Tilt_Angle_1));
    Actual_Thrust_Rotor_2 = (Thrust_2*sin(Tilt_Angle_2));
    Actual_Thrust_Rotor = Actual_Thrust_Rotor_1 + Actual_Thrust_Rotor_2;

    M1 = (Actual_Lift_Wing_1*(Distance_of_COG_Origin_X - Distance_of_COP1_Origin_X)) - (Actual_Drag_Wing_1*(Distance_of_COG_Origin_Z - Distance_of_COP1_Origin_Z)) + (Actual_Thrust_Rotor_1*(Distance_of_COG_Origin_Z - Distance_of_Rotor1_Origin_Z)) + (Actual_Lift_Rotor_1*(Distance_of_COG_Origin_X - Distance_of_Rotor1_Origin_X));
    M2 = (Actual_Lift_Wing_2*(Distance_of_COG_Origin_X - Distance_of_COP2_Origin_X)) - (Actual_Drag_Wing_2*(Distance_of_COG_Origin_Z - Distance_of_COP2_Origin_Z)) + (Actual_Thrust_Rotor_2*(Distance_of_COG_Origin_Z - Distance_of_Rotor2_Origin_Z)) + (Actual_Lift_Rotor_2*(Distance_of_COG_Origin_X - Distance_of_Rotor2_Origin_X));

    eq1 = Actual_Lift_Rotor + Actual_Lift_Wing == Weight;
    eq2 = Actual_Thrust_Rotor == Actual_Drag_Wing;
    eq3 = M1 + M2 == 0;

    eqns = [eq1, eq2, eq3];
    [Tilt_Angle_21, Thrust_11, Thrust_21] = vpasolve(eqns, [Tilt_Angle_2, Thrust_1, Thrust_2]);
    Power_req_representation = (Thrust_11^2 + Thrust_21^2)^0.5;
    Tilt1(i+1) = i;
    Tilt2(i+1) = (180*Tilt_Angle_21/pi);
    Power(i+1) = Power_req_representation;
    Thrust1(i+1) = Thrust_11;
    Thrust2(i+1) = Thrust_21;
end

%% Plotting
tiledlayout(2,2)
nexttile
plot(Tilt1, Power)
xlabel('Tilt of Front Rotors')
ylabel('Power Consumed index')

nexttile
plot(Tilt1, Tilt2)
xlabel('Tilt of Front Rotors')
ylabel('Tilt of Rear Rotors')

nexttile
plot(Tilt1, Thrust1)
xlabel('Tilt of Front Rotors')
ylabel('Thrust of each Front Rotor')

nexttile
plot(Tilt1, Thrust2)
xlabel('Tilt of Front Rotors')
ylabel('Thrust of each Rear Rotor')