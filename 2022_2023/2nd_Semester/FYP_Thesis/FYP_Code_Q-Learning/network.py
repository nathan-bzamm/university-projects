import numpy as np


class QTable:
    def __init__(self, num_states, num_actions, learning_rate, discount_factor, exploration_rate):
        self.q_table = np.zeros((num_states, num_actions))
        self.learning_rate = learning_rate
        self.discount_factor = discount_factor
        self.exploration_rate = exploration_rate

    def update(self, state, action, reward, next_state):
        q_old = self.q_table[state, action]
        q_target = reward + self.discount_factor * \
            np.max(self.q_table[next_state])
        self.q_table[state, action] += self.learning_rate * (q_target - q_old)

    def get_best_action(self, state):
        return np.argmax(self.q_table[state])

    def get_action(self, state):
        if np.random.rand() < self.exploration_rate:
            return np.random.randint(0, self.q_table.shape[1])
        else:
            return self.get_best_action(state)
