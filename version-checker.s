#!/bin/bash
# Simple script to list version numbers of critical development tools
export LC_ALL=C
FAIL=0

checkVersion() {
	echo -n "Checking $1 >= $2....."
	semver -c $2 $1
	if [ $? -eq 0 ]; then
		echo "FAILED"
		FAIL=1
	else
		echo "PASSED"
	fi
}

echo "Checking System Requirements....."
checkVersion "bash" "3.2"
checkVersion "ld" "2.25"
checkVersion "bison" "2.7"

if [ -h /usr/bin/yacc ]; then
  checkVersion "$(readlink -f /usr/bin/yacc)" "2.7"
elif [ -x /usr/bin/yacc ]; then
  checkVersion "yacc" "2.7"
else
  echo "yacc not found"
fi

checkVersion "diff" "2.8.1"
checkVersion "find" "4.2.31"
checkVersion "gawk" "4.0.1"
checkVersion "gcc" "4.9"
checkVersion "g++" "4.9"
checkVersion "ldd"  "2.11"
checkVersion "grep" "2.5.1"
checkVersion "gzip" "1.3.12"
checkVersion "m4" "1.4.10"
checkVersion "make" "4.0"
checkVersion "patch" "2.5.4"
checkVersion "perl" "5.8.8"
checkVersion "sed" "4.1.5"
checkVersion "tar" "1.22"
checkVersion "makeinfo" "4.7"
checkVersion "xz" "5.0.0"

echo 'int main(){}' > dummy.c && g++ -o dummy dummy.c
echo -n "Checking g++ Compilation....."
if [ -x dummy ]
  then echo "PASSED";
  else echo "FAILED"; fi
rm -f dummy.c dummy

checkVersion "kernel" "3.2"

if [ $FAIL -eq 1 ]; then
	echo "Failed minimum system requirements for building LFS"
else
	echo "Minimum System Reuirements Met!"
fi
