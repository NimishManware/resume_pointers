import numpy as np
import itertools
from tqdm import tqdm
import matplotlib.pyplot as plt


''' Do not change anything in this function '''
def generate_random_profiles(num_voters, num_candidates):
    '''
        Generates a NumPy array where row i denotes the strict preference order of voter i
        The first value in row i denotes the candidate with the highest preference
        Result is a NumPy array of size (num_voters x num_candidates)
    '''
    return np.array([np.random.permutation(np.arange(1, num_candidates+1)) 
            for _ in range(num_voters)])


def find_winner(profiles, voting_rule):
    '''
        profiles is a NumPy array with each row denoting the strict preference order of a voter
        voting_rule is one of [plurality, borda, stv, copeland]
        In STV, if there is a tie amongst the candidates with minimum plurality score in a round, then eliminate the candidate with the lower index
        For Copeland rule, ties among pairwise competitions lead to half a point for both candidates in their Copeland score

        Return: Index of winning candidate (1-indexed) found using the given voting rule
        If there is a tie amongst the winners, then return the winner with a lower index
    '''

    winner_index = None
    # TODO
    (n,m) = profiles.shape
    candidates = np.zeros(m)
    if voting_rule == 'plurality':

        for i in range(n):
            candidates[profiles[i,0] - 1] += 1

        winner_index =  (np.argmax(candidates) + 1)

    elif voting_rule == 'borda':
        
        for i in range(n):
            for j in range(m):
                candidates[profiles[i,j] - 1] += m - j - 1
        
        winner_index =  (np.argmax(candidates) + 1)

    elif voting_rule == 'stv':
        
        for i in range(m - 1):
            for j in range(n):
                k = 0
                while candidates[profiles[j,k] - 1] == n + 1:
                    k += 1
                candidates[profiles[j,k] - 1] += 1
            min_cand = np.argmin(candidates)
            candidates[min_cand] = n + 1
            for l in range(m):
                if not (candidates[l] == n + 1):
                    candidates[l] = 0

        winner_index =  (np.argmin(candidates) + 1)


    elif voting_rule == 'copeland':
        
        candidate_ranks = np.zeros_like(profiles.T)
        for i in range(n):
            for j in range(m):
                candidate_ranks[profiles[i,j] - 1,i] = j

        for i in range(m):
            for j in range(i):
                n_i = 0; n_j = 0
                for k in range(n):
                    if candidate_ranks[i,k] < candidate_ranks[j,k]:
                        n_i += 1
                    if candidate_ranks[i,k] > candidate_ranks[j,k]:
                        n_j += 1
                if n_i > n_j:
                    candidates[i] += 1
                elif n_i < n_j:
                    candidates[j] += 1
                else:
                    candidates[i] += 0.5
                    candidates[j] += 0.5

        winner_index =  (np.argmax(candidates) + 1)      

    # END TODO

    return winner_index


def find_winner_average_rank(profiles, winner):
    '''
        profiles is a NumPy array with each row denoting the strict preference order of a voter
        winner is the index of the winning candidate for some voting rule (1-indexed)

        Return: The average rank of the winning candidate (rank wrt a voter can be from 1 to num_candidates)
    '''

    average_rank = 0

    # TODO
    (n,m) = profiles.shape
    for i in range(n):
        j = 0
        while not profiles[i,j] == winner:
            j += 1
        average_rank += j + 1
    average_rank = average_rank / n

    # END TODO

    return average_rank


def check_manipulable(profiles, voting_rule, find_winner):
    '''
        profiles is a NumPy array with each row denoting the strict preference order of a voter
        voting_rule is one of [plurality, borda, stv, copeland]
        find_winner is a function that takes profiles and voting_rule as input, and gives the winner index as the output
        It is guaranteed that there will be at most 8 candidates if checking manipulability of a voting rule

        Return: Boolean representing whether the voting rule is manipulable for the given preference profiles
    '''

    manipulable = None

    # TODO

    if voting_rule == "plurality":
        (n,m) = profiles.shape
        candidates = np.zeros(m)

        for i in range(n):
            candidates[profiles[i,0] - 1] += 1

        winner_score =  np.max(candidates)
        winner = np.argmax(candidates)
        if (candidates == winner_score).sum() == 1:
            return False
        else:
            for i in range(n):
                if not profiles[i,0] == winner + 1:
                    j = 1
                    while not candidates[profiles[i,j] - 1] == winner_score:
                        j += 1
                    if not profiles[i,j] == winner + 1:
                        return True
            return False

    # END TODO

    return manipulable


if __name__ == '__main__':
    np.random.seed(420)

    num_tests = 1
    voting_rules = ['plurality', 'borda', 'stv', 'copeland']

    average_ranks = [[] for _ in range(len(voting_rules))]
    manipulable = [[] for _ in range(len(voting_rules))]
    for _ in tqdm(range(num_tests)):
        # Check average ranks of winner
        num_voters = np.random.choice(np.arange(80, 150))
        num_candidates = np.random.choice(np.arange(10, 80))
        profiles = generate_random_profiles(num_voters, num_candidates)

        for idx, rule in enumerate(voting_rules):
            winner = find_winner(profiles, rule)
            avg_rank = find_winner_average_rank(profiles, winner)
            average_ranks[idx].append(avg_rank / num_candidates)

        # Check if profile is manipulable or not
        num_voters = np.random.choice(np.arange(10, 20))
        num_candidates = np.random.choice(np.arange(4, 8))
        profiles = generate_random_profiles(num_voters, num_candidates)
        
        for idx, rule in enumerate(voting_rules):
            manipulable[idx].append(check_manipulable(profiles, rule, find_winner))


    # Plot average ranks as a histogram
    for idx, rule in enumerate(voting_rules):
        plt.hist(average_ranks[idx], alpha=0.8, label=rule)

    plt.legend()
    plt.xlabel('Fractional average rank of winner')
    plt.ylabel('Frequency')
    plt.savefig('average_ranks.jpg')
    
    # Plot bar chart for fraction of manipulable profiles
    manipulable = np.sum(np.array(manipulable), axis=1)
    manipulable = np.divide(manipulable, num_tests)
    plt.clf()
    plt.bar(voting_rules, manipulable)
    plt.ylabel('Manipulability fraction')
    plt.savefig('manipulable.jpg')