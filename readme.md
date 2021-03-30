#### Program simulating evolution of population.
Project made for Silesian University of Technology.

Population is presented as individuals which are a string of integers (e.g 2 98 56 2 54) in a text file.
Each individual is randomly chosen to cross with another by following rules:
- a gap is created in the "genetic code" of both individuals, which makes it into two parts,
one from start from the string to the crack, and second from crack to the end.
- then the first part of first individual swaps with second part of second individual.

Population increases or decreases by breeding and death factors, set by user, until there are enough generations which is also set by user .  

To use the program you need to set following factors in command line:
-i "input_file" -o "output_file" -w "death_factor" (from 0 to 1) -r "breeding_factor" (from 0 to 1) -p "generations" -k "number of pairs to cross"