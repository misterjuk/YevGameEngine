# Custom 2D engine and DigDug Remake

Welcome to the Custom 2D engine and DigDug Remake project! This is a modern reinterpretation of the classic arcade game **DigDug**, developed using a custom 2d game engine.

## Features

- **Resource Management**: Efficiently handles textures, fonts, and music using RAII. Resources are loaded once and reused as needed.
- **Scene Management**: Manages game scenes with a dedicated Scene Manager that handles rendering, updating, and object management. Only one scene is rendered and updated at a time.
- **Game Loop**: The Scene handles the game loop, updating and rendering the game frame-by-frame.
- **Game Time**: Implements a Singleton pattern for game time management, providing DeltaTime globally.
- **Input Management**: Utilizes a Singleton Input Manager that employs the Command pattern and Observer pattern for input handling.
- **Sound Management**: Runs sound on a separate thread and is implemented using the Service Locator pattern.
- **Component System**: Game objects are managed using a component-based architecture, prioritizing composition over inheritance. Components use the Observer pattern for event handling.
- **Player and Enemies**: player and enemies are handled using the State pattern.

[GitHub Link](https://github.com/misterjuk/YevGameEngine)