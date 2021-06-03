# CERT Rosecheckers

<legal>
CERT Rosecheckers
Copyright 2007-2021 Carnegie Mellon University.

NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING
INSTITUTE MATERIAL IS FURNISHED ON AN "AS-IS" BASIS. CARNEGIE MELLON
UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR
IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF
FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS
OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT
MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT,
TRADEMARK, OR COPYRIGHT INFRINGEMENT.

Released under a BSD (SEI)-style license, please see license.txt or
contact permission@sei.cmu.edu for full terms.

[DISTRIBUTION STATEMENT A] This material has been approved for public
release and unlimited distribution.  Please see Copyright notice for
non-US Government use and distribution.

CERT® is registered in the U.S. Patent and Trademark Office by
Carnegie Mellon University.

This Software includes and/or makes use of the following Third-Party
Software subject to its own license:

1. rose
   (https://github.com/rose-compiler/rose/blob/weekly/LicenseInformation/ROSE_BSD_License.txt)
   Copyright 2005 Lawrence Livermore National Laboratory.

DM21-0505
</legal>


# Rosecheckers Usage

The easiest way to run Rosecheckers is by using the pre-built Docker container:

    docker run -it --rm  docker.pkg.github.com/cmu-sei/cert-rosecheckers/rosebud:latest
    rosecheckers example.c

By default, the container runs `bash`.

This container can also be built and run locally:

    docker build -t docker.pkg.github.com/cmu-sei/cert-rosecheckers/rosebud:latest .
    docker run -it --rm  docker.pkg.github.com/cmu-sei/cert-rosecheckers/rosebud:latest  rosecheckers example.c


# Manual Install
## Prerequesites

This code has been developed and tested on an i386 workstation running Linux (2.6.32-24) and g++ (4.4.3)

This code depends on ROSE 0.9.5a, which is available for free download from

	http://rosecompiler.org/

ROSE 0.9.5a also depends on the BOOST C++ library, version 1.43, which is available for free download from:

	http://www.boost.org/

First make sure that the ROSE environment variable points to the build directory of ROSE:

	export ROSE=/usr/local/rose/compileTree


The build process depends on libtool. It works with GNU libtool version 2.2.6b.
The process also works with G++ 4.4.3.


## Build

To build the 'rosecheckers' program, which runs secure coding rules:

	make pgms

To test rosecheckers on the code samples from the CERT C Secure Coding Rules:

	make tests

To build API documentation pages, you must have doxygen installed:

	make doc

To clean documentation pages and build files:

	make clean


## Running

To run the rosecheckers program on a C or C++ source file, simply pass the file as an argument:

	rosecheckers example.c

If the file takes special arguments to compile properly, such as an include path, simply pass to rosecheckers the same arguments you would pass to GCC or G++.

If the C file violates some secure coding rules, the rosecheckers program will print them out. If the rosecheckers program can not find any violations, it prints nothing.


# Integration of Rosecheckers into a Build Process

The two main ways to run rosecheckers are (1) by hooking into
`gcc/g++` and (2) by using the shell log method. This section also
provides technical detail required to use these methods and examples
of running `rosecheckers`.

Detail: The `rosecheckers` command takes the same arguments as the GCC
compiler, but instead of compiling the code, `rosecheckers` prints
alerts. To run `rosecheckers` on a single file, pass `rosecheckers`
the same arguments that you would pass to GCC. You do not have to
explicitly specify warnings to GCC like you do when harvesting its
output.

## Method 1: Hook into `gcc/g++`

Hook into `gcc/g++` as follows:

   a. Copy the following files from `gcc_as_rosecheckers` or `g++_as_rosecheckers` to `gcc` or `g++`.
   b. Make the renamed files executable: `chmod 700 rosecheckers-code/rosecheckers/g++; chmod 700 rosecheckers-code/rosecheckers/gcc`
   g. Add the path to the scripts to the `PATH` variable:
       * `export PATH=/usr/rosecheckers:$PATH`
   h. Perform a normal build, and redirect the raw output into a text file. (To do this step, see section below about how to run `rosecheckers` with a mounted external source volume.)

## Sample Run

You should hook into `gcc/g++` prior to doing the sample run.
For running rosecheckers on a single file, do the following (example on a single file below named `example.c` )

  * `docker run -it --rm  docker.pkg.github.com/cmu-sei/cert-rosecheckers/rosebud:latest  rosecheckers example.c`


## Method 2: Shell Log

In this approach, you run the normal working build, but log raw text
output produced by `make`. Use that output to build a shell script that
runs rosecheckers on the same files built by GCC. Follow these steps:

  * Build a `makelog` file, which captures standard output and error
    from a successful build. (This assumes that your build process
    prints the commands it executes, which is the default behavior
    of `make`). A suitable command to generate the text data for an example project
    that has a Makefile that builds `all` is:
    ```
    make all >&makelog
    ```
  * Run `$SCALE_HOME/scripts/demake.py` on the `makelog` file, which
    prunes out the '`make`' commands and directory changes.
    ```
    python demake.py < makelog > output
    ```
  * Prune out lines with `:`(they indicate warnings and errors). You could use the following command, which does a per-file search (the `f` of `fgrep`) that inverts matched lines (inversion specified by the `-v` parameter) :

    ```
    fgrep -v : < output > cleaned_up_output
    ```
  * Remove any other lines that would break this shell script.
  * Substitute `rosecheckers` for each occurrence of `gcc` or `g++`, as follows:
    ```
    sed 's/gcc/rosecheckers/' cleaned_up_output > script_that_runs_rosecheckers_for_gcc
    sed 's/g++/rosecheckers/' script_that_runs_rosecheckers_for_gcc > script_that_runs_rosecheckers.sh
    ```
  * Run Bash on the shellscript, and save the output in a text file.
    ```
    ./script_that_runs_rosecheckers.sh >&output_from_rosecheckers.txt
    ```

## Using One of the Two Run Methods in the rosecheckers Container

The technique for using either a hook into `gcc/g++` or a shell log method inside a container is the same as for using it outside a container. First issue the command `docker run -it <other-args> rosecheckers bash`, and then in the container's bash shell do either the `gcc/g++` hook or the shell-log method as described above.

### Running Rosecheckers with a Mounted External Source Volume

Do the following to run the rosecheckers container on your codebase (consisting of multiple files), which will mount a shared volume (sharing the codebase files between the container and your local machine):

 * `export MY_PROJECT=/path/to/my/project/on/host`
 * `docker run -it -v ${MY_PROJECT}:/my_project rosecheckers bash`
 * Then in the bash prompt:
   * `cd /my_project`
 * Next, run `rosecheckers` using Method 1 or Method 2 described above.
   * Method 1: Build the project by using the script(s) you named `gcc` and/or `g++` on your code project. The script will also run rosecheckers.
     * `gcc file1.c file2.c file3.c file4.c`
   * Method 2: Build the project using the shell-log method for Makefiles, described in the section above.

Finally, `exit` or `logout` or `ctrl-d` in bash exits bash and removes the container (but leaves the image intact).

Note: All of the above assumes that your project will build in the `rosecheckers` container, which runs Ubuntu 18 (bionic). If you need extra dependencies, such as `clang`, you can `apt install` them in the container. Or, you could modify the `Dockerfile` if you want those extras included in the image (and all subsequent container)

# CERT Secure Coding Rules Enforced by Rosecheckers

The C and C++ Secure Coding Rules are available at:

	https://www.securecoding.cert.org
