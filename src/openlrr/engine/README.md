# OpenLRR engine code

The engine for OpenLRR is implemented based on the abandonware Gods98 source code. For that reason, this probably can't be placed under the same project-wide license that would be applicable to the game code.


### Contents

* `audio/`

  Music, 3D sound effects playback, and wave file loading.

* `core/`

  Core functionality depended on by the rest of the engine, and the game as whole.

* `drawing/`

  2D drawing, images, fonts, etc.

* `gfx/`

  3D rendering, models, scenes, etc.

* `input/`

  Keyboard and mouse input, and key name lookup.

* `util/`

  Engine-specific utilities not needed by game code.

* `video/`

  AVI animations, and movie playback.

* `colour.h`

  Colour-based types and structures.

* `geometry.h`

  Math-based types, structures, and enums.

* `Graphics`

  Main engine rendering functionality (split off from the `Main` module).

* `Init`

  Mode Selection dialog, and graphics display settings selection.

* `Main`

  Main engine functionality and main loop.

* `undefined.h`

  Common typedefs used for unknown fields in Ghidra.
