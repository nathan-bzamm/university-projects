import numpy as np
from network import QTable


class Agent:
    def __init__(self, state_space, action_space, discount_rate, learning_rate, exploration_rate):
        self.state_space = state_space
        self.action_space = action_space
        self.discount_rate = discount_rate
        self.learning_rate = learning_rate
        self.exploration_rate = exploration_rate

        # Multiply by 2 to account for the opponent's state
        num_states = self.state_space * 2
        num_actions = self.action_space
        self.q_table = QTable(num_states, num_actions, self.learning_rate,
                              self.discount_rate, self.exploration_rate)

    def get_state_index(self, state):
        state_index = 0
        for i in range(len(state)):
            state_index += state[i] * (self.state_space ** i)
        return state_index

    def train_network(self, state, action, reward, next_state, done):
        state = self.get_state_index(state)
        next_state = self.get_state_index(next_state)
        self.q_table.update(state, action, reward, next_state)

    def get_best_action(self, state):
        state = self.get_state_index(state)
        return self.q_table.get_best_action(state)

    def get_action(self, state):
        state = self.get_state_index(state)
        return self.q_table.get_action(state)
