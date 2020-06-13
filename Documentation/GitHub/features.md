# Editor features

* __Asset management__
  * Simple drag and drop import for many popular formats
  * Automatic reimport of externally modified assets (e.g. modify a shader, see changes in editor right away)
  * Asset modifications immediately reflected in-game (resource hot-swap)
  * Version control friendly format
* __Powerful object inspector__
  * Exposes script object properties for artists/designers
  * Automatically generated GUI for custom classes
  * Customize visible elements via attributes or create GUI manually
* __Level creation__
  * Simple drag and drop interface
  * Traditional set of tools (Move/Scale/Rotate/Select, etc.)
  * Interface for creating custom 2D and 3D tools
* __Prefab system__
  * Save parts or entire levels as prefabs so they may be re-used later
  * Separate larger levels into smaller prefabs for easier loading
  * Reduce conflicts when multiple people are working on the same level
  * Customize individual prefab instances without breaking the prefab link
  * Supports nesting and complex hierarchies to ensure maintaining complex levels is easy
* __Play in editor__
  * Compile all scripts within editor
  * Scripts and data transparently reloaded after compilation so changes may be tested immediately
  * Pause and frame-step to better test and debug your game
  * Analyze and modify scene while playing
* __Animation__
  * Built-in animation editor
  * Generic property animation (Animate any script property)
* __Fully extensible__
  * Specialized scripting API only for editor extensions
  * Easy to use without needing to know about engine internals
  * Extend almost anything. Create:
	* Custom editor windows
	* Custom object inspectors
	* Custom 2D/3D tools
	* Code for automating common tasks
* __Game publishing__
  * Build a game ready for distribution from within the editor
  * One click build process, just choose a platform and build
  * Automatically detects only the required resources to minimize build size
  * Automatically packages and outputs an executable
* __Customizable frontend__
  * Dockable layout and floating windows
  * Custom GUI skin & localization support
