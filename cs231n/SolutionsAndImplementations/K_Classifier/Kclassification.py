import sys

from pyparsing import pyparsing_test

sys.path.append("..")

import random
import numpy as np
from cs231n.data_utils import *
from cs231n.classifiers.k_nearest_neighbor import KNearestNeighbor
import matplotlib.pyplot as plt

#loading the raw dataset

cifar10_dir = '../cs231n/datasets/cifar-10-batches-py'

# Cleaning up variables to prevent loading data multiple times (which may cause memory issue)

try:
    del X_train, y_train
    del X_test, y_test
    print("Cleared previous data")
except:
    pass

X_train, y_train, X_test, y_test = load_CIFAR10(cifar10_dir)

print('Training data shape: ', X_train.shape)
print('Training labels shape: ', y_train.shape)
print('Test data shape: ', X_test.shape)
print('Test labels shape: ', y_test.shape)

classes = ["plane", "car", "bird", "cat", "deer", "dog", "frog", "horse", "ship", "truck"]
num_classes = len(classes)

"""samples_per_class = 7
for y, cls in enumerate(classes):
    idxs = np.flatnonzero(y_train == y)
    idxs = np.random.choice(idxs, samples_per_class, replace=False)
    for  i, idx in enumerate(idxs):
        plt_idx = i * num_classes + y + 1
        plt.subplot(samples_per_class, num_classes, plt_idx)
        plt.imshow(X_train[idx].astype(np.uint8))
        plt.axis('off')
        if i == 0:
            plt.title(cls)

plt.show()"""

#Subsamplaling dataset for first 5000 train and 500 test to avoid suffer

num_training = 5000
mask = list(range(num_training))

X_train = X_train[mask]
y_train = y_train[mask]

num_test = 500

mask = list(range(num_test))
X_test = X_test[mask]
y_test = y_test[mask]

#Reshape 4D tensor to 1 D vector

X_train = np.reshape(X_train, (X_train.shape[0], -1))
X_test = np.reshape(X_test, (X_test.shape[0], -1))
print(X_train.shape, y_train.shape, X_test.shape, y_test.shape)

classifier = KNearestNeighbor()
classifier.train(X_train, y_train)

#2 loops - most naive way
"""dists = classifier.compute_distances_two_loops(X_test)"""
"""print(dists.shape)"""

"""plt.imshow(dist, interpolation='none')
plt.axis('off')
plt.show()
"""

#1 loop

# Now lets speed up distance matrix computation by using partial vectorization
# with one loop. Implement the function compute_distances_one_loop and run the
# code below:

"""dists_one = classifier.compute_distances_one_loop(X_test)"""

# To ensure that our vectorized implementation is correct, we make sure that it
# agrees with the naive implementation. There are many ways to decide whether
# two matrices are similar; one of the simplest is the Frobenius norm. In case
# you haven't seen it before, the Frobenius norm of two matrices is the square
# root of the squared sum of differences of all elements; in other words, reshape
# the matrices into vectors and compute the Euclidean distance between them.


"""difference = np.linalg.norm(dists - dists_one, ord='fro')
print('One loop difference was: %f' % (difference, ))
if difference < 0.001:
    print('Good! The distance matrices are the same')
else:
    print('Uh-oh! The distance matrices are different')
"""


# Now implement the fully vectorized version inside compute_distances_no_loops
# and run the code

"""dists_two = classifier.compute_distances_no_loops(X_test)"""

# check that the distance matrix agrees with the one we computed before:

"""difference = np.linalg.norm(dists - dists_two, ord='fro')
print('No loop difference was: %f' % (difference, ))
if difference < 0.001:
    print('Good! The distance matrices are the same')
else:
    print('Uh-oh! The distance matrices are different')"""


# Now implement the function predict_labels and run the code below:
# We use k = 1 (which is Nearest Neighbor).
"""y_test_pred = classifier.predict_labels(dists, k=1)"""

# Compute and print the fraction of correctly predicted examples

"""num_correct = np.sum(y_test_pred == y_test)
accuracy = float(num_correct) / num_test
print('Got %d / %d correct => accuracy: %f' % (num_correct, num_test, accuracy))"""





# Let's compare how fast the implementations are
"""def time_function(f, *args):"""

    # Call a function f with args and return the time (in seconds) that it took to execute.

"""    import time
    tic = time.time()
    f(*args)
    toc = time.time()
    return toc - tic

two_loop_time = time_function(classifier.compute_distances_two_loops, X_test)
print('Two loop version took %f seconds' % two_loop_time)

one_loop_time = time_function(classifier.compute_distances_one_loop, X_test)
print('One loop version took %f seconds' % one_loop_time)

no_loop_time = time_function(classifier.compute_distances_no_loops, X_test)
print('No loop version took %f seconds' % no_loop_time)"""

# You should see significantly faster performance with the fully vectorized implementation!

# NOTE: depending on what machine you're using,
# you might not see a speedup when you go from two loops to one loop,
# and might even see a slow-down.


### Cross-validation

### We have implemented the k-Nearest Neighbor classifier but we set the value k = 5 arbitrarily. We will now determine
# the best value of this hyperparameter with cross-validation.

num_folds = 5
k_choices = [1, 3, 5, 8, 10, 12, 15, 20, 50, 100]


X_train_folds = np.split(X_train, num_folds)

y_train_folds = np.split(y_train, num_folds)

k_to_accuracies = {}
for k in k_choices:
    k_to_accuracies[k] = []

    for fold in range(num_folds):
        X_val = X_train_folds[fold]
        y_val = y_train_folds[fold]

        X_final = np.concatenate(X_train_folds[:fold] + X_train_folds[fold + 1:])
        y_final = np.concatenate(y_train_folds[:fold] + y_train_folds[fold + 1:])

        classifier.train(X_final, y_final)
        dists = classifier.compute_distances_no_loops(X_val)


        y_pred = classifier.predict_labels(dists, k=k)
        accuracy = np.mean(y_pred == y_val)

        k_to_accuracies[k].append(accuracy)


for k in k_choices:
    print(np.mean(k_to_accuracies[k]))
    print("\n")