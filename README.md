# OpenLRR ![app icon](resources/logo/icon.png)

An open source re-implementation of [LEGO Rock Raiders (PC)][Wikipedia_LRR]. This is created by slowly implementing and replacing game functionality, while relying on the original executable and game assets for everything else.

OpenLRR is not associated with The LEGO Group or Data Design Interactive. When using the name "OpenLRR" within this project, the **L** must never be expanded (i.e. do not write "Open _LEGO_ Rock Raiders").


## Instructions

Running OpenLRR requires a working installation of LEGO Rock Raiders, and building in Visual Studio 2019.

* [Installing LEGO Rock Raiders][Wiki_RunningLRR]
    * [Your Master Guide to get LRR to run][Guide_Master]
* [Building and running OpenLRR][Wiki_RunningOpenLRR]


## Contributing

OpenLRR is still missing documentation to aid in contribution, but submittions and/or help is still encouraged.

* [Decompilation and research][Wiki_Decompiling]
* Implementing functions and modules in the **game/** folder.
* Improving and refactoring **engine/** code.
* Fixing bugs or implementing new features (please create an Issue if one doesn't exist).

Submitting decompiled functions for game code is a very involved process. Most game module functions, structures, and enums are still going through heavy refactoring and renaming, with most names not accurately describing their purpose. However, assigning more-accurate names is **not required**.



***

## See also

* [Rock Raiders United][Page_RRU]
* [RRU: Knowledge Base][Page_RRUKB]

### Other LRR projects

* [Manic Miners][Project_ManicMiners]
* [ProjectReversio/LegoRockRaiders][Project_ProjectReversio]

### Similar open source projects

* [OpenRCT2][Project_OpenRCT2]
* [OpenTTD][Project_OpenTTD]



<!-- REFERENCES -->

[Page_RRU]: <https://rockraidersunited.com/> "Rock Raiders United"
[Page_RRUKB]: <https://kb.rockraidersunited.com/Main_Page> "Rock Raiders United: Knowledge Base"

[Project_ManicMiners]: <https://manicminers.baraklava.com/> "Manic Miners: The Rock Raiders remake"
[Project_RRX]: <https://rockraidersx.com/> "Rock Raiders X - Rock Raiders recreated - This site is gone!"
[Project_ProjectReversio]: <https://github.com/ProjectReversio/LegoRockRaiders> "ProjectReversio/LegoRockRaiders: Decompiling and implementing LRR from the top down"

[Project_OpenRCT2]: <https://github.com/OpenRCT2/OpenRCT2> "An open source re-implementation of RollerCoaster Tycoon 2"
[Project_OpenTTD]: <https://github.com/OpenTTD/OpenTTD> "An open source simulation game based upon Transport Tycoon Deluxe"

[Wikipedia_LRR]: <https://wikipedia.org/wiki/Lego_Rock_Raiders_%28video_game%29> "Wikipedia: LEGO Rock Raiders (PC)"
[Guide_Master]: <https://rockraidersunited.com/guides/your-master-guide-to-get-lrr-to-run-r12/> "Your Master Guide to get LRR to run"
[Guide_MusicFix]: <https://rockraidersunited.com/guides/lrr-music-without-cd-fix-r11/> "Rock Raiders Music without CD Fix"

[Wiki_RunningLRR]: <https://github.com/trigger-segfault/OpenLRR/wiki/Running-LEGO-Rock-Raiders> "OpenLRR Wiki: Running LEGO Rock Raiders"
[Wiki_RunningOpenLRR]: <https://github.com/trigger-segfault/OpenLRR/wiki/Running-LEGO-Rock-Raiders> "OpenLRR Wiki: Running OpenLRR"
[Wiki_Decompiling]: <https://github.com/trigger-segfault/OpenLRR/wiki/Decompiling-LEGO-Rock-Raiders> "OpenLRR Wiki: Decompiling LEGO Rock Raiders"
