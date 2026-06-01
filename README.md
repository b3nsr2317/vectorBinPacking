# vectorBinPacking

The vector bin packing problem is NP-Hard, and there are various heuristics to try and find a solution in a reasonable amount of time. In this project I'll implement some existing solutions, and some of my own heuristics.

# currently implemented algorithms/heuristics:

## First Fit Decreasing, Item-Centric

This algorithm takes the dataset, sorts it into descending order (using the sum of the values as the measure of size), then places each item into the first bin that it fits in. A good baseline.

## First Fit Decreasing, Bin-Centric

Similar to its item-centric counterpart, the dataset is sorted into descending order, a new bin is only opened when no items fit in the most recently opened one. Gives the same result as the item-centric version in a slower time, but can be adapted to use other measures of size, such as dot product, as described in "Heuristics for Vector Bin Packing" (Panigrahy et al., 2011).

## Grouped by Most Significant

This is a bin centric algorithm that works in a similar way to FFD. The dataset is split into d lists (where d is the number of dimensions of each vector), corresponding to the dimension of the largest value in each vector. Once split, the current bin takes an item from the list corresponding to its most empty dimension. The idea behind this is to reduce the occurences of one dimension within a bin being relatively full and another being quite empty. This often produces a better result than FFD, but at the expense of a slight reduction in performance.

## First N Items

This heuristic calculates the lower bound for minimum number of bins required by calculating the average of each dimension, then dividing by bin size. It then places the n larget items into separate bins, where n is the lower bound. Following this, it uses FFD to place all other items. From testing, this appears to provide a similar result compared to just using FFD, and with a better time in some cases. The main advantage compared to just using FFD is on very large datasets where time is prioritised over performance.

## First Fit Decreasing, Bin-Centric, Dynamic Sort

The focus of this heuristic is the way in which the items are sorted, rather than the algorithm itself. Instead of sorting the items based on an arbitrary value, the average size of all dimensions and the difference between largest and smallest dimension are used. These are weighted according to the average of each value across the dataset, such that in a case where items are very small in one dimension but large in another, range is more significant in the size, and in a dataset of large unskewed items, the size of the dimensions is more significant. In a dataset made up of only \[25, 75\], the range and average would both be 50, and therefore weighted equally.

## Decaying Lookahead

This heuristic uses a bin-centric FFD, but tests more than one item. It starts with n items, and n is reduced by the decay rate each recursion. After a bin has been filled, the packing that leaves the least empty space is used, and then it moves onto the next bin. Depending on the starting values of the lookahead amount and decay rate, this can find far better results than a greedy algorithm, however this can be at a large compromise on speed if the starting values are too high. In some cases, this was able to find an optimal packing in a reasonable time for 2 dimensional items/bins where FFD (and its variants) could not.

## Panigrahy Bin-Centric

As described in "Heuristics for Vector Bin Packing" (Panigrahy et al., 2011), uses the dot product of items with the remaining capacity of the open bin to determine how well it fits.

## FFD Norms

The basic FFD heuristic implemented in this uses the L1 norm (Manhattan distance) to sort items. Included in the project are variations that use a product sort, which calculates the product of all non-zero dimensions; the L2 norm (Euclidean distance); L-infinity (Chebyshev distance); and a weighted L1 norm, weights each dimension based on its significance over the whole dataset.
