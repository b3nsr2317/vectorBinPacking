# vectorBinPacking
The vector bin packing problem is NP-Hard, and there are various heuristics to try and find a solution in a reasonable amount of time. In this project I'll implement some existing solutions, and some of my own heuristics.

# currently implemented algorithms/heuristics:
## First Fit Decreasing, Item-Centric
This algorithm takes the dataset, sorts it into descending order (using the product of the values as the measure of size), then places each item into the first bin that it fits in.
## First Fit Decreasing, Bin-Centric
Similar to its item-centric counterpart, the dataset is sorted into descending order, a new bin is only opened when no values fit in the most recently opened one.
## Grouped by Most Significant
This is a bin centric algorithm that works in a similar way to FFD. The dataset is split into d lists (where d is the number of dimensions of each vector). Once split, the current bin takes an item from the list corresponding to its most empty dimension. The idea of this is to reduce the occurences of one dimension within a bin being relatively full and another being quite empty.
