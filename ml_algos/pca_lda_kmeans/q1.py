import pickle as pkl
import numpy as np

def pca(X: np.array, k: int) -> np.array:
    """
    X is an (N,a,b) array comprising N images, each of size (a,b).
    Return (a*b,k) np array comprising the k normalised basis vectors comprising the k-dimensional subspace for all images
    where the first column must be the most relevant principal component and so on
    """
    # TODO
    X= X.reshape((X.shape[0],-1))
    cov_mat = np.cov(X,rowvar=False)
    eigenvalues, eigenvectors = np.linalg.eigh(cov_mat)
    sort_indices = np.argsort(eigenvalues)
    sort_indices = sort_indices[::-1]
    sort_indices = sort_indices[:k]
    eigenvectors = eigenvectors[:,sort_indices]

    return eigenvectors

    #END TODO
    

def projection(X: np.array, basis: np.array):
    """
    X is an (N,a,b) array comprising N images, each of size (a,b).
    basis is an (a*b,k) array comprising of k normalised vectors
    Return (N,k) np array comprising the k dimensional projections of the N images on the normalised basis vectors
    """
    # TODO
    X= X.reshape((X.shape[0],-1))
    projections = np.matmul(X,basis)
    return projections
    # END TODO


if __name__ == '__main__':
    mnist_data = 'mnist.pkl'
    with open(mnist_data, 'rb') as f:
        data = pkl.load(f)
    # Now you can work with the loaded object
    X=data[0]
    y=data[1]
    k=10
    basis = pca(X,k)
    print(projection(X,basis))
    