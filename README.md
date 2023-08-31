# Maximum Weight Independent Set Problem

This is the practical part of my master's thesis for my master's degree in computer science and mathematics at the Faculty of Science - Department of Mathematics in Zagreb, Croatia.

## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Setup](#setup)
* [Bibliography](#bibliography)

## General info
This project contains the implementation of ILS-VND heuristic for the maximum weight independent set problem (MWIS) on general graphs and an exact algorithm for the MWIS problem on trees.
	
## Technologies
Project is written in the C++ language, and compiled with GCC 13.1.0. It has been built using CMake 3.27.1.
The algorithm for the MWIS problem on trees also has a Python file, and has been run using Python 3.10.1.

## Setup
The ILS-VND algorithm can be run on test instances from the BHOSLIB and DIMACS datasets:
* https://networkrepository.com/bhoslib.php
* https://networkrepository.com/dimacs.php

Place any instances from the BHOSLIB dataset into /ILS-VND/test/BHOSLIB folder, or any instances from the DIMACS dataset into /ILS-VND/test/DIMACS folder.

The algorithm for the MWIS problem on trees has a hard-coded example.

## Bibliography
[1] B. Nogueira, R. Pinheiro, and A. Subramanian, A hybrid iterated local search heuristic for the maximum weight independent set problem, Optimization Letters, vol. 12, may 2018, pages 567–583.

[2] G. H. Chen, M. T. Kuo, and J. P. Sheu, An Optimal Time Algorithm for Finding a Maximum Weight Independent Set in a Tree, BIT, vol. 28, 1988, pages 353–356.
