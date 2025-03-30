# ALCI
Im making an Another Lambda Calculus Interpreter lol

### Basic Idea

When reducing a Lambda expression, no matter the order of reductions we always get the same result. Problem is some orders will take many more steps to do, or worse get stuck in an infinite loop.

Leftmost and leftmost-innermost reductions are guarranteed to reach the normal form but often take the longest. Rightmost reduction makes fewer steps but is almost guarranteed to get stuck if there's a Y combinator involved.

Solution: do both! and get rid of the problem where you make way too many copies of arguments.

Whenever we beta-reduce a term, instead of immediatelly making copies of the argument for every variable we  make references to the term. Then later of one of these references becomes a function of an application, we go inside and perform leftmost reduction there. After we're done, we "unwrap" the shared term by making exactly one copy of it and returning it.

From the POV of the reduction alg this is basically identical to the leftmost-innermost reduction, i.e. it will reach the normal form. But since the term is shared, any beta reduction done inside will do extra work for free!

### Datastructures

All nodes are stored as `node_t` in `store_t` in a single contiguous array. `node_t` store a type (rn this takes up 12 bytes but wastes 30 whole bits), and two integers either representing indeces to other nodes or just numbers.

The types are:

- Var - variable, stores the number of references pointing to it. If it reaches 0 it gets deleted. During copying, it also points to its copy equivalent.

- Shr - share node, stores the number of references pointing to it, and to the root of the term it shares.

- App - application, stores an index to the function and argument.

- Abs - abstraction, stores an index to its body and its variable.

When a node isnt a part of the tree its actually used as a node in a linked list of free nodes! `store_t` points to the first node in the list. This makes both deletion and insertion of nodes O(1), and doesnt use any extra memory to manage the free spaces. A small issue is that the terms can get "jumbled up" the longer and bigger the reduction runs, possibly causing a lot of cache misses. Eventually id like to implement an unjumbler function.

### Current state

All that's implemented right now is just leftmost reduction, as a proof of concept. I want to add the following:

- Proper conversion to and from LC terms, commandline input

- Optimize term copying and reduction. currently they use a lot of recursion

- Optimize the node representation to save memory

- Unjumbler to lessen cache misses

- Graphically drawing the lambda term trees
