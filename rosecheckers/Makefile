## <legal>
## CERT Rosecheckers
## Copyright 2007-2021 Carnegie Mellon University.
##
## NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING
## INSTITUTE MATERIAL IS FURNISHED ON AN "AS-IS" BASIS. CARNEGIE MELLON
## UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR
## IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF
## FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS
## OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT
## MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT,
## TRADEMARK, OR COPYRIGHT INFRINGEMENT.
##
## Released under a BSD (SEI)-style license, please see license.txt or
## contact permission@sei.cmu.edu for full terms.
##
## [DISTRIBUTION STATEMENT A] This material has been approved for public
## release and unlimited distribution.  Please see Copyright notice for
## non-US Government use and distribution.
##
## CERT® is registered in the U.S. Patent and Trademark Office by
## Carnegie Mellon University.
##
## This Software includes and/or makes use of the following Third-Party
## Software subject to its own license:
##
## 1. rose
##    (https://github.com/rose-compiler/rose/blob/weekly/LicenseInformation/ROSE_BSD_License.txt)
##    Copyright 2005 Lawrence Livermore National Laboratory.
##
## DM21-0505
## </legal>

# You should have ROSE predefined to point to ROSE

# Change this to where you want to install the rosecheckers programs.
bindir = /usr/bin

# Change this to your own installation of ROSE
ROSECHECKERS_CMD = ./rosecheckers

# Location of include directory after "make install"
ROSE_INCLUDE_DIR = $(ROSE)/include/rose

# Location of library directory after "make install"
ROSE_LIB_DIR = $(ROSE)/lib

# Compiler flags.
CXXFLAGS = -Wall -g

# We can't use automake conditional support here
# Support for Gabriel's QRose GUI Library
QT_LDFLAGS          =
LIB_QT              =

BOOST_LIBS = -lboost_regex -lboost_system -lboost_chrono
ROSE_LIBS = -lrose $(MYSQL_DATABASE_LIBS) $(LIB_QT) $(BOOST_LIBS) -lquadmath

SRCS = PRE.C DCL.C EXP.C ARR.C FLP.C INT.C STR.C MEM.C FIO.C ENV.C SIG.C ERR.C OBJ.C MSC.C POS.C \
	categories.C type.C value.C utilities.C utilities_cpp.C
OBJS = $(SRCS:.C=.o)
PGMS = rosecheckers virtualCFG

ANSWERS = $(wildcard test/*.ans)
ANSWERS_C = $(wildcard test/c.*.ans)
ANSWERS_CPP = $(wildcard test/cpp.*.ans)
OUTPUTS = $(ANSWERS:.ans=.output)
TESTS = $(ANSWERS:.ans=.test)
TESTS_C = $(ANSWERS_C:.ans=.test)
TESTS_CPP = $(ANSWERS_CPP:.ans=.test)

.PRECIOUS: $(OUTPUTS)

# Default make rule to use
all: pgms # tests -- not done for Eclipse

pgms: $(PGMS)
$(PGMS): %: %.o $(OBJS)
	libtool --mode=link g++ $(CXXFLAGS) -o $@ -L$(ROSE)/lib $(ROSE_LIBS) $^
%.o: %.C
	libtool --mode=compile g++ $(CXXFLAGS) -c -DHAVE_CONFIG_H -I$(ROSE_INCLUDE_DIR) -o $@ $<

tests:	$(TESTS)

%.output:	%.c $(PGMS)
	@${ROSECHECKERS_CMD} $*.c > $@ 2>&1 || true

%.output:	%.C $(PGMS)
	@${ROSECHECKERS_CMD} $*.C > $@ 2>&1 || true

%.test:	pgms %.output %.ans
	@perl -p -i -e "s|${PWD}|\.|;" $*.output
	@perl -p -i -e 's|rosecheckers: .*/EDG_SAGE_Connection|rosecheckers: .../EDGE_SAGE_Connection|;' $*.output
	@perl -n -i -e 'print if (!m|^ ./rosecheckers |);' $*.output
	@perl -n -i -e 'print if (!m|rosecheckers complete|);' $*.output
	@perl -n -i -e 'print if (!/Aborted/);' $*.output
	@perl -n -i -e "print if (!/^warning:/i);" $*.output
	@perl -n -i -e "print if (!/^error:/i);" $*.output
	@perl -n -i -e "print if (!/^In /);" $*.output
	@perl -e 'undef $$/; while (<>) {s/ *\n {10,11}([^ ^])/ \1/g; print;};' $*.output > /tmp/testdata
	@mv /tmp/testdata $*.output
	@echo "Checking $*"
	@-if [ -n "`cmp $*.output $*.ans 2>&1`" ]; then \
		echo "WARNING: $* failure" ;\
		diff -u $*.ans $*.output ;\
	fi

tests_c:	$(TESTS_C)

tests_cpp:	$(TESTS_CPP)

install: $(PGMS)
	install -d -m 0755 $(DESTDIR)$(bindir)
	install -m 0755 $^ $(DESTDIR)$(bindir)/

clean:
	-libtool --mode=clean rm $(PGMS) a.out *.ti *.loT *flymake.C *.o test/*.output *~ *# */*.o *.lo .libs nul */*~ */*#

doc:
	@echo "bulding documentation..."
	doxygen Doxyfile

# support for flymake.el
check-syntax:
	gcc -std=c99 -Wall -o nul -S -DHAVE_CONFIG_H -I$(ROSE_INCLUDE_DIR) ${CHK_SOURCES}
#check-syntax:
#	${ROSECHECKERS_CMD} --edg:no_wrap_diagnostics --edg:remarks --edg:display_error_number -DHAVE_CONFIG_H -I$(ROSE_INCLUDE_DIR) ${CHK_SOURCES}
