# Doppler laser cooling of trapped ion crystals

This is a compiled molecular-dynamics code in C++ capable of simulating the Doppler laser cooling of large ion crystals confined in a Penning trap.  Individual photon scattering events are simulated semi-classically, with photon absorption along the direction of a given laser beam and emission (and, hence, ion recoil), along a random direction.  Penning trap parameters such as the magnetic field magnitude, axial confinement frequency, and rotating wall strength and frequency can be specified by the user.  Similarly, ion mass and charge can be specified.  While this code has only been tested using particles with the same charge and mass, it should be able to handle the general case of unique charges and masses with minimal, if any, modifications.  To accelerate the calculation of Coulomb forces between ions, the code incorporates the fast multipole method using the FMM3D library. Therefore, the time to compute Coulomb forces scales approximately linearly with ion number when N is sufficiently large, making this code especially useful in studying large crystals. 

Portions of this code were adapted from the GPLv3-licensed cold-atoms project by Dominic Meiser, Wes Johnson and collaborators, including translation and modification of selected routines from Python to C++. This repository is distributed under the GNU General Public License v3.0.

Advantages of this new version include efficiency of the Coulomb calculation, portability across computing platforms, as well as a convenient resume feature discussed below.

## Installation
To compile the code, it is necessary to install the FMM3D library. The Doppler cooling code has been benchmarked using v1.0.1 of the FMM3D library, although it should be possible to use more recent versions.  When including the FMM3D v1.0.1 C headers from C++ code, we found it necessary to add the following compatibility guard to FMM3D/c/utils.h:

```c
#ifdef __cplusplus
#define complex _Complex
#endif
```

Download v1.0.1 of the FMM3D library 

    git clone --branch v1.0.1 --depth 1 https://github.com/flatironinstitute/FMM3D.git FMM3D_v1.0.1

and install using 

    make install PREFIX=/path/to/FMM3D_v1.0.1_install FAST_KER=ON

Further details on the installation of FMM3D can be found at https://fmm3d.readthedocs.io/en/latest/

To compile the Doppler cooling code, first

    cd ioncrystal

then 

    make

### FMM3D paths

By default, the Makefile assumes

    FMM3D_DIR=$HOME/FMM3D_v1.0.1
    FMM3D_LIB=$HOME/FMM3D_v1.0.1_install

These can be overridden at build time:

    make FMM3D_DIR=/path/to/FMM3D \
         FMM3D_LIB=/path/to/directory/containing/libfmm3d.so

At runtime, ensure that the directory containing `libfmm3d.so`
is included in `LD_LIBRARY_PATH`, for example:

    export LD_LIBRARY_PATH=/path/to/directory/containing/libfmm3d.so:$LD_LIBRARY_PATH


## Usage
Jupyter notebook files for pre- and post-processing are provided in the examples folder. Before running the Python examples, set FMM3D_PYTHON_PATH in the notebook ./examples/initialize_crystal to the location of your local FMM3D/python directory.

The file './examples/initialize_crystal.ipynb' allows the user to specify the particle, trap, and laser parameters.  An equilibrium ion configuration is then computed and the crystal is excited to an initial temperature specified by the user. An input text file is then generated with the simulation parameters and initial crystal state.  

An example script to run the Doppler laser cooling simulation is provided in the folder ./examples/run_scripts and can be run from the repository root.  The input file required to run this script is generated in initialize_crystal.ipynb, as described above.  In the file 'run_example_100i.sh', the executable name is followed by the name of the input file and the desired name of the output file.  The code is also capable of starting a cooling simulation from the end state of a previous run.  This resume feature is shown in 'run_example_100i_resume.sh', which reads the same input file, but also takes the previous output file as an input.  

After the simulation has finished, the output file can be read using './examples/read_output.ipynb'. Functions to compute the energy of the crystal over time during laser cooling are provided.

We note that the fast multipole method can be disabled and replaced with the direct Coulomb calculation by commenting out 'coulomb_force_fmm();'  and uncommenting 'coulomb_force();' in the function 'run_trap_potential(double kz)' (both function calls appear twice).   For small crystals, the direct Coulomb force evaluation is faster than the FMM.

## License

This project is distributed under the GNU General Public License v3.0 (GPLv3). See the `LICENSE` file for the full license text.

Portions of this project were adapted from the GPLv3-licensed `cold-atoms` project by Wes Lee Johnson and collaborators (https://github.com/WesLeeJohnson/cold-atoms). Selected routines and algorithms were translated and modified from Python into C++ for the present Penning-trap ion-crystal simulation code. Other portions of this repository were developed independently for this project.

Third-party software included in or used by this project remains subject to its own license terms.

### Third-party software

* **dSFMT** — The repository includes source code from the dSFMT pseudorandom-number generator under `external/dSFMT/`. These files remain under the original dSFMT BSD license. The corresponding license notice is included with the bundled dSFMT source. https://github.com/MersenneTwister-Lab/dSFMT
* **FMM3D / FMM3DPy** — Long-range Coulomb interactions may be evaluated using FMM3D, developed by the Flatiron Institute FMM3D development team. FMM3D is distributed under the Apache License 2.0. https://github.com/flatironinstitute/fmm3d
* **NumPy, SciPy, Matplotlib, and Jupyter** — These Python packages are external dependencies used for initialization, analysis, visualization, and example workflows.

## Acknowledgements

This project builds on ideas and selected routines from the `cold-atoms` project (https://github.com/WesLeeJohnson/cold-atoms). Portions of that code were adapted and translated from Python to C++.

Long-range Coulomb calculations use FMM3D / FMM3DPy, developed by the Flatiron Institute FMM3D development team. Users of this repository are encouraged to cite the FMM3D project and associated publications when appropriate.

The Python analysis and initialization tools make use of NumPy, SciPy, Matplotlib, and Jupyter.

## Dependencies

### C/C++

* GCC / G++ with C++17 support
* FMM3D

### Python

* Python 3
* NumPy
* SciPy
* Matplotlib
* Jupyter
* FMM3DPy
  
## Support
If you encounter any issues or have questions, please email john.zaris@colorado.edu.





