# OpenLRR game code


### Contents

* `audio/SFX`

  SFX name lookup, loading of `Samples` config block, random sample groups, and speech management.

* `effects/`

  3D world effects and particles.

* `front/`

  Screens and menus not tied to gameplay.

* `interface/`

  Game interaction, icon menus, panels, heads up displays, and some shared 2D drawing code.

* `object/`

  In-game entity AI, stats, logic, and graphics resource containers.

* `objective/`

  Level goals, reward quotas, scripting, and PTL files.

* `world/`

  World map, camera, objects tied to block grids, and 3D rendering.

* `Game`

  Main game functionality (originally the `Lego` module in LRR).

* `GameCommon.h`

  Common defines, structures, and enums that haven't been properly tied to a specific module yet.
