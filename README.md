# rosi

How to install ROSI Version 0.20.0


ROSI runs under Linux, Windows with Cygwin environment and SunOS.


You need the following packages:
* lscatgismo-1.4.tar.gz
* ravar-0.8.8.tar.gz
* rosi-0.20.0.tar.gz

The following two packages are recommended but not necessary
* spectra.tar.gz
* materials.tar.gz


Installation on Linux systems
-----------------------------

1. Unzip and untar these packages 

2. Go into lscatgismo-1.4 and install (see INSTALL in lscatgismo-1.4)

3. Go into ravar-0.8 and install (see INSTALL in ravar-0.8)

4. Go into the directory of rosi-0.19

5. If the directories lscatgismo-1.4, ravar-0.8, spectra and materials 
   are located NOT side a side with rosi-0.19 in your file system, than
   change the file 'configure' in rosi-0.19 to your requirements, that means
   adjust the four path variables: GISMOPATH, RAVARPATH, SPECPATH, and MATPATH

6. Invoke configure by:
   > ./configure

7. Start compiling with:
   > make


Installation on Windwos/Cygwin systems
--------------------------------------

1. Unzip and untar these packages 

2. Go into lscatgismo-1.4 and install by:
   > ./configure
   > ./make

3. Go into ravar-0.8 and install (see INSTALL in ravar-0.8)

4. Go into the directory of rosi-0.19

5. If the directories lscatgismo-1.4, ravar-0.8, spectra and materials 
   are located NOT side a side with rosi-0.19 in your file system, than
   change the file 'configure' in rosi-0.19 to your requirements, that means
   adjust the four path variables: GISMOPATH, RAVARPATH, SPECPATH, and MATPATH

6. Invoke configure by:
   > ./configure

7. Start compiling with:
   > make win


Installation on SunOS systems using Sun C++
-------------------------------------------

1. Unzip and untar these packages 

2. Go into lscatgismo-1.4 and install (see INSTALL in lscatgismo-1.3)
   To select Sun C++ compiler use: ./configure CXX=CC CC=cc F77=f77

3. Go into ravar-0.8 and install (see INSTALL in ravar-0.8)
   To select Sun C++ compiler adjust ./configure first.

4. Go into the directory of rosi-0.19

5. If the directories lscatgismo-1.4, ravar-0.8, spectra and materials 
   are located NOT side a side with rosi-0.19 in your file system, than
   change the file 'configure' in rosi-0.19 to your requirements, that means
   adjust the four path variables: GISMOPATH, RAVARPATH, SPECPATH, and MATPATH

6. If Sun C++ will be used adjust the compiler commands in ./configure to:
   CXX="CC"
   HCC="mpCC"
   CC="cc"
   AR="ar"

7. If Sun C++ will be used adjust the LDOPT command in ./example/Makefile.in to:
   LDOPT = -lm -lsocket -lnsl

8. Invoke configure by:
   > ./configure

9. Start compiling with:
   > make


There is no install script, so far!
