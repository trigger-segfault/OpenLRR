project(openlrr)


set(CMDLINE_FILES
        cmdline/CLGen.cpp
        cmdline/CLGen.h
        cmdline/CommandLine.cpp
        cmdline/CommandLine.hpp
)
source_group("cmdline" FILES ${CMDLINE_FILES})

set(PLATFORM_FILES
        platform/d3drm.cpp
        platform/d3drm.h
        platform/ddraw.h
        platform/dinput.h
        platform/dsound.h
        platform/targetver.h
        platform/timeapi.h
        platform/vfw.h
        platform/windows.cpp
        platform/windows.h
)
source_group("platform" FILES ${PLATFORM_FILES})

set(ENGINE_FILES
        engine/colour.h
        engine/geometry.h
        engine/Init.cpp
        engine/Init.h
        engine/Main.cpp
        engine/Main.h
        engine/undefined.h
        engine/Graphics.cpp
        engine/Graphics.h
)
source_group("engine" FILES ${ENGINE_AUDIO_FILES})

set(ENGINE_AUDIO_FILES
        engine/audio/3DSound.cpp
        engine/audio/3DSound.h
        engine/audio/Sound.cpp
        engine/audio/Sound.h
)
source_group("engine/audio" FILES ${ENGINE_AUDIO_FILES})

set(ENGINE_CORE_FILES
        engine/core/Config.cpp
        engine/core/Config.h
        engine/core/Errors.cpp
        engine/core/Errors.h
        engine/core/Files.cpp
        engine/core/Files.h
        engine/core/ListSet.hpp
        engine/core/Maths.cpp
        engine/core/Maths.h
        engine/core/Memory.cpp
        engine/core/Memory.h
        engine/core/Random.cpp
        engine/core/Random.hpp
        engine/core/Utils.cpp
        engine/core/Utils.h
        engine/core/Wad.cpp
        engine/core/Wad.h
)
source_group("engine/core" FILES ${ENGINE_CORE_FILES})

set(ENGINE_DRAWING_FILES
        engine/drawing/Bmp.cpp
        engine/drawing/Bmp.h
        engine/drawing/DirectDraw.cpp
        engine/drawing/DirectDraw.h
        engine/drawing/Draw.cpp
        engine/drawing/Draw.h
        engine/drawing/Flic.cpp
        engine/drawing/Flic.h
        engine/drawing/Fonts.cpp
        engine/drawing/Fonts.h
        engine/drawing/Images.cpp
        engine/drawing/Images.h
        engine/drawing/TextWindow.cpp
        engine/drawing/TextWindow.h
)
source_group("engine/drawing" FILES ${ENGINE_DRAWING_FILES})

set(ENGINE_GFX_FILES
        engine/gfx/Activities.cpp
        engine/gfx/Activities.h
        engine/gfx/AnimClone.cpp
        engine/gfx/AnimClone.h
        engine/gfx/Containers.cpp
        engine/gfx/Containers.h
        engine/gfx/Lws.cpp
        engine/gfx/Lws.h
        engine/gfx/Lwt.cpp
        engine/gfx/Lwt.h
        engine/gfx/Materials.cpp
        engine/gfx/Materials.h
        engine/gfx/Mesh.cpp
        engine/gfx/Mesh.h
        engine/gfx/Viewports.cpp
        engine/gfx/Viewports.h
)
source_group("engine/gfx" FILES ${ENGINE_GFX_FILES})

set(ENGINE_INPUT_FILES
        engine/input/InputButton.cpp
        engine/input/InputButton.hpp
        engine/input/Input.cpp
        engine/input/Input.h
        engine/input/Keys.cpp
        engine/input/Keys.h
        engine/input/MouseButtons.cpp
        engine/input/MouseButtons.h
)
source_group("engine/input" FILES ${ENGINE_INPUT_FILES})

set(ENGINE_UTIL_FILES
        engine/util/Compress.cpp
        engine/util/Compress.h
        engine/util/Dxbug.cpp
        engine/util/Dxbug.h
        engine/util/Registry.cpp
        engine/util/Registry.h
)
source_group("engine/util" FILES ${ENGINE_UTIL_FILES})

set(ENGINE_VIDEO_FILES
        engine/video/Animation.cpp
        engine/video/Animation.h
        engine/video/Animation.hpp
        engine/video/Movie.cpp
        engine/video/Movie.h
        engine/video/Movie.hpp
)
source_group("engine/video" FILES ${ENGINE_VIDEO_FILES})

set(GAME_FILES
        game/audio/SFX.cpp
        game/audio/SFX.h
        game/Debug.cpp
        game/Debug.h
        game/effects/DamageText.cpp
        game/effects/DamageText.h
        game/effects/Effects.cpp
        game/effects/Effects.h
        game/effects/LightEffects.cpp
        game/effects/LightEffects.h
        game/effects/Smoke.cpp
        game/effects/Smoke.h
        game/front/Credits.cpp
        game/front/Credits.h
        game/front/FrontEnd.cpp
        game/front/FrontEnd.h
        game/front/Loader.cpp
        game/front/Loader.h
        game/front/Reward.cpp
        game/front/Reward.h
        game/front/RewardScroll.h
        game/GameCommon.h
        game/Game.cpp
        game/Game.h
        game/GameState.cpp
        game/interface/Advisor.cpp
        game/interface/Advisor.h
        game/interface/Encyclopedia.cpp
        game/interface/Encyclopedia.h
        game/interface/HelpWindow.cpp
        game/interface/HelpWindow.h
        game/interface/hud/Bubbles.cpp
        game/interface/hud/Bubbles.h
        game/interface/hud/ObjInfo.cpp
        game/interface/hud/ObjInfo.h
        game/interface/InfoMessages.cpp
        game/interface/InfoMessages.h
        game/interface/Interface.cpp
        game/interface/Interface.h
        game/interface/Panels.cpp
        game/interface/Panels.h
        game/interface/Pointers.cpp
        game/interface/Pointers.h
        game/interface/Priorities.cpp
        game/interface/Priorities.h
        game/interface/RadarMap.cpp
        game/interface/RadarMap.h
        game/interface/ScrollInfo.cpp
        game/interface/ScrollInfo.h
        game/interface/TextMessages.cpp
        game/interface/TextMessages.h
        game/interface/ToolTip.cpp
        game/interface/ToolTip.h
        game/mission/Messages.cpp
        game/mission/Messages.h
        game/mission/NERPsFile.cpp
        game/mission/NERPsFile.h
        game/mission/NERPsFunctions.cpp
        game/mission/NERPsFunctions.h
        game/mission/NERPsRuntime.cpp
        game/mission/NERPsRuntime.h
        game/mission/Objective.cpp
        game/mission/Objective.h
        game/mission/PTL.cpp
        game/mission/PTL.h
        game/mission/Quota.h
        game/object/AITask.cpp
        game/object/AITask.h
        game/object/BezierCurve.cpp
        game/object/BezierCurve.h
        game/object/Building.cpp
        game/object/Building.h
        game/object/Collision.cpp
        game/object/Collision.h
        game/object/Creature.cpp
        game/object/Creature.h
        game/object/Dependencies.cpp
        game/object/Dependencies.h
        game/object/Flocks.cpp
        game/object/Flocks.h
        game/object/MeshLOD.cpp
        game/object/MeshLOD.h
        game/object/Object.cpp
        game/object/Object.h
        game/object/ObjectRecall.cpp
        game/object/ObjectRecall.h
        game/object/Stats.cpp
        game/object/Stats.h
        game/object/Upgrade.cpp
        game/object/Upgrade.h
        game/object/Vehicle.cpp
        game/object/Vehicle.h
        game/object/Weapons.cpp
        game/object/Weapons.h
        game/Shortcuts.cpp
        game/Shortcuts.hpp
        game/world/Camera.cpp
        game/world/Camera.h
        game/world/Construction.cpp
        game/world/Construction.h
        game/world/Detail.cpp
        game/world/Detail.h
        game/world/ElectricFence.cpp
        game/world/ElectricFence.h
        game/world/Erosion.cpp
        game/world/Erosion.h
        game/world/Fallin.cpp
        game/world/Fallin.h
        game/world/Map3D.cpp
        game/world/Map3D.h
        game/world/Roof.cpp
        game/world/Roof.h
        game/world/SelectPlace.cpp
        game/world/SelectPlace.h
        game/world/SpiderWeb.cpp
        game/world/SpiderWeb.h
        game/world/Teleporter.cpp
        game/world/Teleporter.h
        game/world/Water.cpp
        game/world/Water.h
)
source_group("game" FILES ${GAME_FILES})

add_library(openlrr MODULE
        ${CMDLINE_FILES}

        ${ENGINE_FILES}
        ${ENGINE_AUDIO_FILES}
        ${ENGINE_CORE_FILES}
        ${ENGINE_DRAWING_FILES}
        ${ENGINE_GFX_FILES}
        ${ENGINE_INPUT_FILES}
        ${ENGINE_UTIL_FILES}
        ${ENGINE_VIDEO_FILES}

        ${GAME_FILES}

        ${PLATFORM_FILES}

        common.h

        dllmain.cpp
        dllmain.h

        hook.cpp
        hook.h

        interop.cpp
        interop.h

        legacy.cpp
        legacy.h

        OpenLRR.cpp
        OpenLRR.h

        ../../resources/OpenLRR.rc
        ../../resources/logo/icon.ico
)

target_compile_definitions(openlrr PUBLIC
        "$<$<CONFIG:Debug>:_DEBUG>"
        "$<$<NOT:$<CONFIG:Debug>>:NDEBUG>"
        _GODS98COMPILE
        _GODS98_USEWAD_
        "OPENLRR_HAVE_D3DRM_H=$<BOOL:${HAVE_D3DRM_H}>"
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(openlrr PRIVATE
                -Wno-narrowing # engine/util/Dxbug.cpp
                -fpermissive   # game/interface/Interface.cpp
        )
endif()

target_link_libraries(openlrr PRIVATE
        winmm
        dsound
        d3drm
        dxguid
        dinput8
        strmiids
        vfw32
)

set_target_properties(openlrr PROPERTIES
        DEBUG_POSTFIX "-d"
        PREFIX ""
        CXX_STANDARD 17
        CXX_STANDARD_REQURED ON
)

add_custom_command(TARGET openlrr POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory_if_different "${CMAKE_SOURCE_DIR}/data/" "$<TARGET_FILE_DIR:openlrr>"
        COMMENT "Copying OpenLRR data files"
)
