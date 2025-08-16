import numpy as np
import pickle as pkl

class LDA:
    def __init__(self,k):
        self.n_components = k
        self.linear_discriminants = None

    def fit(self, X, y):
        """
        X: (n,d,d) array consisting of input features
        y: (n,) array consisting of labels
        return: Linear Discriminant np.array of size (d*d,k)
        """
        # TODO
        n,d,d = X.shape
        X = X.reshape((X.shape[0],-1))
        m = np.mean(X,axis=0)
        Sb = np.zeros((d*d,d*d)) 
        Sw = np.zeros((d*d,d*d)) 

        for label in np.unique(y):
            ni = (y == label).sum()
            mi = np.mean(X[y == label],axis=0)
            Sb += ni * ((mi- m).reshape(-1,1)).dot((mi- m).reshape(-1,1).T)
            Sw += (X[y == label] - mi).T.dot(X[y == label] - mi)
    
        final_matrix = np.linalg.pinv(Sw).dot(Sb)
        eigenvalues,eigenvectors = np.linalg.eig(final_matrix)
    
        eig = []
        for i in range(len(eigenvalues)):
            eig.append((np.abs(eigenvalues[i]), eigenvectors[:,i]))
        eig.sort(key=lambda x: x[0], reverse=True)

        linear_discriminants = []
        for i in range(0, self.n_components):
            linear_discriminants.append(eig[i][1].reshape(d*d, 1))
        self.linear_discriminants = np.hstack(linear_discriminants)
        return self.linear_discriminants
        #END TODO 
    
    def transform(self, X, w):
        """
        w:Linear Discriminant array of size (d*d,1)
        return: np-array of the projected features of size (n,k)
        """
        # TODO
        projected=np.matmul(X.reshape((X.shape[0],-1)),w)    # Modify as required
        return projected                   # Modify as required
        # END TODO

if __name__ == '__main__':
    mnist_data = 'mnist.pkl'
    with open(mnist_data, 'rb') as f:
        data = pkl.load(f)
    X=data[0]
    y=data[1]
    k=10
    lda = LDA(k)
    w=lda.fit(X, y)
    X_lda = lda.transform(X,w)
