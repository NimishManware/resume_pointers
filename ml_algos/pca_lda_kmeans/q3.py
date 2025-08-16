import numpy as np
import matplotlib.pyplot as plt
from sklearn.preprocessing import StandardScaler
from numpy.random import uniform
from sklearn.datasets import make_blobs
import seaborn as sns
import random

class KMeans:
    def __init__(self, n_clusters=8, max_iter=300):
        self.n_clusters = n_clusters
        self.max_iter = max_iter
        self.centroids = []

    def initialise(self, X_train):
        """
        Initialize the self.centroids class variable, using the "k-means++" method, 
        Pick a random data point as the first centroid,
        Pick the next centroids with probability directly proportional to their distance from the closest centroid
        Function returns self.centroids as an np.array
        USE np.random for any random number generation that you may require 
        (Generate no more than K random numbers). 
        Do NOT use the random module at ALL!
        """
        # TODO
        self.centroids = []
        self.centroids.append(X_train[np.random.randint(0, len(X_train))])
        for i in range(1, self.n_clusters):
            dist = []
            for point in X_train:
                dist.append(np.min(np.square(np.linalg.norm(np.array(point) - np.array(self.centroids), axis=1))))
            dist = np.array(dist)
            dist = dist/np.sum(dist)
            idx = np.random.choice(len(X_train), p=dist)
            self.centroids.append(X_train[idx])
        self.centroids = np.array(self.centroids)
        return self.centroids
        # END TODO
    def fit(self, X_train):
        """
        Updates the self.centroids class variable using the two-step iterative algorithm on the X_train dataset.
        X_train has dimensions (N,d) where N is the number of samples and each point belongs to d dimensions
        Ensure that the total number of iterations does not exceed self.max_iter
        Function returns self.centroids as an np array
        """
        # TODO
        for _ in range(self.max_iter):
            classification = []
            for point in X_train:
                classification.append(np.argmin(np.linalg.norm(np.array(point) - np.array(self.centroids), axis=1)))
            classification = np.array(classification)
            new_centroids = []
            for i in range(self.n_clusters):
                new_centroids.append(np.mean(X_train[classification == i], axis=0))
            new_centroids = np.array(new_centroids)
            if np.array_equal(self.centroids, new_centroids):
                break
            self.centroids = new_centroids
        return self.centroids
        # END TODO
    
    def evaluate(self, X):
        """
        Given N data samples in X, find the cluster that each point belongs to 
        using the self.centroids class variable as the centroids.
        Return two np arrays, the first being self.centroids 
        and the second is an array having length equal to the number of data points 
        and each entry being between 0 and K-1 (both inclusive) where K is number of clusters.
        """
        # TODO
        classification = []
        for point in X:
            classification.append(np.argmin(np.linalg.norm(np.array(point) - np.array(self.centroids), axis=1)))
        classification = np.array(classification)
        return self.centroids, classification
        # END TODO

def evaluate_loss(X, centroids, classification):
    loss = 0
    for idx, point in enumerate(X):
        loss += np.linalg.norm(point - centroids[classification[idx]])
    return loss

if __name__ == "__main__":
    seed = 42
    random.seed(seed)
    np.random.seed(seed+1)

    random_state = random.randint(10,1000)
    centers = 5

    X_train, true_labels = make_blobs(n_samples=100, centers=centers, random_state=seed)
    X_train = StandardScaler().fit_transform(X_train)

    # Fit centroids to dataset
    kmeans = KMeans(n_clusters=centers)
    kmeans.initialise(X_train)
    kmeans.fit(X_train)
    print(kmeans.evaluate(X_train))
    class_centers, classification = kmeans.evaluate(X_train)
    
    #print(evaluate_loss(X_train,class_centers,classification))

    # View results
    sns.scatterplot(x=[X[0] for X in X_train],
                    y=[X[1] for X in X_train],
                    hue=true_labels,
                    style=classification,
                    palette="deep",
                    legend=None
                    )
    plt.plot([x for x, _ in kmeans.centroids],
            [y for _, y in kmeans.centroids],
            'k+',
            markersize=10,
            )
    plt.savefig("hello.png")