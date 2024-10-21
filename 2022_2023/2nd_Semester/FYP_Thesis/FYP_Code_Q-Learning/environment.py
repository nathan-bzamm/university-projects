import poke_env
from poke_env.environment.battle import Battle
from poke_env.player.random_player import RandomPlayer
from poke_env.player_configuration import PlayerConfiguration
from poke_env.server_configuration import ShowdownServerConfiguration


class Environment(RandomPlayer):
    def __init__(self, username, password, battle_format="gen8randombattle"):
        self.battle_format = battle_format

        player_configuration = PlayerConfiguration(username, password)
        server_configuration = ShowdownServerConfiguration

        super().__init__(player_configuration)

    def reset(self):
        # Start a new battle and return the initial state
        self.start_random_battle(self.battle_format)
        return self.get_state()

    def step(self, action):
        # Perform the action chosen by the agent and observe the outcome
        self.choose_random_move(self.battle)
        next_state = self.get_state()
        reward = self.get_reward()
        done = self.battle.finished
        return next_state, reward, done

    def get_state(self):
        # Extract relevant state information from the battle object
        # This is just an example; you can customize the state representation as needed
        state = [
            self.battle.user.active_pokemon.hp,
            self.battle.opponent.active_pokemon.hp,
        ]
        return state

    def get_reward(self):
        # Compute the reward based on the battle outcome
        # This is just an example; you can customize the reward function as needed
        if self.battle.won:
            reward = 1
        elif self.battle.lost:
            reward = -1
        else:
            reward = 0
        return reward
