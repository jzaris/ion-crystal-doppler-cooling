#include "read_parameters_with_doppler_nersc.h"
#include "array2d.h"
#include "ca_rand.h"
#include "dSFMT/dSFMT.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>
#include <stdbool.h>
#include <random>
#include <iostream>
#include <fstream>
#include <chrono>
#include <sstream>

using namespace std;
using namespace std::chrono;


//struct CARandCtx* ctx_test[5];
//struct CARandCtx* this_ctx;

void initialize(const char *filename){
	//FILE *f;
	//char *line = NULL;
	string line; 
	//size_t len = 0;

	//f = fopen(filename, "r");  Input file with ensemble parameters
	//if (f == NULL)
	//	exit(EXIT_FAILURE);

	ifstream f (filename, std::ifstream::in);
	int line_num = 0;
	int num_particles = 1;

	//getline(&line, &len, f);
	getline(f, line);
	//int j = strlen(line);
	int j = line.length();
	for (int i = 0; i<j; i++){
		if (line[i] == ' '){
			num_particles++;  /*Each space signifies another particle.  num_particles = 1 + num_spaces*/ 
		}
	}

	printf("Number of particles: %d \n", num_particles);
	static double x[10000000]; /*initial positions of each particle*/
	static double y[10000000];
	static double z[10000000];
	static double vx[10000000]; /*initial velocities*/
	static double vy[10000000];
	static double vz[10000000];
	static double q[10000000]; /*charge list*/
	static double m[10000000]; /*mass list*/
	double dt_arr[1]; /*timestep*/
	double duration_arr[1]; /*simulation duration*/
	double steps_arr[1]; /*number of timesteps*/
	double write_per_arr[1]; /*rate at which values are written to output file*/
	double Bz_arr[1]; /*axial magnetic field*/
	double kz_arr[1]; /*stiffness parameter*/
	double delta_arr[1]; /*relative strength between rot. wall and trap potentials*/
        double omegaR_arr[1]; /*rotating wall frequency*/
        double phi0_arr[1]; /*initial rot. wall angle*/
        double dt;
	double duration;	
	double steps;
	double write_per;
	double Bz;
	double kz;
	double delta;
	double omegaR;
	double phi0;
	//double omegaB[num_particles];
	static double omegaB[10000000];
	/*int steps;*/
	size_t char_proc;
	double this_double;
	string line_str;
	stringstream ss;

	while (line_num<17){
		if (line_num==0){
			cout << line;
			ss.str(line);
			//str_to_double(x, line, num_particles);  x-coordinates
			for (int i = 0; i < num_particles; i++)
			{	//this_double = stod(line, &char_proc); //get next double
				ss >> this_double;
				x[i] = this_double;
				//line =  line.substr(char_proc); //delete the double form the beginning of string after processing
				printf("%f ", x[i]);
			}
			printf("\n");
		}

		if (line_num==1){
			auto start2 = high_resolution_clock::now();
			getline(f, line);
			ss.seekg(0);
			ss.str(line);
		        //auto stop2 = high_resolution_clock::now();
			//auto duration2 = duration_cast<microseconds>(stop2-start2);
			//cout << "time 2: " << duration2.count()	<< endl;
			cout << line;
			//auto start3 = high_resolution_clock::now();
			//str_to_double(y, line, num_particles);  y-coordinates
			for (int i = 0; i< num_particles; i++)
			{       //this_double = stod(line, &char_proc); //get next double
                                ss >> this_double;
				y[i] = this_double;
                                //line =  line.substr(char_proc); //delete the double form the beginning of string after processing
                                printf("%f ", y[i]);
                        }
			
			//auto stop3 = high_resolution_clock::now();
			printf("\n");
			auto stop2 = high_resolution_clock::now();
			auto duration2 = duration_cast<microseconds>(stop2-start2);
		        cout << "time read y coord: " << duration2.count() << endl;	
		} 

	        if (line_num==2){
                        getline(f,line);
			ss.seekg(0);
			ss.str(line);
                        cout << line;
                        //str_to_double(z, line, num_particles);  z-coordinates
                        for (int i = 0; i< num_particles; i++)
                        {       //this_double = stod(line, &char_proc); //get next double
                                ss >> this_double;
				z[i] = this_double;
                                //line =  line.substr(char_proc); //delete the double form the beginning of string after processing
                                printf("%f ", z[i]);
                        }

			printf("\n");
                }

		if (line_num==3){
			getline(f,line);
		        ss.seekg(0);	
			ss.str(line);
			cout << line;
			//str_to_double(vx, line, num_particles);  x-velocities
			for(int i = 0; i<num_particles; i++)
			{       //this_double = stod(line, &char_proc); //get next double
                                ss >> this_double;
				vx[i] = this_double;
                                //line =  line.substr(char_proc); //delete the double form the beginning of string after processing
                                printf("%f ", vx[i]);
                        }

			printf("\n");
		}   
                if (line_num==4){
			getline(f,line);
			ss.seekg(0);
			ss.str(line);
			cout << line;
			//str_to_double(vy, line, num_particles);  y-velocities
			for(int i = 0; i<num_particles; i++)
			{       //this_double = stod(line, &char_proc); //get next double
                                ss >> this_double;
				vy[i] = this_double;
                                //line =  line.substr(char_proc); //delete the double form the beginning of string after processing
                                printf("%f ", vx[i]);
                        }

			printf("\n");	
		}

		if (line_num==5){
                        getline(f,line);
			ss.seekg(0);
			ss.str(line);
                        cout << line;
                        //str_to_double(vz, line, num_particles);  z-velocities
                        for (int i = 0; i< num_particles; i++)
                        {       //this_double = stod(line, &char_proc); //get next double
                                ss >> this_double;
				vz[i] = this_double;
                                //line =  line.substr(char_proc); //delete the double form the beginning of string after processing
                                printf("%f ", vz[i]);
                        }

			printf("\n");
                }
		if (line_num==6){
                        getline(f,line);
			ss.seekg(0);
			ss.str(line);
                        cout << line;
                        //str_to_double(q, line, num_particles);  charges
                        for(int i = 0; i<num_particles; i++)
                        {       //this_double = stod(line, &char_proc); //get next double
                                ss >> this_double;
				q[i] = this_double;
                                //line =  line.substr(char_proc); //delete the double form the beginning of string after processing
                                printf("%f ", q[i]);
                        }

			printf("\n");
                }

		if (line_num==7){
                        getline(f,line);
			ss.seekg(0);
			ss.str(line);
                        cout << line;
                        //str_to_double(m, line, num_particles);  masses
                        for(int i = 0; i<num_particles; i++)
                        {       //this_double = stod(line, &char_proc); //get next double
                                ss >> this_double;
				m[i] = this_double;
                                //line =  line.substr(char_proc); //delete the double form the beginning of string after processing
                                printf("%f ", m[i]);
                        }

			printf("\n");
                }


                if (line_num==8){
			 getline(f,line);  /*dt*/
			 ss.seekg(0);
			 ss.str(line);
			 cout << line;
			 //str_to_double(dt_arr, line, 1);
		         //dt = stod(line, &char_proc);
			 ss >> this_double;
			 dt = this_double;
			 printf("%f ", dt);
			 printf("\n");
		}

		if (line_num==9){
                         getline(f,line);  /*duration*/
			 ss.seekg(0);
			 ss.str(line);
                         cout << line;
                         //str_to_double(duration_arr, line, 1);
			 //duration = stod(line, &char_proc);
                         ss >> this_double;
			 duration = this_double;
			 printf("%f ", duration);
			 printf("\n");
                }

		if (line_num==10){
			 getline(f,line);  /*number of steps*/
                         ss.seekg(0);
			 ss.str(line);
			 cout << line;
			 //str_to_double(steps_arr, line, 1);
			 //steps = stod(line, &char_proc);
			 ss >> this_double;
			 steps = this_double;
			 printf("%f ", steps);
                         printf("\n");	 
		}

		if (line_num==11){
                         getline(f,line);
			 ss.seekg(0);
			 ss.str(line); 
                         cout << line;
                         //str_to_double(write_per_arr, line, 1);
                         //write_per = stod(line, &char_proc);
                         ss >> this_double;
			 write_per = this_double;
			 printf("%f ", write_per);
                         printf("\n");
                }


		if (line_num==12){
                         getline(f,line);  /*axial magnetic field*/
                         ss.seekg(0);
			 ss.str(line);
			 cout << line;
                         //str_to_double(Bz_arr, line, 1);
                         //Bz = stod(line, &char_proc);
                         ss >> this_double;
			 Bz = this_double;
			 printf("%f ", Bz);
                         printf("\n");
                }

                if (line_num==13){
                         getline(f,line);  /*kz*/
			 ss.seekg(0);
			 ss.str(line);
                         cout << line;
                         //str_to_double(kz_arr, line, 1);
                         //kz = stod(line, &char_proc);
                         ss >> this_double;
			 kz = this_double;
			 printf("%f ", kz);
                         printf("\n");
                }

		if (line_num==14){
                         getline(f,line);  /*delta*/
                         ss.seekg(0);
			 ss.str(line);
			 cout << line;
                         //str_to_double(delta_arr, line, 1);
                         //delta = stod(line, &char_proc);
                         ss >> this_double;
			 delta = this_double;
			 printf("%f ", delta);
                         printf("\n");
                }

		if (line_num==15){
                         getline(f,line);  /*omegaR*/
                         ss.seekg(0);
			 ss.str(line);
			 cout << line;
                         //str_to_double(omegaR_arr, line, 1);
                         //omegaR = stod(line, &char_proc);
                         ss >> this_double;
			 omegaR = this_double;
			 printf("%f ", omegaR);
                         printf("\n");
                }

		if (line_num==16){
                         getline(f,line);  /*phi0*/
                         ss.seekg(0);
			 ss.str(line);
			 cout << line;
                         //str_to_double(phi0_arr, line, 1);
                         //phi0 = stod(line, &char_proc);
                         ss >> this_double;
			 phi0 = this_double;
			 printf("%f ", phi0);
                         printf("\n");
                }

		line_num = line_num+1;
	}
	

	arr.Initialize(9, num_particles); /*rows correspond to pos (x,y,z), vel (x,y,z), q, m, omegaB*/
	for(int i = 0; i<num_particles; i++){
                arr(0,i) = x[i];
        }
        for(int i = 0; i<num_particles; i++){
                arr(1,i) = y[i];
        }
	for(int i = 0; i<num_particles; i++){
                arr(2,i) = z[i];
        }
        for(int i = 0; i<num_particles; i++){
                arr(3,i) = vx[i];
        }
        for(int i = 0; i<num_particles; i++){
                arr(4,i) = vy[i];
        }
	for(int i = 0; i<num_particles; i++){
                arr(5,i) = vz[i];
        }
        for(int i = 0; i<num_particles; i++){
                arr(6,i) = q[i];
        }
        for(int i = 0; i<num_particles; i++){
                arr(7,i) = m[i];
        }

	/*omega_B for each particle*/
	for(int i = 0; i<num_particles; i++){
		omegaB[i] = Bz *q[i] /m[i];
                arr(8,i) = omegaB[i];
        }

	for (int i = 0; i < num_particles; i++)
        	printf("%f ", omegaB[i]);
        printf("\n");

	/*assign values to global variables*/
	dt_global = dt;
        duration_global = duration;
        steps_global = steps;
	write_per_global = write_per;
        particles_global = num_particles;
        Bz_global = Bz;
        kz_global = kz;
        delta_global = delta;
        omegaR_global = omegaR;
        phi0_global = phi0;
        phi_global = phi0;
	//gamma0_global = 2.0 * M_PI * 18*pow(10,6);
	hbar_global = 1.054571817*pow(10,-34);

	//initialize forces array with all zeros
	forces.Initialize(3, num_particles);
	for(int i = 0; i<3; i++){
        	for(int j = 0; j<num_particles; j++){
                	forces(i,j) = 0.0;
        	}
	}
	
	//read cooling laser data
	double gamma0_arr[1];
	getline(f,line);
	ss.seekg(0);
	//str_to_double(gamma0_arr, line, 1);
	ss.str(line);
	//gamma0_global = stod(line, &char_proc);
	ss >> this_double;
	gamma0_global = this_double;

	int num_beams = 0;	
	getline(f,line);
        //j = strlen(line);
	j = line.length();
        for (int i = 0; i<j; i++){
                if (line[i] == ' '){
                        num_beams++;  /*Each space signifies another beam*/
                }
        }

	beams_global = num_beams;

	double beam_S0[num_beams];
	double beam_khatx[num_beams];
	double beam_khaty[num_beams];
	double beam_khatz[num_beams];
	double beam_waistx[num_beams];
	double beam_waisty[num_beams];
	double beam_waistz[num_beams];
	double beam_dispx[num_beams];
	double beam_dispy[num_beams];
	double beam_dispz[num_beams];
	double beam_det[num_beams];

	for(int i = 0; i < 11; i++)
	{
		if (i==0){
			ss.seekg(0);
			ss.str(line);
                        cout << line;
                        //str_to_double(beam_S0, line, num_beams);  S0
                        for (int i = 0; i < num_beams; i++)
                        {       //this_double = stod(line, &char_proc); //get next double
				ss >> this_double;
                                beam_S0[i] = this_double;
                                //line =  line.substr(char_proc); //delete the double form the beginning of string after processing
                                printf("%f ", beam_S0[i]);
                        }        
                        printf("\n");
                }

                if (i==1){
                        getline(f,line);
			ss.seekg(0);
			ss.str(line);
                        cout << line;
                        //str_to_double(beam_khatx, line, num_beams);  
                        for (int i = 0; i< num_beams; i++)
                        {       //this_double = stod(line, &char_proc); //get next double
                                ss >> this_double;
				beam_khatx[i] = this_double;
                                //line =  line.substr(char_proc); //delete the double form the beginning of string after processing
                                printf("%f ", beam_khatx[i]);
                        }
                        printf("\n");
                }

		if (i==2){
                        getline(f,line);
			ss.seekg(0);
			ss.str(line);
                        cout << line;
                        //str_to_double(beam_khaty, line, num_beams); 
                        for (int i = 0; i< num_beams; i++)
                        {       //this_double = stod(line, &char_proc); //get next double
                                ss >> this_double;
				beam_khaty[i] = this_double;
                                //line =  line.substr(char_proc); //delete the double form the beginning of string after processing
                                printf("%f ", beam_khaty[i]);
                        }
			printf("\n");
                }

		if (i==3){
                        getline(f,line);
			ss.seekg(0);
			ss.str(line);
                        cout << line;
                        //str_to_double(beam_khatz, line, num_beams);  
                        for (int i = 0; i< num_beams; i++)
                        {       //this_double = stod(line, &char_proc); //get next double
                                ss >> this_double;
				beam_khatz[i] = this_double;
                                //line =  line.substr(char_proc); //delete the double form the beginning of string after processing
                                printf("%f ", beam_khatz[i]);
                        }
			printf("\n");
                }

		if (i==4){
                        getline(f,line);
			ss.seekg(0);
			ss.str(line);
                        cout << line;
                        //str_to_double(beam_waist, line, num_beams); 
                        for (int i = 0; i< num_beams; i++)
                        {       //this_double = stod(line, &char_proc); //get next double
                                ss >> this_double;
				beam_waistx[i] = this_double;
                                //line =  line.substr(char_proc); //delete the double form the beginning of string after processing
                                printf("%f ", beam_waistx[i]);
                        }
			printf("\n");
                }

		if (i==5){
                        getline(f,line);
                        ss.seekg(0);
                        ss.str(line);
                        cout << line;
                        //str_to_double(beam_waist, line, num_beams);
                        for (int i = 0; i< num_beams; i++)
                        {       //this_double = stod(line, &char_proc); //get next double
                                ss >> this_double;
                                beam_waisty[i] = this_double;
                                //line =  line.substr(char_proc); //delete the double form the beginning of string after processing
                                printf("%f ", beam_waisty[i]);
                        }
                        printf("\n");
                }

		if (i==6){
                        getline(f,line);
                        ss.seekg(0);
                        ss.str(line);
                        cout << line;
                        //str_to_double(beam_waist, line, num_beams);
                        for (int i = 0; i< num_beams; i++)
                        {       //this_double = stod(line, &char_proc); //get next double
                                ss >> this_double;
                                beam_waistz[i] = this_double;
                                //line =  line.substr(char_proc); //delete the double form the beginning of string after processing
                                printf("%f ", beam_waistz[i]);
                        }
                        printf("\n");
                }

		if (i==7){
                        getline(f,line);
			ss.seekg(0);
			ss.str(line);
                        cout << line;
                        //str_to_double(beam_dispx, line, num_beams);  
                        for (int i = 0; i< num_beams; i++)
                        {       //this_double = stod(line, &char_proc); //get next double
                                ss >> this_double;
				beam_dispx[i] = this_double;
                                //line =  line.substr(char_proc); //delete the double form the beginning of string after processing
                                printf("%f ", beam_dispx[i]);
                        }
			printf("\n");
                }

		if (i==8){
                        getline(f,line);
			ss.seekg(0);
			ss.str(line);
                        cout << line;
                        //str_to_double(beam_dispy, line, num_beams);
                        for (int i = 0; i< num_beams; i++)
                        {       //this_double = stod(line, &char_proc); //get next double
                                ss >> this_double;
				beam_dispy[i] = this_double;
                                //line =  line.substr(char_proc); //delete the double form the beginning of string after processing
                                printf("%f ", beam_dispy[i]);
                        }
			printf("\n");
                }

		if (i==9){
                        getline(f,line);
			ss.seekg(0);
			ss.str(line);
                        cout << line;
                        //str_to_double(beam_dispz, line, num_beams);
                        for (int i = 0; i< num_beams; i++)
                        {       //this_double = stod(line, &char_proc); //get next double
                                ss >> this_double;
				beam_dispz[i] = this_double;
                                //line =  line.substr(char_proc); //delete the double form the beginning of string after processing
                                printf("%f ", beam_dispz[i]);
                        }
			printf("\n");
                }

		if (i==10){
                        getline(f,line);
			ss.seekg(0);
			ss.str(line);
                        cout << line;
                        //str_to_double(beam_det, line, num_beams);  
                        for (int i = 0; i< num_beams; i++)
                        {       //this_double = stod(line, &char_proc); //get next double
                                ss >> this_double;
				beam_det[i] = this_double;
                                //line =  line.substr(char_proc); //delete the double form the beginning of string after processing
                                printf("%f ", beam_det[i]);
                        }
			printf("\n");
                }


	}

	arr_beams.Initialize(11, num_beams); /*rows correspond to pos (x,y,z), vel (x,y,z), q, m, omegaB*/
        for(int i = 0; i<num_beams; i++){
                arr_beams(0,i) = beam_S0[i];
        }
	for(int i = 0; i<num_beams; i++){
                arr_beams(1,i) = beam_khatx[i];
        }
	for(int i = 0; i<num_beams; i++){
                arr_beams(2,i) = beam_khaty[i];
        }
	for(int i = 0; i<num_beams; i++){
                arr_beams(3,i) = beam_khatz[i];
        }
	for(int i = 0; i<num_beams; i++){
                arr_beams(4,i) = beam_waistx[i];
        }
	for(int i = 0; i<num_beams; i++){
                arr_beams(5,i) = beam_waisty[i];
        }
	for(int i = 0; i<num_beams; i++){
                arr_beams(6,i) = beam_waistz[i];
        }
	for(int i = 0; i<num_beams; i++){
                arr_beams(7,i) = beam_dispx[i];
        }
	for(int i = 0; i<num_beams; i++){
                arr_beams(8,i) = beam_dispy[i];
        }
	for(int i = 0; i<num_beams; i++){
                arr_beams(9,i) = beam_dispz[i];
        }
	for(int i = 0; i<num_beams; i++){
                arr_beams(10,i) = beam_det[i];
        }

	std::random_device rd;
        std::mt19937 gen(rd()); //rd is the seed for the mt19937 instance
        std::uniform_int_distribution<std::mt19937::result_type> dist(1,1000000);

	//unused code for populating Arr2d with random seeds for each (particle, beam) pair
	//seeds_global.Initialize(particles_global, beams_global);
	/*for(int i =0; i< particles_global; i++)
        {
                for(int j=0; j< beams_global; j++)
                {
                        seeds_global(i,j) = dist(gen);
			cout << "SEED: " << seeds_global(i,j);
                }
        }*/

	seed_global = dist(gen);

	seed_global = 123456;
	cout<<"SEED: " << seed_global<< "\n";
	ctx_global = ca_rand_create();
	ca_rand_seed(ctx_global, seed_global);


        f.close();
	/*for(int i =0; i< particles_global; i++)
        {
                for(int j=0; j< beams_global; i++)
                {
                        this_ctx = ca_rand_create();
                        //struct CARandCtx this_ctx = NULL;
			//cout << "this seed: " << seeds_global(i,j);
			//ctx_global.push_back(this_ctx);
                        //ca_rand_seed(ctx_global[i*j+i], seeds_global(i,j));
			//ca_rand_destroy(&this_ctx);
                }
        }*/
	/*for (int i = 0; i< 5;i++)
	{	
		//struct CARandCtx* ctxx = ca_rand_create();
		ctx_test[i] = ca_rand_create();
		ca_rand_seed(ctx_test[i], seeds_global(i,0));
	}*/




}



void str_to_double(double *arr, const char *str, int num_particles){   /*takes a string (str) of (num_particles) doubles and fills an array (arr) with the doubles*/
	int math_counter = 0;  /*indexes the digits in the mantissa*/
	int exp_counter =0;    /*indexes the digits in the exponent*/
	int array_counter =0;  /*indexes the number of particles*/
	double this_val = 0;   /*holds value of mantissa*/
	int this_exp = 0;      /*holds value of exponent*/
	bool pre_exp = true;   /*true if the current digit is part of the mantissa, false if it is part of the exponent*/
	bool exp_neg = false;  /*true if the exponent is negative*/
	bool val_neg = false;  /*true if the mantissa is negative*/
	char this_char;
	for(int i = 0; i < strlen(str); i++){   /*indexes each character*/
		/*printf("%c ", str[i]);*/
		/*printf("%f ", this_val);*/
		if(str[i] == ' '){             /*space delimits the end of a value*/
			/*printf("%f ", this_val);
			printf("%i ", this_exp);*/
			if(val_neg){          
				if(exp_neg)
					arr[array_counter] = -1*this_val*pow(10,-1*this_exp);  /*add value, with mantissa 'this_val' and exponent 'this_exp' to arr*/
				else
					arr[array_counter] = -1*this_val*pow(10, this_exp);
			}
			else{
				if(exp_neg)
					arr[array_counter] = this_val*pow(10, -1*this_exp);
				else
					arr[array_counter] = this_val*pow(10, this_exp);
			}

			array_counter++;   /*reset to default values before reading next value*/
			math_counter = 0;
			exp_counter =0;
			this_val = 0;
			this_exp = 0;
			exp_neg = false;
			val_neg = false;
			pre_exp = true;
		}

		else{
			if(pre_exp)
				pre_exp = !(str[i] == 'e');    /*check to see if current character is e*/
			
			if (pre_exp){
				if(math_counter == 0){     
					if(str[i] == '-'){
						val_neg = true;   /*if the first character of the value is the minus sign, then the value is negative*/
					}
					else{
						this_char = str[i];
						this_val += atoi(&this_char);  /*the first numeric character occupies the ones place of the mantissa*/
						math_counter++;
						/*printf("%c ", str[i]);
						printf("%i ",atoi(&str[i]));
						printf("%f ", this_val);*/
					}	
					
				}
				else{
					if(math_counter ==1)        /*this character is the decimal point*/
						math_counter++;
		
					else{
						this_char = str[i];
						this_val += atoi(&this_char)*pow(10, -(math_counter-1));   /*each digit of the mantissa after the decimal point*/
						math_counter++;
					}
				}
			}
			else{  /*means current index corresponds to the exponent*/

				if(str[i] == '-' || str[i] == '+'){
					if (str[i] == '-')
						exp_neg = true;     /*if the first character of the exponent is a minus sign, then the exponent is negative*/
				}

				else{
					if(str[i] != 'e'){  /*ignore e, it doesn't provide any info*/
						if(exp_counter == 0){
							this_char = str[i];
							this_exp += 10*atoi(&this_char); /*the exponent has two digits, the first of which is the tens digit*/
							exp_counter++;		
						}
						else{
							this_char = str[i];
							this_exp += atoi(&this_char);  /*ones digit*/
						}
						/*printf("This exp %i ", this_exp);*/
					}

				}
					
			}

		}
		
		if(i == strlen(str) - 1){    /*denotes the end of the final value*/
			/*printf("%f ", this_val);
			printf("%i ", this_exp);*/
			if(val_neg){
				if(exp_neg)
					arr[array_counter] = -1*this_val*pow(10,-1*this_exp);   /*add final value to arr*/
				else
					arr[array_counter] = -1*this_val*pow(10, this_exp); 
			}	
			else{
				if(exp_neg)
					arr[array_counter] = this_val*pow(10, -1*this_exp);
				else
					arr[array_counter] = this_val*pow(10, this_exp); 
			}
		}
	}
	

}
