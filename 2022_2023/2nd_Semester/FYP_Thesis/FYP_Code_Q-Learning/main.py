from agent import Agent
from environment import Environment
import numpy as np


def main():
    username = "Q-learner2023"
    password = "AIisPlaying2023"

    state_space = 10
    action_space = 18
    discount_rate = 0.99
    learning_rate = 0.001
    exploration_rate = 0.1
    agent = Agent(state_space, action_space, discount_rate,
                  learning_rate, exploration_rate)
    env = Environment(username, password)

    num_episodes = 1000
    for episode in range(num_episodes):
        state = env.reset()
        done = False

        while not done:
            action = agent.get_action(state)
            next_state, reward, done = env.step(action)
            agent.train_network(state, action, reward, next_state, done)
            state = next_state


if __name__ == "__main__":
    main()
