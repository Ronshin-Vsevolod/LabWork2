# Description of requirements and usage scenarios
 

## Functional requirements

### Menu
1. The system shall display a main menu with the options:
   - "New Game".
   - "Continue" (if there is a save).
   - "Survival".
   - "Shop".
2. the player should always be able to return to the main menu.
3. Selecting a game mode ("New Game", "Continue", "Survival").Each mode must be independent of the other modes.

### Shop
4. When going to "Shop", the system should display an array of all skills that are not yet open to the player.
5. The player should be able to buy new skills with skulls.
6. Once purchased, the skill should become available for use in the shopping phase.

### Levels
7. Each level must contain:
   - The size of the playing field.
   - A script describing:
     - Types and number of enemies.
     - Conditions of their spawning (for example, death of all enemies on the field or the beginning of the battle stage).
   - The amount of gold the player receives at the end of the battle stage.
   - The number of skulls that the player receives at the end of the battle stage.
8. The system should automatically advance to the next level after the current level is completed.

### Battle Stage
9. The system must display a playing field consisting of tiles, with the size determined by the current level.
10. The field must contain:
    - Player character.
    - Opponents defined by the level script.
11. The player character must be able to:
    - Move left and right 1 empty tile.
    - Turn around.
    - Prepare skills.
    - Use skills.
    - Cancel the preparation of skills.
    - Swap skills in "prepare_stack".
    - Skip a turn.
    - Swap places with the opponent (special skill)
12. Opponents must be able to:
    - Move left and right 1 empty tile.
    - Turn around.
    - Prepare skills.
    - Use skills.
13. Actions other than cancelling preparation and changing skills must take a turn.
14. Moves must be performed one at a time, starting with the player character.
15. The cycle of moves must be repeated until:
    - The player character dies (HP <= 0). In this case:
      - Money is reset to zero.
      - The character loses all purchased improvements.
      - The player returns to the main menu.
    - All enemies are killed. In this case:
      - The player receives the reward (gold and skulls) determined by the current level.
      - The player proceeds to the shopping stage.
      - If this was the last level, the game ends.
16. After each battle stage, the system should automatically save:
    - Number of coins.
    - Number of skulls
    - The current stage (purchase or battle).
    - The character's skills and improvements.
    - Current level.

### Artificial Intelligence (AI) of opponents
17. The AI of the opponents must use an algorithm based on a decision tree:
    - Each opponent has a "path' variable that determines its current action.
    - The basic states are:
      - "path = 0": Searching for a target and choosing an action.
      - "path > 0": Preparing a skill.
      - "path = -1": Using the skill.
      - "path = -2": Turning around.
      - "path = -3": Move to the right.
      - "path = -4": Movement to the left.
18. AI algorithm:
    - If "path = 0":
      1. If the player character is in the affected area of the skill from "prepare_stack":
         - Set "path = -1" and prepare to use the skill ("use_skills").
         - End the turn.
      2. If "prepare_stack + 1 < max_stack" and the player is in the skill's affected area:
         - Set action = "prepare_stack" and end the turn.
      3. If the player's character is behind:
         - Set "path = -2" and prepare a turn ("turn_around").
         - End the turn.
      4. If "prepare_stack + 1 < max_stack":
         - Set "path = -2" and prepare a skill from "skills_stack".
         - End the turn.
      5. Prepare a move towards the player and set "path = -3" or "-4" depending on the direction.
         - End the move.
     - If "path > 0":
       - Prepare a skill with index "path - 1" from "skills_stack".
     - If "path = -1":
       - Use the skill ("use_skills").
       - Set "path = 0" and end the turn.
     - If "path = -2":
       - Turn around ("turn_around").
       - Set "path = 0" and end the turn.
     - If "path = -3":
       - Move to the right ("move_right").
       - Set "path = 0" and end the move.
     - If "path = -4":
       - Move to the left ("move_left").
       - Set "path = 0" and end the move.

### Graphics and Interface
19. The system shall display:
    - Backgrounds for:
      - Main menu.
      - Battle stage.
      - Shopping stage.
    - Sprites and animations for:
      - Player character (movement, turning around, preparing for an action, using a skill).
      - Opponents (movement, turning around, preparing for action, using a skill).
    - Interface:
      - Player character health as a numeric value.
      - "skills_stack" and "prepare_stack" with skill icons.
      - Skill icons should display damage, current and maximum cooldown as numeric values.
20. Graphics should update in real time according to player and enemy actions.


## 2. Non-functional requirements

### Graphics
1. All graphics must be in PNG or GIF format.
2. Sprites and animations must be the same size for each character.
3. Backgrounds must match the screen resolution and not be distorted when scaled.
4. Numerical values in the interface should be clearly readable and not overlap other elements.

### Scalability and speed
5. The application should be easily scalable: it is easy to create many new levels, characters, abilities due to OOP principles.
6. The programme works in real time

### Saving and loading
7. The system must save the player's progress, including:
   - Number of coins.
   - Current stage (purchases or battles).
   - Character skills and improvements.
   - Current level.
8. Loading a saved game should take no more than 2 seconds.


## 3. Usage Scenarios

### Scenario 1: Starting a new game
- Actor: Player.
- Goal: Start a new game.
- Steps:
  1. The player launches the application.
  2. The system displays the main menu.
  3. The player selects "New Game".
  4. The system starts a new game, loading the first level and moving to the battle stage.
- Result: The game has started and the player is in the first level battle stage.

### Scenario 2: Buying a new skill from the shop
- Actor: Player.
- Goal: Buy a new skill with skulls.
- Steps:
  1. The player is in the main menu.
  2. The player selects "Shop".
  3. The system displays an array of all skills not open to the player.
  4. The player selects a skill to purchase.
  5. The system checks if the player has enough skulls.
  6. If there are enough skulls, the system adds the skill to the player's list of available skills and lists the skulls.
- Result: The skill is purchased and available for use in the purchase phase.

### Scenario 3: Move to the shopping phase after winning a battle
- Actor: Player.
- Goal: Move to the shopping phase after victory.
- Steps:
  1. The player is in the battle phase.
  2. All opponents are killed (HP <= 0).
  3. The system gives the player a reward (coins and skulls) determined by the current level.
  4. The system proceeds to the shopping phase.
  5. The system saves the progress (coins, stage, skills, current level).
- Result: Player moves to the shopping stage, progress is saved.

### Scenario 4: Using a skill in battle
- Actor: Player.
- Goal: Use a skill.
- Steps:
  1. The player is in the battle phase.
  2. The player selects a skill to prepare.
  3. The system places the skill in "prepare_stack".
  4. The player selects the "use skill" option.
  5. The system activates the skill by applying its effects.
- Result: The skill is used, the turn is over.

### Scenario 5: Defeat in battle
- Actor: Player.
- Goal: End the game after a defeat.
- Steps:
  1. The player is in the battle phase.
  2. The player character's HP drops to 0.
  3. The system resets the money to zero and resets the character's improvements.
  4. The system returns the player to the main menu.
- Result: The game is over and the player is returned to the main menu.

### Scenario 6: Improving a skill during the shopping phase
- Actor: Player.
- Goal: To improve a skill.
- Steps:
  1. The player is in the shopping phase.
  2. The system displays a list of available skills and their improvements.
  3. The player selects a skill to improve.
  4. The system checks if the player has enough money.
  5. If there is enough money, the system improves the skill (increases damage and recharge time by 1) and deducts money.
- Result: Skill improved, money deducted.

### Scenario 7: Advance to the next level
- Actor: Player.
- Goal: Go to the next level.
- Steps:
  1. The player completes the battle stage of the current level.
  2. The system checks if the next level exists.
  3. If the next level exists, the system loads it and proceeds to the battle stage.
  4. If it was the last level, the game ends.
- Result: The player moves to the next level or ends the game.