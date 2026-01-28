This is a calculator designed to make calculations with error. This process turns out to be quite simple to do, but takes a very long time to perform by hand especially for complicated calculations involving multiple variables, many terms with error, and complex functions. The calculator simplifies this into simple entry, like a normal calculator.
The updated release, which should function as a standalone executable, may be found in ~/x64/Release.

This calculator supports three basic functions, which include many different operations.

1. Basic Evaluation: typing an expression using the correct syntax, the calculator will automatically evaluate it and give a decimal answer. If any variables with error are included, the error will be shown as well.

2. Variable definition: typing the name of a single-letter (upper- or lower- case) variable will define it and allow it to be used in further calculations. This value may be changed by simply redefining the variable. If the calculation involves evaluation, the calculator will perform this automatically. If this evaluation yields error, the error of the variable will be the vector sum of the input error and the calculated error.

3. Function Definition: typing the name of a function will define it and store it for further use. This function may be any number of arguments and supports error, so calculations made using it will automatically propagate the error.

Syntax

- variable names may only be one uppercase or lowercase character from the standard English alphabet. 'E' is not allowed as a variable name, as it is used for scientific notation.
- the constants e, c, h, and g are predefined to be euler's number, e = 2.71828..., the speed of light, c = 300,000,000, planck's constant, h = 6.62607015*10^-34, and the acceleration due to gravity on Earth, g = 9.80665. These may be overwritten by redefining the variables.
- implicit multiplication is recognized.
- functions with longer names take priority over shorter ones. For instance, 'arccos' will be recognized before 'cos'.
- all functions must be followed by a valid set of parentheses to contain its arguments.
- implicit functions and implicit variable definitions are not supported.
- some functions are predefined. A full list of these may be found in varlist.h, along with a list of predefined variables. These may be altered and will automatically be recognized, should you choose to add more default functions/variables or remove those already there. WARNING: some operations are not supported, such as csc, sec, and cot, and the hyperbolic functions. A list of all functions currently supported may be found further below.
- variables defined first take precedent, but variables with the same name will be replaced with the one most recently created. This includes function variables; if using a function variable with the same name as a preexisting variable, the preexisting variable will be used instead.
- separate arguments in functions by commas.

Erratta
- This calculator may be innaccurate for certain particularly large errors due to C++'s limitations in rounding and doubles. This should not be very noticeable for small error or small variables (which are the main focus of this calculator anyway), but bewarned that they may not always be accurate. This should not generally affect value calculations, however it may be wise to perform sanity checks at times.

- This calculator uses the derivative method of error propagation, calculating the error of a function based on a specific variable as the partial derivative with respect to that variable, evaluated at a point, times the error of the variable at that point. The total error of a function is equal to the vector sum of all of the individual errors with respect to each variable.

- To modify the source code, download the full file and modify as necessary. The compilation was done on main.cpp, and can be recompiled (perhaps using new default funcs) from there.
  
The full list of functions that are currently supported by default are:
    - sqrt
    - cos
    - sin
    - tan
    - arcsin
    - arccos
    - arctan
    - ln
    - log
    - arbitrary log base, loga
    - exponentiation
    - multiplication
    - division
    - addition
    - subtraction (done by inversion then addition)
More may come soon.


developed by Phoebe Violette Streiffer, 2025. please attribute credit in modifications, as well as when using (such as in lab reports).
