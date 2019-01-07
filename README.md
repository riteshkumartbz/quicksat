# QUICKSAT 
## (SAT Solver for CS202 Assignment)

Qicksat is a SAT Solver which accepts the input in DIMACS format and then checks for Satisfiablity of the given set of clauses, just like MINISAT.
This is an implementation of SAT solver using improvised semantic tableaux and Davis–Putnam–Logemann–Loveland (DPLL) algorithm.


Compile the satsolver.cpp via :

```
g++ quicksat.cpp

```

Then execute the executable file created :
```
./a.out yourinput.cnf answer.txt

```
Where yourinput.cnf is the input in DIMACS format and answer.txt will give the final solution of the set of clauses.



## Heuristics Used by our code:


#### 1. Input Optimisation
Any clause having a proposition and its negation is not inserted in the list of clauses as it is true irrespective of valuations of other propositions.
If a clause contains a proposition multiple times then it is added to the list only one time.

#### 2. Pre-Processing
If a clause contains a single proposition then the value of proposition is assigned as that has to be true for the formula to be satisfiable.
The clauses which contain such propositions are removed from list of propositions and the clauses containing negation are removed from the rest of list of set of clauses. They are the clauses which contain literals of the form 'a' and '-a'. They have to be true due to binary nature of a proposition.

#### 3. Unit propogation
When we assign a proposition true then clauses having that proposition are removed from that list.
Also the propositions of size 2 having negation of the proposition automatically assigns value to other proposition.
So again we can remove some clauses from the list and again get values to some of the propositions.
So this step is recursively done unless list gets exhausted, contradiction occurs or we don't have a assigned proposition to propogate.

#### 4. Weightage function
We have assigned extra weigtage to the propositions occuring in clauses of size 2 as they automatically assign value to other proposition or make a proposition true.
By using various values of weightage functions we learned the weightage value to be 2.
So we calculated weighted frequency for each element as 2*(frequency of the proposition occuring in clause of size 2)+frequency of the proposition

#### 5. Deterministic Frequency
Once the maximium frequency of all the clauses in the list becomes 1 then we need not take decisions anymore and we assign values to the
propositions to make them true if possible or return UNSAT if we already determined values of propositions do not satisfy the list of literals.

Sample Output
---------------------
The sample output for Sudoku (9x9):

![Output for 9x9 sudoku encoding](SampleOutput.jpg?raw=true "Output for 9x9 sudoku encoding")

Documentation references and Further Reading
------------------------
- https://en.wikipedia.org/wiki/Unit_propagation
- https://en.wikipedia.org/wiki/DPLL_algorithm

