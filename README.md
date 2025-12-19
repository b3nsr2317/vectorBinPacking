# vectorBinPacking
The vector bin packing problem is NP-Hard, and there are various heuristics to try and find a solution in a reasonable amount of time. In this project I'll implement some existing solutions, and some of my own heuristics.

# currently implemented algorithms/heuristics:
## First Fit Decreasing, Item-Centric
This algorithm takes the dataset, sorts it into descending order (using the product of the values as the measure of size), then places each item into the first bin that it fits in.
## First Fit Decreasing, Bin-Centric
Similar to its item-centric counterpart, the dataset is sorted into descending order, a new bin is only opened when no values fit in the most recently opened one.
## Grouped by Most Significant
This is a bin centric algorithm that works in a similar way to FFD. The dataset is split into d lists (where d is the number of dimensions of each vector). Once split, the current bin takes an item from the list corresponding to its most empty dimension. The idea of this is to reduce the occurences of one dimension within a bin being relatively full and another being quite empty. This often produces a better result than FFD, but at the expense of a slight reduction in performance.
## First N Items
This heuristic calculatest the lower bound for minimum number of bins required by calculating the average of each dimension, then dividing by bin size. It then places the n larget items into separate bins, where n is the lower bound. Following this, it uses FFD to place all other items. From testing, this appears to provide a worse or equal result compared to just using FFD, and with worse performance.
## First Fit Decreasing, Bin-Centric, Dynamic Sort
The focus of this heuristic is the way in which the items are sorted, rather than the algorithm itself. Instead of sorting the items based on an arbitrary value, the average size of all dimensions and the difference between largest and smallest dimension are used. These are weighted according to the average of each value across the dataset, such that in a case where items are very small in one dimension but large in another, range is more significant in the size, and in a dataset of large unskewed items, the size of the dimensions is more significant. In a dataset made up of only \[25, 75\], the range and average would both be 50, and therefore weighted equally.
