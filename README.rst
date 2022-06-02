Artifical Neural Network
#########################
:Authors: David Boyd, Jacob Reola, and others (see References)
:Date: Fall 2018

This repository holds the files for an artificial neural network program that tests: and, or, xor.

Compile
========

Linux Command Line:: 

	$ g++ std=c++1z -o main *.cpp

Run
====

	* argv == "and", "or", "xor", <filename.ext>

::

	$ ./main and 
	$ ./main or
	$ ./main xor
	$ ./main <filename.ext>

How it works
=============
  After testing, weights will be saved to weights.txt.

References
===========

`Huang Zehao - Simple Neural Network <https://github.com/huangzehao/SimpleNeuralNetwork/blob/master/src/neural-net.cpp>`_
    
`Gareth Richards - Machine-Learning-CPP <https://github.com/GarethRichards/Machine-Learning-CPP/blob/master/Chapter1/NeuralNet1.cpp>`_
